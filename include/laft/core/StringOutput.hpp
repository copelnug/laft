#ifndef LAFT_CORE_STRING_OUTPUT_HPP_INCLUDED
#define LAFT_CORE_STRING_OUTPUT_HPP_INCLUDED

#include <laft/core/Output.hpp>
namespace laft
{
	namespace core
	{
		/**
			\brief Utility to build string.
		*/
		class StringOutput : public Output<StringOutput>
		{
			public:
				void write_impl(char const *value, size_t length);
				
				void clear();
				
				std::string extract();
				std::string const& get() const;
			private:
				std::string value_; /**< \brief String buffer. */
		};
	}
}
#endif
