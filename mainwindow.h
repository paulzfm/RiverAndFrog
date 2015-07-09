#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "view.h"
#include "controller.h"

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    View *view;
    QLabel *timer;
    QLabel *process;
    QLabel *info;
    Controller *controller;
};

#endif // MAINWINDOW_H
