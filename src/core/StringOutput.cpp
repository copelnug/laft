#include <laft/core/StringOutput.hpp>

/**
	\brief Write bytes to the string.
	\param value Bytes to write.
	\param length Number of bytes to write.
*/
void laft::core::StringOutput::write_impl(char const *value, size_t length)
{
	value_.append(value, length);
}
/**
	\brief Clear the content of the string.
*/
void laft::core::StringOutput::clear()
{
	value_.clear();
}
/**
	\brief Extract the content of the string. Clearing it in the process.
	\return Value that was in the string.
*/
std::string laft::core::StringOutput::extract()
{
	std::string newVal;
	std::swap(value_, newVal);
	return newVal;
}
/**
	\brief Get the current content of the string.
	\return Current content of the string.
	
	When modifying the StringOutput, the value pointed by the reference may change.
*/
std::string const& laft::core::StringOutput::get()  const
{
	return value_;
}
