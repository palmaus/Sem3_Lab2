#ifndef STUDENT_GUI_H
#define STUDENT_GUI_H

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QSpinBox>
#include <QMessageBox>
#include <QFileDialog>

#include <iostream>
#include <functional>
#include <MutableArraySequence.h>

#include "Student.h"
#include "ISorter.h"
#include "BubbleSort.h"
#include "MutableSequence.h"
#include "SharedPtr.h"
#include "Comparators.h"
#include "File.h"

class StudentGUI : public QWidget {

private:
    QLineEdit* firstNameEdit;
    QLineEdit* lastNameEdit;
    QLineEdit* idEdit;
    QSpinBox* dobDaySpinBox;
    QSpinBox* dobMonthSpinBox;
    QSpinBox* dobYearSpinBox;
    QSpinBox* yearOfStudySpinBox;
    QPushButton* addStudentButton;
    QListWidget* studentListWidget;

    QComboBox* sorterComboBox;
    QComboBox* comparatorComboBox;
    QComboBox* sequenceComboBox;

    QPushButton* sortButton;
    QListWidget* sortedStudentListWidget;

    SharedPtr<MutableSequence<Student>> studentSequence;
    SharedPtr<MutableSequence<Student>> sortedStudentSequence;


    void displayErrorMessage(const QString& message) {
        QMessageBox::critical(this, "Error", message);
    }

    static void displayStudentData(const Student& student, QListWidget* listWidget) {
        std::string firstName = student.getFirstName();
        std::string lastName = student.getLastName();

        listWidget->addItem(QString::fromStdString(
            firstName + " " +
            lastName + ", ID: " +
            std::to_string(student.getID()) + ", DOB: " +
            std::to_string(student.getDateOfBirth()[0]) + "." +
            std::to_string(student.getDateOfBirth()[1]) + "." +
            std::to_string(student.getDateOfBirth()[2]) + ", Year: " +
            std::to_string(student.getYearOfStudy())
        ));
    }

public:
    explicit StudentGUI(QWidget* parent = nullptr) : QWidget(parent), studentSequence(new MutableArraySequence<Student>()) {
        firstNameEdit = new QLineEdit();
        lastNameEdit = new QLineEdit();
        idEdit = new QLineEdit();
        dobDaySpinBox = new QSpinBox();  dobDaySpinBox->setRange(1, 31);
        dobMonthSpinBox = new QSpinBox(); dobMonthSpinBox->setRange(1, 12);
        dobYearSpinBox = new QSpinBox(); dobYearSpinBox->setRange(1900, 2024);
        yearOfStudySpinBox = new QSpinBox(); yearOfStudySpinBox->setRange(1, 6);

        addStudentButton = new QPushButton("Add Student");
        studentListWidget = new QListWidget();

        sorterComboBox = new QComboBox();
        sorterComboBox->addItem("Bubble Sort");
        sorterComboBox->addItem("Shell Sort");
        sorterComboBox->addItem("Merge Sort");
        sorterComboBox->addItem("Heap Sort");

        comparatorComboBox = new QComboBox();
        comparatorComboBox->addItem("By ID");
        comparatorComboBox->addItem("By Year");
        comparatorComboBox->addItem("By Last Name");
        comparatorComboBox->addItem("By First Name");

        sequenceComboBox = new QComboBox();
        sequenceComboBox->addItem("Array Sequence");
        sequenceComboBox->addItem("List Sequence");

        int initialSequenceType = sequenceComboBox->currentIndex();

        if (initialSequenceType == 0) {
            studentSequence = SharedPtr<MutableSequence<Student>>(new MutableArraySequence<Student>());
        } else {
            studentSequence = SharedPtr<MutableSequence<Student>>(new MutableListSequence<Student>());
        }

        sortButton = new QPushButton("Sort");
        sortedStudentListWidget = new QListWidget();

        auto* mainLayout = new QVBoxLayout();

        auto* loadButton = new QPushButton("Load From File");
        auto* saveButton = new QPushButton("Save To File");

        auto* fileOperationsLayout = new QHBoxLayout();
        fileOperationsLayout->addWidget(loadButton);
        fileOperationsLayout->addWidget(saveButton);

        mainLayout->addLayout(fileOperationsLayout);

        auto *inputLayout = new QGridLayout;
        inputLayout->addWidget(new QLabel("First Name:"), 0, 0);
        inputLayout->addWidget(firstNameEdit, 0, 1);
        inputLayout->addWidget(new QLabel("Last Name:"), 1, 0);
        inputLayout->addWidget(lastNameEdit, 1, 1);
        inputLayout->addWidget(new QLabel("ID:"), 2, 0);
        inputLayout->addWidget(idEdit, 2, 1);
        inputLayout->addWidget(new QLabel("Date of Birth:"), 3, 0);

        auto* dobLayout = new QHBoxLayout();
        dobLayout->addWidget(dobDaySpinBox);
        dobLayout->addWidget(dobMonthSpinBox);
        dobLayout->addWidget(dobYearSpinBox);
        inputLayout->addLayout(dobLayout, 3, 1);

        inputLayout->addWidget(new QLabel("Year of Study:"), 4, 0);
        inputLayout->addWidget(yearOfStudySpinBox, 4, 1);
        inputLayout->addWidget(addStudentButton, 5, 0, 1, 2);


        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(studentListWidget);

        auto* sortingLayout = new QHBoxLayout();
        sortingLayout->addWidget(new QLabel("Sorter:"));
        sortingLayout->addWidget(sorterComboBox);
        sortingLayout->addWidget(new QLabel("Comparator:"));
        sortingLayout->addWidget(comparatorComboBox);
        sortingLayout->addWidget(new QLabel("Sequence:"));
        sortingLayout->addWidget(sequenceComboBox);
        mainLayout->addLayout(sortingLayout);

        mainLayout->addWidget(sortButton);
        mainLayout->addWidget(sortedStudentListWidget);

        setLayout(mainLayout);

        connect(addStudentButton, &QPushButton::clicked, this, &StudentGUI::addStudent);
        connect(sortButton, &QPushButton::clicked, this, &StudentGUI::sortStudents);
        connect(sequenceComboBox, &QComboBox::currentIndexChanged, this, &StudentGUI::changeSequenceType);
        connect(loadButton, &QPushButton::clicked, this, &StudentGUI::loadFromFile);
        connect(saveButton, &QPushButton::clicked, this, &StudentGUI::saveToFile);
    }


private slots:

