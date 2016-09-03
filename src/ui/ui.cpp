#include <iostream>
#include "aimatch.h"
#include "board.h"
#include "gpnode.h"
#include "rlutil.h"

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
