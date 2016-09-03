#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include "bitset2d.h"
#include "board.h"
#include "board_intel.h"
#include "board_iter.h"
#include "gpnode_context.h"
#include "gpnodes_experimental.h"
#include "ts_rand.h"

using namespace std;

GPNode* createRandomNode(GPType, int curDepth, int maxDepth);
GPNode* createRandomTree(int depth) {
    return createRandomNode(INT, 1, depth);
}

/* IntIfNode */

IntIfNode::IntIfNode(GPNode* i, GPNode* t, GPNode* e): GPOperatorNode({i, t, e}) { }

int IntIfNode::getImpl(const Context& c) const { 
    if (inputs[0]->get(c)) {
        return inputs[1]->get(c);
    } else {
        return inputs[2]->get(c); }
}

GPNode* IntIfNode::cloneImpl() const {
    return new IntIfNode(inputs[0]->clone(), inputs[1]->clone(), inputs[2]->clone());
}

string IntIfNode::toStringImpl() const {
    stringstream ss;
    ss << "IntIfNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << "," << inputs[2]->toString() << ")";
    return ss.str();
}

/* IntEqualsNode */

IntEqualsNode::IntEqualsNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int IntEqualsNode::getImpl(const Context& c) const { return inputs[0]->get(c) == inputs[1]->get(c); }

GPNode* IntEqualsNode::cloneImpl() const {
    return new IntEqualsNode(inputs[0]->clone(), inputs[1]->clone());
}

