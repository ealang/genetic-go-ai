class Series:
    def __init__(self):
        self._imax = {}
        self.data_x = []
        self.data_y = []

    def push(self, i, score):
        if i not in self._imax:
            self._imax[i] = score
        else:
            self._imax[i] = max(self._imax[i], score)
        self.data_x.append(i)
        self.data_y.append(score)

    @property
    def max_x(self):
        return sorted(self._imax.keys())

    @property
    def max_y(self):
        return [self._imax[i] for i in self.max_x]


def main():
    import sys
    from matplotlib import pyplot
    from itertools import cycle

    def parse_lines(line_iter):
        import re
        for line in line_iter:
            if line.startswith("training_data: "):
                i, ai_score, b_score = re.match(r'^training_data: gen=(\d+) ai=\d+ \(size=\d+ depth=\d+\) ai score=(-?\d+) benchmark score=(-?\d+)$', line).groups()
                yield (int(i), float(ai_score), float(b_score))

    def load_series(series, line_iter):
        for i, ai_score, b_score in parse_lines(line_iter):
            series.push(i, ai_score - b_score)

    def plot_series(series, color_code, label=None):
        pyplot.plot(series.data_x, series.data_y, '.' + color_code)
        pyplot.plot(series.max_x, series.max_y, color_code, linewidth=2, label=label)

    color_codes = ['b', 'r', 'g', 'm', 'c', 'k']
    if len(sys.argv) == 1:
        series = Series()
        load_series(series, sys.stdin)
        plot_series(series, color_codes[0])

    else:
        filenames = sys.argv[1:]
        for filename, color_code in zip(filenames, cycle(color_codes)):
            series = Series()
            with open(filename) as fd:
                load_series(series, fd)
            plot_series(series, color_code, filename)
        pyplot.legend()

    pyplot.xlabel('iteration')
    pyplot.ylabel('performance')
    pyplot.show()

if __name__ == '__main__':
    main()
