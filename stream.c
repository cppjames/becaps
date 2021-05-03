#include "stream.h"

InitMemberFuncs(Stream,
    InitFunc(Stream, init),
    InitFunc(Stream, setFile),
    InitFunc(Stream, write),
);

FuncDef(Stream, void, init) {
    self->file = NULL;
}

FuncDef(Stream, void, setFile) {
    self->file = params.file;
}

FuncDef(Stream, void, write) {
    fputs(params.string, self->file);
}