    void changeSequenceType(int index) {
        SharedPtr<MutableSequence<Student>> newSequence;

        if (index == 0) {
            newSequence = SharedPtr<MutableSequence<Student>>(new MutableArraySequence<Student>());
        } else {
            newSequence = SharedPtr<MutableSequence<Student>>(new MutableListSequence<Student>());
        }


        if (studentSequence) {
            for (int i = 0; i < studentSequence->getLength(); ++i) {
                newSequence->append(studentSequence->get(i));
            }
        }
        studentSequence = newSequence;
        updateStudentListWidget();
    }

    void addStudent() {
        QString firstName = firstNameEdit->text();
        QString lastName = lastNameEdit->text();

        bool idOk;
        int id = idEdit->text().toInt(&idOk);
        if (!idOk) {
            displayErrorMessage("Invalid ID input.");
            return;
        }

        int dob[3] = { dobDaySpinBox->value(), dobMonthSpinBox->value(), dobYearSpinBox->value() };
        int year = yearOfStudySpinBox->value();

        std::string firstNameStr = firstName.toStdString();
        std::string lastNameStr = lastName.toStdString();

        try {
            Student student(firstNameStr, lastNameStr, id, dob, year);
            studentSequence->append(student);

            displayStudentData(student, studentListWidget);

            firstNameEdit->clear();
            lastNameEdit->clear();
            idEdit->clear();

        } catch (const std::exception& e) {
            displayErrorMessage(e.what());
        }
    }

    void sortStudents() {
        int sorterIndex = sorterComboBox->currentIndex();
         ISorter<Student, StudentComparator>* sorter = nullptr;

        switch (sorterIndex) {
            case 0: sorter = new BubbleSort<Student, StudentComparator>(); break;
            case 1: sorter = new ShellSort<Student, StudentComparator>(); break;
            case 2: sorter = new MergeSort<Student, StudentComparator>(); break;
            case 3: sorter = new HeapSort<Student, StudentComparator>(); break;
            default: break;
        }
        StudentComparator* comparator = nullptr;
        switch (comparatorComboBox->currentIndex()) {
                case 0: comparator = new CompareStudentsByID(); break;
                case 1: comparator = new CompareStudentsByYear(); break;
                case 2: comparator = new CompareStudentsByLastName(); break;
                case 3: comparator = new CompareStudentsByFirstName(); break;
                default:  break;
        }
        if (sorter && comparator && studentSequence && studentSequence->getLength() > 0) {
            auto sortedSequence = studentSequence->copy();
            sorter->Sort(sortedSequence, *comparator);
            while (!sorter->isFinished())
            {
                sorter->step();
            }
            sortedStudentListWidget->clear();
            for (int i = 0; i < sortedSequence->getLength(); ++i) {
                displayStudentData(sortedSequence->get(i), sortedStudentListWidget);
            }
            sortedStudentSequence = sortedSequence->copy();
        } else {
            sortedStudentListWidget->clear();
            if (!sorter) {
                displayErrorMessage("Sorting method not selected.");
            } else if (!comparator) {
                displayErrorMessage("Comparator not selected.");
            } else if (!studentSequence || studentSequence->getLength() == 0) {
                displayErrorMessage("No students to sort.");
            }
        }
        if(sorter)
        {
            delete sorter;
            sorter = nullptr;
        }
        if(comparator)
        {
            delete comparator;
            comparator = nullptr;
        }

    }

    void loadFromFile() {
        QFileDialog::Options options = QFileDialog::DontUseNativeDialog;
        QString filePath = QFileDialog::getOpenFileName(
            nullptr,
            "Select a file to load",
            QDir::homePath(),
            "Text Files (*.txt)",
            nullptr,
            options
        );

        if (!filePath.isEmpty()) {
            try {
                studentSequence = ReadStudentsFromFile(filePath.toStdString());
                updateStudentListWidget();
            } catch (const std::exception& e) {
                displayErrorMessage(e.what());
            }
        }
    }



    void updateStudentListWidget() {
          studentListWidget->clear();
          for (int i = 0; i < studentSequence->getLength(); ++i) {
              displayStudentData(studentSequence->get(i), studentListWidget);
          }
      }

    void saveToFile() {
        if (!studentSequence || studentSequence->getLength() == 0) {
            displayErrorMessage("No students to save.");
            return;
        }

        QFileDialog::Options options = QFileDialog::DontUseNativeDialog;
        QString fileName = QFileDialog::getSaveFileName(
            this,
            "Save File",
            QDir::homePath(),
            "Text Files (*.txt)",
            nullptr,
            options
        );

        if (fileName.isEmpty()) {
            return;
        }

        try {
            WriteStudentsToFile(sortedStudentSequence, fileName.toStdString());
        } catch (const std::exception& e) {
            displayErrorMessage(QString("Failed to save file: ") + e.what());
        }
    }


};

#endif