#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

struct QuotRem{
	std::vector<int> quotient;
	std::vector<int> remainder;
};

std::vector<int> readPol(const char* fName){
	std::ifstream fin;
	fin.open(fName);

	std::vector<int> pol;
	int coef;
	while (fin >> coef){
		pol.push_back(coef);
	}
	return pol;
}


int lc(std::vector<int> pol){
	return pol[pol.size() - 1];
}

int deg(std::vector<int> pol){
	return pol.size() - 1;
}

void printPoly(std::vector<int> p){
	for (int i=0; i<p.size(); i++)
		std::cout << p[i] << " ";
	std::cout<<"\n";
}

std::vector<int> add(std::vector<int> a, std::vector<int> b){
	if (a.size() >= b.size())
		for (int i=0; i<b.size(); i++)
			a[i] += b[i];
	else {
		for (int i=0; i<a.size(); i++)
			a[i] += b[i];
		for (int i = a.size(); i<b.size(); i++)
			a.push_back(b[i]);
	}
	
	return a;
}

std::vector<int> subtract(std::vector<int> a, std::vector<int> b){
	for (int i=0; i<b.size(); i++)
		a[i] -= b[i];
	return a;
}

std::vector<int> multiply(std::vector<int> a, std::vector<int> b){

	if (a.size() == 0 || b.size() == 0)
		return std::vector<int>(1,0);

	std::vector<int> res(deg(a) + deg(b) + 1, 0);
	for (int i=0; i<a.size(); i++)
		for (int j=0; j<b.size(); j++)
			res[i+j] += a[i]*b[j];
	return res;
}

std::vector<int> decreaseDegree(std::vector<int> a){
	int start = a.size() - 1;
	while(start > 0 && a[start] == 0){
		a.pop_back();
		start--;
	}
	return a;
}

QuotRem euclideanDivision(std::vector<int> a, std::vector<int> b){
	std::vector<int> q;
	std::vector<int> r = a;
	int d = deg(b);
	int c = lc(b);

	//while deg(r) >= d
	while(deg(r) >= d && deg(r) != 0){
		std::vector<int> s (r.size() - d, 0);
		s[deg(r) - d] = lc(r) / c;
		q = add(q, s);
		std::vector<int> tmp = multiply(s, b);
		r = decreaseDegree(subtract(r, tmp));
	}

	QuotRem res;
	res.quotient = q;
	res.remainder = r;
	return res;

}

void extendedGCD(std::vector<int> a, std::vector<int> b){
	std::vector<std::vector<int>> r;
	r.push_back(a);
	r.push_back(b);

	std::vector<std::vector<int>> s;
	s.push_back(std::vector<int>(1,1));
	s.push_back(std::vector<int>(1,0));
	
	std::vector<std::vector<int>> t;
	t.push_back(std::vector<int>(1, 0));
	t.push_back(std::vector<int>(1, 1));

	int i=1;
	for (i; deg(r[i]) != 0; i++){
		std::vector<int> q = euclideanDivision(r[i-1], r[i]).quotient;
		r.push_back(decreaseDegree(subtract(r[i-1], multiply(q, r[i]))));
		s.push_back(decreaseDegree(subtract(s[i-1], multiply(q, s[i]))));
		t.push_back(decreaseDegree(subtract(t[i-1], multiply(q, t[i]))));
		// break;
	}
	std::vector<int> g = r[i-1];
	printPoly(g);
	std::vector<int> u = s[i-1];
	printPoly(u);
	std::vector<int> v = t[i-1];
	printPoly(v);

}

int main(int argc, char **argv){

	
	std::vector<int> P1 = readPol("p1.in");
	std::vector<int> P2 = readPol("p2.in");
	std::cout<<P1.size() << " " <<deg(P1)<<"\n";
	std::cout<<P2.size() << " " <<deg(P2) << "\n";

	QuotRem res = euclideanDivision(P1, P2);
	std::cout<<"quotient: \n";
	printPoly(res.quotient);

	std::cout<<"\nremainder\n";
	printPoly(res.remainder);

	extendedGCD(P1, P2);

    return 0;
}