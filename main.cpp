#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
    float x, y, z;
};

typedef Point Polygon[3];

typedef vector<Polygon> Mesh;

typedef float Matrix[4][4];

class App {
private:
    void mulMatrixVector(Point& i, Point& o, Matrix& m) {
        o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
        o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
        o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];

        float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
        if (w != 0.0f) {
            o.x /= w; o.y /= w; o.z /= w;
        }
    }


public:
    App() {

    }

    void onCreate() {
        Mesh cube{
            {
                // SOUTH
                { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

                // EAST
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

                // NORTH
                { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

                // WEST
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

                // TOP
                { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
                { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

                // BOTTOM
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
                { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
            }
        };

        float near = 0.1f;
        float far = 1000.0f;
        float fov = 90.0f / 180.0f * M_PI;
        float aspectRatio = (float)27 / (float)48;

        Matrix proj = { 0, };
        proj[0][0] = fov * aspectRatio;
        proj[1][1] = fov;
        proj[2][2] = far / (far - near);
        proj[3][2] = (-far * near) / (far - near);
        proj[2][3] = 1.0f;
        proj[3][3] = 1.0f;

        return;
    }

    void onUpdate() {
        return;
    }
};
