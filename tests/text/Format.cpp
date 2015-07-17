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
TEST_CASE("format_types", "[text]")
{
	// Unsigned
	REQUIRE(laft::text::format("Check for each number: $0.", 1234567890u) == "Check for each number: 1234567890.");
	REQUIRE(laft::text::format("Specific conversion: $0.", laft::text::as_text(432u)) == "Specific conversion: 432.");
	REQUIRE(laft::text::format("Max (64 bits): $0.", UINT64_MAX) == "Max (64 bits): 18446744073709551615.");
	
	// Signed
	REQUIRE(laft::text::format("Check for each number: $0.", -1234567890) == "Check for each number: -1234567890.");
	REQUIRE(laft::text::format("Specific conversion: $0.", laft::text::as_text(432)) == "Specific conversion: 432.");
	REQUIRE(laft::text::format("Max (64 bits): $0.", INT64_MAX) == "Max (64 bits): 9223372036854775807.");
	REQUIRE(laft::text::format("Min (64 bits): $0.", INT64_MIN) == "Min (64 bits): -9223372036854775808.");
	
	// Char
	REQUIRE(laft::text::format("Print some characters: $0 $1 $2.", 'a', 'B', '$') == "Print some characters: a B $.");
	
	// Bool
	REQUIRE(laft::text::format("True: $0 and False: $1.", true, false) == "True: 1 and False: 0.");
	
	// String
	const std::string value{"std::string"};
	REQUIRE(laft::text::format("Format a $0.", value) == "Format a std::string.");
}
