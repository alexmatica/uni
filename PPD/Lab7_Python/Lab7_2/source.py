from Queue import Queue
from threading import Thread
from random import randint


class BigNo:
    def __init__(self, digits=None):
        if digits:
            self.digits = digits[::-1]
        else:
            self.digits = []

    def set_digit(self, pos, new_digit):
        self.digits += [0 for _ in xrange(pos - len(self.digits) + 1)]
        self.digits[pos] = new_digit

    def __str__(self):
        return ''.join([str(dig) for dig in self.digits[::-1]])


class Worker:
    def __init__(self, tid, parent=None, children=None):
        self.tid = tid
        self.parent = parent
        self.children = []
        self.digit_queue = []
        self.done_queue = []
        if children:
            for child in children:
                self.add_child(child)

    def add_child(self, child):
        self.children.append(child)
        self.digit_queue.append(Queue())
        self.done_queue.append(False)

    def enqueue(self, child, value):
        for i in range(len(self.children)):
            if self.children[i].tid == child.tid:
                self.digit_queue[i].put(value)

    def notify_done(self, child):
        for i in range(len(self.children)):
            if self.children[i].tid == child.tid:
                self.digit_queue[i].put(-1)

    def do_work(self):
        carry = 0
        pos = 0
        done = False
        r = BigNo()

        while not done or carry > 0:
            done = True
            s = carry
            digits = []
            for i in range(len(self.digit_queue)):
                if not self.done_queue[i]:
                    d = self.digit_queue[i].get()
                    if d != -1:
                        done = False
                        digits.append(d)
                    else:
                        self.done_queue[i] = True
                    self.digit_queue[i].task_done()
            s += sum(digits)

            if not done or (done and s != 0):
                if self.parent:
                    self.parent.enqueue(self, s % 10)
                else:
                    r.set_digit(pos, s % 10)
            carry = s // 10
            pos += 1

        if self.parent:
            self.parent.notify_done(self)
        else:
            return r

    def prepare_input(self, big_no):
        for dig in big_no.digits:
            self.parent.enqueue(self, dig)
        self.parent.notify_done(self)


class WorkerTree:
    def __init__(self, n):
        output = Worker(tid=-1)

        crt_level = [Worker(tid=x) for x in xrange(0, n)]
        inp = crt_level
        nodes = []
        next_id = n

        while len(crt_level) > 1:
            new_level = [Worker(tid=next_id + x, children=crt_level[x * 2:x * 2 + 2]) for x in
                         xrange(len(crt_level) // 2)]
            for x in xrange(len(new_level)):
                crt_level[x * 2].parent = new_level[x]
                crt_level[x * 2 + 1].parent = new_level[x]
            next_id += len(new_level)
            nodes += new_level
            if len(crt_level) % 2 is 1:
                new_level.append(crt_level[-1])
            crt_level = new_level

        output.add_child(nodes[-1])
        nodes[-1].parent = output

        self.root = output
        self.branches = nodes
        self.leaves = inp

    def run(self, nrs):
        if len(nrs) != len(self.leaves):
            return

        threads = [Thread(target=node.do_work) for node in self.branches]
        for x in xrange(len(nrs)):
            self.leaves[x].prepare_input(nrs[x])

        for t in threads:
            t.start()

        for t in threads:
            t.join()

        return self.root.do_work()


if __name__ == '__main__':
    nr_size = 20
    numbers = []
    n = 10

    for i in xrange(n):
        numbers.append(BigNo([randint(1, 9) for _ in xrange(nr_size)]))

    print ' '.join([str(no) for no in numbers])
    s = sum([int(str(no)) for no in numbers])
    print s

    thread_tree = WorkerTree(n)
    s_threaded = thread_tree.run(numbers)
    print s_threaded
