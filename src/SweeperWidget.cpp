#include "SweeperWidget.h"
#include <iostream>
#include <cmath>
#include <numbers>

const double pi = 3.14159265358979323846;

SweeperWidget::SweeperWidget(int x, int y, int w, int h, const char* label) 
    : Fl_Widget(x, y, w, h, label) {}

void SweeperWidget::setRotation(float angle) {
    rotation = angle - 90.0f;
    redraw();
}

void SweeperWidget::setColor(Fl_Color newColor) {
    color = newColor;
    redraw();
}

void SweeperWidget::setSweeping(bool sweeping) {
    isSweeping = sweeping;
    redraw();
}

void SweeperWidget::setWashing(bool washing) {
    isWashing = washing;
    redraw();
}

void SweeperWidget::draw() {
    // Очищаем область
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());
    
    // Центр виджета
    int centerX = x() + w() / 2;
    int centerY = y() + h() / 2;
    
    // Рисуем машинку (повернутый квадрат)
    drawRotatedRectangle(centerX, centerY, 40, 20, rotation, color);
    
    // Рисуем лобовое стекло (меньший квадрат спереди)
    float frontAngle = rotation * (pi / 180.0f);
    int frontOffsetX = static_cast<int>(15 * cos(frontAngle));
    int frontOffsetY = static_cast<int>(15 * sin(frontAngle));
    drawRotatedRectangle(centerX + frontOffsetX, centerY + frontOffsetY, 20, 15, rotation, FL_CYAN);
    
    // Рисуем фары
    int headlightOffsetX = static_cast<int>(18 * cos(frontAngle));
    int headlightOffsetY = static_cast<int>(18 * sin(frontAngle));
    drawRotatedRectangle(centerX + headlightOffsetX, centerY + headlightOffsetY - 5, 4, 4, rotation, FL_YELLOW);
    drawRotatedRectangle(centerX + headlightOffsetX, centerY + headlightOffsetY + 5, 4, 4, rotation, FL_YELLOW);
    
    // Эффекты
    if (isSweeping) {
        // Щетки сзади
        float backAngle = (rotation + 180) * (pi / 180.0f);
        int backOffsetX = static_cast<int>(18 * cos(backAngle));
        int backOffsetY = static_cast<int>(18 * sin(backAngle));
        
        fl_color(FL_YELLOW);
        fl_line_style(FL_SOLID, 3);
        for (int i = -6; i <= 6; i += 3) {
            int brushX = centerX + backOffsetX + static_cast<int>(i * sin(backAngle));
            int brushY = centerY + backOffsetY - static_cast<int>(i * cos(backAngle));
            int brushEndX = brushX + static_cast<int>(8 * cos(backAngle));
            int brushEndY = brushY + static_cast<int>(8 * sin(backAngle));
            fl_line(brushX, brushY, brushEndX, brushEndY);
        }
        fl_line_style(0);
    }
    
    if (isWashing) {
        // Брызги воды
        fl_color(FL_CYAN);
        for (int i = 0; i < 15; i++) {
            float angle = (rand() % 360) * (pi / 180.0f);
            int distance = 25 + rand() % 15;
            int splashX = centerX + static_cast<int>(distance * cos(angle));
            int splashY = centerY + static_cast<int>(distance * sin(angle));
            fl_circle(splashX, splashY, 1 + rand() % 2);
        }
    }
}

void SweeperWidget::drawRotatedRectangle(int centerX, int centerY, int width, int height, float angle, Fl_Color fillColor) {
    float rad = angle * (pi / 180.0f);
    float cosA = cos(rad);
    float sinA = sin(rad);
    
    // Углы прямоугольника относительно центра
    int halfWidth = width / 2;
    int halfHeight = height / 2;
    
    int pointsX[4] = {-halfWidth, halfWidth, halfWidth, -halfWidth};
    int pointsY[4] = {-halfHeight, -halfHeight, halfHeight, halfHeight};
    
    // Поворачиваем и сдвигаем точки
    int rotatedX[4], rotatedY[4];
    for (int i = 0; i < 4; i++) {
        rotatedX[i] = centerX + static_cast<int>(pointsX[i] * cosA - pointsY[i] * sinA);
        rotatedY[i] = centerY + static_cast<int>(pointsX[i] * sinA + pointsY[i] * cosA);
    }
    
    // Рисуем заполненный прямоугольник
    fl_color(fillColor);
    fl_begin_polygon();
    for (int i = 0; i < 4; i++) {
        fl_vertex(rotatedX[i], rotatedY[i]);
    }
    fl_end_polygon();
    
    // Рисуем контур
    fl_color(FL_BLACK);
    fl_begin_line();
    for (int i = 0; i < 4; i++) {
        fl_vertex(rotatedX[i], rotatedY[i]);
    }
    fl_vertex(rotatedX[0], rotatedY[0]); // Замыкаем контур
    fl_end_line();
}