#include "MainWindow.h"
#include <iostream>

MainWindow::MainWindow(int width, int height, const char* title)
    : Fl_Window(width, height, title) {}

void MainWindow::setController(MachineController* controller) {
    if (controller) {
        this->controller = controller;
    }
}

int MainWindow::handle(int event) {
    std::cout << "MainWindow::handle(" << event << ")" << std::endl;
    
    switch (event) {
        case FL_KEYDOWN:
        case FL_SHORTCUT:
            if (controller) {
                return controller->handleKeyEvent(Fl::event_key());
            }
            break;
            
        case FL_FOCUS:
            return 1; // Принимаем фокус
            
        case FL_UNFOCUS:
            return 1; // Отдаем фокус
            
        default:
            break;
    }
    return Fl_Window::handle(event);
}