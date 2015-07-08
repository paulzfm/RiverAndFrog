#include "controller.h"

#include <QDebug>

void Controller::play()
{
    over = false;

    // drawing
    view->update();

    threads = std::unique_ptr<pthread_t[]>(new pthread_t[view->model.m + 1]);
    params = std::unique_ptr<Param[]>(new Param[view->model.m + 1]);
    pthread_mutex_init(&mutex, NULL);

    // start moving
    for (int i = 0; i < view->model.m; ++i) {
        params[i].controller = this;
        params[i].begin = i * view->model.n;
        params[i].end = (i + 1) * view->model.n;
        pthread_create(&threads[i], NULL, updateData, (void*)&params[i]);
    }

    // start repainting
    params[view->model.m].controller = this;
    pthread_create(&threads[view->model.m], NULL, syncScreen, (void*)&params[view->model.m]);
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
        if (param->begin <= param->controller->view->model.frog.on &&
            param->controller->view->model.frog.on < param->end) {
            param->controller->view->model.frogMove();
        }

//        pthread_mutex_lock(&(param->controller->mutex));
//        param->controller->view->update();
//        pthread_mutex_unlock(&(param->controller->mutex));
        usleep(param->controller->view->model.woods[param->begin].interval);
    }

    pthread_exit(NULL);
}

void* syncScreen(void *args)
{
    Param *param = (Param*)args;
    qDebug() << "Thread create: sync screen";

    while (!param->controller->over) {
        param->controller->view->update();
        usleep(20000);
    }

    pthread_exit(NULL);
}
