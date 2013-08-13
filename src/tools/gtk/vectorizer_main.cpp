#include <gtkmm.h>
#include <stdio.h>

#include "vodk/gfx/vectorizer.hpp"
#include "vodk/core/Blob.hpp"
#include "vodk/parser/png.hpp"
#include "vodk/gfx/ImageSurface.hpp"

struct Vectorizer {

  void vectorize() { vodk::gfx::vectorize(image, points); }

  void clear() { points.clear(); }

  vodk::gfx::ImageSurface* image;
  std::vector<glm::vec2> points;
};

class Canvas : public Gtk::DrawingArea {
public:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override {
		printf("draw\n");

    vodk::gfx::ImageSurface* image = vectorizer->image;

    Cairo::RefPtr<Cairo::Surface> surface =
      Cairo::ImageSurface::create(image->get_data(),
                                  Cairo::FORMAT_ARGB32,
                                  image->get_size().width,
                                  image->get_size().height,
                                  image->get_stride());

    cr->set_source_rgb(1.0, 1.0, 1.0);
    cr->paint();

    cr->set_source(surface, 0, 0);
    cr->paint();

    cr->set_source_rgb(0.6,0.0,0.0);
    for (auto p : vectorizer->points) {
      cr->rectangle(p.x - 3, p.y - 3, 6 , 6);
      cr->fill();
    }
	}

  Vectorizer* vectorizer;
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
  drawing_area.show();

  Vectorizer vectorizer;

  vodk::Blob src_blob;
  const char* image_src = "assets/img/test2.png";

  vodk::gfx::ImageSurface* image = new vodk::gfx::ImageSurface;
  vodk::parser::PNGImage png;
  if (png.load_from_file(image_src)) {
    png.copy_into(*image);
  } else {
    printf("could not load file %s\n", image_src);
  }

  vectorizer.image = image;
  drawing_area.vectorizer = &vectorizer;

  button_vectorize.signal_clicked().connect(
    sigc::mem_fun(vectorizer, &Vectorizer::vectorize)
  );

  return app->run(window);
}

