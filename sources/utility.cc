//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "utility.h"

std::string astrprintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    std::string string = vastrprintf(fmt, ap);
    va_end(ap);
    return string;
}

std::string vastrprintf(const char *fmt, va_list ap)
{
    char *str;
    if (vasprintf(&str, fmt, ap) == -1)
        throw std::bad_alloc();
    std::unique_ptr<char, Stdc_Deleter> strp(str);
    return str;
}
