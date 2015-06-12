#ifndef LAFT_TEXT_FORMAT_HPP_INCLUDED
#define LAFT_TEXT_FORMAT_HPP_INCLUDED
#include <string>

#include <laft/core/Mixin.hpp>
#include <laft/core/StringOutput.hpp>

namespace laft
{
	namespace text
	{
		template <typename Derived>
		class Formatter : protected laft::core::Mixin<Derived>
		{
			public:
				template <typename ...T>
				Formatter& format(size_t length, char const* formatString, const T&... args);
		};
		
		template <typename Output>
		class TextOutputFormatter : public Formatter<TextOutputFormatter<Output>>
		{
			public:
				TextOutputFormatter(Output& output);
			
				void write(char const* text, size_t length);
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
		template <typename Derived>
		template <typename ...T>
		Formatter<Derived>& Formatter<Derived>::format(size_t length, char const* formatString, const T&... args)
		{
			this->self().write(formatString, length);
			return *this;
		}
		template <typename Output>
		TextOutputFormatter<Output>::TextOutputFormatter(Output& output) :
			output_(output)
		{}
		template <typename Output>
		void TextOutputFormatter<Output>::write(char const* text, size_t length)
		{
			output_.write(text, length);
		}
		template <typename ...T>
		std::string format(std::string const& formatString, const T&... args)
		{
			return format(formatString.length(), formatString.c_str());
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
			TextOutputFormatter<Output> out(output);
			out.format(length, formatString, args...);
		}
	}
}

#endif
