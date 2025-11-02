#include "Dust.h"


void Dust::draw(Painter& painter) const {
    painter.draw(pos, 10.0, c);
}