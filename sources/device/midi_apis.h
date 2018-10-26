//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <RtMidi.h>

size_t midi_api_count();
const char *midi_api_id(RtMidi::Api api);
const char *midi_api_name(RtMidi::Api api);
RtMidi::Api find_midi_api(const char *id);
bool is_compiled_midi_api(RtMidi::Api api);
size_t compiled_midi_api_count();
size_t compiled_midi_api_index(RtMidi::Api api);
RtMidi::Api compiled_midi_api_by_index(size_t index);
