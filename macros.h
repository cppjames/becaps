#ifndef MACROS_H_
#define MACROS_H_

#include "becaps.h"

#define setX(...) FuncMacroDef(setX, __VA_ARGS__)
#define setY(...) FuncMacroDef(setY, __VA_ARGS__)
#define getX(...) FuncMacroDef(getX, __VA_ARGS__)
#define getY(...) FuncMacroDef(getY, __VA_ARGS__)
#define magnitude(...) FuncMacroDef(magnitude, __VA_ARGS__)

#define setFile(...) FuncMacroDef(setFile, __VA_ARGS__)
#define write(...) FuncMacroDef(write, __VA_ARGS__)

#endif // MACROS_H_