//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "sysex_send.h"
#include "device/midi.h"
#include <FL/Fl.H>

static void on_sysex_send_timeout(void *user_data)
{
    Sysex_Send_Context &ctx = *(Sysex_Send_Context *)user_data;

    if (ctx.index == ctx.count)
        ctx.finished = true;
    if (ctx.finished) {
#warning TODO stuck in wait() until next event
        return;
    }

    const Sysex_Event &event = ctx.events[ctx.index++];

    Midi_Interface &midi = Midi_Interface::instance();
    midi.send_message(event.data.get(), event.size);

    double delay = event.size / ctx.rate;
    Fl::add_timeout(delay, &on_sysex_send_timeout, &ctx);
}

void sysex_send_begin(Sysex_Send_Context &ctx)
{
    ctx.index = 0;
    ctx.finished = false;
    Fl::add_timeout(0.0, &on_sysex_send_timeout, &ctx);
}

void sysex_send_abort(Sysex_Send_Context &ctx)
{
    ctx.finished = true;
    Fl::remove_timeout(&on_sysex_send_timeout, &ctx);
}
