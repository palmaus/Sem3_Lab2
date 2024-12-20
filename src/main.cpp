#include <BenchmarkRunner.h>
#include <iostream>
#include <string>
#include <memory>

#include "Window.h"
#include "MutableListSequence.h"
#include <functional>
#include <Student.h>

#include "TestRunner.h"
#include "InternalTests.h"
#include "SharedPtr.h"
#include "UniquePtr.h"



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
    runner.runTestGroup("MutableListSequence Tests", {
        internal_tests::testMutableListSequence
    });

    runner.printResults();
}


int main(int argc, char *argv[]) {
    //  bool runTests = false;  //не нужно
    //  if (runTests) {  //не нужно
    //      runAllTests();  //не нужно
    //  } else {


    QApplication a(argc, argv);

    QGraphicsScene scene;
    QGraphicsView view(&scene);
    view.setFixedSize(600, 600);
    view.setSceneRect(0, 0, 550, 580);


    QMainWindow mainWindow;
    QWidget centralWidget;

    QHBoxLayout mainLayout(&centralWidget);


    auto sequenceToSort = MakeShared<MutableListSequence<Student>>();
    for (int i = 0; i < 8; i++)
    {
        int dob[3] = {1, 1, 2000};
        sequenceToSort->append(Student("FirstName" + std::to_string(i), "LastName" + std::to_string(i), i, dob, 2023));

    }

    auto mainWindowPtr = std::make_unique<Window>(&centralWidget, &scene, &view, sequenceToSort, 10);

    mainLayout.addWidget(&view);
    mainLayout.addWidget(mainWindowPtr.get());


    mainWindow.setCentralWidget(&centralWidget);
    mainWindow.showMaximized();


    return a.exec();
}

