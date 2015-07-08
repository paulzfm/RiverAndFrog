#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "view.h"

#include <QWidget>

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

    // updaters
    void* updateData(void* args);
    void* syncScreen(void*);

private:
    int speed; // current speed
    int frog;  // current frog

    const static int MAX_SPEED = 10;

    View* view;
};

#endif // CONTROLLER_H
