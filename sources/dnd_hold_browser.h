//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <FL/Fl_Hold_Browser.H>

class Dnd_Hold_Browser : public Fl_Hold_Browser {
public:
    Dnd_Hold_Browser(int x, int y, int w, int h);
    int handle(int event) override;

    typedef void (*dnd_callback_t)(const char *, void *);
    void dnd_callback(dnd_callback_t cb, void *user_data = nullptr);

    void do_dnd_callback(const char *filename);

private:
    dnd_callback_t dnd_cb_ = nullptr;
    void *dnd_user_data_ = nullptr;
};
