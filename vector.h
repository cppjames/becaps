#ifndef VECTOR_H_
#define VECTOR_H_

#include "macros.h"

TypeDecl(Vector2);
#define Vector2(name) TypeConstruct(Vector2, name)

FuncDecl(Vector2, void, init, { double x, y; });
FuncDecl(Vector2, void, setX, { double x; });
FuncDecl(Vector2, void, setY, { double y; });
FuncDecl(Vector2, double, getX, { });
FuncDecl(Vector2, double, getY, { });
FuncDecl(Vector2, double, magnitude, { });

MemberFuncs(Vector2,
    MemberFuncDecl(Vector2, void, init);
    MemberFuncDecl(Vector2, void, setX);
    MemberFuncDecl(Vector2, void, setY);
    MemberFuncDecl(Vector2, double, getX);
    MemberFuncDecl(Vector2, double, getY);
    MemberFuncDecl(Vector2, double, magnitude);
);

TypeDef(Vector2,
    double x;
    double y;
);

#endif // VECTOR_H_
