
#include <clutter-gtkmm.h>
#include <gtkmm.h>
#include <stdio.h>
#include <iostream>

namespace
{

class StageWindow : public Gtk::Window
{
public:
  StageWindow();
  virtual ~StageWindow() {};

private:
  Clutter::Gtk::Embed* embed;
  Glib::RefPtr<Clutter::Stage> stage;
  bool colored;

  void on_button_clicked();
  static bool on_stagebutton_press(Clutter::ButtonEvent* event);
};

StageWindow::StageWindow()
: embed(0), colored(false)
{
  Gtk::Box *const box = new Gtk::VBox(false, 6);
  add(*Gtk::manage(box));

  Gtk::Button *const button = new Gtk::Button("Change color");
  box->pack_end(*Gtk::manage(button), Gtk::PACK_SHRINK);

  embed = new Clutter::Gtk::Embed();
  box->pack_start(*Gtk::manage(embed), Gtk::PACK_EXPAND_WIDGET);
  embed->set_size_request(200, 200);

  button->signal_clicked().connect(sigc::mem_fun(*this, &StageWindow::on_button_clicked));

  stage = embed->get_stage();
  stage->reference();
  stage->set_color(Clutter::Color(0, 0, 0)); // black
  stage->signal_button_press_event().connect(&StageWindow::on_stagebutton_press);

  show_all();
  stage->show();
}

void StageWindow::on_button_clicked() {
  colored = !colored;
  stage->set_color((colored) ? Clutter::Color(32, 32, 160)
    : Clutter::Color(0, 0, 0));
}

bool StageWindow::on_stagebutton_press(Clutter::ButtonEvent* event) {
  float x = 0.0;
  float y = 0.0;
  // TODO: Wrap properly
  clutter_event_get_coords(reinterpret_cast<Clutter::Event*>(event), &x, &y);

  std::cout << "Stage clicked at (" << x << ", " << y << ")\n";

  return true; // stop further handling of this event
}

} // anonymous namespace

int main(int argc, char** argv) {
  try {
    Clutter::Gtk::init(&argc, &argv);
    Gtk::Main kit (&argc, &argv);
    StageWindow window;
    Gtk::Main::run(window);
  } catch (const Glib::Error& error) {
    std::cerr << "Exception: " << error.what() << std::endl;
    return 1;
  }
  return 0;
}