#include <gtkmm.h>
#include <stdio.h>

#include "vodk/gfx/vectorizer.hpp"

class Canvas : public Gtk::DrawingArea {
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		printf("draw\n");

	}
};

struct Vectorizer {

  void vectorize() { vodk::gfx::vectorize(image, points); }

  void clear() { points.clear(); }

  vodk::gfx::ImageSurface* image;
  std::vector<glm::vec2> points;
};

int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
      "org.vodk.vectorizer");

  Gtk::ApplicationWindow window;

  Gtk::Box main_box(Gtk::ORIENTATION_VERTICAL, 0);

  Gtk::Frame header;
  header.set_size_request(-1, 32);
  header.get_style_context()->add_class(GTK_STYLE_CLASS_MENUBAR);
  Gtk::Box header_box(Gtk::ORIENTATION_HORIZONTAL);

  //Gtk::Toolbar toolbar;
  //toolbar.set_size_request(-1, 32);
  //toolbar.get_style_context()->add_class(GTK_STYLE_CLASS_MENUBAR);

  Gtk::Button button_clear("Clear");
  Gtk::Button button_vectorize("Vectorize");
  Gtk::Button button_tesselate("Tesselate");
  Gtk::Box cvt_box(Gtk::ORIENTATION_HORIZONTAL);
  cvt_box.get_style_context()->add_class(GTK_STYLE_CLASS_RAISED);
  cvt_box.get_style_context()->add_class(GTK_STYLE_CLASS_LINKED);
  cvt_box.add(button_clear);
  cvt_box.add(button_vectorize);
  cvt_box.add(button_tesselate);

  Gtk::Button button_open("Open");

  Gtk::Label name_label("Assets/Images/Foo.png");

  const int b_width = 50;
  const int b_height = 30;
  button_clear.set_size_request(b_width, b_height);
  button_vectorize.set_size_request(b_width, b_height);
  button_tesselate.set_size_request(b_width, b_height);
  button_open.set_size_request(b_width, b_height);

  //Gtk::ToolItem cvt_item;
  //Gtk::ToolItem name_item;
  //Gtk::ToolItem item_open;

  //item_open.add(button_open);
  //cvt_item.add(cvt_box);
  //name_item.add(name_label);

  header_box.pack_start(button_open, false, false, 0);
  header_box.pack_start(name_label, true, true, 0);
  header_box.pack_start(cvt_box, false, false, 0);
  header.add(header_box);

  Canvas drawing_area;
  main_box.pack_start(header, false, false, 0);
  main_box.pack_start(drawing_area, true, true, 0);

  window.add(main_box);
  window.set_default_size(800,600);
  window.set_title("vectorizer");

  main_box.show_all();
  button_vectorize.show();
  //item_vectorize.show();
  //toolbar.show();
  drawing_area.show();

  Vectorizer vectorizer;

  button_vectorize.signal_clicked().connect(
    sigc::mem_fun(vectorizer, &Vectorizer::vectorize)
  );

  return app->run(window);
  printf("bye\n");
}

