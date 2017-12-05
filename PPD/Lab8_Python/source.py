from threading import Thread, Lock
from Queue import Queue


class Graph:
    def __init__(self, nr_vertices, input_file):
        self.matrix = [[0 for _ in xrange(nr_vertices)] for _ in xrange(nr_vertices)]
        self.vertices = nr_vertices
        self.read_edges(input_file)

    def read_edges(self, input_file):
        with open(input_file, 'r') as f:
            for line in f:
                tokens = line.split(' ')
                from_v, to_v = int(tokens[0]), int(tokens[1])
                self.matrix[from_v][to_v] = self.matrix[to_v][from_v] = 1

    def check_hamilton(self, v, path):
        if self.matrix[path[-1]][v] == 0:
            return False
        if v in path:
            return False
        return True

    def __str__(self):
        s = ''
        for i in range(self.vertices):
            for j in range(self.vertices):
                s += str(self.matrix[i][j])
            s += '\n'
        return s


THREADS = 4
cycles = []
queue_path = Queue()


def hamilton_worker(graph):
    global queue_path, cycles

    while True:
        try:
            current_path = queue_path.get(False)

            if len(current_path) == graph.vertices and graph.matrix[current_path[-1]][current_path[0]] == 1:
                cycles.append(current_path)
                queue_path.unfinished_tasks = 1
                queue_path.task_done()
                return

            for vert in range(graph.vertices):
                if graph.check_hamilton(vert, current_path):
                    queue_path.put(current_path + [vert])
            queue_path.task_done()
        except:
            pass


if __name__ == '__main__':
    g = Graph(5, 'g1.txt')
    # print str(g)
    lock = Lock()

    for v in range(g.vertices):
        queue_path.put([v])

    for i in range(THREADS):
        t = Thread(target=hamilton_worker, args=(g,))
        t.daemon = True
        t.start()

    queue_path.join()
    if len(cycles) == 0:
        print 'No Hamiltonian Cycles!'
    else:
        print cycles


