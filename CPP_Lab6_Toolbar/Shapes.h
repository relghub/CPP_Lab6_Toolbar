#pragma once

#include <windows.h>
#include <vector>

enum class ShapeType { SHAPE_POINT, SHAPE_LINE, SHAPE_RECTANGLE, SHAPE_ELLIPSE };

struct Shape {
    ShapeType type;
    POINT pt1;
    POINT pt2;
    COLORREF color;
};

COLORREF GetShapeColor(int studentNumber);
void DrawShapes(HDC hdc, const Shape* shapes, int shapeCount);