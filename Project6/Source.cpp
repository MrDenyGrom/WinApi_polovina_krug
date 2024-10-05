#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <windows.h>
#include <conio.h>

using namespace std;

class World {
protected:
    HWND hwnd;
    HDC hdc;
    RECT rect;
public:
    int width, height;
    HPEN pBlack, pRed;
    HBRUSH bGreen, bBlack;

    World() {
        hwnd = GetForegroundWindow();
        GetClientRect(hwnd, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        hdc = GetDC(hwnd);
        pBlack = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        pRed = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
        bGreen = CreateSolidBrush(RGB(0, 255, 0));
        bBlack = CreateSolidBrush(RGB(0, 0, 0));
    }

    ~World() {
        ReleaseDC(hwnd, hdc);
        DeleteObject(pBlack);
        DeleteObject(pRed);
        DeleteObject(bGreen);
        DeleteObject(bBlack);
    }

    HDC getHDC() const {
        return hdc;
    }

    void clear() const {
        SelectObject(hdc, bBlack);
        SelectObject(hdc, pBlack);
        Rectangle(hdc, 0, 0, width, height);
    }

    void updateSize() {
        GetClientRect(hwnd, &rect);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    }
};

class Location {
protected:
    int x, y;

public:
    Location(int initx, int inity) : x(initx), y(inity) {}

    void moveTo(int newx, int newy) {
        x = newx;
        y = newy;
    }
};

class Graph : public Location {
private:
    int radius;
public:
    Graph(int initx, int inity, int r) : Location(initx, inity), radius(r) {}

    void show(World* wrd) {
        HDC hdc = wrd->getHDC();
        SelectObject(hdc, wrd->bGreen);
        SelectObject(hdc, wrd->pRed);
        Pie(hdc, x - radius, y, x + radius, y + radius * 2, x + radius, y + radius, x - radius, y + radius);
    }

    void go(double angleStep, int centerX, int centerY) {
        int newX = centerX + (radius * cos(angleStep));
        int newY = centerY - (radius * sin(angleStep));
        moveTo(newX, newY);
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    int radius;
    cout << "¬ведите радиус: ";
    cin >> radius;

    World* wrd = new World();
    Graph* graph = new Graph(wrd->width / 2, wrd->height / 2, radius);

    double angle = 0;
    do {

        wrd->clear();
        wrd->updateSize();
        int centerX = wrd->width / 2;
        int centerY = wrd->height / 2;
        graph->go(angle, centerX, centerY);

        graph->show(wrd);

        angle += 0.025;
        if (angle >= M_PI) {
            angle = 0;
        }
        Sleep(50);
    } while (!_kbhit());

    delete graph;
    delete wrd;

    return 0;
}
