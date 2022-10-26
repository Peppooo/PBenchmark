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
#include <conio.h>

using namespace std;

bool check_x_prime_numbers(int x) {
    bool flag=false;
    for (int num = 0; num < x; num++) {
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

double execute_benchmark(int threadstocreate, int prime_numbers_to_check) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    vector<thread> operating_threads;
    double interval = 0.0;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < threadstocreate; i++) {
        operating_threads.push_back(thread(check_x_prime_numbers, prime_numbers_to_check));
    }
    for (int i = 0; i < threadstocreate; i++) {
        operating_threads[i].join();
    }
    QueryPerformanceCounter(&end);
    return (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

int main(int argc, char* argv[]) {
    double lower_time = 0;
    int prime_numbers_to_check = 0;
    int threadstocreate = 0;
    int tests = 0;
    vector<double> times;
    if (argc == 1) {
        cout << "Insert how many prime numbers to check (per thread): ";
        cin >> prime_numbers_to_check;
        cout << "How many threads: ";
        cin >> threadstocreate;
        cout << "How many tests: ";
        cin >> tests;
    }
    else
    {
        for (int i = 1; i < argc; i++) {
            //cout << argv[i] << endl;
            if (string(argv[i]) == "-threads" or string(argv[i]) == "-t") {
                if (argc < i + 1) {
                    cout << "Expected argument after \"" << argv[i] << "\"";
                    exit(1);
                }
                threadstocreate = atoi(argv[i + 1]);
                i++;
            }
            else if (string(argv[i]) == "-primes" or string(argv[i]) == "-p") {
                if (argc <= i + 1) {
                    cout << "Expected argument after \"" << argv[i] << "\"";
                    exit(1);
                }
                prime_numbers_to_check = atoi(argv[i + 1]);
                i++;
            }
            else if (string(argv[i]) == "-repeat" or string(argv[i]) == "-r") {
                if (argc <= i + 1) {
                    cout << "Expected argument after \"" << argv[i] << "\"";
                    exit(1);
                }
                tests = atoi(argv[i + 1]);
                i++;
            }
            else if (string(argv[i]) == "-help" or string(argv[i]) == "-h") {
                cout << "Pbenchmark" << endl
                    << "-threads, -t\t\tinsert how many threads to work on during the benchmark." << endl
                    << "-primes,-p\t\tinsert how many prime numbers to check per thread" << endl
                    << "-repeat, -r\t\tinsert how many time the benchmark will repeat the test for better accuracy";
                exit(0);
            }
            else {
                cout << "Unknown argument \"" << argv[i] << "\", type " << argv[0] << " -help or -h for getting help";
                exit(1);
            }
        }
    }
    for (int i = 0; i < tests; i++) {
        times.push_back(execute_benchmark(threadstocreate, prime_numbers_to_check));
    }
    lower_time = *min_element(times.begin(), times.end());
    cout << lower_time << endl << endl << "Press ENTER to restart the benchmark";
    ofstream benchmarks_results;
    benchmarks_results.open("benchmarks.log", ios_base::app);
    time_t now = time(0);
    string time = ctime(&now); time.erase(std::remove(time.begin(), time.end(), '\n'), time.cend());
    benchmarks_results << "[" << time << "]: threads (" << threadstocreate << "); prime Numbers (" << prime_numbers_to_check << "); best time (" << lower_time << ");" << endl;
    benchmarks_results.close();
    {char c; c = _getch(); }
    system("cls");
    main(argc, argv);
}