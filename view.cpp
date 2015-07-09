#include "view.h"
#include "ui_view.h"

#include <QPainter>
#include <QPalette>
#include <QColor>

View::View(QWidget *parent) :
    QWidget(parent), ui(new Ui::View)
{
    ui->setupUi(this);
    setFixedSize(Rect::WIDTH, Rect::HEIGHT);

    QImage origin;
    origin.load(":/img/frog.png");
    frog = origin.scaled(QSize(Rect::FROG_WIDTH, Rect::FROG_HEIGHT),
                        Qt::KeepAspectRatio, Qt::SmoothTransformation);

    origin.load(":/img/wood.jpeg");
    wood = origin.scaled(QSize(Rect::WOOD_WIDTH, Rect::WOOD_HEIGHT),
                         Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette pal;
    pal.setColor(QPalette::Background, QColor("#81eada"));
    setAutoFillBackground(true);
    setPalette(pal);
}

View::~View()
{
    delete ui;
}

void View::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    for (const auto& w : model.woods) {
        painter.drawImage(w.rect, wood);
    }

    painter.drawImage(model.frog.rect, frog);
}

