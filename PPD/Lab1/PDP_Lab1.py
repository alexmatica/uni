import threading, time, random
from Queue import Queue
from time import sleep

money = 0
bills = []
market = None

class Product:
	def __init__(self, pName, pPrice):
		self.pName = pName
		self.pPrice = pPrice

class SalesObject:
	def __init__(self, product, quantity):
		self.product = product
		self.quantity = quantity

	def __str__(self):
		return '%s - %d' % (self.product.pName, self.quantity)

class Bill:
	def __init__(self, listOfSalesObjects=[]):
		self.listOfSalesObjects = listOfSalesObjects
		self.totalPrice = self.computePrice()

	def computePrice(self):
		p = 0
		for so in self.listOfSalesObjects:
			p += so.product.pPrice * so.quantity
		return p

	def __str__(self):
		s = ''
		pCount = 1
		for so in self.listOfSalesObjects:
			s += '\t%2d. %-16s $%-4d\t x%-4d\t = $%5d\n' % (pCount, so.product.pName, so.product.pPrice, so.quantity, so.product.pPrice*so.quantity)
			pCount += 1
		s += '\n\t\t\t\t\t\t\tTOTAL = $%5d\n' % self.totalPrice
		return s

class Supermarket:
	def __init__(self, allSalesObjects):
		self.stock = allSalesObjects

	def sell(self, salesObject):
		for i in range(len(self.stock)):
			if self.stock[i].product.pName == salesObject.product.pName:
				self.stock[i].quantity -= salesObject.quantity
				return

def worker(idx):
	global market, money
	while True:
		print '[%d] - processing next sale...' % idx
		listOfSalesObjects = allSales.get()
		with marketLock:
			for so in listOfSalesObjects:
				market.sell(so)
				money += so.product.pPrice * so.quantity
			bills.append(Bill(listOfSalesObjects))
		allSales.task_done()

def checker(defaultMarket):
	while True:
		with checkLock:
			print '\n\nChecker started...\n\n'
			if allSales.empty() == True:
				break
			currentTotal = 0
			currentMarket = Supermarket
			with marketLock:
				for b in bills:
					currentTotal += b.totalPrice
					for so in b.listOfSalesObjects:
						for sobj in originalMarket.stock:
							if sobj.product.pName == so.product.pName:
								sobj.quantity -= so.quantity
								break
				if currentTotal != money:
					print 'There is some inconsistency between the sales and the bills, reflected in the total amount of money [%d - %d]!' % (
						currentTotal, money)
				for sobj in originalMarket.stock:
					for so in market.stock:
						if so.product.pName == sobj.product.pName:
							if so.quantity != sobj.quantity:
								print 'There is some inconsistency between the sales and the bills, reflected in the remaining stock [%s: %d - %s: %d]!' % (
									so.product.pName, so.quantity, sobj.product.pName, sobj.quantity)
							break

if __name__ == '__main__':
	milk = 			Product('MILK', 5)
	sugar = 		Product('SUGAR', 10)
	chocolate = 	Product('CHOCOLATE', 8)
	juice = 		Product('JUICE', 15)

	market = Supermarket(
		[SalesObject(milk, 10000),
		SalesObject(sugar, 10000),
		SalesObject(chocolate, 100000),
		SalesObject(juice, 500000)])
	defaultMarket = market 	#used for sells checking

	#generate a list of random transactions which include products from the available stock
	allSales = Queue()
	transactions = [None, SalesObject(milk, 5), SalesObject(sugar, 5), SalesObject(chocolate, 50), SalesObject(juice, 100)] #predefined transactions
	for i in range(200):
		workerTransactions = random.sample([1,2,3,4], random.randint(1,4))
		allSales.put([transactions[k] for k in workerTransactions])
	
	marketLock = threading.Lock()
	checkLock = threading.Lock()
	for i in range(4):
		t = threading.Thread(target=worker, args=(i,))
		t.setDaemon(True)
		t.start()

	check = threading.Thread(target=checker, args=(defaultMarket,))
	check.setDaemon(True)
	check.start()

	allSales.join()

	counter = 1
	for b in bills:
		print 'Bill %d' % counter
		counter += 1
		print b
	print 'TOTAL INCOME = $%5d' % money
