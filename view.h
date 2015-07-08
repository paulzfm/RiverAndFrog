#ifndef VIEW_H
#define VIEW_H

#include "model.h"

#include <QWidget>
#include <QKeyEvent>
#include <QPaintEvent>

namespace Ui {
class View;
}

class View : public QWidget
{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();

    // update view
    void paintEvent(QPaintEvent*);

    Model model;

private:
    Ui::View *ui;

    // paint
    void paintFrog();
    void paintWood(const Wood& wood);
};

#endif // VIEW_H

