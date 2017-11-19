import sys, os


class State:
    def __init__(self, name, is_final):
        self.name = name
        self.destinations = {}
        self.is_final = is_final

    def addDestination(self, alphabet_token, dest_name):
        if alphabet_token not in self.destinations:
            # print '[AD] New alphabet token',self.name,  alphabet_token, dest_name
            self.destinations[alphabet_token] = set([dest_name])
        else:
            # print '[AD] Old alphabet token', self.name, alphabet_token, dest_name
            self.destinations[alphabet_token].add(dest_name)

    def __str__(self):
        return ('State = %-3s, F = %-6s -- ' % (self.name, str(self.is_final))) + str(self.destinations)


class StateMachine:
    def __init__(self):
        self.initState = None
        self.states = {}

    def addState(self, state):
        self.states[state.name] = state

    def stateExists(self, state_name):
        if state_name in self.states:
            # print 'State exists:', state_name
            return True
        # print 'State does not exist:', state_name
        return False

    def setStateDetails(self, state_name, is_final=None):
        if state_name in self.states:
            if is_final is not None:
                self.states[state_name].is_final = is_final

    def addDestinationForState(self, state_name, alphabet_token, dest_name):
        if state_name in self.states:
            self.states[state_name].addDestination(alphabet_token, dest_name)

    def getDestinationsForState(self, state_name, alphabet_token):
        if state_name in self.states:
            if alphabet_token in self.states[state_name].destinations:
                return list(self.states[state_name].destinations[alphabet_token])
        return []

    def printStatesOnly(self):
        s = 'The set of states:\n\t{'
        for st in self.states:
            s += '%s, ' % st
        s = s[:-2] + '}'
        print s

    def printAllTransitions(self):
        s = 'The transitions:\n'
        for st in self.states.values():
            for alph in st.destinations:
                for dest in st.destinations[alph]:
                    s += '\t%s --(%s)--> %s\n' % (st.name, alph, dest)
        print s[:-1]

    def printFinalStates(self):
        s = 'The set of final states:\n\t{'
        for st in self.states.values():
            if st.is_final is True:
                s += '%s, ' % st.name
        s = s[:-2] + '}'
        print s

    def printStates(self):
        for s in self.states:
            print self.states[s]


class Automata:
    def __init__(self, fa_input):
        self.alphabet = set([])
        self.state_machine = StateMachine()
        self.replace = {}
        self.initStateMachine(fa_input)

    # self.mainMenu()

    def printAlphabet(self):
        s = 'The alphabet:\n\t{'
        for a in self.alphabet:
            s += '%s, ' % a
        s = s[:-2] + '}'
        print s

    def verifySequence(self, seq):
        # print 'checking sequence', seq
        if seq[0] not in self.state_machine.initState.destinations:
            return False, ''

        queue = [(0, k) for k in self.state_machine.getDestinationsForState(self.state_machine.initState.name, seq[0])]
        if self.state_machine.initState.is_final:
            prefix = seq[0]
        else:
            prefix = ''
        # print 'prefix after initial state:', prefix
        # print 'queue on initial state:', queue

        idx = 1
        while len(queue) > 0:
            prev_idx, st_name = queue.pop(0)
            if self.state_machine.states[st_name].is_final is True:
                prefix = seq[:idx]
                if prev_idx == len(seq) - 1:
                    # print 'match: ', seq
                    return True, seq
            if idx == len(seq):
                continue
            alph = seq[idx]
            temp_queue = [(idx, k) for k in
                          self.state_machine.getDestinationsForState(self.state_machine.states[st_name].name, alph)]
            # print temp_queue
            queue += temp_queue
            if len(temp_queue) > 0:
                idx += 1
        return False, prefix

    def initStateMachine(self, fa_input):
        with open(fa_input, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('I'):
                    init = line.split('=')[1].strip()
                    self.state_machine.addState(State(init, False))
                    self.state_machine.initState = State(init, False)
                elif line.startswith('F'):
                    fin = line.split('=')[1].strip()
                    all_fin = [f_state.strip() for f_state in fin.split(',')]
                    for af in all_fin:
                        if self.state_machine.stateExists(af):
                            self.state_machine.setStateDetails(af, is_final=True)
                        else:
                            self.state_machine.addState(State(af, True))
                elif line.startswith('Q'):
                    states = line.split('=')[1].strip()
                    all_states = [s.strip() for s in states.split(',')]
                    # print all_states
                    for s in all_states:
                        if not self.state_machine.stateExists(s):
                            self.state_machine.addState(State(s, False))
                elif line.startswith('A'):
                    all_alph = [a.strip() for a in line.split('=')[1].strip().split(',')]
                    for a in all_alph:
                        self.alphabet.add(a)
                elif line.startswith('T'):
                    trans = line.split('=')[1].strip()
                    all_trans = [at.strip() for at in trans.split(',')]
                    for at in all_trans:
                        tran_tokens = [tt.strip() for tt in at.split('~')]
                        # print tran_tokens
                        if len(tran_tokens) > 3:
                            print 'Invalid transition...'
                            sys.exit(0)
                        if not self.state_machine.stateExists(tran_tokens[0]) or not self.state_machine.stateExists(
                                tran_tokens[2]):
                            print 'Transition startpoint / endpoint invalid...', tran_tokens[0], tran_tokens[2], \
                            tran_tokens[1]
                            sys.exit(0)
                        tran_alph = [ta.strip() for ta in tran_tokens[1].split(';')]
                        for ta in tran_alph:
                            if ta not in self.alphabet:
                                print 'Unkown alphabet token [%s]...' % tran_tokens[1]
                                sys.exit(0)
                            self.state_machine.addDestinationForState(tran_tokens[0], ta, tran_tokens[2])
        self.state_machine.initState = self.state_machine.states[self.state_machine.initState.name]
