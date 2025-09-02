#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once

#include "MachineController.h"
#include <FL/Fl_Window.H>

class MainWindow : public Fl_Window {
public:
	MainWindow(int width, int height, const char* title = 0);
    ~MainWindow() override = default;

    void setController(MachineController* controller);
    int handle(int event) override;

private:
    MachineController* controller = nullptr;
};

#endif