#include <iostream>
#include <ctime>
#include <random>
#include <math.h>
#include <algorithm>
#include <chrono>
using namespace std;

class BF {
public:
    unsigned int* f;
    int n, nw;

    BF(int n, int type);
    BF(const char*);
    int weightF();

    BF(const BF&);
    ~BF() {
        delete[] f;
    };
    
    void output() { 
        for (int i = 0; i < nw; i++) {
            for (int j = 0; j < 32; j++) {
                cout << ((f[i] >> j) & 1);
            }
            cout << endl;
        }
    }

    BF operator = (const BF&);
    bool operator == (const BF&);
    BF Mobius();
    void ANF();
    void degree();

    int* Hadamard();
    unsigned int cor();
    
    unsigned int nextComb(unsigned int);
    int nonLinearity();
    void bestAffine();

    int* autoCor();
    int compNonLinearity();

    friend ostream& operator << (ostream&, const BF&);
    void Fict();
    void Linear();
    BF balancedF();

};

BF::BF(int nn, int type) {
    if (nn > 0) {
        n = nn;
        nw = ((unsigned int)(1 << n) + 31) >> 5;
        f = new unsigned int[nw];

        switch (type){
        case 0:
            // zero
            for (int i = 0; i < nw; i++)
                f[i] = 0;
            break;
        case 1:
            // one
            if (nn < 5) {
                f[0] = unsigned int(1 << (1 << nn)) - 1;
            }
            else {
                for (int i = 0; i < nw; i++)
                    f[i] = ~0;
            }
            break;
        case 2:
            // random
            std::random_device rd; 
            std::mt19937 gen(rd());
            std::uniform_int_distribution<unsigned int> dis(0, UINT_MAX);

            if (nn < 5) {
                f[0] = dis(gen);
                f[0] = f[0] >> unsigned int(32 - (unsigned int)(1 << nn));
            }
            else {
                for (int i = 0; i < nw; i++) {
                    f[i] = dis(gen);
                }  
            }
            
            break;
        }
    }
    else {
        puts("Error");
        exit(0);
    }
}

ostream& operator<<(ostream& out, const BF& b)
{
    for (int i = 0; i < (1<<b.n); i++)
        if ((b.f[i / 32] & (1 << (i % 32))) == 0) out << 0;
        else out << 1;

    cout << endl;
    return out;
}

BF::BF(const char* st) {
    int length = strlen(st);
    char* s = new char[length + 1];

    for (int i = 0; i < length; ++i) {
        s[i] = st[length - i - 1];
    }
    s[length] = '\0';

    int len = strlen(s);
    if (len != 0 && (len & (len - 1)) == 0) {
        for (int i = 0; i < strlen(s); i++) {
            if (s[i] != '0' && s[i] != '1') {
                cout << "String is incorrect";
                exit(0);
            }
        }


        n = log2(len);
        nw = (len - 1) / 32 + 1;
        f = new unsigned int[nw] {};
        unsigned int mask = 1;
        int index = nw - 1;

        for (int i = len - 1; i >= 0; i--, mask <<= 1) {
            if (!mask) {
                mask = 1;
                index--;
            }
            if (s[i] == '1') {
                f[index] |= mask;
            }
        }
    }
    else {
        puts("Error");
        exit(0);
    }
}

BF::BF(const BF& copy) {
    n = copy.n;
    nw = copy.nw;
    f = new unsigned int[nw];
    for (int i = 0; i < nw; i++) {
        f[i] = copy.f[i];
    }
}

BF BF::operator=(const BF& f1) {
    if (this != &f1) {
        n = f1.n;
        nw = f1.nw;
        delete[] f;
        f = new unsigned int[nw];
        for (int i = 0; i < nw; i++)
            f[i] = f1.f[i];
    }
    return *this;
}

