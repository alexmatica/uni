#include <gmpxx.h>
#include <string.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <time.h>
#include <ratio>
#include <vector>

typedef std::chrono::high_resolution_clock Clock;

mpz_class gcd_euclid(mpz_class a, mpz_class b){
	mpz_class r;
	while (b > 0){
		r = a % b;
		a = b;
		b = r;
	}
	return a;
}

mpz_class gcd_binary(mpz_class a, mpz_class b){
	if (a == b)
		return a;
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	if (a % 2 == 0){
		if (b % 2 != 0)
			return gcd_binary(a / 2, b);
		else
			return gcd_binary(a / 2, b / 2) * 2;
	}
	if (b%2 == 0){
		return gcd_binary(a, b / 2);
	}
	if (a > b){
		return gcd_binary((a-b) / 2, b);
	}
	return gcd_binary((b-a) / 2, a);
}

mpz_class gcd_naive(mpz_class a, mpz_class b){
	while (a != b){
		if (a > b)
			a -= b;
		else
			b -= a;
	}
	return a;
}

unsigned readNumbers(char first[1000][255], char second[1000][255], const char *fileName){
	std::ifstream fin(fileName);
	std::string str;
	std::string delim = ",";
	unsigned i = 0;
	while(std::getline(fin, str))
	{
		int pos = 0;
		std::string fNum = str.substr(pos, str.find(delim));
		std::string sNum = str.substr(pos + fNum.size() + 1, fNum.size());
		strcpy(first[i], fNum.c_str());
		strcpy(second[i], sNum.c_str());
		++i;
	}
	fin.close();
	return i;
}

int main(int argc, char **argv){

	//FOR TESTING...

	// char first[1000][255], second[1000][255];
	// if (argc < 2){
	// 	std::cout << "Usage: GCD.exe <algorithm>\n";
	// 	std::cout << "\talgorithm = {euclid, binary, naive}\n";
	// 	return 1;
	// }

	// std::vector<std::string> files {"4digits.txt", "5digits.txt", "6digits.txt", "10digits.txt",
	// 								"17digits.txt", "18digits.txt", "50digits.txt", "100digits.txt",
	// 								"200digits.txt"};


	// for (std::string file: files){
	// 	unsigned lines = readNumbers(first, second, file.c_str());

	// 	if (!strcmp(argv[1], "binary") or !strcmp(argv[1], "naive") or !strcmp(argv[1], "euclid")){
	// 		auto start = Clock::now();
	// 		for(unsigned i=0; i<lines; i++){
	// 			std::string s1{first[i]};
	// 			std::string s2{second[i]};
	// 			mpz_class a, b;
	// 			a = s1;
	// 			b = s2;

	// 			if (strcmp(argv[1], "binary") == 0){
	// 				mpz_class gcd = gcd_binary(a, b);
	// 				// printf("GCD (%lld, %lld) = %lld\n", x, y, gcd);
	// 			}
	// 			else if (strcmp(argv[1], "naive") == 0){
	// 				mpz_class gcd = gcd_naive(a, b);
	// 				// printf("GCD (%lld, %lld) = %lld\n", x, y, gcd);
	// 			}
	// 			else if (strcmp(argv[1], "euclid") == 0){
	// 				mpz_class gcd = gcd_euclid(a, b);
	// 			}
	// 		}
	// 		auto end = Clock::now();
	// 		auto intMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	// 		std::cout << intMs.count() << " microseconds on " << file.c_str() << "\n";
	// 	}
	// }

	if (argc < 4){
		std::cout << "gcd2.exe {euclid, binary, naive} first_number second_number\n";
		return 0;
	}

	mpz_class x, y;
	x = std::string{argv[2]};
	y = std::string{argv[3]};
	if (!strcmp(argv[1], "naive")){
		auto start = Clock::now();
		std::cout << gcd_naive(x, y).get_str();
		auto end = Clock::now();
		auto intMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "    ("<< intMs.count() << " microseconds)\n";
	}
	else if (!strcmp(argv[1], "binary")){
		auto start = Clock::now();
		std::cout << gcd_binary(x, y).get_str();
		auto end = Clock::now();
		auto intMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "    ("<< intMs.count() << " microseconds)\n";
	}
	else if (!strcmp(argv[1], "euclid")){
		auto start = Clock::now();
		std::cout << gcd_euclid(x, y).get_str();
		auto end = Clock::now();
		auto intMs = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "    ("<< intMs.count() << " microseconds)\n";
	}
	

 	return 0;
}