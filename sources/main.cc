//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "window.h"
#include "app_i18n.h"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl.H>
#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
#if ENABLE_NLS
    setlocale(LC_ALL, "");
    const char *locale_path = LOCALE_DIRECTORY "/";
    bindtextdomain("sysexxer-ng", locale_path);
    textdomain("sysexxer-ng");
#endif

    bool valid_args = true;
    const char *arg_filename = nullptr;

    for (int c; (c = getopt(argc, argv, "")) != -1;) {
        switch (c) {
        default:
            valid_args = false;
            break;
        }
    }

    if (valid_args) {
        switch (argc - optind) {
        case 1:
            arg_filename = argv[optind];
            break;
        case 0:
            break;
        default:
            valid_args = false;
            break;
        }
    }

    if (!valid_args) {
        fprintf(stderr, "%s\n", _("Invalid arguments"));
        return 1;
    }

    Fl_Double_Window frame(Main_Window::width, Main_Window::height, _("Sysexxer NG"));
    Main_Window contents(0, 0, Main_Window::width, Main_Window::height);
    frame.end();
    frame.show();

    if (arg_filename)
        contents.load_file(arg_filename);

    Fl::run();
    return 0;
}
