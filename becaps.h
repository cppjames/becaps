#ifndef BECAPS_H_
#define BECAPS_H_

#define TypeDecl(type) struct t_ ## type

#define TypeConstruct(type, name) struct t_ ## type name = { .funcs = & type ## _funcs }; ref(name)

#define FuncDecl(type, ret, func, ...) \
    struct t_ ## type ## _ ## func ## _params __VA_ARGS__; \
    ret t_ ## type ## _ ## func(struct t_ ## type *self, struct t_ ## type ## _ ## func ## _params params)

#define MemberFuncDecl(type, ret, func) \
    struct t_ ## type ## _ ## func ## _params func ## _params; \
    ret (*func)(struct t_ ## type *self, struct t_ ## type ## _ ## func ## _params params)

#define MemberFuncs(type, ...) \
    extern struct t_ ## type ## _funcs { __VA_ARGS__ } type ## _funcs

#define TypeDef(type, ...) struct t_ ## type { __VA_ARGS__ struct t_ ## type ## _funcs *funcs; }

#define FuncMacroDef(func, ...) \
    func(self, (typeof(self->funcs->func ## _params)) { __VA_ARGS__ }); })

#define InitMemberFuncs(type, ...) \
    struct t_ ## type ## _funcs type ## _funcs = { __VA_ARGS__ }

#define InitFunc(type, func) .func = t_ ## type ## _ ## func

#define FuncDef(type, ret, func) \
    ret t_ ## type ## _ ## func (struct t_ ## type *self, struct t_ ## type ## _ ## func ## _params params)

#define ref(obj) ({ typeof(obj) *self = &obj; self->funcs

#define init(...) FuncMacroDef(init, __VA_ARGS__)

#endif // BECAPS_H_