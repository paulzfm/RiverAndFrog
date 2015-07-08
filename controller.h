#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"

#include <QWidget>
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

    View* getView() { return view; }

    // start game
    void play();

    void gameOver();

    // speed control
    void updateIntervals(int range);

    // keyboard responser
    void keyboardResponse(int key);

    View* view;
    bool over; // game over?
    int second;
    std::unique_ptr<pthread_t[]> threads;
    std::unique_ptr<Param[]> params;
    std::unique_ptr<int[]> intervals;
    pthread_mutex_t mutex;

    const static int MAX_SPEED = 10;
};

#endif // CONTROLLER_H
