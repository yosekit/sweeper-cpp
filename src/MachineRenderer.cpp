#include "MachineRenderer.h"

#include <sstream>
#include <iomanip>

MachineRenderer::MachineRenderer(std::shared_ptr<HarvestingMachine> machine) 
    : machine(machine), mainWindow(nullptr), machineVisual(nullptr), 
      statusLabel(nullptr), fuelLabel(nullptr), harvestLabel(nullptr) {
    machine->setRenderer(std::shared_ptr<MachineRenderer>(this));
}

MachineRenderer::~MachineRenderer() {
    // FLTK автоматически удаляет виджеты
}

void MachineRenderer::createUI(int width, int height) {
    mainWindow = new MainWindow(width, height, "Harvesting Machine Simulator");
    mainWindow->setController(machine->getController()); // TODO

    mainWindow->begin();
    
    // Группа для машины
    machineGroup = new Fl_Group(50, 50, 300, 300);
    machineGroup->box(FL_FLAT_BOX);
    machineGroup->color(FL_WHITE);
    
    createMachineVisual();
    createStatusPanel();
    
    machineGroup->end();
    mainWindow->end();
}

void MachineRenderer::createMachineVisual() {  
    machineVisual = new Fl_Box(175, 175, 50, 50);
    machineVisual->box(FL_OVAL_BOX);
    machineVisual->color(FL_BLUE);
    machineVisual->labelcolor(FL_WHITE);
    machineVisual->labelsize(12);
}

void MachineRenderer::createStatusPanel() {
    statusLabel = new Fl_Box(400, 50, 200, 30, "Status: IDLE");
    statusLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    fuelLabel = new Fl_Box(400, 90, 200, 30, "Fuel: 100%");
    fuelLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    harvestLabel = new Fl_Box(400, 130, 200, 30, "Harvested: 0 kg");
    harvestLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
}

void MachineRenderer::update(float posX, float posY, float rotation) {
    if (machineVisual) {
        machineVisual->position(150 + posX, 150 + posY);
        // Здесь можно добавить вращение и другую визуализацию
    }
    refreshUI();
}

void MachineRenderer::refreshUI() {
    if (!machine || !statusLabel) return;
    
    // Обновляем статус
    MachineState state = machine->getCurrentState();
    std::stringstream status;
    status << "Status: " << machineStateToString(state);
    statusLabel->copy_label(status.str().c_str());
    statusLabel->labelcolor(FL_BLACK);
    
    // Обновляем топливо
    std::stringstream fuel;
    fuel << "Fuel: " << std::fixed << std::setprecision(1) 
         << machine->getFuelLevel() << "%";
    fuelLabel->copy_label(fuel.str().c_str());
    
    // Цвет топлива в зависимости от уровня
    if (machine->getFuelLevel() < 20.0) {
        fuelLabel->labelcolor(FL_RED);
    } else if (machine->getFuelLevel() < 50.0) {
        fuelLabel->labelcolor(FL_YELLOW);
    } else {
        fuelLabel->labelcolor(FL_DARK_GREEN);
    }
    
    // Обновляем урожай
    std::stringstream harvest;
    harvest << "Harvested: " << std::fixed << std::setprecision(1) 
            << machine->getHarvestedAmount() << " kg";
    harvestLabel->copy_label(harvest.str().c_str());
    
    // Обновляем цвет машины в зависимости от состояния
    if (machineVisual) {
        std::string color = getStateColor(state);
        if (color == "red") machineVisual->color(FL_RED);
        else if (color == "green") machineVisual->color(FL_GREEN);
        else if (color == "blue") machineVisual->color(FL_BLUE);
        else if (color == "yellow") machineVisual->color(FL_YELLOW);
        else machineVisual->color(FL_BLUE);
        
        machineVisual->redraw();
    }
    
    mainWindow->redraw();
}

std::string MachineRenderer::getStateColor(MachineState state) const {
    switch (state) {
        case MachineState::IDLE: return "gray";
        case MachineState::MOVING_FORWARD: return "green";
        case MachineState::MOVING_BACKWARD: return "yellow";
        case MachineState::HARVESTING: return "blue";
        case MachineState::UNLOADING: return "cyan";
        case MachineState::BROKEN: return "red";
        default: return "white";
    }
}