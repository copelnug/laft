#ifndef LAFT_CORE_OUTPUT_HPP_INCLUDED
#define LAFT_CORE_OUTPUT_HPP_INCLUDED

#include <laft/core/Mixin.hpp>

#include <string>
#include <cstring>

namespace laft
{
	namespace core
	{
		/**
			\brief A mixin that represent an output for bytes.
			\tparam Implementation Concrete class.
			
			This mixin is there to implement wrapper to handle the
			multiple ways to receive bytes. 
			
			Require that Implementation has the following method:
			> _ write_impl(char const *buffer, size_t length)
		*/
		template <typename Implementation>
		class Output : private Mixin<Implementation>
		{
			public:
				void write(char value);
				void write(char const *value);
				void write(std::string const& value);
				void write(char const *value, size_t length);
		};
	}
}


// Implementation
/**
	\brief Write a char to the output.
	\tparam Implementation Concrete class.
	\param value Character to write.
*/
template <typename Implementation>
void laft::core::Output<Implementation>::write(char value)
{
	this->self().write_impl(&value, 1);
}
/**
	\brief Write a C-string to the output.
	\tparam Implementation Concrete class.
	\param value C-string to write.
*/
template <typename Implementation>
void laft::core::Output<Implementation>::write(char const *value)
{
	this->self().write_impl(value, std::strlen(value));
}
/**
	\brief Write a std::string to the output.
	\tparam Implementation Concrete class.
	\param value String to write.
*/
template <typename Implementation>
void laft::core::Output<Implementation>::write(std::string const& value)
{
	this->self().write_impl(value.c_str(), value.length());
}
/**
	\brief Write bytes to the output.
	\tparam Implementation Concrete class.
	\param value Bytes to write.
	\param length Number of bytes to write.
*/
template <typename Implementation>
void laft::core::Output<Implementation>::write(char const *value, size_t length)
{
	this->self().write_impl(value, length);
}
#endif
