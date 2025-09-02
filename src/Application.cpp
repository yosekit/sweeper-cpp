#include "Application.h"

Application::Application() {
    click_count = 0;

    main_window = new MainWindow(500, 500, "State Machine App");
    // window = new Fl_Window(400, 300, "My App");
    input = new Fl_Input(150, 50, 200, 30, "Name:");
    button = new Fl_Button(150, 100, 100, 40, "Press");
    label = new Fl_Box(150, 160, 200, 30, "Welcome");

    button->callback(button_callback, this);

    main_window->end();
}

int Application::run() {
    if (main_window) {
        main_window->show();
        return Fl::run();
    }
        
    return -1;
}

void Application::button_callback(Fl_Widget* w, void* data) {
    if (data) {
        Application* app = static_cast<Application*>(data);
        app->onButtonClicked();
    }
}

void Application::onButtonClicked() {
    click_count++;
    
    std::string text = "Click " + std::to_string(click_count);
    
    label->copy_label(text.c_str());
}