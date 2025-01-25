#pragma once

#include <QWidget>
#include <QGraphicsScene>
#include <functional>

#include "MutableListSequence.h"
#include "SharedPtr.h"
#include "ISorter.h"
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QColor>
#include <vector>
#include <QPropertyAnimation>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include "Student.h"

#define ORANGE QColor(72, 209, 204)  // Turquoise

class VisualizationWidget : public QWidget {
public:
    explicit VisualizationWidget(QGraphicsScene* scene, QWidget* parent = nullptr, int delay = 500) : QWidget(parent), m_scene(scene), delay(delay) {}
    public slots:
    void setData(SharedPtr<MutableSequence<Student>> seq);
    void updateVisualization(int index1, int index2);

private:
    void render(int i, QColor C);
    void createRect(int i);
    void clearScene();

    QGraphicsScene* m_scene;
    SharedPtr<MutableSequence<Student>> sequence;
    double m_width;
    std::vector<QGraphicsProxyWidget*> proxyWidgets;
    std::vector<QWidget*> itemWidgets;
    std::vector<QPropertyAnimation *> animations;
    int delay;
};