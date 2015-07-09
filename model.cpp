#include "model.h"

#include <stdlib.h>
#include <QDebug>

Model::Model()
{
    m = (Rect::HEIGHT - Rect::SPAN_HEIGHT) / (Rect::SPAN_HEIGHT + Rect::WOOD_HEIGHT); // rows
    n = (Rect::WIDTH - Rect::SPAN_WIDTH) / (Rect::SPAN_WIDTH + Rect::WOOD_WIDTH); // columns

    // create m * n woods
    for (int i = 0; i < m; ++i) {
        Point dir;
        if (i % 2 == 0) {
            dir = Point(1, 0);
        } else {
            dir = Point(-1, 0);
        }

        for (int j = 0; j < n; ++j) {
            woods.push_back(Wood(Rect(Rect::SPAN_WIDTH * (j + 1) + Rect::WOOD_WIDTH * j,
                                      Rect::SPAN_HEIGHT * (i + 1) + Rect::WOOD_HEIGHT * i,
                                      Rect::WOOD_WIDTH, Rect::WOOD_HEIGHT),
                                 dir));
        }
    }

    // locale frogs on the middle two woods at the bottom row
    frog.row = m - 1;
    frog.on = m * n - n / 2 - 1;
    frog.rect = Rect(woods[frog.on].rect.center(),
            Rect::FROG_WIDTH, Rect::FROG_HEIGHT);
}

void Model::reset()
{
    woods.clear();

    // create m * n woods
    for (int i = 0; i < m; ++i) {
        Point dir;
        if (i % 2 == 0) {
            dir = Point(1, 0);
        } else {
            dir = Point(-1, 0);
        }

        for (int j = 0; j < n; ++j) {
            woods.push_back(Wood(Rect(Rect::SPAN_WIDTH * (j + 1) + Rect::WOOD_WIDTH * j,
                                      Rect::SPAN_HEIGHT * (i + 1) + Rect::WOOD_HEIGHT * i,
                                      Rect::WOOD_WIDTH, Rect::WOOD_HEIGHT),
                                 dir));
        }
    }

    // locale frogs on the middle two woods at the bottom row
    frog.row = m - 1;
    frog.on = m * n - n / 2 - 1;
    frog.rect = Rect(woods[frog.on].rect.center(),
            Rect::FROG_WIDTH, Rect::FROG_HEIGHT);
}

int Model::frogJump(bool up)
{
    int offset = Rect::SPAN_HEIGHT + Rect::WOOD_HEIGHT;
    if (up) {
        frog.rect.adjust(0, -offset, 0, -offset);
        frog.row--;
    } else {
        if (frog.row == m - 1) { // last row
            return CONTINUE;
        }
        frog.rect.adjust(0, offset, 0, offset);
        frog.row++;
    }

    for (int i = frog.row * n; i < (frog.row + 1) * n; ++i) {
        if (frog.rect.intersects(woods[i].rect)) {
            frog.on = i;
            if (frog.row == 0) {
                return WIN;
            }
            return CONTINUE;
        }
    }

    return GAME_OVER;
}

bool Model::frogMove()
{
    QPoint offset = woods[frog.on].dir;
    frog.rect.adjust(offset.x(), offset.y(), offset.x(), offset.y());
    return (frog.rect.left() <= Rect::RIGHT && frog.rect.right() >= Rect::LEFT);
}

