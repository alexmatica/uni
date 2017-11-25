#
# 	MATICA Florian-Alexandru
# 	Group 935
#
# 	Syntax parser for a Pascal-like mini-language
#

import sys
import os
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

        # print self.fa_const.verifySequence('-0.123121231111')
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

    def updatePifAndSymTable(self, prefix):
        if prefix in self.atoms:
            self.PIF.append(Atom(self.atoms[prefix], '-', prefix))
        else:
            n = CustomNode(prefix, self.symTableAddress)
            nFind = self.symTable.findNode(n)
            if nFind is None:
                # print 'This is not a keytoken:', prefix
                self.symTableAddress += 1
                self.symTable.addNode(n)
                nFind = n
            if self.fa_id.verifySequence(prefix)[0]:
                self.PIF.append(Atom(self.atoms['ID'], nFind.code, prefix))
            else:
                self.PIF.append(Atom(self.atoms['CONST'], nFind.code, prefix))

    def startParser(self, programFile):
        isPartOfId = False
        with open(programFile, 'r') as f:
            for line in f:
                line = line.strip()
                lastPos = 0
                i = 0
                while i < len(line):
                    # print i
                    crtSeq = line[lastPos:i+1]
                    # print 'checking sequence', repr(crtSeq)
                    status, prefix = self.fa_id.verifySequence(crtSeq)
                    if not status and prefix == '':
                        status2, prefix2 = self.fa_const.verifySequence(crtSeq)
                        if status2 or prefix2 != '':
                            status, prefix = status2, prefix2
                    # print 'Crt seq:', repr(crtSeq), status, repr(prefix)
                    if not status:

                        if i < len(line) - 1 and line[i:i+2] in {'<=', '>=', '==', '<>'}:
                            if prefix != '':
                                self.updatePifAndSymTable(prefix)
                                isPartOfId = False
                            self.updatePifAndSymTable(line[i:i+2])
                            i += 2
                        elif line[i] in {' ', ';', ',', '(', ')', '<', '>', '_', '.'}:
                            if line[i] != '.':
                                if prefix != '':
                                    self.updatePifAndSymTable(prefix)
                                    isPartOfId = False

                                if isPartOfId is True and line[i] != ' ':
                                    print 'Syntax error at line [%d] - [%s]!' % (self.lineCount, line[i])
                                    sys.exit(0)

                                if line[i] not in {' ', '_'}:
                                    self.updatePifAndSymTable(line[i])
                                    if line[i] == '_':
                                        isPartOfId = True
                            else:
                                isPartOfId = True
                            i += 1
                        elif line[i] == ':':
                            if prefix != '':
                                self.updatePifAndSymTable(prefix)
                                isPartOfId = False
                            if i < len(line) - 1 and line[i+1] == '=':
                                self.updatePifAndSymTable(':=')
                                i += 2
                            else:
                                self.updatePifAndSymTable(':')
                                i += 1
                        elif line[i] in {'+', '-', '/', '*'}:
                            if prefix != '':
                                self.updatePifAndSymTable(prefix)
                                isPartOfId = False
                            if line[i] in {'+', '-'}:
                                if not self.fa_id.verifySequence(line[i+1])[0] and not self.fa_const.verifySequence(line[i+1]):
                                    self.updatePifAndSymTable(line[i])
                                else:
                                    isPartOfId = True
                            i += 1
                        elif line[i] in {'$', '%', '&'}:
                            isPartOfId = True
                            i += 1
                        else:
                            print 'Syntax error at line %d! Unexpected token [%s].' % (self.lineCount, line[i])
                            sys.exit(0)

                        if not isPartOfId:
                            lastPos = i
                    else:
                        i += 1
                    if i == len(line) and (status or prefix != ''):
                        self.updatePifAndSymTable(prefix)
                self.lineCount += 1

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
