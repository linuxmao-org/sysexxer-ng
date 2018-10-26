//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <memory>
#include <type_traits>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

struct FILE_Deleter {
    void operator()(FILE *x) { fclose(x); }
};
typedef std::unique_ptr<FILE, FILE_Deleter> FILE_u;

struct Stdc_Deleter {
    void operator()(void *x) { free(x); }
};

__attribute__((format(printf, 1, 2)))
std::string astrprintf(const char *fmt, ...);
std::string vastrprintf(const char *fmt, va_list ap);
