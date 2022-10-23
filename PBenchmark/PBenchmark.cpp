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
#include <string>

using namespace std;

bool check_x_prime_numbers(int x) {
    bool flag=false;
    for (int num = 0; num < x; num++) {
        //SetConsoleTitleA((string("checking ").append(std::to_string(num)).c_str()));
        if (num <= 1) flag=false;
        if (num <= 3)  flag=true;

        double range = sqrt(num);
        if (num % 2 == 0 || num % 3 == 0)
            flag=false;


        for (int i = 5; i <= range; i += 6)
            if (num % i == 0 || num % (i + 2) == 0)
                flag=false;
    }
    return flag;
}

int main(int argc, char* argv[]) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    vector<std::thread> operating_threads;
    double interval = 0.0;
    int prime_numbers_to_check = 0;
    int threadstocreate = 0;
    cout << "Insert how many prime numbers to check (per thread): ";
    cin >> prime_numbers_to_check;
    cout << "How many threads: ";
    cin >> threadstocreate;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    // - start work -
    for (int i = 0; i < threadstocreate; i++) {
        operating_threads.push_back(thread(check_x_prime_numbers, prime_numbers_to_check));
    }
    for (int i = 0; i < threadstocreate; i++) {
        operating_threads[i].join();
    }
    // - end work -
    QueryPerformanceCounter(&end);
    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    cout << endl << "Time: " << interval << endl;
    ofstream benchmarks_results;
    benchmarks_results.open("benchmarks.log", ios_base::app);
    time_t now = time(0);
    string time = ctime(&now); time.erase(std::remove(time.begin(), time.end(), '\n'), time.cend());
    benchmarks_results << "[" << time << "]: Threads (" << threadstocreate << "); Prime Numbers (" << prime_numbers_to_check << "); Time (" << interval << ");" << endl;
    benchmarks_results.close();
    main(NULL, NULL);
}