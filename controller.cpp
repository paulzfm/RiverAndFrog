#include "controller.h"

#include <QDebug>

void Controller::play()
{
    over = false;

    // drawing
    view->update();

    threads = std::unique_ptr<pthread_t[]>(new pthread_t[view->model.m]);
    params = std::unique_ptr<Param[]>(new Param[view->model.m]);

    // start moving
    for (int i = 0; i < view->model.m; ++i) {
        params[i].controller = this;
        params[i].begin = i * view->model.n;
        params[i].end = (i + 1) * view->model.n;
        pthread_create(&threads[i], NULL, updateData, (void*)&params[i]);
    }
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

void* updateData(void *args)
{
    Param *param = (Param*)args;
    qDebug() << "Thread create:" << param->begin << "~" << param->end;

    while (!param->controller->over) {
        for (int i = param->begin; i < param->end; ++i) {
            param->controller->view->model.woods[i].move();
        }

        param->controller->view->update();
        sleep(1);
    }

    pthread_exit(NULL);
}
