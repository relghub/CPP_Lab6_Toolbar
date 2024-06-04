#include "Shapes.h"

COLORREF GetShapeColor(int studentNumber) {
    switch (studentNumber % 6) {
    case 0: return RGB(192, 192, 192); // Grey
    case 1: return RGB(255, 255, 0);  // Yellow
    case 2: return RGB(0, 255, 0); // Lime
    case 3: return RGB(0, 255, 255); // Cyan
    case 4: return RGB(255, 0, 255); // Magenta
    case 5: return RGB(255, 165, 0);   // Orange
    default: return RGB(255, 255, 255); // White
    }
}

void DrawShapes(HDC hdc, const Shape* shapes, int shapeCount) {
    for (int i = 0; i < shapeCount; ++i) {
        HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        if (shapes[i].type == ShapeType::SHAPE_POINT) {
            SetPixel(hdc, shapes[i].pt1.x, shapes[i].pt1.y, shapes[i].color);
        }
        else if (shapes[i].type == ShapeType::SHAPE_LINE) {
            MoveToEx(hdc, shapes[i].pt1.x, shapes[i].pt1.y, nullptr);
            LineTo(hdc, shapes[i].pt2.x, shapes[i].pt2.y);
        }
        else if (shapes[i].type == ShapeType::SHAPE_RECTANGLE) {
            MoveToEx(hdc, shapes[i].pt1.x, shapes[i].pt1.y, nullptr);
            LineTo(hdc, shapes[i].pt2.x, shapes[i].pt1.y);
            LineTo(hdc, shapes[i].pt2.x, shapes[i].pt2.y);
            LineTo(hdc, shapes[i].pt1.x, shapes[i].pt2.y);
            LineTo(hdc, shapes[i].pt1.x, shapes[i].pt1.y);
        }
        else if (shapes[i].type == ShapeType::SHAPE_ELLIPSE) {
            hOldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(shapes[i].color));
            Ellipse(hdc, shapes[i].pt1.x, shapes[i].pt1.y, shapes[i].pt2.x, shapes[i].pt2.y);
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);
    }
}
