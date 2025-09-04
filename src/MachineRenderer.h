#ifndef MACHINERENDERER_H
#define MACHINERENDERER_H

#pragma once

#include "Sweeper.h"
#include "MainWindow.h"

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>

#include <memory>
#include <string>

class Sweeper; // Forward declaration


class MachineRenderer {
public:
    MachineRenderer(std::shared_ptr<Sweeper> machine);
    ~MachineRenderer();

    void createUI(int width, int height);
    void update();
    void refreshUI();
    
    Fl_Window* getWindow() const { return mainWindow; }

private:
    std::shared_ptr<Sweeper> machine;
    
    MainWindow* mainWindow;
    Fl_Group* machineGroup;
    Fl_Box* machineVisual;
    Fl_Box* statusLabel;
    Fl_Box* fuelLabel;
    Fl_Box* harvestLabel;
    
    void createMachineVisual();
    void createStatusPanel();

    void updateMachineVisual();

    Fl_Color getStateColor(SweeperState state) const;
};

#endif