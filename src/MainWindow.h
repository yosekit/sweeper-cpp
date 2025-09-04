#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include <FL/Fl_Window.H>
#include <functional>

class MainWindow : public Fl_Window {
public:
	MainWindow(int width, int height, const char* title = 0);
    ~MainWindow() override = default;

    int handle(int event) override;

    void onKeyDown(std::function<void(int)> callback) { keyDownCallback = callback; }
    void onKeyUp(std::function<void(int)> callback) { keyUpCallback = callback; }

private:
    std::function<void(int)> keyDownCallback;
    std::function<void(int)> keyUpCallback;
};

#endif