#include "../catch.hpp"
#include "laft/text/Format.hpp"

TEST_CASE("format_basic", "[text]")
{
	std::string BASIC = "basic";
	REQUIRE(laft::text::format(BASIC) == BASIC);

	std::string EMPTY = "";
	REQUIRE(laft::text::format(EMPTY) == EMPTY);
}
