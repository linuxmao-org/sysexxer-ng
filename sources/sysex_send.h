//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include "system_exclusive.h"

struct Sysex_Send_Context {
    const Sysex_Event *events = nullptr;
    size_t count = 0;
    size_t index = 0;
    bool finished = false;
    double rate = 0.0;
};

void sysex_send_begin(Sysex_Send_Context &ctx);
void sysex_send_abort(Sysex_Send_Context &ctx);
