#include "model.h"

#include <stdlib.h>

Model::Model()
{
    m = Rect::HEIGHT / (Rect::SPAN + Rect::WOOD_HEIGHT); // rows
    n = Rect::WIDTH / (Rect::SPAN + Rect::WOOD_WIDTH); // columns

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
                                 dir));
        }
    }

    // locale frogs on the middle two woods at the bottom row
    frog.on = m * n - n / 2 - 1;
    frog.rect = Rect(woods[frog.on].rect.center(),
            Rect::FROG_WIDTH, Rect::FROG_HEIGHT);
}

bool Model::frogJump(bool up)
{
    int offset = Rect::SPAN + Rect::FROG_HEIGHT;
    if (up) {
        frog.rect.adjust(0, -offset, 0, -offset);
    } else {
        frog.rect.adjust(0, offset, 0, offset);
    }

    for (int i = 0; i < woods.size(); ++i) {
        if (frog.rect.intersects(woods[i].rect)) {
            frog.on = i;
            return true;
        }
    }

    return false;
}

bool Model::frogMove()
{
    QPoint offset = woods[frog.on].dir;
    frog.rect.adjust(offset.x(), offset.y(), offset.x(), offset.y());
    return (frog.rect.left() <= Rect::RIGHT && frog.rect.right() >= Rect::LEFT);
}
