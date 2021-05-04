# `becaps` - Behavior Encapsulation in C
The C programming language supports object-oriented programming in the form of attribute encapsulation - that is, an object's attributes are syntactically associated with itself, and they are isolated from the attributes of other objects. `clock.color` is different from `mug.color`, because the attribute is **encapsulated** within its object.
However, C does not allow encapsulation of **behavior**: the link between a function and the object it operates on is purely conventional, since an object _can_ contain a pointer to a function, but that function has no knowledge of its parent object. As such, the only reliable way to tell "this is a `mug` method" was to include `mug` in the name of the function: `mug_smash(mugtype *mug)`. Until now...

### A word of warning
This text is **satirical**, as the techniques described here are impractical, extremely tedious to maintain, and they alter C syntax significantly. As a consequence, they make for very hard-to-read code for anyone who is not familiar with them (pretty much everyone). What you are about to read is the product of a **very bored mind**. Please take everything with a grain of salt and read this text **for your own entertainment**.

## The solution to an age-old problem
Have you ever wanted to do `mug.smash()` in C? You could say, "if you want that --- just use C++". Well, C++ is a good programming language, but has a ton of features besides encapsulated functions. Using C++ just for its classes, effectively writing C++ in C style, using just about 1% of its quirky features, is considered bad practice both by the C and the C++ communities.
The solution is now here: `becaps` allows you to write functions that are aware of their parent objects. The syntax is not identical to the "classic" `object.method()`, but is very close, and eliminates the need for specifying the object as a parameter (e.g. no `smash(mug)`, just `smash()`). This was achieved by abusing C macros and GCC extensions such as expression statements and empty structures. Additional benefits are the bonus ability to specify named parameters (`mug.changeColor(red)` can be called as `mug.changeColor(.color = red)`), and default parameters (those that aren't specified default to 0. Unfortunately, this value cannot be changed). The syntax for using `becaps`-powered user-defined types is as follows:
```c
Mug (mug).init(red)

ref (mug).smash()
```
The first line creates a variable of type `Mug`, with the name `mug`, and initializes it with the color `red`. We don't know what's going on inside the constructor (`init` function), we only pass the color and let it do the initializing. We don't have to know `mug`'s internal structure.
The second line calls the `smash` method of the `mug` variable. The general syntax is `ref (object).method(parameters)`.

### How does it even work?
In order to achieve such a thing, functions need knowledge about the object they are contained in. Replacing a function with a macro won't do the trick: it is impossible to pass the object into the function this way.
However, `becaps` defines the special `ref ()` macro, which essentially starts a block, defines a pointer to the object, and leaves space for a dot (`.`). The dot is provided by the user, and then the function is a macro that passes the pointer we defined earlier to the actual function, and then closes the block, which avoids all possible name collisions.
Let's see it in action one more time before we get into creating your own types.

### Another example
The following is a snippet from the test program in this repository.
```c
Vector2 (v1).init(3,  4);

ref (v1).setX(6);
ref (v1).setY(8);

double magnitude = ref (v1).magnitude();
printf("Magnitude is: %f\n", magnitude);
```
In order to understand exactly what is going on, a look should be taken at the preprocessed file:
```c
struct t_Vector2 v1 = { .funcs = & Vector2_funcs }; ({ typeof(v1) *self = &v1; (*self->funcs).init(self, (typeof(self->funcs->init_params)) { 3, 4 }); });

({ typeof(v1) *self = &v1; (*self->funcs).setX(self, (typeof(self->funcs->setX_params)) { 6 }); });
({ typeof(v1) *self = &v1; (*self->funcs).setY(self, (typeof(self->funcs->setY_params)) { 8 }); });

double magnitude = ({ typeof(v1) *self = &v1; (*self->funcs).magnitude(self, (typeof(self->funcs->magnitude_params)) { }); });

printf("Magnitude is: %f\n", magnitude);
```
The generated code is complex, and that is exactly what has driven the desire to automate the process of writing it. Let's take an even closer look.
The first line begins like this:
```c
struct t_Vector2 v1 =  {  .funcs =  & Vector2_funcs };
```
The global object `Vector2_funcs` contains pointers to all of the `Vector2`-related functions. It is necessary that the vector object points to it, because other types will have other sets of functions. The line continues as follows:
```c
({ typeof(v1) *self = &v1;
```
This starts a statement expression (the `({})` GNU extension. Read more about it [here](https://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html)), which can contain multiple statements, and evaluates to whatever its last statement evaluates. A pointer to the object is created inside, with the name `self`.The line ends like so:
```c
(*self->funcs).init(self, (typeof(self->funcs->init_params)) { 3, 4 }); });
```
Then, the `funcs` object of `self` is accessed. The `init` function of the object's `funcs` is called with the `self` pointer and with a newly constructed structure that contains the parameters needed for the function.

The statement expression is used for consistency with other functions, but in this case it is not needed, since no value is returned from the init function.

The next line is very similar to the first (except for the declaration which is not needed anymore).
The fourth line is also similar, except the `magnitude` function actually returns a value, and statement expressions are the only way to achieve this. The function call is placed last, so the entire `({})` construct will evaluate to whatever the function returns.

This system allows for many types to have functions with identical names but different parameters and behaviors.

## Creating your own types
Everything relies on the macros in the header file `becaps.h`. They are used to define and use custom types. A `funcs.h` file also has to exist, and it must declare all of the function names once, even if they belong to multiple types. An example can be seen in the `stream.h` and `stream.c` test files. Here are some snippets from both of them:

```c
// stream.h

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
```
This is the minimum required to define a type called `Stream`, that has a `FILE*` attribute, and three `void` functions (`init`, `setFile` and `write`).

The header file must first include the `funcs.h` header, which contains function name macros. Then, the type defined by the header file must be `TypeDecl`'d as seen on the third line of code, and the declaration construct must be #define-d as follows:
```c
// Replace "type" with the name of the custom type.
#define type(name) TypeConstruct(type, name)
```
After that, functions should be declared with the following syntax:
```c
// { parameters } has a struct-like syntax. A function that accepts one integer and one floating-point number would do { int x; double y; }, for example.
FuncDecl(type, return_type, function_name, { parameters });
```
Unfortunately, it is required to re-declare all functions inside the `MemberFuncs` construct, as seen above. However, the parameter list does not need to be specified again.

At the end, the type is defined by `TypeDef`, and all attributes can be specified here, in a struct-like syntax.

```c
// stream.c

#include "stream.h"

InitMemberFuncs(Stream,
    InitFunc(Stream, init),
    InitFunc(Stream, setFile),
    InitFunc(Stream, write),
);

FuncDef(Stream, void, init)  {
    self->file = NULL;
}

FuncDef(Stream, void, setFile)  {
    self->file = params.file;
}

FuncDef(Stream, void, write)  {
    fputs(params.string, self->file);
}
```

As can be seen, defining functions is fairly straightforward (`type, return_type, function_name`). The function has access to `self`, which is a pointer to the parent object, and to `params`, which is a struct that contains the parameters.

The source file must also declare all the functions again in the `InitMemberFuncs` construct at the top, which effectively creates the `funcs` object that instances are set to point to when they are created.

Finally, the `funcs.h` file must contain the following:
```c
// funcs.h

#include  "becaps.h"

#define setFile(...) FuncMacroDef(setFile, __VA_ARGS__)
#define write(...) FuncMacroDef(write, __VA_ARGS__)
```
These function macros do not need to know anything about their types or parameters. They only need to be declared like that so they can be replaced and close the blocks opened by `ref` as needed.

A program that wants to use the `Stream` datatype can now do so:
```c
Stream (stream).init();
ref (stream).setFile(stdout);
ref (stream).write("Writing from stream object!\n");

Stream (errStream).init();
ref (errStream).setFile(stderr);
ref (errStream).write("Error message goes here.\n");
```

This example constructs two separate `Stream` objects and calls their functions with different values.

## License
This project is MIT-licensed. Just take it. I don't need it, and I don't want to see it ever again.

Copyright (c) 2021 Cosmin James

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.