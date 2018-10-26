//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "midi_apis.h"
#include <string.h>

static const char *g_midi_api_ids[] = {
    "unspecified",
    "core",
    "alsa",
    "jack",
    "mm",
    "dummy"
};

static const char *g_midi_api_names[] = {
    "Unspecified",
    "Core MIDI",
    "ALSA",
    "JACK",
    "Windows Multimedia",
    "Dummy"
};

static constexpr size_t g_midi_api_count =
    sizeof(g_midi_api_ids) / sizeof(g_midi_api_ids[0]);

size_t midi_api_count()
{
    return g_midi_api_count;
}

const char *midi_api_id(RtMidi::Api api)
{
    return (api < g_midi_api_count) ? g_midi_api_ids[api] : nullptr;
}

const char *midi_api_name(RtMidi::Api api)
{
    return (api < g_midi_api_count) ? g_midi_api_names[api] : nullptr;
}

RtMidi::Api find_midi_api(const char *id)
{
    for (size_t i = 0; i < g_midi_api_count; ++i)
        if (!strcmp(id, g_midi_api_ids[i]))
            return (RtMidi::Api)i;
    return RtMidi::UNSPECIFIED;
}

bool is_compiled_midi_api(RtMidi::Api api)
{
    std::vector<RtMidi::Api> apis;
    RtMidi::getCompiledApi(apis);
    for (RtMidi::Api current : apis)
        if (api == current)
            return true;
    return false;
}

size_t compiled_midi_api_count()
{
    size_t count = 0;
    for (size_t i = 0; i < g_midi_api_count; ++i)
        count += is_compiled_midi_api((RtMidi::Api)i) ? 1 : 0;
    return count;
}

size_t compiled_midi_api_index(RtMidi::Api api)
{
    if (!is_compiled_midi_api(api))
        return ~(size_t)0;
    size_t index = 0;
    for (size_t i = 0; i < (size_t)api; ++i)
        index += is_compiled_midi_api((RtMidi::Api)i) ? 1 : 0;
    return index;
}

RtMidi::Api compiled_midi_api_by_index(size_t index)
{
    for (size_t i = 0; i < g_midi_api_count; ++i) {
        if (is_compiled_midi_api((RtMidi::Api)i)) {
            if (index-- == 0)
                return (RtMidi::Api)i;
        }
    }
    return RtMidi::UNSPECIFIED;
}
