#include "controller.h"

#include <QDebug>
#include <QMessageBox>

Controller::Controller(QWidget *parent)
    : QWidget(parent)
{
    view = new View(this);

    threads = std::unique_ptr<pthread_t[]>(new pthread_t[view->model.m + 2]);
    params = std::unique_ptr<Param[]>(new Param[view->model.m + 2]);
    intervals = std::unique_ptr<int[]>(new int[view->model.m]);
    pthread_mutex_init(&mutex, NULL);
}

void Controller::play()
{
    over = false;
    second = 0;

    // random intervals
    updateIntervals(1000);

    // start moving
    for (int i = 0; i < view->model.m; ++i) {
        params[i].controller = this;
        params[i].begin = i * view->model.n;
        params[i].end = (i + 1) * view->model.n;
        params[i].row = i;
        pthread_create(&threads[i], NULL, updateData, (void*)&params[i]);
    }

    // start repainting
    params[view->model.m].controller = this;
    pthread_create(&threads[view->model.m], NULL, syncScreen, (void*)&params[view->model.m]);

    // start timing
    params[view->model.m + 1].controller = this;
    pthread_create(&threads[view->model.m + 1], NULL, clock, (void*)&params[view->model.m + 1]);
}

void Controller::gameOver()
{
    QMessageBox::warning(NULL, "Game over", "Game over!",
                         QMessageBox::Ok, QMessageBox::Ok);
}

void Controller::keyboardResponse(int key)
{
    switch (key) {
    case Qt::Key_Up:
    case Qt::Key_W:
        over = view->model.frogJump(true);
        if (over) {
            gameOver();
        }
        view->update();
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        over = view->model.frogJump(false);
        if (over) {
            gameOver();
        }
        view->update();
        break;
    default:
        break;
    }
}

void Controller::updateIntervals(int range)
{
    if (range > 18000) { // maximum
        range = 18000;
    }

    for (int i = 0; i < view->model.m; ++i) {
        intervals[i] = 20000 - rand() % range;
    }
}

void* updateData(void *args)
{
    Param *param = (Param*)args;
    qDebug() << "Thread create: update data" << param->row;

    while (!param->controller->over) {
        for (int i = param->begin; i < param->end; ++i) {
            param->controller->view->model.woods[i].move();
        }
        if (param->begin <= param->controller->view->model.frog.on &&
            param->controller->view->model.frog.on < param->end) {
            if (!param->controller->view->model.frogMove()) {
                param->controller->over = true;
                param->controller->gameOver();
            }
        }

//        pthread_mutex_lock(&(param->controller->mutex));
//        param->controller->view->update();
//        pthread_mutex_unlock(&(param->controller->mutex));
        usleep(param->controller->intervals[param->row]);
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

void* clock(void *args)
{
    Param *param = (Param*)args;
    qDebug() << "Thread create: clock";

    while (!param->controller->over) {
        ++param->controller->second;
        qDebug() << param->controller->second;
        param->controller->updateIntervals(1000 * param->controller->second);
        sleep(1);
    }

    pthread_exit(NULL);
}
