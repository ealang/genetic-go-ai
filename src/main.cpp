#include <cstring>
#include <iostream>
#include "aimatch.h"
#include "board.h"
#include "evolution_algs.h"
#include "generate_ai.h"
#include "gpnode.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "rlutil.h"

using namespace std;

void printFancyBoard(const Board& board) {
    int size = board.size();
    auto printBorderCell = [size](int i) {
        rlutil::setBackgroundColor(rlutil::GREY);
        rlutil::setColor(rlutil::DARKGREY);
        if (i >= 1 && i <= size) 
            cout << i % 10;
        else
            cout << " ";
    };
    auto printTBBorder = [&]() {
        for (int i = 0; i < size + 2; i++) {
            printBorderCell(i);
        }
    };
    auto printStone = [](int x, int y, Color stone) {
        static int colors[] = {rlutil::BLACK, rlutil::BLACK, rlutil::WHITE};
        int bgColor = ((y + x) % 2) ? rlutil::BLUE : rlutil::CYAN;
        rlutil::setBackgroundColor(bgColor);
        rlutil::setColor(colors[stone]);
        cout << (stone == NONE ? ' ' : 'o');
    };

    rlutil::saveDefaultColor();

    printTBBorder();
    cout << endl;
    for (int y = 0; y < size; y++) {
        printBorderCell(y + 1);
        for (int x = 0; x < size; x++) {
            printStone(x, y, board.get(x, y));
        }
        printBorderCell(y + 1);
        cout << endl;
    }
    printTBBorder();
    cout << endl;

    rlutil::resetColor();
}

void playInteractiveVsAI(int boardSize, const GPNode* ai) {
    while (true) {
        Color turn = WHITE;
        Board b(boardSize);
        int nturns = (int)(boardSize * boardSize * 0.8);
        for (int i = 0; i < nturns; i++) {
            cout << "move " << i + 1 << " of " << nturns << endl;
            int x, y;
            if (turn == WHITE) {
                auto move = getBestMove(b, *ai, turn);
                cout << "ai move: " << move.x + 1 << " " << move.y + 1 << endl;
                x = move.x;
                y = move.y;
            } else {
                cout << "your move: ";
                cin >> x;
                cin >> y;
                --x;
                --y;
            }
            try {
                b.set(x, y, turn);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            } catch (const KoRuleViolated&) {
                cout << "Move not allowed" << endl;
            }

            printFancyBoard(b);
            turn = turn == BLACK ? WHITE : BLACK;
        }
        cout << "ai score: " << b.score(WHITE) << endl <<
                "your score: " << b.score(BLACK) << endl;
    }
}

void logTrainingData(const TrainingData& data) {
    cout << "training_data:"
         << " gen=" << data.generationNum
         << " ai=" << data.aiNum << " (size=" << treeNodeCount(data.ai) << " depth=" << treeDepth(data.ai) << ")"
         << " ai score=" << data.aiScore
         << " benchmark score=" << data.benchmarkScore << endl;
}

GPNode* trainAI(int boardSize) {
    RandomNode benchmarkAI(0, 10000);

    TrainingOptions options;
    options.populationSize = 50;
    options.numGenerations = 50;
    options.gamesPerEvaluation = 20;
    options.benchmarkAI = &benchmarkAI;
    options.createNewAI = createRandomAI;
    options.evolveNextGeneration = evolvePopulationCrossover;

    int maxTurnsPerGame = boardSize * boardSize / 2;
    return generateAI(boardSize, maxTurnsPerGame, options, logTrainingData);
}

int main(int arglen, char **argv) {
    const int boardSize = 6;
    GPNode* ai = trainAI(boardSize);

    cout << ai->toString() << endl;

    if (arglen == 2 && strcmp(argv[1], "--play") == 0)
        playInteractiveVsAI(boardSize, ai);

    cleanupTree(ai);
    return 0;
}
