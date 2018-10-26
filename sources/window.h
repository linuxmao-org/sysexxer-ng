// generated by Fast Light User Interface Designer (fluid) version 1.0304

#ifndef window_h
#define window_h
#include <FL/Fl.H>
#include <FL/Fl_Hold_Browser.H>
#include <memory>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Slider.H>

class Main_Window : public Fl_Group {
public:
  Main_Window(int X, int Y, int W, int H, const char *L = 0);
  static int const width = 450, height = 250; 
  Fl_Tabs *tabs;
  Fl_Group *tab_send;
  Fl_Button *btn_load;
private:
  inline void cb_btn_load_i(Fl_Button*, void*);
  static void cb_btn_load(Fl_Button*, void*);
public:
  Fl_Button *btn_send;
private:
  inline void cb_btn_send_i(Fl_Button*, void*);
  static void cb_btn_send(Fl_Button*, void*);
public:
  Fl_Hold_Browser *br_sendlist;
private:
  inline void cb_br_sendlist_i(Fl_Hold_Browser*, void*);
  static void cb_br_sendlist(Fl_Hold_Browser*, void*);
public:
  Fl_Text_Display *txt_senddata;
  Fl_Progress *pb_send;
  Fl_Group *tab_recv;
  Fl_Button *btn_receive;
private:
  inline void cb_btn_receive_i(Fl_Button*, void*);
  static void cb_btn_receive(Fl_Button*, void*);
public:
  Fl_Button *btn_save;
private:
  inline void cb_btn_save_i(Fl_Button*, void*);
  static void cb_btn_save(Fl_Button*, void*);
public:
  Fl_Hold_Browser *br_recvlist;
private:
  inline void cb_br_recvlist_i(Fl_Hold_Browser*, void*);
  static void cb_br_recvlist(Fl_Hold_Browser*, void*);
public:
  Fl_Text_Display *txt_recvdata;
  Fl_Box *lbl_recv;
  Fl_Group *tab_options;
  Fl_Choice *cb_midi_interface;
private:
  inline void cb_cb_midi_interface_i(Fl_Choice*, void*);
  static void cb_cb_midi_interface(Fl_Choice*, void*);
public:
  Fl_Slider *val_sendrate;
private:
  inline void cb_val_sendrate_i(Fl_Slider*, void*);
  static void cb_val_sendrate(Fl_Slider*, void*);
public:
  Fl_Box *lbl_sendrate;
private:
  struct Impl;
  std::unique_ptr<Impl> P; 
public:
  ~Main_Window();
};
#endif
