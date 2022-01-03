#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <libpreferance/preferance.hpp>


void checkScore(const RoundResult& roundResult, const ScoreUpdate& expected) {
	auto actual = updateScore(roundResult);
	REQUIRE(actual == expected);
}


TEST_CASE("All Pass", "[all-pass]") {
	checkScore(
        { PlayerBid::PASS, {DefenderBid::PASS, DefenderBid::PASS}, {2, 3, 5} },
		{{{ 4, 0, 0 }, { 6, 0, 0 }, { 10, 0, 0 }}}
	);
}


// Misere
TEST_CASE("Misere Good", "[misere][good]") {
	checkScore(
        { PlayerBid::MISERE, {DefenderBid::PASS, DefenderBid::PASS}, {0, 3, 6} },
		{ {{ 0, 10, 0}, { 0, 0, 0 }, { 0, 0 ,0 }} }
	);
}

TEST_CASE("Misere Bad", "[misere][bad]") {
	checkScore(
		{ PlayerBid::MISERE, {DefenderBid::PASS, DefenderBid::PASS}, {2, 3, 4} },
		{{{ 20, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }}}
	);
}

// Six Good
TEST_CASE("Six Good All Pass", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::PASS, DefenderBid::PASS}, {6, 2, 2} },
	    {{{ 0, 2, 0 }, { 0, 0, 0 }, { 0, 0, 0 }}}
	);
}

TEST_CASE("Six Good First Whist", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::PASS}, {6, 2, 2} },
	    {{{ 0, 2, 0 }, { 0, 0, 8 }, { 0, 0, 0 }}}
	);
}

TEST_CASE("Six Good Second Whist", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::PASS, DefenderBid::WHIST}, {6, 1, 3} },
	    {{{ 0, 2, 0 }, { 0, 0, 0 }, { 0, 0, 8 }}}
	);
}

TEST_CASE("Six Good Two Whists", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::WHIST}, {6, 3, 1} },
	    {{{ 0, 2, 0 }, { 0, 0, 6 }, { 0, 0, 2 }}}
	);
}

// Six Bad
TEST_CASE("Six Bad First Whist", "[six][play][bad]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::PASS, DefenderBid::WHIST}, {4, 2, 4} },
	    {{{ 4, 0, 0 }, { 0, 0, 4 }, { 0, 0, 16 }}}
	);
}

TEST_CASE("Six Bad Second Whist", "[six][play][bad]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::PASS}, {4, 2, 4} },
		{ {{ 4, 0, 0} , { 0, 0, 16 }, { 0, 0, 4 }} }
	);
}

TEST_CASE("Six Bad Two Whists", "[six][play][bad]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::WHIST}, {4, 4, 2} },
	    {{{ 4, 0, 0 }, { 0, 0, 12 }, { 0, 0, 8 }}}
	);
}

// Six Over
TEST_CASE("Six Over All Pass", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::PASS, DefenderBid::PASS}, {8, 0, 2} },
	    {{{ 0, 2, 0 }, { 0, 0, 0 }, { 0, 0, 0 }}}
	);
}

TEST_CASE("Six Over First Whist", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::PASS}, {8, 0, 2} },
	    {{{ 0, 2, 0 }, { 4, 0, 4 }, { 0, 0, 0 }}}
	);
}

TEST_CASE("Six Over Second Whist", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::PASS, DefenderBid::WHIST}, {8, 1, 1} },
	    {{{ 0, 2, 0 }, { 0, 0, 0 }, { 4, 0, 4 }}}
	);
}

TEST_CASE("Six Over Two Whists", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::WHIST}, {8, 1, 1} },
	    {{{ 0, 2, 0 }, { 2, 0, 2 }, { 2, 0, 2 }}}
	);
}

TEST_CASE("Six Over Two Whists 2", "[six][play][good]") {
	checkScore(
        { PlayerBid::SIX, {DefenderBid::WHIST, DefenderBid::WHIST}, {8, 2, 0} },
		{{{ 0, 2, 0 }, { 0, 0, 4 }, { 4, 0, 0 }}}
	);
}

// Seven Good
TEST_CASE("Seven Good All Pass", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::PASS}, {7, 1, 2} },
		{ {{ 0, 4, 0 }, { 0, 0, 0 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Seven Good First Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::PASS}, {7, 2, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 12 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Seven Good Second Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::WHIST}, {7, 2, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 0 }, { 0, 0, 12 }} }
	);
}

TEST_CASE("Seven Good Two Whists", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::WHIST}, {7, 2, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 8 }, { 0, 0, 4 }} }
	);
}

