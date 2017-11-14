import threading, random, os, sys
from Queue import Queue
from time import sleep

products = []
prices = {}
stock = {}
money = 0
bills = []
transactionsQueue = Queue()

def initProblem(fileName):
	if not os.path.exists(fileName) or not os.path.isfile(fileName):
		print 'Invalid input file'
		sys.exit(1)
	with open(fileName, 'r') as f:
		for line in f:
			try:
				product, price, stck = line.split(' ')
				products.append(product)
				prices[product] = int(price)
				stock[product] = int(stck)
			except:
				'Invalid line format. It should be PRODUCT_NAME (str), PRODUCT_PRICE (int), STOCK (int)'

def generateTransactions(nrOfTrans = 1000):
	maxQuantityPerProd = {}
	for p in stock:
		maxQuantityPerProd[p] = stock[p] // nrOfTrans

	for i in range(nrOfTrans):
		crtTran = {}
		noOfProducts = random.randint(1, len(products))
		productsSample = random.sample(products, noOfProducts)
		for p in productsSample:
			crtTran[p] = random.randint(1, maxQuantityPerProd[p])
		transactionsQueue.put(crtTran)

def printBill(billDict):
	count = 1
	for prod in billDict:
		if prod != 'TOTAL':
			print '\t%2d. %-16s $%-4d\t x%-4d\t = $%5d' % (count, prod, prices[prod], billDict[prod], prices[prod] * billDict[prod])
			count += 1
	print '\t\t\t\t\t\tTOTAL = $%5d\n' % billDict['TOTAL']

def worker(idx):
	'''
		The market lock protects individual products, at first
		It then protects adding the income to the total money as well as creating the bill for the sold products
		The same lock protects the whole checking process
	'''
	global stock, money
	while True:
		print 'Worker [%d] started transaction...\n' % idx
		crtTran = transactionsQueue.get()
		crtTotal = 0
		
		for prd in crtTran:
			with marketLock:
				stock[prd] -= crtTran[prd]
				crtTotal += (crtTran[prd] * prices[prd])

		with marketLock:		
			money += crtTotal	
			crtTran['TOTAL'] = crtTotal
			bills.append(crtTran)
		transactionsQueue.task_done()

def checker():
	while True:
		with marketLock:
			print 'Checker started...'
			issues = False
			billsTotal = sum([b['TOTAL'] for b in bills])
			if billsTotal != money:
				print 'Current money differ from the total amount of the bills! [%d - %d]' % (money, billsTotal)

		# sleep(random.random())
		if transactionsQueue.empty() == True:
			break

if __name__ == '__main__':

	'''
		Run statistics (Intel i7 6600U, 16GB RAM DDR4)
			4 threads (prints included, checker sleep excluded)
				1. 5 transactions - 0.1 seconds
				2. 500 transactions - 0.1 seconds
				3. 1000 transactions - 0.2 seconds
				4. 2000 transactions - 0.4 seconds
				5. 10000 transactions - 4.1 seconds


	'''

	initProblem('supermarket.txt')
	backupStock = {}
	for prod in stock:
		backupStock[prod] = stock[prod]
	generateTransactions(10000)
	
	marketLock = threading.Lock()
	for i in range(4):
		t = threading.Thread(target=worker, args=(i+1,))
		t.setDaemon(True)
		t.start()

	c = threading.Thread(target=checker)
	c.setDaemon(True)
	c.start()

	transactionsQueue.join()

	for b in bills:
		printBill(b)

	print '---- END CHECK ----'
	checker()
	print 'Total income: $ %-8d' % money

	print 'Remaining stock...'
	for prod in stock:
		print '\t%s: %-8d pieces' % (prod, stock[prod])


