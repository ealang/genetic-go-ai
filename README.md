# Genetic AI for Go

Genetic programming is used to train a function that can score a board. To make a move, the AI player scores all possible moves and selects the highest scoring move.

The bots can easily defeat random moving players, but are not very human competitive. An enhancement to improve performance would be to look ahead at future moves instead of only considering the current move.

The program creates an initial group of randomly initialized bots, and has pluggable algorithms for scoring and evolving each generation of bots. Bots tended to overspecialize when scored against a static benchmark player, but I found scoring using peer tournaments produced more well rounded and human competitive bots.

# Instructions

Run tests (Google test framework is required):

    make run_tests

Run main app to train a bot and play against it:

    make run_main

Run main app and graph ai progress:

    make run_graph
