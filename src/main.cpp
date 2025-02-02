#include <BenchmarkRunner.h>
#include <iostream>
#include <string>
#include <memory>

#include "Window.h"
#include "MutableListSequence.h"
#include "MutableArraySequence.h"
#include <functional>
#include <Student.h>

#include "TestRunner.h"
#include "InternalTests.h"
#include "SharedPtr.h"
#include "UniquePtr.h"
#include <random>
#include <string>
#include <vector>

// Генератор случайных чисел
std::random_device rd;
std::mt19937 gen(rd());

SharedPtr<MutableSequence<Student>> generateStudents(int count) {
    std::vector<std::string> firstNames = {"John", "Jane", "Peter", "Mary", "David", "Sarah", "Michael", "Jessica", "Christopher", "Ashley"};
    std::vector<std::string> lastNames = {"Doe", "Smith", "Jones", "Brown", "Wilson", "Davis", "Garcia", "Rodriguez", "Williams", "Johnson"};

    auto sequence = SharedPtr<MutableSequence<Student>>(new MutableArraySequence<Student>());
    std::uniform_int_distribution<> nameDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> dayDist(1, 28);
    std::uniform_int_distribution<> monthDist(1, 12);
    std::uniform_int_distribution<> yearDist(1990, 2005);
    std::uniform_int_distribution<> yearOfStudyDist(1, 5);

    for (int i = 0; i < count; ++i) {
        std::string firstName = firstNames[nameDist(gen)];
        std::string lastName = lastNames[nameDist(gen)];
        int day = dayDist(gen);
        int month = monthDist(gen);
        int year = yearDist(gen);
        int dob[3] = {day, month, year};
        int yearOfStudy = yearOfStudyDist(gen);
        sequence->append(Student(firstName, lastName, i, dob, yearOfStudy));
    }
    return sequence;
}

// Функция для запуска всех тестов
void runAllTests() {
    TestRunner runner;

    runner.runTestGroup("SharedPtr Tests", {
        internal_tests::testSharedPtr
    });
    runner.runTestGroup("UniquePtr Tests", {
        internal_tests::testUniquePtr
    });
    runner.runTestGroup("WeakPtr Tests", {
        internal_tests::testWeakPtr
    });

    runner.runTestGroup("LinkedList Tests", {
        internal_tests::testLinkedList
    });

    runner.runTestGroup("DynamicArray Tests", {
        internal_tests::testDynamicArray
    });

    runner.runTestGroup("MutableListSequence Tests", {
        internal_tests::testMutableListSequence
    });

    runner.runTestGroup("Sorting Algorithms Tests", {
         internal_tests::testSortingAlgorithms
    });

    runner.runTestGroup("MutableArraySequence Tests", {
        internal_tests::testMutableArraySequence
    });

    runner.printResults();
}

void benchmark(int startIterations, int endIterations, int step) {
   BenchmarkRunner benchmarkRunner;
   auto comp = CompareStudentsByFirstName();

   for (int iterations = startIterations; iterations <= endIterations; iterations += step) {
        auto sequence = generateStudents(iterations);
        benchmarkRunner.registerBenchmark("BubbleSort," + std::to_string(iterations) , [&, sequence, comp](){
             BubbleSort<Student, StudentComparator> sorter;
             sorter.Sort(sequence, comp);
             while(!sorter.isFinished()) {
                 sorter.step();
             }
        });
        benchmarkRunner.registerBenchmark("HeapSort," + std::to_string(iterations), [&, sequence, comp](){
            HeapSort<Student, StudentComparator> sorter;
            sorter.Sort(sequence, comp);
            while(!sorter.isFinished()) {
                sorter.step();
            }
        });
        benchmarkRunner.registerBenchmark("MergeSort," + std::to_string(iterations), [&, sequence, comp](){
            MergeSort<Student, StudentComparator> sorter;
            sorter.Sort(sequence, comp);
            while(!sorter.isFinished()) {
                sorter.step();
            }
        });
        benchmarkRunner.registerBenchmark("ShellSort," + std::to_string(iterations), [&, sequence, comp](){
            ShellSort<Student, StudentComparator> sorter;
            sorter.Sort(sequence, comp);
            while(!sorter.isFinished()) {
                sorter.step();
            }
        });
   }

    std::ofstream outfile("../benchmarks/benchmark_results.csv");
    if (!outfile.is_open()) {
        std::cerr << "Unable to open file: ../benchmarks/benchmark_results.csv" << std::endl;
        return;
    }
    outfile << "Benchmark,Iterations,Time(ns)\n";

    benchmarkRunner.clearResults();
    benchmarkRunner.runBenchmarks(1);
    for (const auto& pair : benchmarkRunner.getResults()) {
        outfile << pair.first << "," << pair.second << "\n";
    }
    outfile.close();
}


int main(int argc, char *argv[]) {
    bool runTests = false;
    if (runTests) {
        runAllTests();
        // benchmark(2000, 10000, 2000);
    } else {
        QApplication a(argc, argv);

        QGraphicsScene scene;
        QGraphicsView view(&scene);
        view.setFixedSize(600, 600);
        view.setSceneRect(0, 0, 550, 580);


        QMainWindow mainWindow;
        QWidget centralWidget;

        QHBoxLayout mainLayout(&centralWidget);


        auto sequenceToSort = generateStudents(10);

        auto mainWindowPtr = std::make_unique<Window>(&centralWidget, &scene, &view, sequenceToSort, 10);

        mainLayout.addWidget(&view);
        mainLayout.addWidget(mainWindowPtr.get());

        mainWindow.setCentralWidget(&centralWidget);
        mainWindow.showMaximized();


        return a.exec();
    }
}

