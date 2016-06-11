import re
import sys
from collections import defaultdict
from matplotlib import pyplot


class Series:
    def __init__(self, offset):
        self._offset = offset
        self._icount = defaultdict(int)
        self._isum = defaultdict(float)
        self.data_x = []
        self.data_y = []

    def push(self, i, score):
        self._icount[i] += 1
        self._isum[i] += score
        self.data_x.append(i + self._offset)
        self.data_y.append(score)

    @property
    def trend_x(self):
        return sorted(self._isum.keys())

    @property
    def trend_y(self):
        return [self._isum[i] / self._icount[i] for i in self.trend_x]

benchmark = Series(0.05)
generated = Series(-0.05)
for line in sys.stdin:
    if line.startswith("training_data: "):
        i, ai_score, b_score = re.match(r'^training_data: gen=(\d+) ai=\d+ \(size=\d+ depth=\d+\) ai score=(\d+) benchmark score=(\d+)$', line).groups()
        benchmark.push(int(i), float(b_score))
        generated.push(int(i), float(ai_score))

pyplot.plot(generated.data_x, generated.data_y, 'og',
            benchmark.data_x, benchmark.data_y, 'or',
            generated.trend_x, generated.trend_y, 'g',
            benchmark.trend_x, benchmark.trend_y, 'r',
            linewidth=2)
pyplot.xlabel('iteration')
pyplot.ylabel('relative score')
pyplot.legend(['genetic ai', 'benchmark ai'])
pyplot.show()
