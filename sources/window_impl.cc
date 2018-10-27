//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "window.h"
#include "system_exclusive.h"
#include "sysex_send.h"
#include "utility.h"
#include "app_i18n.h"
#include "device/midi.h"
#include "device/midi_apis.h"
#include <FL/Fl_Native_File_Chooser.H>
#include <ring_buffer.h>
#include <math.h>

struct Main_Window::Impl {
    Main_Window *Q = nullptr;
    std::vector<Sysex_Event> event_sendlist_;
    Fl_Text_Buffer txb_senddata_;
    std::vector<Sysex_Event> event_recvlist_;
    Fl_Text_Buffer txb_recvdata_;
    bool receive_first_ = false;
    std::unique_ptr<Ring_Buffer> receive_buffer_;
    std::vector<uint8_t> receive_tmpbuf_;
    std::string last_directory_;
    void init(Main_Window *Q);
    void on_load();
    void on_save();
    void on_send();
    bool do_load(const char *filename);
    bool do_save(const char *filename);
    void on_receive(bool enable);
    void on_change_midi_interface();
    void update_event_list_display(int mode);
    void update_event_data_display(int mode);
    bool handle_midi_input_message(const uint8_t *msg, size_t length);
    static void handle_realtime_midi_input(const uint8_t *msg, size_t length, void *user_data);
    static void handle_midi_input_buffer(void *user_data);
};

static const double midi_input_check_interval = 0.05;

void Main_Window::Impl::init(Main_Window *Q)
{
    this->Q = Q;

    receive_buffer_.reset(new Ring_Buffer(512 * 1024));

    Midi_Interface &midi = Midi_Interface::instance();
    midi.open_input_port(~0u);
    midi.open_output_port(~0u);

    for (size_t i = 0, n = compiled_midi_api_count(); i < n; ++i) {
        RtMidi::Api api = compiled_midi_api_by_index(i);
        const char *name = midi_api_name(api);
        Q->cb_midi_interface->add(name);
    }
    Q->cb_midi_interface->value(compiled_midi_api_index(midi.current_api()));

    Q->val_sendrate->range(1, 100);
    Q->val_sendrate->step(1);
    Q->val_sendrate->value(10);
    Q->val_sendrate->do_callback();

    Q->pb_send->hide();
    Q->lbl_recv->hide();
}

void Main_Window::Impl::on_load()
{
    Fl_Native_File_Chooser fnfc;
    fnfc.title(_("Load"));
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.filter(_("System-exclusive dump\t*.syx"));
    if (!last_directory_.empty())
        fnfc.directory(last_directory_.c_str());

    if (fnfc.show() != 0)
        return;

    const char *filename = fnfc.filename();
    do_load(filename);

    size_t filenamepos = strlen(filename);
    while (filenamepos > 0 && !is_path_separator(filename[filenamepos - 1]))
        --filenamepos;
    last_directory_.assign(filename, filenamepos);
}

void Main_Window::Impl::on_save()
{
    const std::vector<Sysex_Event> &event_list = event_recvlist_;
    if (event_list.empty())
        return;

    Fl_Native_File_Chooser fnfc;
    fnfc.title(_("Save"));
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.filter(_("System-exclusive dump\t*.syx"));
    if (!last_directory_.empty())
        fnfc.directory(last_directory_.c_str());

    if (fnfc.show() != 0)
        return;

    std::string filename = fnfc.filename();
    const char *ext = fl_filename_ext(filename.c_str());
    filename.append((ext && *ext) ? "" : ".syx");

    if (fl_access(filename.c_str(), 0) == 0) {
        fl_message_title(_("Confirm overwrite"));
        if (fl_choice("%s", _("No"), _("Yes"), nullptr, _("The file already exists. Replace it?")) != 1)
            return;
    }

    do_save(filename.c_str());

    size_t filenamepos = filename.size();
    while (filenamepos > 0 && !is_path_separator(filename[filenamepos - 1]))
        --filenamepos;
    last_directory_.assign(filename, filenamepos);
}

bool Main_Window::Impl::do_load(const char *filename)
{
    std::vector<Sysex_Event> event_sendlist;
    if (!load_sysex_file(filename, event_sendlist))
        return false;

    event_sendlist_ = std::move(event_sendlist);
    update_event_list_display(1);
    Q->br_sendlist->value(1);
    update_event_data_display(1);

    return true;
}

bool Main_Window::Impl::do_save(const char *filename)
{
    FILE_u fh(fl_fopen(filename, "wb"));
    if (!fh)
        return false;

    const std::vector<Sysex_Event> &event_list = event_recvlist_;
    for (const Sysex_Event &event : event_list)
        fwrite(event.data.get(), event.size, 1, fh.get());

    if (fflush(fh.get()) != 0) {
        fl_unlink(filename);
        return false;
    }

    return true;
}

void Main_Window::Impl::on_send()
{
    Sysex_Send_Context ctx;
    ctx.events = event_sendlist_.data();
    ctx.count = event_sendlist_.size();
    ctx.rate = Q->val_sendrate->value() * (1000.0 / 8.0);  // byte/s

    if (ctx.count == 0)
        return;

    Q->tab_recv->deactivate();
    Q->tab_options->deactivate();
    Q->tabs->redraw();

    Q->btn_load->deactivate();
    Fl_Callback_p send_cb = Q->btn_send->callback();
    void *send_arg = (void *)(fl_intptr_t)Q->btn_send->argument();
    Q->btn_send->callback(
        [](Fl_Widget *, void *user_data)
            { sysex_send_abort(*(Sysex_Send_Context *)user_data); }, &ctx);
    const char *send_label = Q->btn_send->label();
    Q->btn_send->label(_("@undo  Cancel"));

    sysex_send_begin(ctx);
    Q->pb_send->minimum(0);
    Q->pb_send->maximum(ctx.count);
    Q->pb_send->value(0);
    Q->pb_send->label("0%");
    Q->pb_send->show();
    while (!ctx.finished) {
        if (!Fl::wait())
            exit(0);
        Q->pb_send->copy_label((std::to_string(lround(100.0 * ctx.index / ctx.count)) + '%').c_str());
        Q->pb_send->value(ctx.index);
    }
    Q->pb_send->hide();

    Q->btn_load->activate();
    Q->btn_send->callback(send_cb, send_arg);
    Q->btn_send->label(send_label);

    Q->tab_recv->activate();
    Q->tab_options->activate();
    Q->tabs->redraw();
}

