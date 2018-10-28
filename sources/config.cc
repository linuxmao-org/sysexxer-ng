//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "config.h"
#include <FL/fl_utf8.h>
#include <memory>
#include <stdlib.h>

namespace Config {

static std::unique_ptr<CSimpleIni> settings_;

static const std::string &user_home_directory()
{
    static std::string home = getenv("HOME");
    return home;
}

static std::string user_config_file()
{
    return user_home_directory() +
        "/.config/linuxmao.org/sysexxer-ng/settings.ini";
}

CSimpleIni &get_settings()
{
    CSimpleIni *settings = settings_.get();
    if (settings)
        return *settings;

    settings = new CSimpleIni;
    settings_.reset(settings);

    settings->LoadFile(user_config_file().c_str());
    return *settings;
}

void save_settings()
{
    CSimpleIni *settings = settings_.get();
    if (!settings)
        return;

    std::string path = user_config_file();
    fl_make_path_for_file(path.c_str());
    settings->SaveFile(path.c_str());
}

}  // namespace Config
