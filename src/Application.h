#ifndef APPLICATION_H
#define APPLICATION_H

#pragma once

#include "MainWindow.h"
// #include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>

#include <string>

class Application {
private:
    int click_count;

    MainWindow* main_window;
    // Fl_Window *window;
    Fl_Input* input;
    Fl_Button* button;
    Fl_Box* label;

    static void button_callback(Fl_Widget* w, void* data);
    void onButtonClicked();

public:
    Application();
    ~Application();

    int run();
};

#endif