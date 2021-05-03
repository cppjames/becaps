#include "vector.h"
#include <math.h>

InitMemberFuncs(Vector2,
    InitFunc(Vector2, init),
    InitFunc(Vector2, setX),
    InitFunc(Vector2, setY),
    InitFunc(Vector2, getX),
    InitFunc(Vector2, getY),
    InitFunc(Vector2, magnitude)
);

FuncDef(Vector2, void, init) {
    self->x = params.x;
    self->y = params.y;
}

FuncDef(Vector2, void, setX) {
    self->x = params.x;
}

FuncDef(Vector2, void, setY) {
    self->y = params.y;
}

FuncDef(Vector2, double, getX) {
    return self->x;
}

FuncDef(Vector2, double, getY) {
    return self->y;
}

FuncDef(Vector2, double, magnitude) {
    return sqrt(self->x * self->x + self->y * self->y);
}
