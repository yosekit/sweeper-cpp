#ifndef SWEEPERWIDGET_H
#define SWEEPERWIDGET_H

#pragma once

#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>

class SweeperWidget : public Fl_Widget {
public:
    SweeperWidget(int x, int y, int w, int h, const char* label = 0);
    ~SweeperWidget() = default;
    
    void setRotation(float angle); // Угол в градусах
    void setColor(Fl_Color color);
    void setSweeping(bool sweeping);
    void setWashing(bool washing);
    
    void draw() override;
    
private:
    float rotation = 0.0f; // Угол поворота в градусах
    Fl_Color color = FL_BLUE;
    bool isSweeping = false;
    bool isWashing = false;
    
    // Вспомогательные методы для рисования с поворотом
    void drawRotatedRectangle(int centerX, int centerY, int width, int height, float angle, Fl_Color fillColor);
};

#endif