void Main_Window::Impl::on_receive(bool enable)
{
    Midi_Interface &midi = Midi_Interface::instance();
    Ring_Buffer &receive_buffer = *receive_buffer_;

    if (enable) {
        receive_first_ = true;
        receive_buffer.discard(receive_buffer.size_used());
        midi.install_input_handler(&handle_realtime_midi_input, this);
        Fl::add_timeout(midi_input_check_interval, &handle_midi_input_buffer, this);
    }
    else {
        Fl::remove_timeout(&handle_midi_input_buffer, this);
        midi.uninstall_input_handler(&handle_realtime_midi_input, this);
    }
}

void Main_Window::Impl::on_change_midi_interface()
{
    RtMidi::Api api = compiled_midi_api_by_index(Q->cb_midi_interface->value());
    if (api == RtMidi::Api::UNSPECIFIED)
        return;

    Midi_Interface &midi = Midi_Interface::instance();
    midi.switch_api(api);
    midi.open_input_port(~0u);
    midi.open_output_port(~0u);
}

void Main_Window::Impl::update_event_list_display(int mode)
{
    const std::vector<Sysex_Event> &event_list = mode ? event_sendlist_ : event_recvlist_;
    Fl_Text_Buffer &txb = mode ? txb_senddata_ : txb_recvdata_;
    Fl_Browser &br = mode ? *Q->br_sendlist : *Q->br_recvlist;

    br.clear();
    for (size_t i = 0, n = event_list.size(); i < n; ++i) {
        const Sysex_Event &event = event_list[i];
        std::string text = event.description();
        br.add(text.c_str());
    }
}

void Main_Window::Impl::update_event_data_display(int mode)
{
    const std::vector<Sysex_Event> &event_list = mode ? event_sendlist_ : event_recvlist_;
    Fl_Text_Buffer &txb = mode ? txb_senddata_ : txb_recvdata_;
    Fl_Browser &br = mode ? *Q->br_sendlist : *Q->br_recvlist;
    Fl_Text_Display &txt = mode ? *Q->txt_senddata : *Q->txt_recvdata;

    unsigned index = br.value() - 1;
    if ((int)index == -1)
        return;

    const Sysex_Event &event = event_list[index];
    const uint8_t *data = event.data.get();
    size_t size = event.size;

    std::string text;
    const char digits[] = "0123456789ABCDEF";

    for (size_t i = 0; i < size; ++i) {
        uint8_t byte = data[i];
        char bytetext[3] = { digits[byte >> 4], digits[byte & 0xf] };
        if (i > 0)
            text += (i & 15) ? ' ' : '\n';
        if ((i & 15) == 8)
            text += ' ';
        text += bytetext;
    }

    txt.buffer(txb);
    txb.text(text.c_str());
}

bool Main_Window::Impl::handle_midi_input_message(const uint8_t *msg, size_t length)
{
    std::vector<Sysex_Event> &event_recvlist = event_recvlist_;

    if (length < 2 || msg[0] != 0xf0 || msg[length - 1] != 0xf7)
        return false;

    if (receive_first_) {
        event_recvlist.clear();
        receive_first_ = false;
    }

    Sysex_Event event;
    event.data.reset(new uint8_t[length]);
    event.size = length;
    memcpy(event.data.get(), msg, length);

    event_recvlist.push_back(std::move(event));
    return true;
}

void Main_Window::Impl::handle_realtime_midi_input(const uint8_t *msg, size_t length, void *user_data)
{
    Impl *P = (Impl *)user_data;
    Ring_Buffer &receive_buffer = *P->receive_buffer_;

    if (receive_buffer.size_free() < sizeof(length) + length)
        return;

    receive_buffer.put(length);
    receive_buffer.put(msg, length);
}

void Main_Window::Impl::handle_midi_input_buffer(void *user_data)
{
    Impl *P = (Impl *)user_data;
    Ring_Buffer &receive_buffer = *P->receive_buffer_;
    std::vector<uint8_t> &receive_tmpbuf = P->receive_tmpbuf_;

    size_t length;
    bool update = false;
    while (receive_buffer.peek(length) &&
           receive_buffer.size_used() >= sizeof(length) + length) {
        receive_buffer.discard(sizeof(length));
        uint8_t *msg = (receive_tmpbuf.resize(length), receive_tmpbuf.data());
        receive_buffer.get(msg, length);
        update = P->handle_midi_input_message(msg, length);
    }

    if (update) {
        Main_Window *Q = P->Q;
        std::vector<Sysex_Event> &event_recvlist = P->event_recvlist_;
        P->update_event_list_display(0);
        P->update_event_data_display(0);
        Q->lbl_recv->show();
        Q->lbl_recv->copy_label(std::to_string(event_recvlist.size()).c_str());
    }

    Fl::repeat_timeout(midi_input_check_interval, &handle_midi_input_buffer, user_data);
}
