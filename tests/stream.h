#ifndef STREAM_H_
#define STREAM_H_

#include "funcs.h"
#include <stdio.h>

TypeDecl(Stream);
#define Stream(name) TypeConstruct(Stream, name)

FuncDecl(Stream, void, init, { });
FuncDecl(Stream, void, setFile, { FILE *file; });
FuncDecl(Stream, void, write, { const char *string; });

MemberFuncs(Stream,
    MemberFuncDecl(Stream, void, init);
    MemberFuncDecl(Stream, void, setFile);
    MemberFuncDecl(Stream, void, write);
);

TypeDef(Stream,
    FILE *file;
);

#endif // STREAM_H_