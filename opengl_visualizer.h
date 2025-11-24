#ifndef OPENGL_VISUALIZER_H
#define OPENGL_VISUALIZER_H

#include <vector>

class OpenGLVisualizer {
public:
    static void initialize(int argc, char** argv, const std::vector<std::vector<double>>& depthData);
    static void run();

private:
    static std::vector<std::vector<double>> depthData;
    static float rotationX, rotationY;

    static void display();
    static void keyboard(unsigned char key, int x, int y);
    static void specialKeys(int key, int x, int y);
    static void reshape(int width, int height);
    static void setupLighting();
};

#endif