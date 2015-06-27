#include "../catch.hpp"
#include "laft/text/Format.hpp"

TEST_CASE("format_nothing", "[text]")
{
	std::string basic{"basic"};
	REQUIRE(laft::text::format(basic) == basic);

	std::string empty{""};
	REQUIRE(laft::text::format(empty) == empty);
	
	std::string numbers{"a0a1a2a3a4a5a6a7a8a9a"};
	REQUIRE(laft::text::format(numbers) == numbers);
}
TEST_CASE("format_basic", "[text]")
{
	REQUIRE(laft::text::format("This is test #$0.", "0") == "This is test #0.");
	REQUIRE(laft::text::format("#$0 come after #$1.", "1", "0") == "#1 come after #0.");
	REQUIRE(laft::text::format("This $0 is longer.", "value") == "This value is longer.");
	REQUIRE(laft::text::format("Lets $0 and $0.", "repeat") == "Lets repeat and repeat.");
	REQUIRE(laft::text::format("$0 with a replacement.", "Start") == "Start with a replacement.");
	REQUIRE(laft::text::format("Replacement at the $0", "end.") == "Replacement at the end.");
	REQUIRE(laft::text::format("$0", "Only.") == "Only.");
}
TEST_CASE("format_escape", "[text]")
{
	REQUIRE(laft::text::format("This is worth 10$$.") == "This is worth 10$.");
	REQUIRE(laft::text::format("Symbol followed by a variable: $$$0.", "var") == "Symbol followed by a variable: $var.");
	REQUIRE(laft::text::format("Symbol $0 can be in replacement without escaping.", "$") == "Symbol $ can be in replacement without escaping.");
}
