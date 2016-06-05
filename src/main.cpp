#include <iostream>
#include "aimatch.h"
#include "board.h"
#include "generate_ai.h"
#include "gpnode.h"

using namespace std;

int main() {
    int boardSize = 5, numRounds = 20, numAI = 20;
    auto ai = generateAI(boardSize, numRounds, numAI);
    cout << ai->toString() << endl;

    while (true) {
        cout << "=======================================" << endl;
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
            } catch (runtime_error& e) {
                cout << e.what() << endl;
            }
            cout << b.toString();
            turn = turn == BLACK ? WHITE : BLACK;
        }
        cout << "ai territory: " << b.whiteTerritory() << endl << "your territory: " << b.blackTerritory() << endl;
    }
    return 0;
}