bool BF::operator==(const BF& f1) {
    if (this != &f1) {
        if (nw != f1.nw) return false;
        else {
            for (int i = 0; i < nw; i++) {
                if (f[i] != f1.f[i]) {
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}

int BF::weightF() {
    int count = 0;

    for (int i = 0; i < nw; i++) {
        unsigned int x = f[i];
        while (x) {
            x &= (x - 1);
            count++;
        }
    }

    return count;
}

unsigned int w(unsigned int x) {
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);

    return x & 0x3F;
}

BF BF::Mobius() {
    BF g(*this);
    
    for (int i = 0; i < nw; i++) {
        g.f[i] = ((g.f[i] << 1) & 0xaaaaaaaa) ^ g.f[i];
        g.f[i] = ((g.f[i] << 2) & 0xcccccccc) ^ g.f[i];
        g.f[i] = ((g.f[i] << 4) & 0xf0f0f0f0) ^ g.f[i];
        g.f[i] = ((g.f[i] << 8) & 0xff00ff00) ^ g.f[i];
        g.f[i] = ((g.f[i] << 16) & 0xffff0000) ^ g.f[i];
    }

    if (n < 5) g.f[0] = g.f[0] & (1 << (1 << n)) - 1;

    for (int k = 1; k < nw; k = k << 1)
        for (int j = 0; j < nw; j += (k << 1))
            for (int s = j; s < k + j; s++) {
                g.f[s + k] ^= g.f[s];
            }

    return g;
}

void BF::ANF() {
    BF bf = Mobius();

    if (weightF() == 0) {
        cout << "0";
        return;
    }

    if ((bf.f[0] & (unsigned int)1) == 1) cout << "1 ";

    unsigned int mask = 2;
    unsigned int maskSet = 1;
    unsigned int currentCell = bf.f[0];

    for (unsigned int i = 1; i < (1 << n); i++, maskSet = 1, mask = mask << 1) {
        if (i % 32 == 0 ) {
            currentCell = bf.f[i >> 5];
            mask = 1;
        }

        if ((currentCell & mask) != 0) {
            for (unsigned int j = 0; j < n; j++, maskSet = maskSet << 1) {
                if ((i & maskSet) != 0) {
                    cout << "x" << j;
                }
            }
        }
        cout << " ";
    }
}

void BF::degree() {
    BF bf = Mobius();

    unsigned int mask = 2;
    unsigned int currentCell = bf.f[0];

    int count = 0; 
    int countM = 0;
    int k = 0;
    for (unsigned int i = 0; i < (1 << n); i++, mask = mask << 1) {
        if (i % 32 == 0) {
            currentCell = bf.f[i >> 5];
            mask = 1;
        }

        if ((currentCell & mask) != 0) {
            k = i;
            count = 0;
            while (k) {
                k &= (k - 1);
                count++;
            }
        }
        if (count > countM) countM = count;

    }
    cout << countM << endl;
}

int* BF::Hadamard() {
    unsigned int len = 1 << n;
    int* hmard = new int[len];

    for (unsigned int i = 0; i < len; i++) {
        if ((f[i >> 5] & (1 << (i % 32))) != 0)
            hmard[i] = -1;
        else
            hmard[i] = 1;
    }

    for (int k = 1; k < len; k = k << 1)
        for (int j = 0; j < len; j += (k << 1))
            for (int s = j; s < k + j; s++) {
                unsigned int a = hmard[s];
                unsigned int b = hmard[s + k];
                hmard[s] = a + b;
                hmard[s + k] = a - b;

            }
    
    cout << "Hadamar: ";
    for (int i = 0; i < len; i++) {
        cout << hmard[i] << " ";
    }
    

    return hmard;

}

//(a < (1 << n))
unsigned int BF::cor() {
    int* wh = Hadamard();
    for (int k = 1; k <= n; k++) {
        unsigned int a = ((1 << k) - 1) << (n - k);

        while (w(a) == k) {
            if (wh[a] == 0) {
                //cout << a << endl;
                a = nextComb(a);
            }
            else
                return k - 1;
        }
    }
    return n;
}

unsigned int BF::nextComb(unsigned int a) {
    unsigned int b = (a + 1) & a;
    unsigned int c = w((b - 1) ^ a) - 2;
    a = (((((a + 1) ^ a) << 1) + 1) << c) ^ b;
    return a;
}

int findMax(int a[], int n) {
    int max = a[0];

    for (int i = 1; i < n; ++i) {
        if (a[i] > max) {
            max = a[i];
        }
    }

    return max;
}

int BF::nonLinearity() {
    int* _f = Hadamard();
    int len = 1 << n;
    int max = 0;
    int maxValue = abs(_f[max]);
    for (int i = 0; i < len; i++) {
        if (abs(_f[i]) > maxValue) {
            max = i;
            maxValue = abs(_f[i]);
        }
    }
    cout << "\nmax(_f):" << max;
    int nonLine = (1 << (n - 1)) - (max >> 1);
    return nonLine;
}

void printBA(int i, unsigned int n) {
    if (i == 0) {
        cout << "0";
        return;
    }
    unsigned int mask = 1;
    for (int j = 0; j < n; j++, mask <<= 1) {
        if ((i & mask) != 0) {
            cout << "x" << j << " + ";
        }
    }
    cout << "\n";
}

void BF::bestAffine() {
    int* _f = Hadamard();
    int len = 1 << n;
    unsigned int mask = 1;
    cout << "\nBest Affine Approx:\n";

    int max = 0;
    int maxValue = abs(_f[max]);
    for (int i = 0; i < len; i++) {
        if(abs(_f[i]) > maxValue) {
            max = i;
            maxValue = abs(_f[i]);
        }
    }

    if (_f[max] < 0)
        cout << "1 + ";

    printBA(max, n);
}

int* BF::autoCor() {
    int* wh = Hadamard();
    int len = 1 << n;
    for (int i = 0; i < len; i++) {
        wh[i] *= wh[i];
    }

    for (int k = 1; k < len; k = k << 1)
        for (int j = 0; j < len; j += (k << 1))
            for (int s = j; s < k + j; s++) {
                unsigned int a = wh[s];
                unsigned int b = wh[s + k];
                wh[s] = a + b;
                wh[s + k] = a - b;
            }

    for (int i = 0; i < len; i++) {
        wh[i] = wh[i] >> n;
    }

    return wh;
}

int BF::compNonLinearity() {
    int* ac = autoCor();
    int len = 1 << n;
    int max = 1;
    int maxValue = abs(ac[max]);
    for (int i = 2; i < len; i++) {
        if (abs(ac[i]) > maxValue) {
            max = i;
            maxValue = abs(ac[i]);
        }
    }

    return (1 << (n - 2)) - (maxValue >> 2);
}

void test() {
    for (int i = 2; i < 10; i++) {
        BF bf(i, 2);
        double res = (double)bf.weightF() / static_cast<unsigned int>(1 << i);
        cout << i << ": " << res << endl;
    }
}

void test2() {
    cout << "ANF 0-func: ";
    BF bf(6, 0);
    bf.ANF();
    cout << "\nANF 1-func: ";
    bf = BF(6, 1);
    bf.ANF();
    for (int i = 3; i < 30; i++) {
        BF a(i, 2);
        BF b = a.Mobius();
        BF c = b.Mobius();
        if (a == c) {
            cout << "\nCorrect";
        }
        else {
            cout << "\nError";
            return;
        }
    }
}

void test3() {
    BF bf("00000000");
    cout << "\nBF: " << bf;
    cout << "\nMobius: " << bf.Mobius();
    cout << "\nANF: ";
    bf.ANF();
    bf.Fict();
    bf.Linear();
    cout << "\ndegree: ";
    bf.degree();

    int* wh = bf.Hadamard();
    cout << "Hadamar: ";
    for (int i = 0; i < 1<<bf.n; i++) {
        cout << wh[i] << " ";
    }

    cout << "\nCor:" << bf.cor() << endl;
    cout << "\nnonLinearity: " << bf.nonLinearity() << endl;
    bf.bestAffine();
}

void BF::Fict() {
    BF bf = Mobius();

    unsigned int mem = 0;
    unsigned int mask = 1;
    unsigned int currentCell = bf.f[0];

    for (unsigned int i = 0; i < (1 << n); i++, mask = mask << 1) {
        if ((i & 31) == 0) {
            currentCell = bf.f[i >> 5];
            mask = 1;
        }

        if ((currentCell & mask) != 0) {
            mem |= i;
        }
    }
    cout << endl << "Fict vars" << endl;
    for (int i = 0; i < n; i++) {
        if (!((mem >> i) & 1)) {
            cout << "x" << i << " ";
        }
    }
}

void BF::Linear() {
    BF bf = Mobius();

    unsigned int f1 = 0, f2 = 0;

    unsigned int mask = 1;
    unsigned int currentCell = bf.f[0];

    for (unsigned int i = 0; i < (1 << n); i++, mask = mask << 1) {
        if ((i & 31) == 0) {
            currentCell = bf.f[i >> 5];
            mask = 1;
        }

        if ((currentCell & mask) != 0) {
            if (w(i) == 1) {
                f1 |= i;
            }
            else {
                f2 |= i;
            }
        }
    }
    f1 = f1 & (~f2);
    cout << endl << "Linear vars" << endl;
    for (int i = 0; i < n; i++) {
        if (((f1 >> i) & 1)) {
            cout << "x" << i << " ";
        }
    }
}

BF BF::balancedF() {
    BF bf(*this);
    int weight = weightF();
    int balancedWeight = 1 << (n - 1);

    int difs = abs(balancedWeight - weight);

    if (weight > balancedWeight) {
        for (int i = 0, j = 0; i < difs; i++) {
            while (bf.f[j] == 0)
                j++;
            bf.f[j] &= (bf.f[j] - 1);
        }
    } else {
        for (int i = 0, j = 0; i < difs; i++) {
            while (bf.f[j] == ~0)
                j++;
            bf.f[j] |= (bf.f[j] + 1);
        }
    }

    return bf;
}


int main() {
    BF bf("0001");
    cout << bf.Mobius();
    //BF bf1 = bf.balancedF();
    //cout << bf1;
    /*
    int* wh = bf.autoCor();
    cout << "Autocorrelation: ";
    for (int i = 0; i < 1 << bf.n; i++) {
        cout << wh[i] << " ";
    }
    cout << endl << "Complete nonLinearity: " << bf.compNonLinearity() << endl;
    */
    cout << endl << "Complete nonLinearity: " << bf.compNonLinearity() << endl;
    bf.ANF();
    //bf.Fict();
    bf.Linear();
    //test3();
    
    //BF bf("10011111");
    cout << "\nBF: " << bf;

    cout << "\nCor:" << bf.cor() << endl;
    //cout << "\nnonLinearity: " << bf.nonLinearity() << endl;
    /*
    cout << endl;
    bf.bestAffine();

    auto start = std::chrono::high_resolution_clock::now();
    //int* wh = bf.Hadamard();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    //cout << endl << duration.count();
    */
    
}
