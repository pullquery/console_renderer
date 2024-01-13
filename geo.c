#include <stdio.h>
#include <math.h>

#define PI 3.141592653589
#define RAD(X) X / 180 * PI
// #define _MAX(X, Y) (X) > (Y) ? (X) : (Y)
// #define MAX(X, Y, Z) _MAX(_MAX(X, Y), Z)

#include <windows.h>

typedef struct {
    float x;
    float y;
    float z;
} Point;

typedef struct {
    float xx, xy, xz;
    float yx, yy, yz;
    float zx, zy, zz;
} Transform;

typedef struct {
    float a, b, c;
} Rotation;

struct {
    int x;
    int y;
} origin;

struct {
    int width;
    int height;
    char* buffer;
} buffer;

void init() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

    int width = info.srWindow.Right - info.srWindow.Left + 1;
    int height = info.srWindow.Bottom - info.srWindow.Top + 1;

    origin.x = width / 2;
    origin.y = height / 2;

    buffer.width = width;
    buffer.height = height;
    buffer.buffer = calloc((width - 1) * (height - 1), sizeof(char));

    printf("\033[?25l");
}

void setBuffer() {
    memset(buffer.buffer, ' ', buffer.width * buffer.height);
}

void printBuffer() {
    printf("\033[2J");
    printf("\033[H");
    for (int h = 0; h < buffer.height; ++h) {
        for (int w = 0; w < buffer.width; ++w) {
            printf("%c", buffer.buffer[h * buffer.width + w]);
        }
    }
}

Point transformPoint(Point p, Transform t) {
    float x = p.x;
    float y = p.y;
    float z = p.z;

    Point pt = {
        x * t.xx + y * t.xy + z * t.xz,
        x * t.yx + y * t.yy + z * t.yz,
        x * t.zx + y * t.zy + z * t.zz,
    };

    return pt;
}

Point rotatePoint(Point p, Rotation r) {
    float a = r.a;
    float b = r.b;
    float c = r.c;

    Transform t = {
        cos(a) * cos(b), cos(a) * sin(b) * sin(c) - sin(a) * cos(c), cos(a) * sin(b) * cos(c) + sin(a) * sin(c),
        sin(a) * cos(b), sin(a) * sin(b) * sin(c) + cos(a) * cos(c), sin(a) * sin(b) * cos(c) - cos(a) * sin(c),
                -sin(b),                            cos(b) * sin(c),                            cos(b) * cos(c),
    };

    return transformPoint(p, t);
}

Point projectPoint(Point p, float distance) {
    float ratio = distance / (p.z + distance);

    Point pp = {
        p.x * ratio ,
        p.y * ratio,
        0.0f,
    };

    return pp;
}

void drawPoint(Point p) {
    int x = origin.x + (int)p.x;
    int y = origin.y - (int)p.y;
    buffer.buffer[y * buffer.width + x] = '.';
}

void drawLine(Point p1, Point p2) {
    float x1 = p1.x;
    float y1 = p1.y;
    float x2 = p2.x;
    float y2 = p2.y;


    if (x2 == x1) {
        float x = x1;
        for (float y = min(y1, y2); y <= max(y1, y2); ++y) {
            Point p = { x, y, 0.0f };
            drawPoint(p);
        }
    }

    float slope = (y2 - y1) / (x2 - x1);

    if (abs(slope) >= 1) {
        for (float y = min(y1, y2); y <= max(y1, y2); ++y) {
            float x = (1 / slope) * (y - y1) + x1;
            Point p = { x, y, 0.0f };
            drawPoint(p);
        }
    }

    if (abs(slope) < 1) {
        for (float x = min(x1, x2); x <= max(x1, x2); ++x) {
            float y = slope * (x - x1) + y1;
            Point p = { x, y, 0.0f };
            drawPoint(p);
        }
    }
}

