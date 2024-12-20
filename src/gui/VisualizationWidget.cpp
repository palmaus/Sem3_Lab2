#include "VisualizationWidget.h"

#include <QTimer>


void VisualizationWidget::setData(SharedPtr<MutableListSequence<Student>> seq)
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
    if (!sequence || i < 0 || i >= sequence->getLength() || proxyWidgets.size() <= i) return;

    int height = (sequence->get(i).getID() + 1) * m_width;
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
            itemWidgets[i]->setGeometry(QRect(0,0, m_width, height)); // устанавливаем начальное положение
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
        // Получаем текущие координаты и высоты
        int height1 = (sequence->get(index1).getID() + 1) * m_width;
        int left1 = 10 + index1 * m_width;
        int up1 = 540 - height1;
        QPointF pos1 = QPointF(left1, up1);


        int height2 = (sequence->get(index2).getID() + 1) * m_width;
        int left2 = 10 + index2 * m_width;
        int up2 = 540 - height2;
        QPointF pos2 = QPointF(left2, up2);


        // Анимируем перемещение
        QPropertyAnimation* animation1 = animations[index1 * 2];
        animation1->setTargetObject(proxyWidgets[index1]);
        animation1->setPropertyName("pos");
        animation1->setStartValue(proxyWidgets[index1]->pos());
        animation1->setEndValue(pos2);

        QPropertyAnimation* animation2 = animations[index2 * 2];
        animation2->setTargetObject(proxyWidgets[index2]);
        animation2->setPropertyName("pos");
        animation2->setStartValue(proxyWidgets[index2]->pos());
        animation2->setEndValue(pos1);

        for (auto animation : {animation1, animation2}) {
            animation->setDuration(delay);
            animation->setEasingCurve(QEasingCurve::InOutQuad);
            animation->start();
        }


        QTimer::singleShot(delay, [this, index1, index2]() {

            std::swap(proxyWidgets[index1], proxyWidgets[index2]); // меняем местами прокси
            std::swap(itemWidgets[index1], itemWidgets[index2]);// меняем местами itemWidgets
            std::swap(animations[index1*2], animations[index2*2]); //меняем местами анимации



        });
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