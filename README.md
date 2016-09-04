# Experimental AI for Go

A program to train a Go AI and play against it.

![alt text](/ss.png "go")

The more CPU cores the better :+1:

# Implementation

Each AI player is driven by a function that it can use to assign a score to a move. When picking a move, the AI player scores all possible moves it can make and selects the highest scoring move.

Genetic programming is used to create the function. The program starts with 100 randomly initialized functions and evolves them over 30 rounds. At the end of the run, the best function is returned to play against the user. In each round, AI vs AI games are used to pick the best functions to proceed to the next round, and random mutations are applied to the functions.

Being an experiment, the code allows pluggable algorithms for deciding how to pick the best functions and apply mutations. I found that using AI vs AI games tended to produce more competitive AIs.

While the AIs can easily defeat a player that picks moves at random, they are not human competitive. Some enhancements to improve performance would be to look ahead at future moves instead of only considering the current move, and to provide more ways for AI to read the state of the board.

# Instructions

Run tests (Google test framework is required):

    make run_tests

Run main app to train AI and play against it:

    make run_main

Graph AI progress:

	python graph_progress.py <logfile>

# Demo Run

Meet a bot:

	// Forming clusters of stones is good, scoring is good, having more liberties is good.
	PlusNode(
		3 * MaxClusterDeltaNode(),
		PlusNode(
			PlusNode(
				3 * LibertiesDeltaNode(friendly),
				2 * PlayerScoreDeltaNode()),
			3 * MaxClusterDeltaNode()))


Progress of the AIs over a few generations. Each data point is an AI. Score is the average score of the bot playing 4 games against a random player on a 7x7 board.

![alt text](/example.png "Training progress")
