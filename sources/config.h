//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <SimpleIni.h>

namespace Config {

CSimpleIni &get_settings();
void save_settings();

}  // namespace Config
