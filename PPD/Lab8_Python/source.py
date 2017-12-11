from threading import Thread, Lock
from Queue import Queue
from random import randint


class Graph:
    def __init__(self, nr_vertices, input_file, is_random = False, edges = None):
        self.matrix = [[0 for _ in xrange(nr_vertices)] for _ in xrange(nr_vertices)]
        self.vertices = nr_vertices
        if not is_random:
            self.read_edges(input_file)
        else:
            self.init_edges(edges)

    def init_edges(self, edges):
        for pair in edges:
            self.matrix[pair[0]][pair[1]] = 1

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
done = False
lock = Lock()


def hamilton_worker(graph):
    global queue_path, cycles, done

    while True:
        try:
            current_path = queue_path.get(False)

            with lock:
                if len(current_path) == graph.vertices and graph.matrix[current_path[-1]][current_path[0]] == 1 \
                        and not done:
                    cycles.append(current_path)
                    done = True
                    queue_path.task_done()
                    return

            for vert in range(graph.vertices):
                if graph.check_hamilton(vert, current_path):
                    queue_path.put(current_path + [vert])
            queue_path.task_done()
        except:
            pass


def generate_graph(vertices, fill_percent):
    edges = []
    rand_threshold = 100 - fill_percent - 1
    for x in range(vertices):
        for y in range(vertices):
            if x != y:
                if randint(0, 100) > rand_threshold:
                    edges.append((x, y))
    g = Graph(vertices, '', True, edges)
    return g


if __name__ == '__main__':
    g = generate_graph(6, 50)
    # print str(g)

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