string IntEqualsNode::toStringImpl() const {
    stringstream ss;
    ss << "IntEqualsNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* BoolEqualsNode */

BoolEqualsNode::BoolEqualsNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int BoolEqualsNode::getImpl(const Context& c) const { return inputs[0]->get(c) == inputs[1]->get(c); }

GPNode* BoolEqualsNode::cloneImpl() const {
    return new BoolEqualsNode(inputs[0]->clone(), inputs[1]->clone());
}

string BoolEqualsNode::toStringImpl() const {
    stringstream ss;
    ss << "BoolEqualsNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* LessThanNode */

LessThanNode::LessThanNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int LessThanNode::getImpl(const Context& c) const { return inputs[0]->get(c) < inputs[1]->get(c); }

GPNode* LessThanNode::cloneImpl() const {
    return new LessThanNode(inputs[0]->clone(), inputs[1]->clone());
}

string LessThanNode::toStringImpl() const {
    stringstream ss;
    ss << "LessThanNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* AndNode */

AndNode::AndNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int AndNode::getImpl(const Context& c) const { return inputs[0]->get(c) && inputs[1]->get(c); }

GPNode* AndNode::cloneImpl() const {
    return new AndNode(inputs[0]->clone(), inputs[1]->clone());
}

string AndNode::toStringImpl() const {
    stringstream ss;
    ss << "AndNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* NORNode */

NORNode::NORNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int NORNode::getImpl(const Context& c) const { 
    int v1 = inputs[0]->get(c);
    int v2 = inputs[1]->get(c);
    return (v1 && !v2) || (!v1 && v2);
}

GPNode* NORNode::cloneImpl() const {
    return new NORNode(inputs[0]->clone(), inputs[1]->clone());
}

string NORNode::toStringImpl() const {
    stringstream ss;
    ss << "NORNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* OrNode */

OrNode::OrNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int OrNode::getImpl(const Context& c) const { return inputs[0]->get(c) || inputs[1]->get(c); }

GPNode* OrNode::cloneImpl() const {
    return new OrNode(inputs[0]->clone(), inputs[1]->clone());
}

string OrNode::toStringImpl() const {
    stringstream ss;
    ss << "OrNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}


/* PlusNode */

PlusNode::PlusNode(GPNode* l, GPNode* r): GPOperatorNode({l, r}) { }

int PlusNode::getImpl(const Context& c) const { return inputs[0]->get(c) + inputs[1]->get(c); }

GPNode* PlusNode::cloneImpl() const {
    return new PlusNode(inputs[0]->clone(), inputs[1]->clone());
}

string PlusNode::toStringImpl() const {
    stringstream ss;
    ss << "PlusNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* MultiplyNode */

MultiplyNode::MultiplyNode(GPNode* l, GPNode* r) : GPOperatorNode({l, r}) { }

int MultiplyNode::getImpl(const Context& c) const { return inputs[0]->get(c) * inputs[1]->get(c); }

GPNode* MultiplyNode::cloneImpl() const {
    return new MultiplyNode(inputs[0]->clone(), inputs[1]->clone());
}

string MultiplyNode::toStringImpl() const {
    stringstream ss;
    ss << "MultiplyNode(" << inputs[0]->toString() << "," << inputs[1]->toString() << ")";
    return ss.str();
}

/* BoolConstNode */

BoolConstNode::BoolConstNode(bool v): v(v) {}

int BoolConstNode::getImpl(const Context&) const { return v; }

GPNode* BoolConstNode::cloneImpl() const {
    return new BoolConstNode(v);
}

string BoolConstNode::toStringImpl() const {
    stringstream ss;
    ss << "BoolConstNode(" << v << ")";
    return ss.str();
}

/* IntConstNode */

IntConstNode::IntConstNode(int v): v(v) {}

int IntConstNode::getImpl(const Context&) const { return v; }

GPNode* IntConstNode::cloneImpl() const {
    return new IntConstNode(v);
}

string IntConstNode::toStringImpl() const {
    stringstream ss;
    ss << "IntConstNode(" << v << ")";
    return ss.str();
}

/* RandomIntNode */

RandomIntNode::RandomIntNode(int min, int max): min(min), max(max) { }

int RandomIntNode::getImpl(const Context&) const {
    return min + ts_rand() % (max - min + 1);
}

GPNode* RandomIntNode::cloneImpl() const {
    return new RandomIntNode(min, max);
}

string RandomIntNode::toStringImpl() const {
    stringstream ss;
    ss << "RandomIntNode(" << min << "," << max << ")";
    return ss.str();
}

/* NetworkStrengthDeltaNode */

int NetworkStrengthDeltaNode::getImpl(const Context& c) const {
    return stoneNetworkStrength(c.board, c.color) -
           stoneNetworkStrength(c.prevBoard, c.color);
}

GPNode* NetworkStrengthDeltaNode::cloneImpl() const {
    return new NetworkStrengthDeltaNode();
}

string NetworkStrengthDeltaNode::toStringImpl() const {
    return "NetworkStrengthDeltaNode()";
}

/* NetworkStrengthNode */

int NetworkStrengthNode::getImpl(const Context& c) const {
    return stoneNetworkStrength(c.board, c.color);
}

GPNode* NetworkStrengthNode::cloneImpl() const {
    return new NetworkStrengthNode();
}

string NetworkStrengthNode::toStringImpl() const {
    return "NetworkStrengthNode()";
}

/* PlayerScoreDeltaNode */

int PlayerScoreDeltaNode::getImpl(const Context& c) const {
    return c.board.score(c.color) - c.prevBoard.score(c.color);
}

GPNode* PlayerScoreDeltaNode::cloneImpl() const {
    return new PlayerScoreDeltaNode();
}

string PlayerScoreDeltaNode::toStringImpl() const {
    return "PlayerScoreDeltaNode()";
}

/* PlayerScoreNode */
 
int PlayerScoreNode::getImpl(const Context& c) const {
    return c.board.score(c.color);
}

GPNode* PlayerScoreNode::cloneImpl() const {
    return new PlayerScoreNode();
}

string PlayerScoreNode::toStringImpl() const {
    return "PlayerScoreNode()";
}

/* LibertiesDeltaNode */
 
LibertiesDeltaNode::LibertiesDeltaNode(bool mine): mine(mine) { }

int LibertiesDeltaNode::getImpl(const Context& c) const {
    Color color = mine ? c.color : otherColor(c.color);
    return (numLiberties(c.board, color) -
            numLiberties(c.prevBoard, color));
}

GPNode* LibertiesDeltaNode::cloneImpl() const {
    return new LibertiesDeltaNode(mine);
}

string LibertiesDeltaNode::toStringImpl() const {
    stringstream ss;
    ss << "LibertiesDeltaNode(" << (mine ? "friendly" : "opponent") << ")";
    return ss.str();
}

/* LibertiesNode */
 
LibertiesNode::LibertiesNode(bool mine): mine(mine) { }

int LibertiesNode::getImpl(const Context& c) const {
    Color color = mine ? c.color : otherColor(c.color);
    return numLiberties(c.board, color);
}

GPNode* LibertiesNode::cloneImpl() const {
    return new LibertiesNode(mine);
}

string LibertiesNode::toStringImpl() const {
    stringstream ss;
    ss << "LibertiesNode(" << (mine ? "friendly" : "opponent") << ")";
    return ss.str();
}

/* MaxClusterDeltaNode */

int MaxClusterDeltaNode::getImpl(const Context& c) const {
    return maxClusterSize(c.board, c.color) -
           maxClusterSize(c.prevBoard, c.color);
}

GPNode* MaxClusterDeltaNode::cloneImpl() const {
    return new MaxClusterDeltaNode();
}

string MaxClusterDeltaNode::toStringImpl() const {
    return "MaxClusterDeltaNode()";
}

/* CanBeCapturedNode */

int CanBeCapturedNode::getImpl(const Context& c) const {
    return canBeCaptured(c.board, c.color);
}

GPNode* CanBeCapturedNode::cloneImpl() const {
    return new CanBeCapturedNode();
}

string CanBeCapturedNode::toStringImpl() const {
    return "CanBeCapturedNode()";
}

/* IsWinningNode */

int IsWinningNode::getImpl(const Context& c) const {
    return c.board.score(c.color) > c.board.score(otherColor(c.color));
}

GPNode* IsWinningNode::cloneImpl() const {
    return new IsWinningNode();
}

string IsWinningNode::toStringImpl() const {
    return "IsWinningNode()";
}

/* CanCaptureNode */

int CanCaptureNode::getImpl(const Context& c) const {
    return canBeCaptured(c.board, otherColor(c.color));
}

GPNode* CanCaptureNode::cloneImpl() const {
    return new CanCaptureNode();
}

string CanCaptureNode::toStringImpl() const {
    return "CanCaptureNode()";
}

/* AdjacentStonesNode */

AdjacentStonesNode::AdjacentStonesNode(bool mine, bool inclDiag): mine(mine), inclDiag(inclDiag) { }

int AdjacentStonesNode::getImpl(const Context& c) const {
    Color color = mine ? c.color : otherColor(c.color);
    return numAdjacentStones(c.board, inclDiag, c.x, c.y, color);
}

GPNode* AdjacentStonesNode::cloneImpl() const {
    return new AdjacentStonesNode(mine, inclDiag);
}

string AdjacentStonesNode::toStringImpl() const {
    stringstream ss;
    ss << "AdjacentStonesNode(" << (mine ? "friendly" : "opponent") << "," << (inclDiag ? "includeDiag" : "excludeDiag" ) << ")";
    return ss.str();
}

/* StoneSpacingDeltaNode */

int StoneSpacingDeltaNode::getImpl(const Context& c) const {
    return maxStoneSpacing(c.board, c.color) -
           maxStoneSpacing(c.prevBoard, c.color);
}

GPNode* StoneSpacingDeltaNode::cloneImpl() const {
    return new StoneSpacingDeltaNode();
}

string StoneSpacingDeltaNode::toStringImpl() const {
    return "StoneSpacingDeltaNode()";
}

/* random tree */

GPNode* createRandomNode(GPType type, int curDepth, int maxDepth) {
    static const int CONST_MIN = -10, CONST_MAX = 10;
    static auto randint = [](int min, int max){ return min + ts_rand() % (max - min); };
    static auto randbool = [](){ return (bool)(ts_rand() % 2); };
    static vector<function<GPNode*(void)>> intTermFac = {
        [](){ return new IntConstNode(randint(CONST_MIN, CONST_MAX + 1)); },
        [](){ return new IntConstNode(randint(0, 2)); },
        [](){ return new RandomIntNode(CONST_MIN, CONST_MAX); },

        [](){ return new NetworkStrengthDeltaNode(); },
        [](){ return new NetworkStrengthNode(); },

        [](){ return new PlayerScoreDeltaNode(); },
        [](){ return new PlayerScoreNode(); },

        [](){ return new LibertiesDeltaNode(randbool()); },
        [](){ return new LibertiesNode(randbool()); },

        [](){ return new MaxClusterDeltaNode(); },

        [](){ return new AdjacentStonesNode(randbool(), randbool()); },
        [](){ return new StoneSpacingDeltaNode(); },
    };
    static vector<function<GPNode*(void)>> boolTermFac = {
        [](){ return new BoolConstNode(randbool()); },
        [](){ return new CanCaptureNode(); },
        [](){ return new CanBeCapturedNode(); },
        [](){ return new IsWinningNode(); },
    };
    static vector<function<GPNode*(int, int)>> boolInternalFac = {
        [](int curDepth, int maxDepth) {
            return new IntEqualsNode(createRandomNode(INT, curDepth + 1, maxDepth),
                                     createRandomNode(INT, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new BoolEqualsNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                                      createRandomNode(BOOL, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new LessThanNode(createRandomNode(INT, curDepth + 1, maxDepth),
                                    createRandomNode(INT, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new AndNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                               createRandomNode(BOOL, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new OrNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                              createRandomNode(BOOL, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new NORNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                               createRandomNode(BOOL, curDepth + 1, maxDepth));
        },
    };
    static vector<function<GPNode*(int, int)>> intInternalFac = {
        [](int curDepth, int maxDepth) {
            return new IntIfNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                                 createRandomNode(INT, curDepth + 1, maxDepth),
                                 new IntConstNode(0));
        },
        [](int curDepth, int maxDepth) {
            return new IntIfNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                                 createRandomNode(INT, curDepth + 1, maxDepth),
                                 createRandomNode(INT, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth) {
            return new PlusNode(createRandomNode(INT, curDepth + 1, maxDepth),
                                createRandomNode(INT, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth){
            return new MultiplyNode(createRandomNode(INT, curDepth + 1, maxDepth),
                                    createRandomNode(INT, curDepth + 1, maxDepth));
        },
        [](int curDepth, int maxDepth){
            return new IntIfNode(createRandomNode(BOOL, curDepth + 1, maxDepth),
                                 createRandomNode(INT, curDepth + 1, maxDepth),
                                 createRandomNode(INT, curDepth + 1, maxDepth));
        },
    };

    if (curDepth == maxDepth) {
        if (type == INT) {
            return intTermFac[randint(0, intTermFac.size())]();
        } else {
            return boolTermFac[randint(0, boolTermFac.size())]();
        }
    } else {
        if (type == INT) {
            return intInternalFac[randint(0, intInternalFac.size())](curDepth, maxDepth);
        } else {
            return boolInternalFac[randint(0, boolInternalFac.size())](curDepth, maxDepth);
        }
    }
}
