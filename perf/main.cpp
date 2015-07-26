#include <iostream>
#include <sstream>

#include <laft/core/StringOutput.hpp>
#include <laft/text/Format.hpp>

constexpr const char* content = "performance";

void testStringStream(size_t loop)
{
	std::ostringstream sout;
	for(size_t i = 0; i < loop; ++i)
	{
		sout << "Ceci est un test de " << content << ".\n";
		sout.str("");
	}
}
void testLaftStringOutput(size_t loop)
{
	laft::core::StringOutput lout;
	char message[] = "Ceci est un test de $0.\n";
	for(size_t i = 0; i < loop; ++i)
	{
		laft::text::format_in(lout, sizeof(message)-1, message, content);
		lout.clear();
	}
}
void testSprintf(size_t loop)
{
	char buffer[1024];
	for(size_t i = 0; i < loop; ++i)
	{
		sprintf(buffer, "Ceci est un test de %s.\n", content);
	}
}
int main()
{
	time_t start;
	constexpr size_t LOOP = 1000000;
	
	start = time(nullptr);
	testStringStream(LOOP);
	std::cout << "ostsringstream: " << time(nullptr) - start << std::endl;
	
	start = time(nullptr);
	testLaftStringOutput(LOOP);
	std::cout << "StringOutput: " << time(nullptr) - start << std::endl;
	
	start = time(nullptr);
	testSprintf(LOOP);
	std::cout << "Sprintf: " << time(nullptr) - start << std::endl;
	
	return 0;
}
