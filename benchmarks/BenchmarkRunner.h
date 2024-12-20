#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

class BenchmarkRunner {
public:
    using BenchmarkFunction = std::function<void()>;

    void registerBenchmark(const std::string& name, BenchmarkFunction func);
    void runBenchmarks(int iterations = 1000);
    std::map<std::string, double> getResults() const;
    void clearResults();
    void saveResultsToFile(const std::string& filename) const;
    void printResults() const;


private:
    std::map<std::string, BenchmarkFunction> benchmarks;
    std::map<std::string, double> results;
};
