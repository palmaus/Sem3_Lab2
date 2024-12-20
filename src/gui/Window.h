#pragma once

#include <Distributor.h>
#include <sstream>
#include <iostream>
#include <string>


#include <QMainWindow>
#include <QLineEdit>
#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QVector>
#include <functional>
#include "OptionMgr.h"
#include "XSlider.h"

#include "SharedPtr.h"
#include "MutableSequence.h"
#include "MutableListSequence.h"
#include "SortController.h"
#include "ISorter.h"
#include "BubbleSort.h"
#include "HeapSort.h"
#include "ShellSort.h"
#include "VisualizationWidget.h"
#include "Comparators.h"
#include "StudentGUI.h"
#include <QTabWidget>


class Window : public QWidget {
    Q_OBJECT

    public:
        Window(QWidget *parent, QGraphicsScene *scene, QGraphicsView *view, const SharedPtr<MutableListSequence<Student>>& seq, int delay = 500);


    signals:
        void runSignal();
        void doneSignal();
        void resizeSignal(int);
        void killSignal();
        void abortSignal();
        void playSignal();
        void recordSignal();
        void pauseSignal();

    private slots:
        void startSorting();
        void resetSorting();
        void onSortStarted();
        void onSortFinished();
        void resizeSignalHandlerString(const QString &s);

        void scrambleChangedHandler(int);
        void comparatorChangedHandler(int);


    public slots:
        void run() { emit runSignal(); }
        void done() { emit doneSignal(); }
        void Resize(int size) { emit resizeSignal(size); }
        void kill() { emit killSignal(); }
        void abort() { emit abortSignal(); }
        void play() { emit playSignal(); }
        void record() { emit recordSignal(); }
        void pause() { emit pauseSignal(); }


    private:
        SharedPtr<MutableListSequence<Student>> sequence;
        StudentComparator* currentComparator;
        QWidget *m_parent;
        QGraphicsScene *m_scene;
        QGraphicsView *m_view;
        QComboBox *m_chooseAlgo;
        OptionMgr *m_optmgr;
        QLineEdit *m_chooseDelay;
        QLineEdit *m_chooseSize;
        QComboBox *m_chooseScramble;
        QLCDNumber *m_nbCmp;
        QPushButton *m_start;
        QPushButton *m_exit;
        QPushButton *m_reset;
        QVBoxLayout *m_chooseMenu;
        QIntValidator* validator;

        QComboBox* m_chooseComparator;
        std::vector<StudentComparator*> comparators; // Вектор для хранения компараторов


         QTabWidget *m_tabWidget;
         QWidget *m_visualisationTab;
         QWidget *m_fileTab;


        VisualizationWidget *visualizationWidget;
        SortController *sortController;
        int delay;


        std::string b62(long long unsigned int n);

        void scrambleData(int scramble);
        void generateData(int size);
        StudentGUI* studentGUI;


        QLineEdit* filePathEdit;
        QLineEdit* outputFilePathEdit;
};