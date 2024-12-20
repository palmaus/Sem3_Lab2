#ifndef FILE_H
#define FILE_H

#include "MutableListSequence.h"
#include "Student.h"
#include "SharedPtr.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

inline SharedPtr<MutableListSequence<Student>> ReadStudentsFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    SharedPtr<MutableListSequence<Student>> students = MakeShared<MutableListSequence<Student>>();
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        Student student;
        std::string firstName;
        std::string lastName;
        int id, day, month, year, yearOfStudy;

        if (iss >> firstName >> lastName >> id >> day >> month >> year >> yearOfStudy) {
            int dob[3] = {day, month, year};
            student = Student(firstName, lastName, id, dob, yearOfStudy);
            students->append(student);

        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
            continue;
        }
    }

    inputFile.close();
    return students;
}

template<typename T>
void WriteStudentsToFile(const SharedPtr<MutableListSequence<T>>& students, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    for (int i = 0; i < students->getLength(); ++i) {
        outputFile << students->get(i) << std::endl;
    }

    outputFile.close();
}

#endif