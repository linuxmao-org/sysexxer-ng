//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#include <string>
#include <vector>
#include <memory>

struct Sysex_Event {
    std::unique_ptr<uint8_t[]> data;
    size_t size;
    const char *manufacturer() const;
    std::string description() const;
};

enum class File_Format {
    System_Exclusive,
    Standard_Midi,
};

bool load_sysex_file(const char *filename, std::vector<Sysex_Event> &event_list);
bool load_sysex_data(const uint8_t *data, size_t size, std::vector<Sysex_Event> &event_list);

bool load_smf_file(const char *filename, std::vector<Sysex_Event> &event_list);
bool load_smf_data(const uint8_t *data, size_t size, std::vector<Sysex_Event> &event_list);
