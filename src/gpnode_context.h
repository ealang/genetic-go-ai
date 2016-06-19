#ifndef GPNODE_CONTEXT
#define GPNODE_CONTEXT

class Board;

#include "color.h"

// Context to pass to GPNode functions
struct Context {
    int x, y; // last move
    Color color; // player color
    const Board& prevBoard; // board before current move
    const Board& board; // current board state after making move
};

#endif
