#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QPoint>
#include <QRect>

struct Point : public QPoint
{
    Point(int x = 0, int y = 0)
        : QPoint(x, y) {}

    QPoint toQPoint() const
    {
        return QPoint(x(), y());
    }
};

struct Rect : public QRect
{
    Rect(int left = 0, int right = 0, int width = 0, int height = 0)
        : QRect(left, right, width, height) {}

    Rect(const QPoint& center, int width = 0, int height = 0)
        : QRect(center.x() - width / 2, center.y() - height / 2,
                width, height) {}

    // scene size
    const static int TOP = 0;
    const static int BOTTOM = 600;
    const static int LEFT = 0;
    const static int RIGHT = 1200;
    const static int WIDTH = RIGHT - LEFT;
    const static int HEIGHT = BOTTOM - TOP;
    const static int SPAN_WIDTH = 80;
    const static int SPAN_HEIGHT = 20;
    const static int WOOD_WIDTH = 70;
    const static int WOOD_HEIGHT = 40;
    const static int FROG_WIDTH = 40;
    const static int FROG_HEIGHT = 40;
};

struct Wood
{
    Rect rect;
    Point dir; // (1, 0) or (-1, 0)

    Wood(const Rect& rect, const Point& dir)
        : rect(rect), dir(dir) {}

    void move()
    {
        QPoint offset = dir;
        rect.adjust(offset.x(), offset.y(), offset.x(), offset.y());
        if (rect.left() > Rect::RIGHT) { // right overflow
            rect.setCoords(Rect::LEFT - Rect::WOOD_WIDTH,
                           rect.bottom() - Rect::WOOD_HEIGHT,
                           Rect::LEFT, rect.bottom());
        } else if (rect.right() < Rect::LEFT) { // left overflow
            rect.setCoords(Rect::RIGHT, rect.top(),
                           Rect::RIGHT + Rect::WOOD_WIDTH,
                           rect.top() + Rect::WOOD_HEIGHT);
        }
    }
};

struct Frog
{
    Rect rect;
    int on;
    int row;
};

struct Model
{
    // generate random sample
    Model();

    // new random sample
    void reset();

    // frog jump
    int frogJump(bool up);
    // return:
    const static int CONTINUE = 0;
    const static int GAME_OVER = 1;
    const static int WIN = 2;

    // frog move: follow wood
    bool frogMove();

    // m * n woods
    int m, n;

    // objects
    Frog frog;
    std::vector<Wood> woods;
};

#endif // MODEL_H

