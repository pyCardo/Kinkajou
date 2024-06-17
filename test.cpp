#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <chrono>
#include <ctime>

#include "core.hpp"
#include "doctest.h"

TEST_CASE("Testing move generation with PERFT") {
  SUBCASE("Starting position") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board;

    CHECK(test::perft(0, board) == 1);
    CHECK(test::perft(1, board) == 20);
    CHECK(test::perft(2, board) == 400);
    CHECK(test::perft(3, board) == 8902);
    CHECK(test::perft(4, board) == 197281);
    CHECK(test::perft(5, board) == 4865609);
    CHECK(test::perft(6, board) == 119060324);    
    CHECK(test::perft(7, board) == 3195901860);    
    CHECK(test::perft(8, board) == 84998978956);

    const auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;
  }
}