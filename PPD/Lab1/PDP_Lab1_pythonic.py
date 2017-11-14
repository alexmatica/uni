import threading, random, os, sys
from Queue import Queue

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

def generateTransactions(nrOfTrans = 20):
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
		The market lock protects one big transaction, which happens exactly like in a real store:
		-the customer places the products in a bakset
		-the products are scanned at the cashing point
		-when the last product is scanned, the bill is released
		The reason for which we choose to protect the entire transaction and not only the selling of
		individual products, it's because of the periodical checking. The checking would fail in most of the cases,
		because while the stock of a product is decreased and the price*quantity of the product is added to the total amount,
		a bill might not be released yet because there are more products to be sold in the same transaction, so the current bills
		won't match the current stock and amount of money.

		If it were just for the final checking to be performed, then only individual products would be protected (that is,
		the operations on the stock and the operations on the money earned)
	'''
	global stock, money
	while True:
		print 'Worker [%d] started transaction...\n' % idx
		crtTran = transactionsQueue.get()
		crtTotal = 0
		with marketLock:
			for prd in crtTran:
				stock[prd] -= crtTran[prd]
				money += (crtTran[prd] * prices[prd])
				crtTotal += (crtTran[prd] * prices[prd])
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
				issues = True
			productsSold = {}
			for prod in products:
				productsSold[prod] = 0
			for b in bills:
				for prod in b:
					if prod != 'TOTAL':
						productsSold[prod] += b[prod]

			for prod in stock:
				if backupStock[prod] - productsSold[prod] != stock[prod]:
					print 'Inconsistency for product "%s" - current stock [%d], sold on bills [%d]' % (prod, stock[prod], productsSold[prod])
					issues = True

		if transactionsQueue.empty() == True:
			break

if __name__ == '__main__':

	'''
		Run statistics (Intel i5 4690k, 8GB RAM DDR3)
			4 threads (prints included)
				1. 5 transactions - instant
				2. 500 transactions - 0.1 seconds
				3. 1000 transactions - 0.1 seconds
				4. 2000 transactions - 0.3 seconds
				5. 10000 transactions - 6.1 seconds
			8 threads (prints included)
				1. 5 transactions - instant
				2. 500 transactions - 0.1 seconds
				3. 1000 transactions - 0.1 seconds
				4. 2000 transactions - 0.2 seconds
				5. 10000 transactions - 3.3 seconds

		Run statistics (Intel i7 6600U, 16GB RAM DDR4)
			4 threads (prints included)
				1. 5 transactions - 0.1 seconds
				2. 500 transactions - 0.1 seconds
				3. 1000 transactions - 0.2 seconds
				4. 2000 transactions - 0.6 seconds
				5. 10000 transactions - 8.7 seconds


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

