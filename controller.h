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
    Controller *controller;
};

// updaters
void* updateData(void* args);
void* syncScreen(void*);

class Controller : public QWidget
{
    Q_OBJECT

public:
    explicit Controller(QWidget *parent, int speed)
        : QWidget(parent)
    {
        view = new View(this, speed);
    }

    View* getView() { return view; }

    // start game
    void play();

    // speed control
    void speedUp();
    void slowDown();

    // keyboard responser
    void keyboardResponse(int key);

    View* view;

//private:
    int speed; // current speed
    int frog;  // current frog
    bool over; // game over?

    const static int MAX_SPEED = 10;

    std::unique_ptr<pthread_t[]> threads;
    std::unique_ptr<Param[]> params;
};

#endif // CONTROLLER_H
