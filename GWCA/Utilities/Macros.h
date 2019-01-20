#pragma once

#define COUNT(array) (sizeof(array) / sizeof((array)[0]))


#define BIT_SET(var, data)       ((var) |=  (data))
#define BIT_UNSET(var, data)     ((var) &= ~(data))
#define BIT_TOGGLE(var, data)    ((var) ^=  (data))

#define BIT_CHECKANY(var, data) (((var) &   (data)) > 0)
#define BIT_CHECKALL(var, data) (((var) &   (data)) == (data))

#define OFFSET_OF(st, field) (unsigned long)(&((st*)nullptr)->field)

// Defining game classes to behave like normal classes
#define FOREIGN_MEMBER_FN(addr, fnName, retnType, cconv,  ...)                  \
    template <class... Params>                                                  \
    __forceinline retnType fnName(Params... params) {                           \
        struct empty_struct {};                                                 \
        typedef retnType(cconv empty_struct::*_##fnName##_type)(__VA_ARGS__);   \
        const static unsigned address = unsigned(addr);                         \
        _##fnName##_type fn = *(_##fnName##_type*)&address;                     \
        return (reinterpret_cast<empty_struct*>(this)->*fn)(params...);         \
    }

#define FOREIGN_FN(addr, fnName, retnType, cconv,  ...)                         \
    template <class... Params>                                                  \
    __forceinline retnType fnName(Params... params) {                           \
        typedef retnType(cconv _##fnName##_type)(__VA_ARGS__);                  \
        _##fnName##_type* fn = (_##fnName##_type*)addr;                         \
        return fn(params...);                                                   \
    }

#define FOREIGN_VAR(addr, varName, varType)                                     \
        varType* varName = (varType*) addr


#define FOREIGN_VAR_DECL(addr, varName, varType)                                        \
        extern varType* varName

#define STATIC_ASSERT(predicate) _x_compiler_assert_LINE(predicate,__LINE__)
#define _x_compiler_assert_LINE(predicate, line) typedef char constraint_violated_on_line_##line[2*((predicate)!=0)-1]
        