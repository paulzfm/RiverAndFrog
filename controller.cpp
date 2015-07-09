#include "controller.h"

#include <QMessageBox>
#include <QCoreApplication>

Controller::Controller(QWidget *parent)
    : QWidget(parent)
{
    view = new View(this);

    threads = std::unique_ptr<pthread_t[]>(new pthread_t[view->model.m + 2]);
    params = std::unique_ptr<Param[]>(new Param[view->model.m + 2]);
    intervals = std::unique_ptr<int[]>(new int[view->model.m]);

    connect(this, SIGNAL(gameIsOver()), this, SLOT(gameOver()));
}

void Controller::play()
{
    over = false;
    second = 0;
    updateTimer(0);
    updateProcess(view->model.m - 1);

    // random intervals
    updateIntervals(3000);

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

void Controller::win()
{
    view->update();
    QMessageBox::information(NULL, "Win", "You win!",
                             QMessageBox::Ok, QMessageBox::Ok);
    restart();
}

void Controller::gameOver()
{
    view->update();
    QMessageBox::warning(NULL, "Game over", "Game over!",
                         QMessageBox::Ok, QMessageBox::Ok);
    restart();
}

void Controller::restart()
{
    for (int i = 0; i < view->model.m + 2; ++i) {
        pthread_kill(threads[i], NULL);
    }

    int ret = QMessageBox::question(NULL, "Information", "Do you want to play again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::Yes) {
        view->model.reset();
        play();
    } else {
        exitGame();
    }
}

void Controller::exitGame()
{
    for (int i = 0; i < view->model.m + 2; ++i) {
        pthread_kill(threads[i], NULL);
    }

    QCoreApplication::quit();
}

void Controller::keyboardResponse(int key)
{
    if (over) {
        return;
    }

    int result;
    switch (key) {
    case Qt::Key_Up:
    case Qt::Key_W:
        result = view->model.frogJump(true);
        updateProcess(view->model.frog.row);
        if (result == Model::GAME_OVER) {
            over = true;
            emit gameIsOver();
        } else if (result == Model::WIN) {
            over = true;
            win();
        }
        break;
    case Qt::Key_Down:
    case Qt::Key_S:
        result = view->model.frogJump(false);
        updateProcess(view->model.frog.row);
        if (result == Model::GAME_OVER) {
            over = true;
            emit gameIsOver();
        } else if (result == Model::WIN) {
            over = true;
            win();
        }
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

void Controller::setTimer(QLabel *label)
{
    timer = label;
}

void Controller::setProcess(QLabel *label)
{
    process = label;
}

void Controller::updateTimer(int second)
{
    timer->setText("Second: " + QString::number(second));
}

void Controller::updateProcess(int row)
{
    float percentage = 100 * (1.0 - (float)row / (view->model.m - 1));
    process->setText("Process: " + QString::number(percentage, 'g', 3) + "%");
}

void* updateData(void *args)
{
    Param *param = (Param*)args;

    while (!param->controller->over) {
        for (int i = param->begin; i < param->end; ++i) {
            param->controller->view->model.woods[i].move();
        }
        if (param->begin <= param->controller->view->model.frog.on &&
            param->controller->view->model.frog.on < param->end) {
            if (!param->controller->view->model.frogMove()) {
                param->controller->over = true;
                emit param->controller->gameIsOver();
            }
        }

        usleep(param->controller->intervals[param->row]);
    }

    pthread_exit(NULL);
}

void* syncScreen(void *args)
{
    Param *param = (Param*)args;

    while (!param->controller->over) {
        param->controller->view->update();
        usleep(20000);
    }

    pthread_exit(NULL);
}

void* clock(void *args)
{
    Param *param = (Param*)args;

    while (!param->controller->over) {
        ++param->controller->second;
        param->controller->updateTimer(param->controller->second);
        param->controller->updateIntervals(3000 * param->controller->second);
        sleep(1);
    }

    pthread_exit(NULL);
}

