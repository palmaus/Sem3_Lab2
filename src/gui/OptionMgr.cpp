#include "OptionMgr.h"

OptionMgr::OptionMgr(QComboBox *menu) {
    m_menu = menu;
    setParent(menu);
}

void OptionMgr::updateOptions(int algorithm) {
    m_menu->clear();
    switch (algorithm) {
        case 0: // Bubblesort
            m_menu->addItem("Standard");  // Изменено:  оставлен только стандартный BubbleSort
        m_menu->setItemData(0, "Ω(n) | Θ(n²) | O(n²) || O(1)", Qt::ToolTipRole); //  Изменена подсказка
        break;
        case 1: // Quicksort
            m_menu->addItem("Standard"); // Изменено:  оставлен только стандартный QuickSort
        m_menu->setItemData(0, "Ω(n log n) | Θ(n log n) | O(n²) || O(log n)", Qt::ToolTipRole); // Изменена подсказка
        break;
        default: // Для остальных алгоритмов пока ничего не делаем
            break;

    }
}
bool OptionMgr::run() {
    return true;
}

void OptionMgr::end() {
    return;
}


