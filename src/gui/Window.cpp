#include "Window.h"
#include "Comparators.h"
#include <QFileDialog>
#include <fstream>
#include <QScrollArea>

Window::Window(QWidget *parent, QGraphicsScene *scene, QGraphicsView *view, const SharedPtr<MutableListSequence<Student>>& seq, int delay)
    : QWidget(parent), sequence(seq),  delay(delay), m_parent(parent), m_scene(scene), m_view(view)
{

    move(550, 10);


    m_chooseAlgo = new QComboBox(this);
    m_chooseAlgo->addItem("Bubblesort");
    m_chooseAlgo->addItem("Heapsort");
    m_chooseAlgo->addItem("Shellsort");
    m_chooseAlgo->addItem("Mergesort");

    m_chooseAlgo->setCurrentIndex(1);
    m_chooseAlgo->setCurrentIndex(0);

    m_chooseScramble = new QComboBox(this);
    m_chooseScramble->addItem("Random");
    m_chooseScramble->addItem("Sorted-ish");
    m_chooseScramble->addItem("Reversed-ish");
    m_chooseScramble->addItem("Shuffle");


    m_chooseDelay = new QLineEdit("500", this);
    QIntValidator* validator = new QIntValidator(0, 10000, this);
    m_chooseDelay->setValidator(validator);
    m_chooseSize = new QLineEdit("8", this);
    validator = new QIntValidator(8, 1024, this);
    m_chooseSize->setValidator(validator);;

    m_nbCmp = new QLCDNumber(this);
    m_nbCmp->setDigitCount(10);

    m_start = new QPushButton("Start", this);
    m_exit = new QPushButton("Exit", this);
    m_reset = new QPushButton("Reset", this);

    m_start->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_exit->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_reset->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    m_start->setMinimumWidth(70);
    m_exit->setMinimumWidth(70);
    m_reset->setMinimumWidth(70);


    m_chooseComparator = new QComboBox(this);
    m_chooseComparator->addItem("By ID");
    m_chooseComparator->addItem("By Year");
    m_chooseComparator->addItem("By Last Name");
    m_chooseComparator->addItem("By First Name");

    comparators.push_back(new CompareStudentsByID());
    comparators.push_back(new CompareStudentsByYear());
    comparators.push_back(new CompareStudentsByLastName());
    comparators.push_back(new CompareStudentsByFirstName());

    currentComparator = comparators[0];
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_visualisationTab = new QWidget();
    m_fileTab = new QWidget();
    m_fileTab->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    m_fileTab->setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QVBoxLayout* visualisationLayout = new QVBoxLayout(m_visualisationTab);

    m_chooseMenu = new QVBoxLayout();
    m_chooseMenu->addWidget(new QLabel("Algorithm:", this));
    m_chooseMenu->addWidget(m_chooseAlgo);
    m_chooseMenu->addWidget(new QLabel("Delay:", this));
    m_chooseMenu->addWidget(m_chooseDelay);
    m_chooseMenu->addWidget(new QLabel("Size:", this));
    m_chooseMenu->addWidget(m_chooseSize);
    m_chooseMenu->addWidget(new QLabel("Scramble:", this));
    m_chooseMenu->addWidget(m_chooseScramble);
    m_chooseMenu->addWidget(new QLabel("Compare By:", this));
    m_chooseMenu->addWidget(m_chooseComparator);
    m_chooseMenu->addWidget(new QLabel("Comparisons", this));
    m_chooseMenu->addWidget(m_nbCmp);

    auto buttons = new QGridLayout();
    buttons->addWidget(m_start, 1, 1);
    buttons->addWidget(m_exit, 2, 1);
    buttons->addWidget(m_reset, 1, 2);

    QSpacerItem *hSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    buttons->addItem(hSpacer,1,0,3,1);

    QSpacerItem *vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    buttons->addItem(vSpacer, 0, 0, 1, 3);

    buttons->setSizeConstraint(QLayout::SetFixedSize);

    m_chooseMenu->addLayout(buttons);
    visualisationLayout->addLayout(m_chooseMenu);



    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(m_fileTab);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QVBoxLayout* fileLayout = new QVBoxLayout(m_fileTab);


    studentGUI = new StudentGUI(m_fileTab);
    fileLayout->addWidget(studentGUI);
    fileLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    m_fileTab->setLayout(fileLayout);
    m_fileTab = scrollArea;




  // Добавляем mainLayout в основное окно
    mainLayout->addWidget(m_tabWidget);


// Добавляем вкладки в QTabWidget
    m_tabWidget->addTab(m_visualisationTab, "Visualization");
    m_tabWidget->addTab(m_fileTab, "File Sort");

    setLayout(mainLayout);


    visualizationWidget = new VisualizationWidget(m_scene, this, delay);
    sortController = new SortController(this, delay);

    connect(m_start, &QPushButton::clicked, this, &Window::startSorting);
    connect(m_exit, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(m_reset, &QPushButton::clicked, this, &Window::resetSorting);

    connect(sortController, &SortController::sortStepCompleted, visualizationWidget, &VisualizationWidget::updateVisualization);
    connect(sortController, &SortController::sortStarted, this, &Window::onSortStarted);
    connect(sortController, &SortController::sortFinished, this, &Window::onSortFinished);

    connect(m_chooseSize, &QLineEdit::textChanged, this, &Window::resizeSignalHandlerString);
    connect(m_chooseScramble, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Window::scrambleChangedHandler);
    connect(m_chooseComparator, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Window::comparatorChangedHandler);



    setWindowTitle("A visualizer for sorting algorithms");
    int size = 8;
    generateData(size);
    visualizationWidget->setData(sequence);


}

void Window::startSorting() {
    ISorter<Student, StudentComparator>* sorter = nullptr;

    if (m_chooseAlgo->currentIndex() == 0) {
        sorter = new BubbleSort<Student, StudentComparator>();
    } else if (m_chooseAlgo->currentIndex() == 1) {
        sorter = new HeapSort<Student, StudentComparator>();
    } else if (m_chooseAlgo->currentIndex() == 2) {
        sorter = new ShellSort<Student, StudentComparator>();
    } else if (m_chooseAlgo->currentIndex() == 3) {
        sorter = new MergeSort<Student, StudentComparator>();
    }

    bool ok;
    if(m_chooseDelay->text().toInt(&ok))
        delay = m_chooseDelay->text().toInt();
    if (sorter) {
        sortController->setDelay(delay);
        sortController->setSequence(sequence);
        sortController->setComparator(*currentComparator);
        sortController->setCurrentSorter(sorter);
        sortController->startSorting();
    }
}

void Window::resetSorting() {
    bool ok;
    int size = m_chooseSize->text().toInt(&ok);
    if (ok)
    {
        generateData(size);
        scrambleData(m_chooseScramble->currentIndex());
        visualizationWidget->setData(sequence);
        sortController->resetSorting();
        visualizationWidget->update();
    } else{
        m_chooseSize->setText("8");
    }
}

void Window::onSortStarted() {
    m_chooseAlgo->setEnabled(false);
    m_chooseDelay->setEnabled(false);
    m_chooseSize->setEnabled(false);
    m_chooseScramble->setEnabled(false);
    m_chooseComparator->setEnabled(false);

    if (sortController)
        m_nbCmp->display(sortController->getComparisons());

    m_start->setText("Sorting");
    m_start->setEnabled(false);
}

void Window::onSortFinished() {
    m_chooseAlgo->setEnabled(true);
    m_chooseDelay->setEnabled(true);
    m_chooseSize->setEnabled(true);
    m_chooseScramble->setEnabled(true);
    m_chooseComparator->setEnabled(true);
    m_start->setText("Start");
    m_start->setEnabled(true);
    if (sortController)
        m_nbCmp->display(sortController->getComparisons());
    visualizationWidget->update();

}

void Window::resizeSignalHandlerString(const QString& s){
    bool ok;
    int size = s.toInt(&ok);
    if (ok) {
        generateData(size);
        scrambleData(m_chooseScramble->currentIndex());

        if (size == 512) {
            m_parent->setFixedSize(1310, 620);
            m_view->setFixedSize(1100, 580);
            m_view->setSceneRect(0, 0, 1035, 580);

        } else {
           //  m_parent->setFixedSize(820, 620); убираем
            m_view->setFixedSize(600, 580);
            m_view->setSceneRect(0, 0, 550, 580);
        }

        visualizationWidget->setData(sequence);
    }
    else {
        m_chooseSize->setText("8");
    }
}

void Window::scrambleChangedHandler(int index)
{
    scrambleData(index);
    visualizationWidget->setData(sequence);

}


void Window::comparatorChangedHandler(int index) {
    currentComparator = comparators[index];
}


std::string Window::b62(long long unsigned int n) {
    std::string s;
    std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    do {
        s = chars[n % 62] + s;
        n /= 62;
    } while (n > 0);
    return s;
}

void Window::scrambleData(int scramble)
{
    int size = sequence->getLength();
    item *temp_array = new item[size];
    for(int i = 0; i < size; ++i)
    {
        int dob[3] = {1, 1, 2000};
        temp_array[i] = {sequence->get(i).getID(), -1, -1, 0, 0, 0};

    }

    Distributor distributor(size, temp_array);

    switch (scramble) {
        case 0:
            distributor.Srandom();
            break;
        case 1:
            distributor.Ssorted();
            break;
        case 2:
            distributor.Sreversed();
            break;
        case 3:
            distributor.Sshuffle();
            break;

        default:

            break;
    }
    sequence->clear();
    for(int i = 0; i < size; ++i)
    {
        int dob[3] = {1, 1, 2000};
        sequence->append(Student("FirstName" + std::to_string(temp_array[i].val), "LastName" + std::to_string(temp_array[i].val), temp_array[i].val, dob, 2023));
    }
    delete[] temp_array;

}

void Window::generateData(int size)
{
    auto sequenceToSort = MakeShared<MutableListSequence<Student>>();
    for(int i = 0; i < size; ++i)
    {
        int dob[3] = {1, 1, 2000};
        sequenceToSort->append(Student("FirstName" + std::to_string(i), "LastName" + std::to_string(i), i, dob, 2023));
    }
    sequence = sequenceToSort;
}