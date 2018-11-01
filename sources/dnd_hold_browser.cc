//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "dnd_hold_browser.h"
#include <FL/filename.H>
#include <string>
#include <memory>
#include <string.h>

Dnd_Hold_Browser::Dnd_Hold_Browser(int x, int y, int w, int h)
    : Fl_Hold_Browser(x, y, w, h)
{
}

int Dnd_Hold_Browser::handle(int event)
{
    switch (event) {
    case FL_DND_ENTER:
    case FL_DND_DRAG:
    case FL_DND_RELEASE:
        return 1;
    case FL_PASTE: {
        size_t length = Fl::event_length();
        std::unique_ptr<char[]> buf(new char[length + 1]);

        memcpy(buf.get(), Fl::event_text(), length);
        buf[length] = 0;

        if (char *p = (char *)memchr(buf.get(), '\n', length)) {
            length = p - buf.get();
            *p = 0;
        }

        if (length >= 7 && !memcmp(buf.get(), "file://", 7)) {
            fl_decode_uri(buf.get());
            const char *filename = buf.get() + 7;
            do_dnd_callback(filename);
        }

        return 1;
    }
    default:
        break;
    }

    return Fl_Hold_Browser::handle(event);
}

void Dnd_Hold_Browser::dnd_callback(dnd_callback_t cb, void *user_data)
{
    dnd_cb_ = cb;
    dnd_user_data_ = user_data;
}

void Dnd_Hold_Browser::do_dnd_callback(const char *filename)
{
    if (dnd_cb_)
        dnd_cb_(filename, dnd_user_data_);
}
