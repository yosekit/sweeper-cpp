#include "MachineRenderer.h"

#include <sstream>
#include <iomanip>

MachineRenderer::MachineRenderer(std::shared_ptr<Sweeper> machine) 
    : machine(machine), mainWindow(nullptr), sweeperWidget(nullptr), 
      statusLabel(nullptr), fuelLabel(nullptr) {
    machine->setRenderer(std::shared_ptr<MachineRenderer>(this));
}

MachineRenderer::~MachineRenderer() {
    // FLTK автоматически удаляет виджеты
}

void MachineRenderer::createUI(int width, int height) {
    mainWindow = new MainWindow(width, height, "Sweeper Simulator");

    mainWindow->onKeyDown([this](int key) {
        machine->getController()->handleKeyPress(key, true);
    });
    mainWindow->onKeyUp([this](int key) {
        machine->getController()->handleKeyPress(key, false);
    });

    mainWindow->begin();
    
    // Группа для машины
    machineGroup = new Fl_Group(50, 50, 300, 300);
    machineGroup->box(FL_FLAT_BOX);
    machineGroup->color(FL_WHITE);
    machineGroup->begin();
    createMachineVisual();
    machineGroup->end();

    createStatusPanel();

    mainWindow->end();
}

void MachineRenderer::createMachineVisual() {
    sweeperWidget = new SweeperWidget(175, 175, 50, 50);
    sweeperWidget->setColor(FL_WHITE);
}

void MachineRenderer::createStatusPanel() {
    statusLabel = new Fl_Box(400, 50, 200, 30, "Status: NONE");
    statusLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    
    fuelLabel = new Fl_Box(400, 90, 200, 30, "Fuel: 100%");
    fuelLabel->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
}

void MachineRenderer::update() {
    if (!machine || !mainWindow) return;

    updateMachineVisual();
    refreshUI();
}

void MachineRenderer::updateMachineVisual() {
    if (!sweeperWidget) return;

        // Обновляем позицию машины
    int centerX = (machineGroup->w() - 50) / 2;
    int centerY = (machineGroup->h() - 50) / 2;

    int posX = centerX + machine->getPositionX() * 10; // Масштабируем для видимости
    int posY = centerY + machine->getPositionY() * 10;

    sweeperWidget->position(posX, posY);
    sweeperWidget->setRotation(machine->getRotation());

    sweeperWidget->setColor(getStateColor(machine->getCurrentState()));
}

void MachineRenderer::refreshUI() {
    if (!machine || !statusLabel) return;
    
    // Обновляем статус
    std::stringstream status;
    status << "Status: " << machine->getCurrentStateAsString();
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
    
    mainWindow->redraw();
}

Fl_Color MachineRenderer::getStateColor(SweeperState state) const {
    switch (state) {
        case SweeperState::ACTIVATED: return FL_BLACK;
        case SweeperState::MOVING: {
            MovementDirection dir = machine->getMovementDirection();
            if (hasDirection(dir, MovementDirection::FORWARD))
                return FL_GREEN;
            if (hasDirection(dir, MovementDirection::BACKWARD))
                return FL_RED;
        };
        case SweeperState::BROKEN: return FL_YELLOW;
        default: return FL_WHITE;
    }
}