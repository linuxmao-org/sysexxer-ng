//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <FL/Fl_Button.H>

class Hyperlink_Button : public Fl_Button {
public:
    Hyperlink_Button(int x, int y, int w, int h, const char *l = nullptr);
    int handle(int event) override;
};
