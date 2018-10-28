//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if ENABLE_NLS
#include <gettext.h>
#define _(x) ((const char *)gettext(x))
#define _P(c, x) ((const char *)pgettext(c, x))
#else
#define _(x) x
#define _P(c, x) x
#endif
