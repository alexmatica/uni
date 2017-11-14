import threading
from Queue import Queue
import random

workerQueue = Queue()
C = []
THREADS = 4

def generateMatrices(commonLength = 8, ALines = 8, BColumns = 8):
	columnsInA = commonLength
	linesInA = ALines
	columnsInB = BColumns
	linesInB = commonLength

	A = [[random.randint(1,9) for _ in range(columnsInA)] for _ in range(linesInA)]
	B = [[random.randint(1,9) for _ in range(columnsInB)] for _ in range(linesInB)]

	global C
	C = [[0 for _ in range(columnsInB)] for _ in range(linesInA)]

	return A, linesInA, columnsInA, B, linesInB, columnsInB

def printMatrix(M):
	for line in M:
		print line
	print '\n'

def setUpQueue(linesInA, columnsInB):
	for i in range(linesInA):
		for j in range(columnsInB):
			workerQueue.put((i,j))

def setUpIndexes(linesInA, columnsInB):
	idx = []
	for i in range(linesInA):
		for j in range(columnsInB):
			idx.append((i,j))
	return idx

def splitIndexesEqually(linesInA, columnsInB, noOfThreads):
	equalShare = (linesInA * columnsInB) // noOfThreads
	remaining = (linesInA * columnsInB) % noOfThreads
	shares = [equalShare for _ in range(noOfThreads)]
	addTo = 0
	while True:
		if remaining == 0:
			break
		if addTo >= len(shares):
			addTo = 0
		shares[addTo] += 1
		remaining -= 1
		addTo += 1
	allIndexes = setUpIndexes(linesInA, columnsInB)
	indexesShared, i, k = [], 0, 0
	for i in shares:
		crtIdx = []
		for j in range(i):
			crtIdx.append(allIndexes[k])
			k += 1
		indexesShared.append(crtIdx)
	return indexesShared



def worker(idx, A, B):
	global C
	while True:
		Li, Co = workerQueue.get()
		# print 'Thread [%d] on L=%d, C=%d' % (idx, Li, Co)
		outC = 0
		for x in range(len(A[Li])):
			outC += (A[Li][x] * B[x][Co])

		C[Li][Co] = outC
		workerQueue.task_done()


def workerEqual(idx, indexes):
	global A, B, C
	for Li, Co in indexes:
		# print 'Thread [%d] on L=%d, C=%d' % (idx, Li, Co)
		outC = 0
		for x in range(len(A[Li])):
			outC += (A[Li][x] * B[x][Co])
		C[Li][Co] = outC

if __name__ == '__main__':
	A, lA, cA, B, lB, cB = generateMatrices(250, 250, 250)
	# printMatrix(A)
	# printMatrix(B)
	# setUpQueue(lA, cB)
	idxShared = splitIndexesEqually(lA, cB, THREADS)

	# for i in range(4):
	# 	t = threading.Thread(target=worker, args=(i,A,B,))
	# 	t.setDaemon(True)
	# 	t.start()
	# workerQueue.join()

	threadids = []
	for i in range(THREADS):
		t = threading.Thread(target=workerEqual, args=(i, idxShared[i], ))
		t.start()
		threadids.append(t)

	for th in threadids:
		th.join()
	# printMatrix(C)
