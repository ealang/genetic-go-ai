# Experimental AI for Go

A program to train a bot and play against it.

Genetic programming is used to train a function that can score a board. To make a move, the bot scores all possible moves and selects the highest scoring move.

The program creates an initial group of randomly initialized bots, and has pluggable algorithms for scoring and evolving each generation of bots. Bots tended to overspecialize when scored against a static benchmark player, but I found scoring using peer tournaments produced more well rounded bots.

The bots can easily defeat a player that picks moves at random, but they are not human competitive. An enhancement to improve performance would be to look ahead at future moves instead of only considering the current move.

# Instructions

Run tests (Google test framework is required):

    make run_tests

Run main app to train a bot and play against it:

    make run_main

Run main app and graph ai progress:

    make run_graph

# Demo Run

An example run showing the progress of bots over a few generations. Each data point is a bot. Score is the average score of the bot playing 4 games against a random player on a 7x7 board.

![alt text](/example.png "Training progress")
