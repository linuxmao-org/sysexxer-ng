//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "midi.h"
#include "app_i18n.h"
#include <stdio.h>

std::unique_ptr<Midi_Interface> Midi_Interface::instance_;

Midi_Interface &Midi_Interface::instance()
{
    if (instance_)
        return *instance_;

    Midi_Interface *out = new Midi_Interface;
    instance_.reset(out);
    return *out;
}

Midi_Interface::Midi_Interface()
{
    switch_api(RtMidi::UNSPECIFIED);
}

Midi_Interface::~Midi_Interface()
{
}

RtMidi::Api Midi_Interface::current_api() const
{
    return input_client_->getCurrentApi();
}

void Midi_Interface::switch_api(RtMidi::Api api)
{
    if (input_client_ && api == input_client_->getCurrentApi())
        return;

    RtMidiIn *input_client = new RtMidiIn(api, _("Sysexxer In"), 512 * 1024);
    const bool ignoreSysex = false;
    input_client->ignoreTypes(ignoreSysex);
    input_client_.reset(input_client);
    input_client->setErrorCallback(&on_midi_error, this);
    input_client->setCallback(&on_midi_input, this);
    has_open_input_port_ = false;

    RtMidiOut *output_client = new RtMidiOut(api, _("Sysexxer Out"));
    output_client_.reset(output_client);
    output_client->setErrorCallback(&on_midi_error, this);
    has_open_output_port_ = false;
}

bool Midi_Interface::supports_virtual_port() const
{
    switch (current_api()) {
    case RtMidi::MACOSX_CORE: case RtMidi::LINUX_ALSA: case RtMidi::UNIX_JACK:
        return true;
    default:
        return false;
    }
}

std::vector<std::string> Midi_Interface::get_real_input_ports()
{
    RtMidiIn &client = *input_client_;
    unsigned count = client.getPortCount();

    std::vector<std::string> ports;
    ports.reserve(count);

    for (unsigned i = 0; i < count; ++i)
        ports.push_back(client.getPortName(i));
    return ports;
}

std::vector<std::string> Midi_Interface::get_real_output_ports()
{
    RtMidiOut &client = *output_client_;
    unsigned count = client.getPortCount();

    std::vector<std::string> ports;
    ports.reserve(count);

    for (unsigned i = 0; i < count; ++i)
        ports.push_back(client.getPortName(i));
    return ports;
}

void Midi_Interface::close_input_port()
{
    RtMidiIn &client = *input_client_;
    if (has_open_input_port_) {
        client.closePort();
        has_open_input_port_ = false;
    }
}

void Midi_Interface::close_output_port()
{
    RtMidiOut &client = *output_client_;
    if (has_open_output_port_) {
        client.closePort();
        has_open_output_port_ = false;
    }
}

void Midi_Interface::open_input_port(unsigned port)
{
    RtMidiIn &client = *input_client_;
    close_input_port();

    std::string name = _("Sysexxer MIDI in");
    if (port == ~0u) {
        client.openVirtualPort(name);
        has_open_input_port_ = true;
    }
    else {
        client.openPort(port, name);
        has_open_input_port_ = client.isPortOpen();
    }
}

void Midi_Interface::open_output_port(unsigned port)
{
    RtMidiOut &client = *output_client_;
    close_output_port();

    std::string name = _("Sysexxer MIDI out");
    if (port == ~0u) {
        client.openVirtualPort(name);
        has_open_output_port_ = true;
    }
    else {
        client.openPort(port, name);
        has_open_output_port_ = client.isPortOpen();
    }
}

void Midi_Interface::send_message(const uint8_t *data, size_t length)
{
    RtMidiOut &client = *output_client_;
    if (has_open_output_port_)
        client.sendMessage(data, length);
}

void Midi_Interface::on_midi_error(RtMidiError::Type type, const std::string &text, void *user_data)
{
    fprintf(stderr, "[Midi Out] %s\n", text.c_str());
}

void Midi_Interface::install_input_handler(input_handler *handler, void *user_data)
{
    std::lock_guard<std::mutex> lock(input_handlers_mutex_);
    input_handlers_.push_back(std::make_pair(handler, user_data));
}

void Midi_Interface::uninstall_input_handler(input_handler *handler, void *user_data)
{
    std::lock_guard<std::mutex> lock(input_handlers_mutex_);
    for (size_t i = 0, n = input_handlers_.size(); i < n; ++i) {
        bool match = input_handlers_[i].first == handler &&
            input_handlers_[i].second == user_data;
        if (match) {
            input_handlers_.erase(input_handlers_.begin() + i);
            return;
        }
    }
}

void Midi_Interface::on_midi_input(double time_stamp, std::vector<unsigned char> *message, void *user_data)
{
    Midi_Interface *self = reinterpret_cast<Midi_Interface *>(user_data);

    std::unique_lock<std::mutex> lock(self->input_handlers_mutex_, std::try_to_lock);
    if (!lock.owns_lock())
        return;

    const uint8_t *msg = message->data();
    size_t len = message->size();

    for (size_t i = 0, n = self->input_handlers_.size(); i < n; ++i)
        self->input_handlers_[i].first(msg, len, self->input_handlers_[i].second);
}
