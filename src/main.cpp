// #include "Application.h"

// #include <iostream>

#include "Sweeper.h"
#include "MachineController.h"
#include "MachineRenderer.h"
#include <memory>
#include <thread>
#include <chrono>

int main(int argc, char **argv) {
    // Application* app = new Application();

    // std::cout << std::endl;

    // return app->run();

    // Создаем машину
    auto machine = std::make_shared<Sweeper>();
    
    // Создаем и настраиваем контроллер
    auto controller = std::make_shared<MachineController>(machine);
    
    // Создаем и настраиваем рендерер
    auto renderer = std::make_shared<MachineRenderer>(machine);
    renderer->createUI(800, 600);
    
    // Настраиваем callbacks
    // controller->setupWindowCallbacks(renderer->getWindow());
    
    // Запускаем машину
    machine->start();
    
    // Показываем окно
    renderer->getWindow()->show();
    
    // Главный цикл симуляции
    while (renderer->getWindow()->shown()) {
        Fl::check(); // Обрабатываем события FLTK
    

        // Обновляем машину
        machine->update(0.016f); // ~60 FPS
        
        // Небольшая задержка
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    machine->stop();
    return 0;
}