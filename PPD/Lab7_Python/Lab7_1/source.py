from threading import Thread
import random

dummy_tree = []
totalAdditions = 0


class Info:
    def __init__(self, sum, fl):
        self.sum = sum
        self.fl = fl

    def __str__(self):
        return str(self.sum)


def bottoms_up_worker(data, level, tid):
    s = sum([v.sum for v in data])
    i = Info(s, 0)
    dummy_tree[level][tid] = i


def top_down_worker(level, tid):
    # left child inherits from-left of parent
    dummy_tree[level+1][2*tid].fl = dummy_tree[level][tid].fl

    # right child from-left = sum + fl of left sibling
    dummy_tree[level+1][2*tid+1].fl = dummy_tree[level+1][2*tid].sum + dummy_tree[level+1][2*tid].fl


def prefix_sum(depth):
    global dummy_tree
    global totalAdditions

    size = 2 ** depth
    arr = [random.randint(0,9) for _ in xrange(size)]
    print arr
    # print arr

    # on bottoms-up step, leaves level is counted as 0
    dummy_tree = [[Info(arr[i], 0) for i in range(size)]]
    start_width = size
    level = 1
    while start_width != 1:
        threads = []
        next_size = start_width // 2
        dummy_tree.append([Info(-1, 0) for _ in xrange(next_size)])
        totalAdditions += next_size
        for i in range(0, start_width, 2):
            t = Thread(target=bottoms_up_worker, args=(dummy_tree[level-1][i:i+2], level, i/2))
            threads.append(t)

        for t in threads:
            t.start()
        for t in threads:
            t.join()

        start_width = start_width // 2
        level += 1

    dummy_tree = dummy_tree[::-1]
    # for level in dummy_tree:
    #     for x in level:
    #         print x,
    #     print ''

    level = 0
    while start_width != size:
        threads = []
        totalAdditions += start_width
        for i in range(0, start_width):
            t = Thread(target=top_down_worker, args=(level, i))
            threads.append(t)

        for t in threads:
            t.start()
        for t in threads:
            t.join()

        start_width *= 2
        level += 1
    #
    for i in range(1, len(dummy_tree[-1])):
        print dummy_tree[-1][i].fl,
    print dummy_tree[-1][-1].fl + dummy_tree[-1][-1].sum
    print 'Array size: ', size
    print 'Additions: ', totalAdditions


if __name__ == '__main__':
    prefix_sum(4)
