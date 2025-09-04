cmake_minimum_required(VERSION 3.10)
project(Sweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Явно указываем компиляторы
set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
set(CMAKE_MAKE_PROGRAM "C:/msys64/mingw64/bin/mingw32-make.exe")

# Пути для FLTK
set(FLTK_INCLUDE_DIR "C:/msys64/mingw64/include")
set(FLTK_LIBRARY_DIR "C:/msys64/mingw64/lib")

# Include директории - ВАЖНО: добавляем текущую директорию src
include_directories(
    ${FLTK_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/src  # Это добавит src/ в include path
)

# Все исходные файлы - ВАЖНО: добавляем ВСЕ .cpp файлы
set(SOURCES
    src/main.cpp
    src/Application.cpp
    src/MainWindow.cpp
    src/Sweeper.cpp
    src/MachineController.cpp
    src/MachineRenderer.cpp
    src/InputSystem.cpp
    src/InputAction.cpp
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Линковка
target_link_libraries(${PROJECT_NAME}
    fltk
    ole32
    uuid
    comctl32
    gdi32
)

# Флаги для Windows
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()

# Вывод информации для отладки
message(STATUS "Source files: ${SOURCES}")
message(STATUS "Include directories: ${CMAKE_CURRENT_SOURCE_DIR}")