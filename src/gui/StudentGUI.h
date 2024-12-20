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

#include "Student.h"
#include "ISorter.h"
#include "BubbleSort.h"
#include "MutableListSequence.h"
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

    QPushButton* sortButton;
    QListWidget* sortedStudentListWidget;

    SharedPtr<MutableListSequence<Student>> studentSequence;


    void displayErrorMessage(const QString& message) {
        QMessageBox::critical(this, "Error", message);
    }

    void displayStudentData(const Student& student, QListWidget* listWidget) {
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
    StudentGUI(QWidget* parent = nullptr) : QWidget(parent), studentSequence(MakeShared<MutableListSequence<Student>>()) {

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


        sortButton = new QPushButton("Sort");
        sortedStudentListWidget = new QListWidget();

        QVBoxLayout* mainLayout = new QVBoxLayout();

        QPushButton* loadButton = new QPushButton("Load From File");
        QPushButton* saveButton = new QPushButton("Save To File");

        QHBoxLayout* fileOperationsLayout = new QHBoxLayout();
        fileOperationsLayout->addWidget(loadButton);
        fileOperationsLayout->addWidget(saveButton);

        mainLayout->addLayout(fileOperationsLayout);

        QGridLayout *inputLayout = new QGridLayout;
        inputLayout->addWidget(new QLabel("First Name:"), 0, 0);
        inputLayout->addWidget(firstNameEdit, 0, 1);
        inputLayout->addWidget(new QLabel("Last Name:"), 1, 0);
        inputLayout->addWidget(lastNameEdit, 1, 1);
        inputLayout->addWidget(new QLabel("ID:"), 2, 0);
        inputLayout->addWidget(idEdit, 2, 1);
        inputLayout->addWidget(new QLabel("Date of Birth:"), 3, 0);

        QHBoxLayout* dobLayout = new QHBoxLayout();
        dobLayout->addWidget(dobDaySpinBox);
        dobLayout->addWidget(dobMonthSpinBox);
        dobLayout->addWidget(dobYearSpinBox);
        inputLayout->addLayout(dobLayout, 3, 1);

        inputLayout->addWidget(new QLabel("Year of Study:"), 4, 0);
        inputLayout->addWidget(yearOfStudySpinBox, 4, 1);
        inputLayout->addWidget(addStudentButton, 5, 0, 1, 2);


        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(studentListWidget);

        QHBoxLayout* sortingLayout = new QHBoxLayout();
        sortingLayout->addWidget(new QLabel("Sorter:"));
        sortingLayout->addWidget(sorterComboBox);
        sortingLayout->addWidget(new QLabel("Comparator:"));
        sortingLayout->addWidget(comparatorComboBox);
        mainLayout->addLayout(sortingLayout);

        mainLayout->addWidget(sortButton);
        mainLayout->addWidget(sortedStudentListWidget);

        setLayout(mainLayout);

        connect(addStudentButton, &QPushButton::clicked, this, &StudentGUI::addStudent);
        connect(sortButton, &QPushButton::clicked, this, &StudentGUI::sortStudents);
        connect(loadButton, &QPushButton::clicked, this, &StudentGUI::loadFromFile);
        connect(saveButton, &QPushButton::clicked, this, &StudentGUI::saveToFile);
    }


private slots:

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
        }



        StudentComparator* comparator = nullptr;
        switch (comparatorComboBox->currentIndex()) {
                case 0: comparator = new CompareStudentsByID(); break;
                case 1: comparator = new CompareStudentsByYear(); break;
                case 2: comparator = new CompareStudentsByLastName(); break;
                case 3: comparator = new CompareStudentsByFirstName(); break;
        }




        if (sorter && comparator && studentSequence && studentSequence->getLength() > 0) {
            SharedPtr<MutableListSequence<Student>> sortedSequence = MakeShared<MutableListSequence<Student>>();
            for(int i = 0; i < studentSequence->getLength(); ++i)
            {
                sortedSequence->append(studentSequence->get(i));
            }

            sorter->Sort(sortedSequence, *comparator);
            while (!sorter->isFinished())
            {
                sorter->step();
            }

            sortedStudentListWidget->clear();
            for (int i = 0; i < sortedSequence->getLength(); ++i) {
                displayStudentData(sortedSequence->get(i), sortedStudentListWidget);
            }
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
        QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt)");
        std::cout<<fileName.toStdString() << " 1 " << std::endl;
        if (fileName.isEmpty()) {
            displayErrorMessage("File not selected.");
            fileName = "/home/radioactive/CLionProjects/Sem3-Lab2/unsorted_data.txt";
        }

        try {
            SharedPtr<MutableListSequence<Student>> loadedStudents = ReadStudentsFromFile(fileName.toStdString());
            studentSequence = loadedStudents; // Replace existing sequence
            updateStudentListWidget();
        } catch (const std::exception& e) {
            displayErrorMessage(e.what());
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

        QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt)");
        if (fileName.isEmpty()) {
            displayErrorMessage("File not selected for saving.");
            fileName = "/home/radioactive/CLionProjects/Sem3-Lab2/output.txt";
        }

        try {
            WriteStudentsToFile(studentSequence, fileName.toStdString());
        } catch (const std::exception& e) {
            displayErrorMessage(e.what());
        }
    }

};

#endif