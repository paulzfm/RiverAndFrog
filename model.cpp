#include "model.h"

Model::Model(int speed)
{
    int m = Rect::HEIGHT / (Rect::SPAN + Rect::WOOD_HEIGHT); // rows
    int n = Rect::WIDTH / (Rect::SPAN + Rect::WOOD_WIDTH); // columns

    // create m * n woods
    for (int i = 0; i < m; ++i) {
        Point dir;
        if (i % 2 == 0) {
            dir = Point(1, 0);
        } else {
            dir = Point(-1, 0);
        }
        for (int j = 0; j < n; ++j) {
            woods.push_back(Wood(Rect(Rect::SPAN * (j + 1) + Rect::WOOD_WIDTH * j,
                                      Rect::SPAN * (i + 1) + Rect::WOOD_HEIGHT * i,
                                      Rect::WOOD_WIDTH, Rect::WOOD_HEIGHT),
                                 speed + rand() % RANGE * 2 - RANGE, dir));
        }
    }

    // locale frogs on the middle two woods at the bottom row
    frog = m * n - n / 2 - 1;
}

Rect Model::frogRect() const
{
    return Rect(woods[frog].rect.center(), Rect::FROG_WIDTH, Rect::FROG_HEIGHT);
}

bool Model::frogJump(bool up)
{
    Rect rect = frogRect();
    int offset = Rect::SPAN + Rect::FROG_HEIGHT;
    if (up) {
        rect.adjust(0, -offset, 0, -offset);
    } else {
        rect.adjust(0, offset, 0, offset);
    }

    for (int i = 0; i < woods.size(); ++i) {
        if (rect.intersects(woods[i].rect)) {
            frog = i;
            return true;
        }
    }

    return false;
}


