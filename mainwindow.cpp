#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"

#include <QGridLayout>
#include <QBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->setHidden(true);

    setWindowTitle("River and Frog");

    // central widget layout
    QGridLayout *layout = new QGridLayout(ui->centralWidget);
    layout->setMargin(0);

    // box layout
    QBoxLayout *boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);

    // game info
    timer = new QLabel(this);
    timer->setAlignment(Qt::AlignCenter);
    timer->setText("Second: 0");
    boxLayout->addWidget(timer);

    process = new QLabel(this);
    process->setAlignment(Qt::AlignCenter);
    process->setText("Process: 0%");
    boxLayout->addWidget(process);

    info = new QLabel(this);
    info->setAlignment(Qt::AlignCenter);
    info->setText("Using ↑↓ to control frog.");
    boxLayout->addWidget(info);

    layout->addLayout(boxLayout, 0, 0);

    // controller
    controller = new Controller(this);
    layout->addWidget(controller->view, 1, 0);
    controller->setTimer(timer);
    controller->setProcess(process);

    // resize window
    setFixedSize(Rect::WIDTH, Rect::HEIGHT + 10);

    // start to play
    controller->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    controller->keyboardResponse(event->key());
}

