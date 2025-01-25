#include "VisualizationWidget.h"

#include <QTimer>


void VisualizationWidget::setData(SharedPtr<MutableSequence<Student>> seq)
{
    clearScene();
    sequence = seq;

    if (sequence)
    {
         m_width = std::max(512.0 / sequence->getLength(), 1.0);


        for (int i = 0; i < sequence->getLength(); i++) {
            createRect(i);
        }

    }
}

void VisualizationWidget::render(int i, QColor color) {
    if (!sequence || i < 0 || i >= proxyWidgets.size() || proxyWidgets.size() <= i) return; // Используем proxyWidgets.size() для проверки индекса

    int height = (sequence->get(i).getID() + 1) * m_width;
    // Рассчитываем left, используя i как визуальный индекс
    int left = 10 + i * m_width;
    int up = 540 - height;

    if (itemWidgets.size() > i) {
        QLabel* label = itemWidgets[i]->findChild<QLabel*>();
        if (label) {
            label->setStyleSheet(QString("background-color: %1; color: black;").arg(color.name()));
        }

        if (proxyWidgets.size() > i)
        {
            proxyWidgets[i]->setGeometry(QRectF(left, up, m_width, height));
            itemWidgets[i]->setGeometry(QRect(0,0, m_width, height));
        }

    }
}


void VisualizationWidget::createRect(int i) {
    if (!sequence) return;

    int height = (sequence->get(i).getID() + 1) * m_width;
    int left = 10 + i * m_width;
    int up = 540 - height;

    QWidget* itemWidget = new QWidget();
    QLabel* label = new QLabel(QString::number(sequence->get(i).getID()), itemWidget);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(QString("background-color: %1; color: black;").arg(QColor("orange").name()));

    itemWidget->setGeometry(0, 0, m_width, height); // устанавливаем начальное положение

    QGraphicsProxyWidget* proxy = m_scene->addWidget(itemWidget);
    proxy->setGeometry(QRectF(left, up, m_width, height));


    proxyWidgets.push_back(proxy);
    itemWidgets.push_back(itemWidget);

    animations.push_back(new QPropertyAnimation(proxy, "pos"));
    animations.push_back(new QPropertyAnimation(proxy, "geometry"));
    proxy->setWidget(itemWidget);
}

void VisualizationWidget::updateVisualization(int index1, int index2) {
    if (!sequence) return;

    if (index1 != -1 && index2 != -1 && index1 != index2 && index1 < proxyWidgets.size() && index2 < proxyWidgets.size()) {
        std::swap(proxyWidgets[index1], proxyWidgets[index2]); // меняем местами прокси
        std::swap(itemWidgets[index1], itemWidgets[index2]);// меняем местами itemWidgets
        std::swap(animations[index1*2], animations[index2*2]); //меняем местами анимации

        // Полный перерендер всех столбцов
        clearScene(); // Очищаем сцену
        for (int k = 0; k < sequence->getLength(); ++k) {
            createRect(k); // Создаем прямоугольники заново
        }
    }
}

void VisualizationWidget::clearScene()
{
        for (auto& proxy : proxyWidgets)
        {

            m_scene->removeItem(proxy);

            delete proxy;
        }


        for(auto& animation : animations)
        {
            delete animation;
        }

        proxyWidgets.clear();
        itemWidgets.clear();
        animations.clear();

}