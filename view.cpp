#include "view.h"
#include "ui_view.h"

#include <QDebug>
#include <QPainter>
#include <QBrush>
#include <QPen>

View::View(QWidget *parent) :
    QWidget(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    setFixedSize(Rect::WIDTH, Rect::HEIGHT);
}

View::~View()
{
    delete ui;
}

void View::paintEvent(QPaintEvent*)
{
    for (const auto& wood : model.woods) {
        paintWood(wood);
    }

    paintFrog();
}

void View::paintFrog()
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(QPen());
    painter.drawRect(model.frog.rect);
}

void View::paintWood(const Wood &wood)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::black));
    painter.setPen(QPen());
    painter.drawRect(wood.rect);
}

