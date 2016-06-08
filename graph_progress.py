import re
import sys
from matplotlib import pyplot

ai_iter = []
ai_score = []
rand_iter = []
rand_score = []
for line in sys.stdin:
    print line
    if line.startswith("round"):
        i, ai, rand = re.match(r'^round=(\d+) ai=\d+ \(size=\d+ depth=\d+\) ai score=(\d+) rand score=(\d+)$', line).groups()
        ai_iter.append(int(i))
        ai_score.append(int(ai))
        rand_iter.append(int(i))
        rand_score.append(int(rand))

pyplot.plot(ai_iter, ai_score, 'og', rand_iter, rand_score, 'or')
pyplot.xlabel('iteration')
pyplot.ylabel('score')
pyplot.legend(['genetic ai', 'random ai'])
pyplot.show()
