#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"

#include <QWidget>
#include <QLabel>
#include <memory>
#include <pthread.h>
#include <unistd.h>

class Controller;

struct Param
{
    int begin;
    int end;
    int row;
    Controller *controller;
};

// updaters
void* updateData(void* args);
void* syncScreen(void* args);
void* clock(void* args);

class Controller : public QWidget
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent = 0);

    // game control
    void play(); // start
    void win();  // win
    void restart(); // restart game?

    // speed control
    void updateIntervals(int range);

    // keyboard responser
    void keyboardResponse(int key);

    // status updater
    void setTimer(QLabel *label);
    void setProcess(QLabel *label);
    void updateTimer(int second);
    void updateProcess(int row);

    View* view; // view map
    bool over;  // game over?
    int second; // current time
    std::unique_ptr<pthread_t[]> threads;
    std::unique_ptr<Param[]> params;
    std::unique_ptr<int[]> intervals;

public slots:
    void gameOver(); // lose
    void exitGame(); // safety exit game

signals:
    void gameIsOver();

private:
    QLabel *timer;
    QLabel *process;
};

#endif // CONTROLLER_H