void drawPolygon(Point points[], int c) {
    for (int i = 0; i < c - 1; ++i) {
        drawLine(points[i], points[i + 1]);
    }

    drawLine(points[0], points[c - 1]);
}

int main() {
    init();

    Point plane1[] = {
        {-15.0f, -15.0f, -15.0f},
        {15.0f, -15.0f, -15.0f},
        {15.0f, 15.0f, -15.0f},
        {-15.0f, 15.0f, -15.0f},
    };

    Point plane2[] = {
        {15.0f, -15.0f, -15.0f},
        {15.0f, -15.0f, 15.0f},
        {15.0f, 15.0f, 15.0f},
        {15.0f, 15.0f, -15.0f},
    };

    Point plane3[] = {
        {-15.0f, -15.0f, 15.0f},
        {15.0f, -15.0f, 15.0f},
        {15.0f, 15.0f, 15.0f},
        {-15.0f, 15.0f, 15.0f},
    };

    Point plane4[] = {
        {-15.0f, -15.0f, -15.0f},
        {-15.0f, -15.0f, 15.0f},
        {-15.0f, 15.0f, 15.0f},
        {-15.0f, 15.0f, -15.0f},
    };

    Rotation rotation1 = { RAD(15.0f), RAD(0.0f), RAD(-15.0f) };

    Point rotated1[] = {
        rotatePoint(plane1[0], rotation1),
        rotatePoint(plane1[1], rotation1),
        rotatePoint(plane1[2], rotation1),
        rotatePoint(plane1[3], rotation1),
    };

    Point rotated2[] = {
        rotatePoint(plane2[0], rotation1),
        rotatePoint(plane2[1], rotation1),
        rotatePoint(plane2[2], rotation1),
        rotatePoint(plane2[3], rotation1),
    };

    Point rotated3[] = {
        rotatePoint(plane3[0], rotation1),
        rotatePoint(plane3[1], rotation1),
        rotatePoint(plane3[2], rotation1),
        rotatePoint(plane3[3], rotation1),
    };

    Point rotated4[] = {
        rotatePoint(plane4[0], rotation1),
        rotatePoint(plane4[1], rotation1),
        rotatePoint(plane4[2], rotation1),
        rotatePoint(plane4[3], rotation1),
    };

    float distance = 10.0f;

    for (float angle = 0.0f; angle < 360.0f; angle += 10.0f) {
        setBuffer();

        Rotation rotation2 = { RAD(0.0f), RAD(angle), RAD(0.0f) };

        Point rendered1[] = {
            rotatePoint(rotated1[0], rotation2),
            rotatePoint(rotated1[1], rotation2),
            rotatePoint(rotated1[2], rotation2),
            rotatePoint(rotated1[3], rotation2),
        };

        Point rendered2[] = {
            rotatePoint(rotated2[0], rotation2),
            rotatePoint(rotated2[1], rotation2),
            rotatePoint(rotated2[2], rotation2),
            rotatePoint(rotated2[3], rotation2),
        };

        Point rendered3[] = {
            rotatePoint(rotated3[0], rotation2),
            rotatePoint(rotated3[1], rotation2),
            rotatePoint(rotated3[2], rotation2),
            rotatePoint(rotated3[3], rotation2),
        };

        Point rendered4[] = {
            rotatePoint(rotated4[0], rotation2),
            rotatePoint(rotated4[1], rotation2),
            rotatePoint(rotated4[2], rotation2),
            rotatePoint(rotated4[3], rotation2),
        };

        drawPolygon(rendered1, sizeof(rendered1) / sizeof(Point));
        drawPolygon(rendered2, sizeof(rendered2) / sizeof(Point));
        drawPolygon(rendered3, sizeof(rendered3) / sizeof(Point));
        drawPolygon(rendered4, sizeof(rendered4) / sizeof(Point));

        printBuffer();

        Sleep(10);
    }

    free(buffer.buffer);

    return 0;
}