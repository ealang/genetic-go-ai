#include <cstring>
#include <iostream>
#include "aimatch.h"
#include "board.h"
#include "evolution_algs.h"
#include "generate_ai.h"
#include "gpnodes_experimental.h"
#include "gptree.h"
#include "rlutil.h"
#include "scoring_algs.h"

using namespace std;

void printFancyBoard(const Board& board) {
    int size = board.size();
    auto printBorderCell = [size](int i) {
        rlutil::setBackgroundColor(rlutil::BLUE);
        rlutil::setColor(rlutil::GREY);
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
    auto printStone = [](Color stone) {
        static int colors[] = {rlutil::BLACK, rlutil::BLACK, rlutil::RED};
        rlutil::setColor(colors[stone]);
        cout << (stone == NONE ? ' ' : 'O');
    };

    rlutil::saveDefaultColor();

    printTBBorder();
    cout << endl;
    for (int y = 0; y < size; y++) {
        printBorderCell(y + 1);
        rlutil::setBackgroundColor(rlutil::GREY);
        for (int x = 0; x < size; x++) {
            printStone(board.get(x, y));
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
        Color turn = BLACK;
        Board b(boardSize);
        int nturns = (int)(boardSize * boardSize * 0.8);
        for (int i = 0; i < nturns; i++) {
            cout << "move " << i + 1 << " of " << nturns << endl;
            int x, y;
            if (turn == BLACK) {
                auto move = getBestMove(b, *ai, turn);
                x = move.x + 1;
                y = move.y + 1;
                cout << "ai move: " << x << " " << y << endl;
            } else {
                cout << "your move: ";
                cin >> x;
                cin >> y;
            }
            try {
                b.set(x - 1, y - 1, turn);
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            } catch (const KoRuleViolated&) {
                cout << "Move not allowed" << endl;
            }

            printFancyBoard(b);
            turn = otherColor(turn);
        }
        cout << "ai score: " << b.score(BLACK) << endl <<
                "your score: " << b.score(WHITE) << endl;
    }
}

void logTrainingData(const TrainingData& data) {
    cout << "training_data:"
         << " gen=" << data.generationNum
         << " ai=" << data.aiNum << " (size=" << treeNodeCount(data.ai) << " depth=" << treeDepth(data.ai) << ")"
         << " score=" << data.score << endl;
}

GPNode* trainAI(int boardSize) {
    CompetitiveScore evScoring(30);

    RandomIntNode benchmarkAI(0, 10000);
    BenchmarkScore cvScoring(4, &benchmarkAI);
    
    TrainingOptions options;
    options.populationSize = 100;
    options.numGenerations = 50;
    options.createNewAI = createRandomAI;
    options.evolveFunc = evolvePopulationCrossover;
    options.evScoreFunc = [&evScoring](int boardSize, const vector<const GPNode*>& pop) {
        return evScoring(boardSize, pop);
    };
    options.cvScoreFunc = [&cvScoring](int boardSize, const vector<const GPNode*>& pop) {
        return cvScoring(boardSize, pop);
    };

    return generateAI(boardSize, options, logTrainingData);
}

int main(int arglen, char **argv) {
    const int boardSize = 7;
    GPNode* ai = trainAI(boardSize);

    cout << ai->toString() << endl;

    if (arglen == 2 && strcmp(argv[1], "--play") == 0)
        playInteractiveVsAI(boardSize, ai);

    cleanupTree(ai);
    return 0;
}
