#include "main.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

void button_callback(Fl_Widget* widget, void* data)
{
    printf("Hallo von FLTK!\n");
}

int main()
{
    Fl_Window window(400, 300, "Mein FLTK Fenster");

    Fl_Button button(150, 120, 100, 40, "Klick mich");
    button.callback(button_callback);

    window.end();
    window.show();

    return Fl::run();
}
