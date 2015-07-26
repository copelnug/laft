#ifndef LAFT_TEXT_FORMAT_HPP_INCLUDED
#define LAFT_TEXT_FORMAT_HPP_INCLUDED
#include <algorithm>
#include <stdexcept>
#include <string>

#include <laft/core/Mixin.hpp>
#include <laft/core/StringOutput.hpp>

namespace laft
{
	namespace text
	{
		/**
			\brief Concept of a class that is used to format a value.
		*/
		class Format {};
		/**
			\brief Concept for a class that build text.
			\tparam Derive Type implementing theh concept.
		*/
		template <typename Derived>
		class Builder : protected laft::core::Mixin<Derived>
		{
			public:
				template <typename ...T>
				Derived& format(size_t length, char const* formatString, const T&... args);
		};
		/**
			\brief Class to build string in an Output.
			\tparam Output Type of the output to build for.
		*/
		template <typename Output>
		class OutputBuilder : public Builder<OutputBuilder<Output>>
		{
			public:
				OutputBuilder(Output& output);
			
				void writeText(char const* text, size_t length);
				template <typename T, typename std::enable_if<!std::is_base_of<Format, T>::value, bool>::type = true>
				void writeValue(T const& value);
				template <typename T, typename std::enable_if<std::is_base_of<Format, T>::value, bool>::type = true>
				void writeValue(T const& value);
			private:
				Output& output_;
		};
		
		template <typename ...T>
		std::string format(std::string const& formatString, const T&... args);
		template <typename ...T>
		std::string format(size_t length, char const* formatString, const T&... args);
		
		template <typename Output, typename ...T>
		void format_in(Output& output, size_t length, char const* formatString, const T&... args);
		
		class CharFormat : public Format {
			public:
				using const_iterator = const char*;
				
				CharFormat(char value);
				
				const_iterator begin() const;
				const_iterator end() const;
			private:
				char value_[2];
		};
		class DecimalSignedFormat : public Format { // TODO Variable size? uint8_t, uint
			public:
				using const_iterator = const char*;
				
				DecimalSignedFormat(int64_t value);
				
				const_iterator begin() const;
				const_iterator end() const;
			private:
				char representation_[1+19+1]; /* std::ceil(std::log10(std::exp2(63))) */
				char* begin_;
		};
		class DecimalUnsignedFormat : public Format {
			public:
				using const_iterator = const char*;
				
				DecimalUnsignedFormat(uint64_t value);
				
				const_iterator begin() const;
				const_iterator end() const;
			private:
				char representation_[20+1]; /* std::ceil(std::log10(std::exp2(64))) */
				char* begin_;
		};
		class CStringFormat : public Format {
			public:
				using const_iterator = const char*;
				
				CStringFormat(const char* value);
				
				const_iterator begin() const;
				const_iterator end() const;
			private:
				const char* value_;
		};
		class StringFormat : public Format {
			public:
				using const_iterator = std::string::const_iterator;
				
				StringFormat(const std::string& value);
				
				const_iterator begin() const;
				const_iterator end() const;
			private:
				const std::string& value_;
		};
		
		CharFormat as_text(bool value);
		CharFormat as_text(char value);
		DecimalSignedFormat as_text(int16_t value);
		DecimalSignedFormat as_text(int32_t value);
		DecimalSignedFormat as_text(int64_t value);
		DecimalUnsignedFormat as_text(uint16_t value);
		DecimalUnsignedFormat as_text(uint32_t value);
		DecimalUnsignedFormat as_text(uint64_t value);
		CStringFormat as_text(const char* value);
		StringFormat as_text(const std::string& value);
	}
}


namespace laft
{
	namespace text
	{
		namespace impl
		{
			// TODO: Replace Form with a functor to allow reuse of ArgsHelpers. Or give the functor to handle. This allow to reuse a same ArgsHelper for multiples functions
			template <typename Form, typename ...T>
			class ArgsHelpers;
			
			template <typename Form, typename T, typename ...R>
			class ArgsHelpers<Form, T, R...> : public ArgsHelpers<Form, R...>
			{
				public:
					ArgsHelpers(Form& formatter, T const& arg, R const& ... rest) :
						ArgsHelpers<Form, R...>(formatter, rest...),
						arg_(arg)
					{}
					void handle(size_t i)
					{
						if(i == 0)
							ArgsHelpers<Form>::formatter_.writeValue(arg_);
						else
							ArgsHelpers<Form, R...>::handle(i - 1);
					}
				private:
					T const& arg_;
			};
			template <typename Form>
			class ArgsHelpers<Form>
			{
				public:
					ArgsHelpers(Form& formatter) :
						formatter_(formatter)
					{}
					void handle(size_t)
					{
						// TODO: New exception?
						throw std::out_of_range("Format args is not in valid range.");
					}
				protected:
					Form& formatter_;
			};
		}
		template <typename Derived>
		template <typename ...T>
		Derived& Builder<Derived>::format(size_t length, char const* formatString, const T&... args)
		{
			impl::ArgsHelpers<Derived, T...> helper(this->self(), args...);
			
			char const* start = formatString;
			char const* end = start + length;
			
			while(true)
			{
				char const* current = std::find(start, end, '$');
				
				this->self().writeText(start, current - start);
				
				if(current != end)
				{
					++current;
					
					if(current == end)
						throw std::logic_error("Missing value after $");
					
					if(*current == '$')
					{
						this->self().writeText(current, 1);
						start = current + 1;
					}
					else
					{
						size_t index = 0;
						start = std::find_if(current, end, [&](char c) {
							switch(c)
							{
								case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
									index = index * 10 + c - '0';
									return false;
								default:
									return true;
							}
						});
						
						if(start == current)
							throw std::logic_error("Missing value after $");
						
						helper.handle(index);
					}
				}
				else
					break;
			}
			return this->self();
		}
		template <typename Output>
		OutputBuilder<Output>::OutputBuilder(Output& output) :
			output_(output)
		{}
		template <typename Output>
		void OutputBuilder<Output>::writeText(char const* text, size_t length)
		{
			output_.write(text, length);
		}
		template <typename Output>
		template <typename T, typename std::enable_if<!std::is_base_of<Format, T>::value, bool>::type>
		void OutputBuilder<Output>::writeValue(T const& value)
		{
			writeValue(as_text(value));
		}
		template <typename Output>
		template <typename T, typename std::enable_if<std::is_base_of<Format, T>::value, bool>::type>
		void OutputBuilder<Output>::writeValue(T const& format)
		{
			for(char c : format)
			{
				output_.write(c);
			}
		}
		template <typename ...T>
		std::string format(std::string const& formatString, const T&... args)
		{
			return format(formatString.length(), formatString.c_str(), args...);
		}
		template <typename ...T>
		std::string format(size_t length, char const* formatString, const T&... args)
		{
			laft::core::StringOutput out;
			format_in(out, length, formatString, args...);
			return out.extract();
		}
		template <typename Output, typename ...T>
		void format_in(Output& output, size_t length, char const* formatString, const T&... args)
		{
			OutputBuilder<Output> out(output);
			out.format(length, formatString, args...);
		}
	}
}

#endif
