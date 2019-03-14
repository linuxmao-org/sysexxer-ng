//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "hyperlink_button.h"
#include <FL/Fl_Window.H>

Hyperlink_Button::Hyperlink_Button(int x, int y, int w, int h, const char *l)
    : Fl_Button(x, y, w, h, l)
{
}

int Hyperlink_Button::handle(int event)
{
    if (event == FL_ENTER) {
        Fl_Window *win = window();
        win->cursor(FL_CURSOR_HAND);
    }
    else if (event == FL_LEAVE) {
        Fl_Window *win = window();
        win->cursor(FL_CURSOR_DEFAULT);
    }

    return Fl_Button::handle(event);
}
