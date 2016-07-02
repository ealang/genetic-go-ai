class ScoreTrend:
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


class NodeRank:
    def __init__(self):
        from collections import defaultdict
        self._node_count = defaultdict(int)
        self._node_score_sum = defaultdict(int)

    def push(self, program, score):
        for n in self._parse_nodes(program):
            self._node_count[n] += 1
            self._node_score_sum[n] += score

    @property
    def scores(self):
        return [{'node': node,
                 'score': self._node_score_sum[node] / float(self._node_count[node]),
                 'count': self._node_count[node]}
                for node in self._node_count.keys()]

    @staticmethod
    def _parse_nodes(program):
        import re
        return set(re.findall(r'(\w+)\(', program))


def main():
    import sys
    from matplotlib import pyplot
    from itertools import cycle

    def print_node_rank(noderank):
        for i, row in enumerate(sorted(noderank.scores, key=lambda p: p['score'], reverse=True)):
            print('{0:<3} {1:<30} {2:4} {3:10.2}'.format(i, row['node'], row['count'], row['score']))

    def ingest_log(trend, noderank, line_iter):
        def parse_lines(line_iter):
            import json
            for line in line_iter:
                if line.startswith('training_data '):
                    yield json.loads(line[line.find('{'):])
        for data in parse_lines(line_iter):
            noderank.push(data['program'], data['cv_score'])
            trend.push(data['gen'], data['cv_score'])

    def plot_trend(trend, color_code, label=None):
        pyplot.plot(trend.data_x, trend.data_y, '.' + color_code)
        pyplot.plot(trend.max_x, trend.max_y, color_code, linewidth=2, label=label)

    color_codes = ['b', 'r', 'g', 'm', 'c', 'k']
    if len(sys.argv) == 1:
        trend = ScoreTrend()
        noderank = NodeRank()
        ingest_log(trend, noderank, sys.stdin)
        print_node_rank(noderank)
        plot_trend(trend, color_codes[0])
    else:
        filenames = sys.argv[1:]
        for filename, color_code in zip(filenames, cycle(color_codes)):
            trend = ScoreTrend()
            noderank = NodeRank()
            with open(filename) as fd:
                ingest_log(trend, noderank, fd)

            print(filename)
            print_node_rank(noderank)
            plot_trend(trend, color_code, filename)
        pyplot.legend()

    pyplot.xlabel('iteration')
    pyplot.ylabel('performance')
    pyplot.show()

if __name__ == '__main__':
    main()
