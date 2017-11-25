#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int modInverse(int a, int m)
{
	//computes the modular multiplicative inverse of a with respect to mod m
	int m0 = m, t, q;
    int x0 = 0, x1 = 1;
 
    if (m == 1)
      return 0;
 
    while (a > 1)
    {
        q = a / m;
        t = m;
 
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
 
    if (x1 < 0)
       x1 += m0;
 
    return x1;
}

int gcd(int a, int b)
{
	while (b > 0)
	{
		int r = a%b;
		a=b;
		b=r;
	}
	return a;
}

int main(int argc, char **argv)
{
	if (argc < 4){
		std::cout << "Solves the equation ax = b (mod m)\n";
		std::cout << "Usage: LC.cpp <a> <b> <m>\n";
		return 1;
	}
	int a, b, m, d, a1, b1, m1, x0;
	int inv;

	sscanf(argv[1], "%d", &a);
	sscanf(argv[2], "%d", &b);
	sscanf(argv[3], "%d", &m);

	d = gcd(a, m);
	if (b % d != 0){
		std::cout << "No solution!\n";
		return 0;
	}

	a1 = a / d;
	b1 = b / d;
	m1 = m / d;
	inv = modInverse(a1, m1);
	x0 = (b1 * inv) % m1;
	std::cout << "General solution: x = "<< x0 << " + " << m1 << "*k, k>=0\n";

	return 0;
}