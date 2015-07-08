#include "controller.h"

void Controller::play()
{
    // drawing
    view->update();
}

void Controller::keyboardResponse(int key)
{
    switch (key) {
    case Qt::Key_Up:
    case Qt::Key_W:
        view->model.frogJump(true);
        view->update();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        view->model.frogJump(false);
        view->update();
        break;
    default:
        break;
    }
}

void Controller::speedUp()
{
    speed = (speed + 1) % MAX_SPEED;
}

void Controller::slowDown()
{
    speed = speed <= 1 ? 1 : speed - 1;
}

