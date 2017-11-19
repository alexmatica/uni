#
# 	MATICA Florian-Alexandru
# 	Group 935
#
# 	Syntax parser for a Pascal-like mini-language
#

import sys, os
from time import gmtime, strftime
from fa import Automata

class Atom:
    def __init__(self, key, value, extra=None):
        self.key = key
        self.value = value
        self.extra = extra

    def __lt__(self, other):
        return self.key < other.key

    def __str__(self):
        return '%-10d|\t%-10s|\t%-40s' % (self.key, self.value, self.extra)


class CustomNode:
    def __init__(self, atom, code):
        self.atom = atom
        self.code = code

    def __lt__(self, other):
        return self.atom < other.atom

    def __gt__(self, other):
        return self.atom > other.atom

    def __eq__(self, other):
        if other is None:
            return False
        return self.atom == other.atom

    def __str__(self):
        if self.code is None:
            return '%s - None' % self.atom
        return '%-50s - &[%-3d]' % (self.atom, self.code)


class Node:
    def __init__(self, nodeValue):
        self.left = None
        self.right = None
        self.value = nodeValue

    def __str__(self):
        return 'Node: value = %s' % self.value


class Tree:
    def __init__(self):
        self.root = None

    def getRoot(self):
        return self.root

    def addNode(self, nodeValue):
        if self.root is None:
            self.root = Node(nodeValue)
        else:
            self.add(nodeValue, self.root)

    def add(self, nodeValue, startNode):
        if nodeValue < startNode.value:
            if startNode.left is None:
                startNode.left = Node(nodeValue)
            else:
                self.add(nodeValue, startNode.left)
        else:
            if startNode.right is None:
                startNode.right = Node(nodeValue)
            else:
                self.add(nodeValue, startNode.right)

    def findNode(self, nodeValue):
        if self.root is None:
            return None
        return self.find(nodeValue, self.root)

    def find(self, nodeValue, startNode):
        # print 'Compare: ', nodeValue, startNode.value
        if nodeValue == startNode.value:
            # print 'exiting find ', startNode.value
            return startNode.value
        elif nodeValue < startNode.value and startNode.left is not None:
            return self.find(nodeValue, startNode.left)
        elif nodeValue > startNode.value and startNode.right is not None:
            return self.find(nodeValue, startNode.right)

    def printTree(self):
        if self.root is None:
            print 'Empty tree'
        else:
            self._print(self.root)

    def _print(self, startNode):
        if startNode is not None:
            self._print(startNode.left)
            print startNode.value
            self._print(startNode.right)


