#include "BenchmarkRunner.h"

void BenchmarkRunner::registerBenchmark(const std::string& name, BenchmarkFunction func) {
    benchmarks[name] = func;
}

void BenchmarkRunner::runBenchmarks(int iterations) {
    results.clear();
    for (const auto& pair : benchmarks) {
        const std::string& name = pair.first;
        BenchmarkFunction func = pair.second;

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

        results[name] = duration.count();
    }
}

std::map<std::string, double> BenchmarkRunner::getResults() const {
    return results;
}

void BenchmarkRunner::clearResults() {
    results.clear();
}

void BenchmarkRunner::saveResultsToFile(const std::string& filename) const {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << "Benchmark,Time(ns)\n";
        for (const auto& pair : results) {
            outfile << pair.first << "," << pair.second << "\n";
        }
        outfile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void BenchmarkRunner::printResults() const {
    std::cout << "Benchmark Results:\n";
    for (const auto& pair : results) {
        std::cout << pair.first << ": " << pair.second << " ns\n";
    }
}