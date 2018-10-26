//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "window.h"
#include "app_i18n.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>

int main()
{
    Fl_Double_Window frame(Main_Window::width, Main_Window::height, _("Sysexxer NG"));
    Main_Window contents(0, 0, Main_Window::width, Main_Window::height);
    frame.end();
    frame.show();

    Fl::run();
    return 0;
}