class Program:
    def __init__(self, atomsFile, programFile):
        self.atoms = {}
        self.prog = ''
        self.PIF = []
        self.symTable = Tree()
        self.outCodes = {}
        self.lineCount = 1
        self.symTableAddress = 1
        self.fa_const = Automata('fa_const.txt')
        self.fa_id = Automata('fa_id.txt')

        with open(atomsFile, 'r') as f:
            for line in f:
                atom, code = line.split(' ')
                self.atoms[atom] = int(code)

        self.logName = 'parser_log_' + strftime('%Y%m%d_%H%M%S', gmtime()) + '.txt'
        if os.path.exists(self.logName):
            os.remove(self.logName)
        self.startParser(programFile)

    def Log(self, msg):
        pass

    # with open(self.logName, 'a') as f:
    # 	f.write(msg+'\n')

    def initSymTable(self, tokens):
        for t in tokens:
            t = t.strip()
            if ' ' in t:
                print 'Error at line %d. Missing comma between variables [%s]' % (self.lineCount, t)
                sys.exit(0)
            if len(t) > 255:
                print 'Error at line %d. Identifier length exceeds 255 (%d)' % (self.lineCount, len(t))
                sys.exit(0)
            if not self.fa_id.verifySequence(t)[0]:
                print 'Syntax error at line %d [%s]' % (self.lineCount, t)
                sys.exit(0)
            n = CustomNode(t, self.symTableAddress)
            self.symTableAddress += 1
            self.symTable.addNode(n)
            self.PIF.append(Atom(self.atoms['ID'], n.code, t))

    def startParser(self, programFile):
        begins = 0
        ends = 0
        ifs = 0
        elses = 0
        firstLine = True
        lastLine = False
        with open(programFile, 'r') as f:
            for line in f:
                if lastLine:
                    print 'Error at line %d! Found more code after END. keyword!' % self.lineCount
                    sys.exit(0)
                line = line.strip()
                if line == '':
                    self.lineCount += 1
                    continue
                if firstLine:
                    firstLine = False
                    if not line.upper().startswith('VAR'):
                        print 'Error at line %d. Could not find declaration of variables!' % self.lineCount
                        sys.exit(0)
                    line = line[3:].lstrip()
                    self.PIF.append(Atom(self.atoms['VAR'], '-', 'VAR'))
                    self.Log('Found starting VAR')

                if ',' in line:
                    self.Log('Found declarations line: %s' % line)
                    self.parseDeclarations(line)
                elif line.startswith('READ') or line.startswith('WRITE'):
                    self.Log('Found READ/WRITE line: %s' % line)
                    self.parseReadWrite(line)
                elif line == 'BEGIN':
                    begins += 1
                    self.PIF.append(Atom(self.atoms['BEGIN'], '-', 'BEGIN'))
                elif 'END' in line and ';' in line:
                    ends += 1
                    if ends > begins:
                        print 'Error at line %d. No BEGIN to match the current END!' % self.lineCount
                        sys.exit(0)
                    self.PIF.append(Atom(self.atoms['END'], '-', 'END'))
                    self.PIF.append(Atom(self.atoms[';'], '-', ';'))
                elif line == 'END.':
                    self.PIF.append(Atom(self.atoms['END'], '-', 'END.'))
                    lastLine = True
                elif ':=' in line:
                    self.Log('Found assignment line: %s' % line)
                    self.parseAssign(line)
                elif line.startswith('IF') and line.endswith('THEN'):
                    self.Log('Found if stmt line: %s' % line)
                    self.parseIf(line)
                    ifs += 1
                elif line == 'ELSE':
                    elses += 1
                    if elses > ifs:
                        print 'Error at line %d. No IF to match the current ELSE!' % self.lineCount
                        sys.exit(0)
                    self.PIF.append(Atom(self.atoms['ELSE'], '-', 'ELSE'))
                elif line.startswith('WHILE') and line.endswith('DO'):
                    self.parseWhile(line)
                else:
                    print 'Error at line %d. Unmatched syntax [%s].' % (self.lineCount, line)
                    sys.exit(0)
                self.lineCount += 1
        if not lastLine:
            print 'Error! Could not find END. keyword!'
            sys.exit(0)

    def parseDeclarations(self, line):
        varsAndType = line.split(':')
        if len(varsAndType) != 2:
            print 'Error at line %d [%s].' % (self.lineCount, line)
            sys.exit(0)

        variables, declType = varsAndType
        tokens = variables.split(',')
        if ';' not in declType:
            print 'Error at line %d. Missing ending ;.' % self.lineCount
            sys.exit(0)
        declType = declType.replace(';','').strip()
        self.initSymTable(tokens)
        self.PIF.append(Atom(self.atoms[declType], '-', declType))
        self.PIF.append(Atom(self.atoms[';'], '-', ';'))

    def parseReadWrite(self, line):
        if 'READ' in line:
            readOrWrite = 'READ'
        else:
            readOrWrite = 'WRITE'
        line = line.replace(readOrWrite, '').strip()
        if not line.startswith('('):
            print 'Error at line %d. Missing open bracket (.' % self.lineCount
            sys.exit(0)

        self.PIF.append(Atom(self.atoms[readOrWrite.upper()], '-', readOrWrite))
        self.PIF.append(Atom(self.atoms['('], '-', '('))

        line = line.replace('(', '').strip()
        tokens = line.split(')')
        if len(tokens) != 2 or (len(tokens) == 2 and tokens[1].strip() != ';'):
            print 'Syntax error at line %d [%s]' % (self.lineCount, line)
            sys.exit(0)

        identifier = tokens[0].strip()
        identMatch = self.fa_id.verifySequence(identifier)[0]
        numMatch = self.fa_const.verifySequence(identifier)[0]
        if numMatch:
            if readOrWrite == 'READ':
                print 'Error at line %d. Cannot read constants!' % self.lineCount
                sys.exit(0)
            n = CustomNode(identifier, self.symTableAddress)
            nFind = self.symTable.findNode(n)
            if nFind is None:
                self.symTableAddress += 1
                self.symTable.addNode(n)
            self.PIF.append(Atom(self.atoms['CONST'], n.code, identifier))
        elif identMatch:
            n = CustomNode(identifier, self.symTableAddress)
            nFind = self.symTable.findNode(n)
            if nFind is None:
                self.symTableAddress += 1
                self.symTable.addNode(n)
            self.PIF.append(Atom(self.atoms['ID'], n.code, identifier))
        else:
            print 'Syntax error at line %d [%s]' % (self.lineCount, identifier)
            sys.exit(0)
        self.PIF.append(Atom(self.atoms[')'], '-', ')'))
        self.PIF.append(Atom(self.atoms[';'], '-', ';'))

    def parseExpression(self, line, hasEnding=True):
        operators = []
        tokens = []
        crtToken = ''
        i = 0
        while i < len(line):
            if line[i] not in '+-/*' and line[i:i+3] != 'MOD':
                crtToken += line[i]
                i += 1
            elif line[i] in '+-/*':
                tokens.append(crtToken)
                crtToken = ''
                operators.append(line[i])
                i += 1
            elif line[i:i+3] == 'MOD':
                # print 'found mod on line ', line, 'at index', i
                tokens.append(crtToken)
                crtToken = ''
                operators.append('MOD')
                i += 3
        if crtToken != '':
            tokens.append(crtToken)

        tokens = [t.strip() for t in tokens]

        if hasEnding:
            if ';' not in tokens[-1]:
                print 'Missing ";" on line %d: [%s]' % (self.lineCount, line)
                sys.exit(0)
            tokens[-1] = tokens[-1].replace(';', '').strip()
        self.Log('Separated expression tokens: ' + str(tokens))
        self.Log('Operators between tokens: ' + str(operators))
        # print tokens, operators
        crtOperator = 0
        for t in tokens:
            identMatch = self.fa_id.verifySequence(t)[0]
            numMatch = self.fa_const.verifySequence(t)[0]
            if identMatch:
                if len(t) > 255:
                    print 'Error at line %d. Identifier length exceeds 255 (%d)' % (self.lineCount, len(t))
                    sys.exit(0)
                n = CustomNode(t, self.symTableAddress)
                nFind = self.symTable.findNode(n)
                if nFind is None:
                    self.symTableAddress += 1
                    self.symTable.addNode(n)
                self.PIF.append(Atom(self.atoms['ID'], n.code, t))
            elif numMatch:
                n = CustomNode(t, self.symTableAddress)
                nFind = self.symTable.findNode(n)
                if nFind is None:
                    self.symTable.addNode(n)
                    self.symTableAddress += 1
                self.PIF.append(Atom(self.atoms['CONST'], n.code, t))
            else:
                self.errSyntax(t)

            if len(operators) > 0 and crtOperator < len(operators):
                # self.Log(str(len(operators)) + ' - ' + str(crtOperator))
                self.PIF.append(Atom(self.atoms[operators[crtOperator]], '-', operators[crtOperator]))
                crtOperator += 1
        if hasEnding:
            self.PIF.append(Atom(self.atoms[';'], '-', ';'))

    def parseAssign(self, line):
        leftRight = [t.strip() for t in line.split(':=')]
        self.Log('Separated assignment parts: ' + str(leftRight))
        left, right = leftRight[0], leftRight[1]
        if not self.fa_id.verifySequence(left)[0]:
            self.errSyntax(left)
        if len(left) > 255:
            print 'Error at line %d. Identifier length exceeds 255 (%d)' % (self.lineCount, len(left))
            sys.exit(0)
        n = CustomNode(left, self.symTableAddress)
        nFind = self.symTable.findNode(n)
        if nFind is None:
            self.symTable.addNode(n)
            self.symTableAddress += 1
        self.PIF.append(Atom(self.atoms['ID'], n.code, left))
        self.PIF.append(Atom(self.atoms[':='], '-', ':='))
        self.parseExpression(right)

    def parseIf(self, line):
        self.PIF.append(Atom(self.atoms['IF'], '-', 'IF'))
        line = line.replace('IF', '').strip()
        line = line.replace('THEN', '').strip()

        crtComparator = None
        for comp in {'>', '<', '>=', '<=', '<>', '=='}:
            if comp in line:
                crtComparator = comp
                break

        if crtComparator is None:
            print 'Error at line %d. Bad expression after IF: %s!' % (self.lineCount, line)
            sys.exit(0)

        leftRight = line.split(crtComparator)
        self.Log('Separated expression parts: ' + str(leftRight))
        left, right = leftRight[0], leftRight[1]
        self.parseExpression(left, False)
        self.PIF.append(Atom(self.atoms[crtComparator], '-', crtComparator))
        self.parseExpression(right, False)
        self.PIF.append(Atom(self.atoms['THEN'], '-', 'THEN'))

    def parseWhile(self, line):
        self.PIF.append(Atom(self.atoms['WHILE'], '-', 'WHILE'))
        line = line.replace('WHILE', '').replace('DO', '').strip()

        crtComparator = None
        for comp in {'>', '<', '>=', '<=', '<>', '=='}:
            if comp in line:
                crtComparator = comp
                break

        if crtComparator is None:
            print 'Error at line %d. Bad expression after IF: %s!' % (self.lineCount, line)
            sys.exit(0)

        leftRight = line.split(crtComparator)
        self.Log('Separated expression parts: ' + str(leftRight))
        left, right = leftRight[0], leftRight[1]
        self.parseExpression(left, False)
        self.PIF.append(Atom(self.atoms[crtComparator], '-', crtComparator))
        self.parseExpression(right, False)
        self.PIF.append(Atom(self.atoms['DO'], '-', 'DO'))

    def errUndeclaredVar(self, varName):
        print 'Error at line %d. Usage of undeclared variable [%s]' % (self.lineCount, varName)
        sys.exit(0)

    def errSyntax(self, unkToken):
        print 'Syntax error at line %d. Unknown token [%s]' % (self.lineCount, unkToken)
        sys.exit(0)

    def printAtomRules(self):
        print self.atoms

    def printProgram(self):
        print self.prog

    def printPIF(self):
        print '%-10s|\t%-10s|\t%-40s' % ('CODE', 'ADDRESS', 'VALUE')
        print '%-10s|\t%-10s|\t%-40s' % ('_________', '__________', '___________')
        for at in self.PIF:
            print at

    def printSymTable(self):
        self.symTable.printTree()


if __name__ == '__main__':
    p = Program('inputs.txt', 'program2.txt')
    p.printSymTable()
    print '\n'
    p.printPIF()
