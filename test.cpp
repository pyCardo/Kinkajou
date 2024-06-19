#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <chrono>
#include <ctime>

#include "core.hpp"
#include "doctest.h"

TEST_CASE("Testing move generation with PERFT") {
  SUBCASE("Starting position") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board;

    CHECK(test::perft(1, board) == 20);
    CHECK(test::perft(2, board) == 400);
    CHECK(test::perft(3, board) == 8902);
    CHECK(test::perft(4, board) == 197281);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    CHECK(test::perft(5, board) == 4865609);
    // CHECK(test::perft(6, board) == 119060324);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(7, board) == 3195901860);
    // CHECK(test::perft(8, board) == 84998978956);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  SUBCASE("Position 2") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board(
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

    CHECK(test::perft(1, board) == 48);
    CHECK(test::perft(2, board) == 2039);
    CHECK(test::perft(3, board) == 97862);
    CHECK(test::perft(4, board) == 4085603);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(5, board) == 193690690);
    // CHECK(test::perft(6, board) == 8031647685);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  SUBCASE("Position 3") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

    CHECK(test::perft(1, board) == 14);
    CHECK(test::perft(2, board) == 191);
    CHECK(test::perft(3, board) == 2812);
    CHECK(test::perft(4, board) == 43238);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    CHECK(test::perft(5, board) == 674624);
    // CHECK(test::perft(6, board) == 11030083);
    // end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(7, board) == 178633661);
    // CHECK(test::perft(8, board) == 3009794393);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  SUBCASE("Position 4") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board(
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq -");

    CHECK(test::perft(1, board) == 6);
    CHECK(test::perft(2, board) == 264);
    CHECK(test::perft(3, board) == 9467);
    CHECK(test::perft(4, board) == 422333);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(5, board) == 15833292);
    // CHECK(test::perft(6, board) == 706045033);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  SUBCASE("Position 5") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -");

    CHECK(test::perft(1, board) == 44);
    CHECK(test::perft(2, board) == 1486);
    CHECK(test::perft(3, board) == 62379);
    CHECK(test::perft(4, board) == 2103487);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(5, board) == 89941194);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  SUBCASE("Position 6") {
    const auto start = std::chrono::high_resolution_clock::now();
    core::Board board(
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -");

    CHECK(test::perft(1, board) == 46);
    CHECK(test::perft(2, board) == 2079);
    CHECK(test::perft(3, board) == 89890);
    CHECK(test::perft(4, board) == 3894594);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time elapsed: "
              << std::chrono::duration<double>(end - start).count() << "s"
              << std::endl;

    // CHECK(test::perft(5, board) == 164075551);
    // CHECK(test::perft(6, board) == 6923051137);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;

    // CHECK(test::perft(7, board) == 287188994746);
    // CHECK(test::perft(8, board) == 11923589843526);
    // end = std::chrono::high_resolution_clock::now();
    // std::cout << "Time elapsed: "
    //           << std::chrono::duration<double>(end - start).count() << "s"
    //           << std::endl;
  }

  // SUBCASE("Counting Nodes") {
  //   core::Board board(
  //       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");

  //   std::vector<core::Move> moves;
  //   u64 nmoves{0};
  //   for (int i{0}; i < 64; ++i) {
  //     if (board.accessBoard(i) != 0) {
  //       core::generateMoves(board, moves, i);
  //     }
  //   }

  //   for (auto move : moves) {
  //     core::Board pseudoBoard(board);
  //     pseudoBoard.makeMove(move);

  //     nmoves += test::perft(1, pseudoBoard);
  //     std::cout << move.algebraic() << ": " << test::perft(1, pseudoBoard)
  //               << std::endl;
  //   }

  //   std::cout << "Total moves: " << nmoves << std::endl;
  //   CHECK(test::perft(2, board) == 2039);

  //   moves.clear();
  //   core::Board pseudoboard(board);
  //   pseudoboard.makeMove(core::Move{28, 18});
  //   int n{0};
  //   for (int i{0}; i < 64; ++i) {
  //     if (pseudoboard.accessBoard(i) != 0) {
  //       moves.clear();
  //       core::generateMoves(pseudoboard, moves, i);
  //       for (auto move : moves) {
  //         std::cout << move.algebraic() << std::endl;
  //         ++n;
  //       }
  //     }
  //   }
  //   std::cout << n;

  //   // for (auto move : moves) {
  //   //   std::cout << move.algebraic() << std::endl;
  //   // }
  // }
}