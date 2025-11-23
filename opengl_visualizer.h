#ifndef OPENGL_VISUALIZER_H
#define OPENGL_VISUALIZER_H
#include <vector>

// Класс для 3D визуализации карты глубины с использованием OpenGL
class OpenGLVisualizer {
public:
    static void initialize(int argc, char** argv, const std::vector<std::vector<double>>& depthData);
    static void run();

private:
    static std::vector<std::vector<double>> depthData; // Данные для визуализации
    static float rotationX, rotationY; // Углы вращения модели

    static void display(); // Функция отрисовки
    static void keyboard(unsigned char key, int x, int y); // Обработка клавиатуры
    static void reshape(int width, int height); // Изменение размера окна
    static void setupLighting(); // Настройка освещения
};
#endif