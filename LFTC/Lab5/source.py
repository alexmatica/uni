from enum import Enum
from copy import deepcopy
import sys


class Rule:
    def __init__(self, name):
        self.name = name
        self.paths = []

    def add_path(self, path):
        current_path = [token for token in path]
        self.paths.append(current_path)

    def __str__(self):
        return self.name + '-' + str(self.paths)


class CFG:
    def __init__(self):
        self.rules = []

    def init_from_file(self, file_name):
        with open(file_name, 'r') as f:
            for line in f:
                rule_name, paths = line.split(':')
                paths = paths.strip()
                all_paths = [path.strip() for path in paths.split('|')]
                idx = self.rule_exists(rule_name)
                if idx != -1:
                    for path in all_paths:
                        self.rules[idx].add_path(path)
                else:
                    self.rules.append(Rule(rule_name))
                    for path in all_paths:
                        self.rules[-1].add_path(path)

    def init_from_file_for_program(self, file_name):
        with open(file_name, 'r') as f:
            for line in f:
                rule_name, paths = line.split(':')
                paths = paths.strip()
                all_paths = [path.strip() for path in paths.split('|')]
                all_all_paths = []
                for path in all_paths:
                    by_space = [pth.strip() for pth in path.split(' ')]
                    all_all_paths.append(by_space)

                idx = self.rule_exists(rule_name)
                if idx != -1:
                    for path in all_all_paths:
                        self.rules[idx].add_path(path)
                else:
                    self.rules.append(Rule(rule_name))
                    for path in all_all_paths:
                        self.rules[-1].add_path(path)

    def rule_exists(self, rule_name):
        for i in range(len(self.rules)):
            if self.rules[i].name == rule_name:
                return i
        return -1

    def __str__(self):
        s = ''
        for rule in self.rules:
            s += str(rule) + '\n'
        return s


class State(Enum):
    normal = 0
    back = 1
    end = 2
    error = 3


class Automaton:
    def __init__(self, file_name, for_program=False):
        self.c = CFG()
        if not for_program:
            self.c.init_from_file(file_name)
        else:
            self.c.init_from_file_for_program(file_name)

        if not self.check_cfg():
            print 'There is no rule which starts with a terminal! Cannot apply RDP!'
            sys.exit(0)

        self.state = State.normal
        self.position = 1
        self.work_stack = []
        self.input_stack = []
        self.input_stack = [self.c.rules[0].name]
        print 'State: {0}\n\tPosition={1}\n\tWork stack: {2}\n\tInput stack: {3}\n\n'.format(
            self.state, self.position, self.work_stack, self.input_stack
        )

    def check_cfg(self):
        all_rules = [rule.name for rule in self.c.rules]
        for rule in self.c.rules:
            for path in rule.paths:
                if path[0] not in all_rules:
                    return True
        return False

    def parse(self, sequence):
        if len(self.input_stack) == 0:
            if self.position == len(sequence) + 1:
                self.state = State.end
                return

        if self.position > len(sequence):
            print 'TOTAL FAILURE!'
            sys.exit(1)

        if len(self.input_stack) > 0:
            idx = self.c.rule_exists(self.input_stack[0])
            if idx != -1:
                print '\t->Expand'
                self.state = State.normal
                for i in range(len(self.c.rules[idx].paths)):
                    work_stack_copy = deepcopy(self.work_stack)
                    input_stack_copy = deepcopy(self.input_stack)

                    self.work_stack.append((self.input_stack[0], i))
                    self.input_stack.pop(0)
                    self.input_stack = self.c.rules[idx].paths[i] + self.input_stack

                    if self.state != State.end:
                        print 'State: {0}\n\tPosition={1}\n\tWork stack: {2}\n\tInput stack: {3}\n\n'.format(
                            self.state, self.position, self.work_stack, self.input_stack
                        )
                    self.parse(sequence)
                    if self.state == State.end:
                        return
                    self.work_stack = deepcopy(work_stack_copy)
                    self.input_stack = deepcopy(input_stack_copy)
            else:
                if self.input_stack[0] == sequence[self.position - 1]:
                    print '\t->Advance'
                    self.state = State.normal
                    self.position += 1
                    self.work_stack.append(self.input_stack[0])
                    save = [self.input_stack[0]]
                    self.input_stack.pop(0)

                    if self.state != State.end:
                        print 'State: {0}\n\tPosition={1}\n\tWork stack: {2}\n\tInput stack: {3}\n\n'.format(
                            self.state, self.position, self.work_stack, self.input_stack
                        )
                    self.parse(sequence)
                    if self.state == State.end:
                        return
                    self.position -= 1
                    self.work_stack.pop(len(self.work_stack) - 1)
                    self.input_stack = save + self.input_stack
                else:
                    print '\t->Local failure (& go back, another try)'
                    self.state = State.back


def init_codes(file_name):
    codes = {}
    with open(file_name, 'r') as f:
        for line in f:
            tokens = line.split(':')
            codes[tokens[1].strip()] = tokens[0].strip()
    return codes


def pif_to_cfg_sequence(codes, file_name):
    sequence = []
    with open(file_name, 'r') as f:
        for line in f:
            sequence.append(codes[line.strip()])
    return sequence


if __name__ == '__main__':
    # a = Automaton('cfg.in', False)
    a = Automaton('pyte.in', True)
    seq = pif_to_cfg_sequence(init_codes('pyte_codes.in'), 'pif.in')
    # print seq
    a.parse(seq)
    if a.state == State.end:
        print 'SUCCESS!'
    else:
        print 'FAILURE!'
    # print a.c
