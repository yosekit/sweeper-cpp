#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height, const char* title)
    : Fl_Window(width, height, title) {}

int MainWindow::handle(int event) {
    switch (event) {
        case FL_KEYDOWN:
            keyDownCallback(Fl::event_key());
            return 1;
            
        case FL_KEYUP:
            keyUpCallback(Fl::event_key());
            return 1;
            
        case FL_FOCUS:
            return 1; // Принимаем фокус
            
        case FL_UNFOCUS:
            
            return 1; // Отдаем фокус
            
        default:
            break;
    }
    return Fl_Window::handle(event);
}