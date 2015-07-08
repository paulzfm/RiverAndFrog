#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"

#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("River and Frog");
//    setMinimumSize(100, 100);

    // central widget layout
    QGridLayout *layout = new QGridLayout(ui->centralWidget);
    layout->setMargin(0);

    // game info
    info = new QLabel(this);
    info->setAlignment(Qt::AlignCenter);
    info->setText("I am info");
    layout->addWidget(info, 0, 0);

    // controller
    controller = new Controller(this);
    layout->addWidget(controller->getView(), 1, 0);

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
