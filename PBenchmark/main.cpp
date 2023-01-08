#include <iostream>
#include <thread>
#include <vector>
#include <Windows.h>
#include <mutex>

using namespace std;

typedef unsigned long long int uint;

struct vprime {
	bool isprime;
	uint value;
};

int cc = 0;
int vv = 0;
bool uStats = false;
bool vStats = false;
bool eStats = false;

uint primes;
uint startp;
int wthreads;

mutex mgResults;

vector<thread> workers;
vector<vprime> presults;

bool prime(uint x) {
	if(x <= 1) return true;
	if(x <= 3)  return true;
	double range = sqrt(x);
	if(x % 2 == 0 || x % 3 == 0) return false;
	for(int i = 5; i <= range; i += 6) {
		if(x % i == 0 || x % (i + 2) == 0) return false;
	}
	return true;
}

vprime fprime(uint x) {
	vprime res;
	res.isprime = prime(x);
	res.value = x;
	return res;
}

void worker(vector<uint> x) {
	for(uint t = 0; t < x.size(); t++) {
		fprime(x[t]);
		mgResults.lock();
		cc++;
		mgResults.unlock();
	}
}

vector<vector<uint>> splitft(vector<uint> t,int bx) {
	vector<vector<uint>> ret;
	ret.resize(bx);
	for(uint i=0; i < t.size(); i++) {
		ret[i%ret.size()].push_back(t[i]);
	}
	return ret;
};

vector<uint> createFvect(uint s,uint x) {
	vector<uint> r;
	for(uint v = s; v < x+s; v++) {
		r.push_back(v);
		mgResults.lock();
		vv++;
		mgResults.unlock();
	}
	return r;
}

void initE(vector<vector<uint>> t) {
	for(uint i = 0; i < t.size(); i++) {
		workers.push_back(thread(worker, t[i]));
	}
	for(uint j = 0; j < workers.size(); j++) {
		workers[j].join();
		
	}
}

void stats() {
	while(true) {
		if(vStats) {
			if(vv > 0) {
				cout << round((100.0 / ((double)primes / vv)) * 10) / 10 << "% done\r";
				this_thread::sleep_for(chrono::milliseconds(10));
			}
		}
		if(uStats) {
			if(cc > 0) {
				cout << round((100.0 / ((double)primes / cc))*10)/10 << "% done\r";
				this_thread::sleep_for(chrono::milliseconds(10));
			}
		}
		if(eStats) {
			return;
		}
	}
}


int main() {
	cout << "starting point: ";  cin >> startp; cout;
	cout << "primes: ";  cin >> primes; cout;
	cout << "threads: "; cin >> wthreads; cout << endl;
	thread statsUp(stats);
	cout << "started vector writing (expected " << (sizeof(vector<uint>)*primes)/1000000 << "MB of ram used)\n"; vStats = true;
	vector<uint> values = createFvect(startp, primes);
	vStats = false;  cout << "finished vector writing\n";
	cout << "splitting vector for every thread\r";
	vector<vector<uint>> wvalues = splitft(values,wthreads);
	values.clear();
	cout << "finished splitting" << endl;
	LARGE_INTEGER frequency;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	cout << "benchmark started" << endl;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);
	uStats = true; vStats = true;
	initE(wvalues);
	wvalues.clear();
	uStats = false; eStats = true;
	QueryPerformanceCounter(&end);
	double timeP = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	cout << "time elapsed: " << timeP << ", " << round(primes / (timeP*1000)) << " average prime per millisecond";
	statsUp.join();
	cin >> (startp);
}