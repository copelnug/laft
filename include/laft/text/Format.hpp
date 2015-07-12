#ifndef LAFT_TEXT_FORMAT_HPP_INCLUDED
#define LAFT_TEXT_FORMAT_HPP_INCLUDED
#include <stdexcept>
#include <string>

#include <laft/core/Mixin.hpp>
#include <laft/core/StringOutput.hpp>

namespace laft
{
	namespace text
	{
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
			
				void write(char const* text, size_t length);
				template <typename T>
				void write(T const& value);
			private:
				Output& output_;
		};
		
		template <typename ...T>
		std::string format(std::string const& formatString, const T&... args);
		template <typename ...T>
		std::string format(size_t length, char const* formatString, const T&... args);
		
		template <typename Output, typename ...T>
		void format(Output& output, size_t length, char const* formatString, const T&... args);
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
							ArgsHelpers<Form>::formatter_.write(arg_);
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
			// TODO use string::find. Do not write characters 1 by 1.
			bool parsing = false;
			bool found = false;
			size_t index = 0;
			for(size_t i = 0; i < length; ++i)
			{
				char c = formatString[i];
				
				switch(c)
				{
					case '$':
						found = true;
						if(parsing)
						{
							parsing = false;
							this->self().write(&c, 1);
						}
						else
						{
							index = 0;
							found = false;
							parsing = true;
						}
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						if(parsing)
						{
							found = true;
							index = index * 10 + c - '0';
							break;
						}
						// Continue
					default:
						if(parsing)
						{
							if(!found)
								throw std::logic_error("Missing value after $");
							helper.handle(index);
							parsing = false;
						}
						this->self().write(&c, 1);
						break;
				}
			}
				
			if(parsing)
			{
				if(!found)
					throw std::logic_error("Missing value after $");
				helper.handle(index);
			}
			return this->self();
		}
		template <typename Output>
		OutputBuilder<Output>::OutputBuilder(Output& output) :
			output_(output)
		{}
		template <typename Output>
		void OutputBuilder<Output>::write(char const* text, size_t length)
		{
			output_.write(text, length);
		}
		template <typename Output>
		template <typename T>
		void OutputBuilder<Output>::write(T const& value)
		{
			output_.write(value);
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
			format(out, length, formatString, args...);
			return out.extract();
		}
		template <typename Output, typename ...T>
		void format(Output& output, size_t length, char const* formatString, const T&... args)
		{
			OutputBuilder<Output> out(output);
			out.format(length, formatString, args...);
		}
	}
}

#endif
