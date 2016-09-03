#ifndef UI_H
#define UI_H

class GPNode;
class Board;

void playInteractiveVsAI(int boardSize, const GPNode* ai);
void printFancyBoard(const Board& board);

#endif
