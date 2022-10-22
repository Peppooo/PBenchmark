#include "tchar.h"
#include <iostream>
#include <thread>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>

using namespace std;

bool check_prime_number(int n) {
    int i, m = 0, flag = 0;
    m = n / 2;
    for (i = 2; i <= m; i++)
    {
        if (n % i == 0)
        {
            flag = 1;
            break;
        }
    }
    return flag;
}

int main(int argc, char* argv[]) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    vector<std::thread> operating_threads;
    double interval = 0.0;
    int w = 0;
    int n = 0;
    cout << "Insert how many prime numbers to check: ";
    cin >> w;
    cout << "How many threads: ";
    cin >> n;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    for (int x = 0; x < n; x++) {
        //cout << "calculating " << w/n << " prime numbers at thread " << x << endl;
        for (int i = 0; i < w/n; i++) {
            operating_threads.push_back(thread(check_prime_number, i));
        }
    }


    QueryPerformanceCounter(&end);
    for (int y = 0; y < n; y++) {
        operating_threads[y].join();
    }
    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    cout << endl << "Time: " << interval << endl;
    ofstream benchmarks_results;
    benchmarks_results.open("benchmarks.log", ios_base::app);
    time_t now = time(0);
    string time = ctime(&now); time.erase(std::remove(time.begin(), time.end(), '\n'), time.cend());
    benchmarks_results << "[" << time << "]: Threads (" << n << "); Prime Numbers (" << w << "); Time (" << interval << ");" << endl;
    benchmarks_results.close();
    main(NULL, NULL);
}