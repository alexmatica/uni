#include <iostream>
#include <cmath>
#include <chrono>
#include <cstring>
#include <fstream>

typedef std::chrono::high_resolution_clock Clock;

long long evalFunc(const int *func, int degree, long long val){
    int res = 0;
    for (int i=0; i <= degree; i++) {
        auto xpow = (long long) pow(val, i);
        res += xpow * func[i];
    }
    return res;
}

long long GCD(long long a, long long b){
    long long r;
    while (b != 0){
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

bool isPrime(int x){
    if (x < 2 || x > 2 && x % 2 == 0)
        return false;

    for (int d = 3; d*d <= x; d+=2)
        if (x % d  == 0)
            return false;
    return true;
}

int Pollard(int *f, int degree, long long n, int x0, int tries, bool multipleFactors){
    int xlimit = x0 + tries;
    int limit = 1000000;
    long long initN = n;

    auto *x = new long long[limit];
    int initialX = x0;
    long long d = -1;

    std::cout << "Pollard: \n";
    auto start = Clock::now();

    while (n != 1 && initialX <= xlimit) {
        x[0] = initialX;
        int j;
        for (j = 1; j <= limit; j += 2) {

            x[j] = evalFunc(f, degree, x[j - 1]) % n;
            x[j + 1] = evalFunc(f, degree, x[j]) % n;

            d = GCD(llabs(x[(j + 1)] - x[(j + 1) / 2]), n);

            if (d == n) {
                initialX ++;
                break;
            }

            if (d != 1) {
                std::cout << "[!]Found a factor of n: " << d << "\n";
                n /= d;
                initialX = x0;
                break;
            }
        }

        if (n == 1 || !multipleFactors) {
            break;
        }
    }

    auto end = Clock::now();

    if (multipleFactors) {
        if (n == initN)
            std::cout << "FAILURE!\n";
        else if (n == 1 || (isPrime(d) && n / d == 1))
            std::cout << "Found a factor of n: " << d << "\n";
    } else{
        if (initN == n)
            std::cout << "FAILURE!\n";
    }

    std::cout << "Time: ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << " ms" << '\n';
    delete(x);
}

void Trivial(long long n, bool multipleFactors){
    int factors = 0;

    long long x = 3;

    std::cout << "\nTrivial: \n";
    auto start = Clock::now();

    while (n != 1){
        int p = 0;
        while (n % x == 0){
            n /= x;
            p++;
        }
        if (p != 0){
            std::cout << "Found a factor of n: " << x << "^" << p << "\n";
            factors ++;
            if (!multipleFactors)
                break;
        }
        x += 2;
        if (x == n && factors == 0)
            break;
    }

    auto end = Clock::now();
    if (factors == 0)
        std::cout << "Failure!\n";
    std::cout << "Time: ";
    std::cout << (std::chrono::duration_cast<std::chrono::microseconds>(end - start)).count() << " ms" << '\n';

}

bool fromKeyboard(long long &n, int &degree, int *f, int &x0, int &tries){

    std::cout << "n = ";
    std::cin >> n;
    if (n % 2 == 0){
        std::cout << "n must be odd!\n";
        return false;
    }

    std::cout << "degree of function = ";
    std::cin >> degree;
    std::cout << "function coefficients from smallest (x^0) degree to largest degree (x^deg):\n";
    for (int i=0; i <= degree; i++)
        std::cin >> f[i];

    std::cout << "x0 = ";
    std::cin >> x0;

    std::cout << "incremental tries for x0 = ";
    std::cin >> tries;

    return true;
}

bool fromFile(const char *in, long long &n, int &degree, int *f, int &x0, int &tries){
    std::ifstream fin;
    fin.open(in);

    fin>>n;
    if (n % 2 == 0){
        std::cout <<"n must be odd!\n";
        return false;
    }
    fin>>degree;
    int val;
    for (int i=0; i<=degree; i++) {
        fin>>f[i];
    }
    fin>>x0;
    fin>>tries;
    fin.close();
    return true;
}

int main(int argc, char **argv) {

    if (argc < 2){
        std::cout << "usage: ./PKC_Lab4 -file / -kb\n";
        return 1;
    }

    long long n;
    int f[100];
    int degree, x0, tries;

    if (!strcmp(argv[1], "-kb")){
        if (!fromKeyboard(n, degree, f, x0, tries))
            return 1;
    }
    else if (!strcmp(argv[1], "-file")){
        if (!fromFile("in.txt", n, degree, f, x0, tries))
            return 1;
    }

//    std::cout << n << " " << degree << " " << x0 << " " << tries << "\n";
//    for (int i=0; i <= degree; i++)
//        std::cout << f[i] << " ";
//    std::cout << "\n";

    std::cout << "Trying for one factor...\n";
    Pollard(f, degree, n, x0, tries, false);
    Trivial(n, false);

//    std::cout << "\n\n\nTrying for all factors...\n";
//    Pollard(f, degree, n, x0, tries, true);
//    Trivial(n, true);

    return 0;
}