// Seven Bad
TEST_CASE("Seven Bad First Whist", "[seven][play][bad]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::WHIST}, {5, 2, 3} },
		{ {{ 8, 0, 0 }, { 0, 0, 8 }, { 0, 0, 28 }} }
	);
}

TEST_CASE("Seven Bad Second Whist", "[seven][play][bad]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::PASS}, {6, 2, 2} },
		{ {{ 4, 0, 0} , { 0, 0, 20 }, { 0, 0, 4 }} }
	);
}

TEST_CASE("Seven Bad Two Whists", "[seven][play][bad]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::WHIST}, {6, 3, 1} },
		{ {{ 4, 0, 0 }, { 0, 0, 16 }, { 0, 0, 8 }} }
	);
}

// Seven Over
TEST_CASE("Seven Over All Pass", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::PASS}, {9, 0, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 0 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Seven Over by 1 First Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::PASS}, {8, 0, 2} },
		{ {{ 0, 4, 0 }, { 0, 0, 8 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Seven Over by 2 First Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::PASS}, {9, 0, 1} },
		{ {{ 0, 4, 0 }, { 4, 0, 4 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Seven Over by 1 Second Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::WHIST}, {8, 1, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 0 }, { 0, 0, 8 }} }
	);
}

TEST_CASE("Seven Over by 2 Second Whist", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::PASS, DefenderBid::WHIST}, {9, 0, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 0 }, { 4, 0, 4 }} }
	);
}

TEST_CASE("Seven Over by 1 Two Whists", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::WHIST}, {8, 1, 1} },
		{ {{ 0, 4, 0 }, { 0, 0, 4 }, { 0, 0, 4 }} }
	);
}

TEST_CASE("Seven Over by 2 Two Whists", "[seven][play][good]") {
	checkScore(
		{ PlayerBid::SEVEN, {DefenderBid::WHIST, DefenderBid::WHIST}, {9, 0, 1} },
		{ {{ 0, 4, 0 }, { 4, 0, 0 }, { 0, 0, 4 }} }
	);
}

// Eight Over
TEST_CASE("Eight Over by 1 Two Whists", "[eight][play][good]") {
	checkScore(
		{ PlayerBid::EIGHT, {DefenderBid::WHIST, DefenderBid::WHIST}, {9, 1, 0} },
		{ {{ 0, 6, 0 }, { 0, 0, 6 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Eight Over by 2 Two Whists", "[eight][play][good]") {
	checkScore(
		{ PlayerBid::EIGHT, {DefenderBid::WHIST, DefenderBid::WHIST}, {10, 0, 0} },
		{ {{ 0, 6, 0 }, { 0, 0, 0 }, { 6, 0, 0 }} }
	);
}

// Nine Over
TEST_CASE("Nine Over by 1 Two Whists", "[nine][play][good]") {
	checkScore(
		{ PlayerBid::NINE, {DefenderBid::WHIST, DefenderBid::WHIST}, {10, 0, 0} },
		{ {{ 0, 8, 0 }, { 0, 0, 0 }, { 8, 0, 0 }} }
	);
}

// Ten Good
TEST_CASE("Ten Good First Whist", "[ten][play][good]") {
	checkScore(
		{ PlayerBid::TEN, {DefenderBid::WHIST, DefenderBid::PASS}, {10, 0, 0} },
		{ {{ 0, 10, 0 }, { 10, 0, 0 }, { 0, 0, 0 }} }
	);
}

TEST_CASE("Ten Good Second Whist", "[ten][play][good]") {
	checkScore(
		{ PlayerBid::TEN, {DefenderBid::PASS, DefenderBid::WHIST}, {10, 0, 0} },
		{ {{ 0, 10, 0 }, { 0, 0, 0 }, { 10, 0, 0 }} }
	);
}

TEST_CASE("Ten Good Two Whists", "[ten][play][good]") {
	checkScore(
		{ PlayerBid::TEN, {DefenderBid::WHIST, DefenderBid::WHIST}, {10, 0, 0} },
		{ {{ 0, 10, 0 }, { 0, 0, 0 }, { 10, 0, 0 }} }
	);
}

// Round Score
TEST_CASE("Round Score", "[score]") {
	REQUIRE(roundScore({ {{ 4, 0, 0 }, { 0, 0, 16 }, { 0, 0, 8 }} }, 3) == RoundScore{ {-80-32-16, 40+32, 40+16} });
	REQUIRE(roundScore({ {{ 0, 2, 0 }, { 0, 0, 6 }, { 0, 0, 2 }} }, 4) == RoundScore{ {60-18-6, 18-20, 6-20} });
}