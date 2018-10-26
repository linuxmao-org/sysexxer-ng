//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <RtMidi.h>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <stdint.h>

class Midi_Interface {
public:
    static Midi_Interface &instance();

    Midi_Interface();
    ~Midi_Interface();

    RtMidi::Api current_api() const;
    void switch_api(RtMidi::Api api);
    bool supports_virtual_port() const;
    std::vector<std::string> get_real_input_ports();
    std::vector<std::string> get_real_output_ports();

    void close_input_port();
    void close_output_port();
    void open_input_port(unsigned port);
    void open_output_port(unsigned port);

    typedef void (input_handler)(const uint8_t *, size_t, void *);
    void install_input_handler(input_handler *handler, void *user_data);
    void uninstall_input_handler(input_handler *handler, void *user_data);

    void send_message(const uint8_t *data, size_t length);

private:
    static void on_midi_error(RtMidiError::Type type, const std::string &text, void *user_data);
    static void on_midi_input(double time_stamp, std::vector<unsigned char> *message, void *user_data);

    std::unique_ptr<RtMidiIn> input_client_;
    std::unique_ptr<RtMidiOut> output_client_;
    bool has_open_input_port_ = false;
    bool has_open_output_port_ = false;
    std::mutex input_handlers_mutex_;
    std::vector<std::pair<input_handler *, void *>> input_handlers_;

    static std::unique_ptr<Midi_Interface> instance_;
};
