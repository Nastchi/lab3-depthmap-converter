#include "opengl_visualizer.h"
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <cmath>

std::vector<std::vector<double>> OpenGLVisualizer::depthData;
float OpenGLVisualizer::rotationX = 0.0f;
float OpenGLVisualizer::rotationY = 0.0f;

void OpenGLVisualizer::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Устанавливаем камеру
    gluLookAt(0, 20, 50, 0, 0, 0, 0, 1, 0);

    // Применяем вращение
    glRotatef(rotationX, 1, 0, 0);
    glRotatef(rotationY, 0, 1, 0);

    // Рисуем координатные оси
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X - красный
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(2, 0, 0);
    // Y - зеленый
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 2, 0);
    // Z - синий
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 2);
    glEnd();
    glEnable(GL_LIGHTING);

    // Рисуем 3D модель из карты глубины
    if (!depthData.empty()) {
        int height = depthData.size();
        int width = depthData[0].size();

        GLfloat materialColor[] = { 0.45f, 0.4f, 0.35f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);

        // Рисуем как сетку треугольников
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < height - 1; i++) {
            for (int j = 0; j < width - 1; j++) {
                // Первый треугольник
                double x1 = (j - width / 2.0) * 0.1;
                double z1 = (i - height / 2.0) * 0.1;
                double y1 = depthData[i][j] * 0.05;

                double x2 = (j + 1 - width / 2.0) * 0.1;
                double z2 = (i - height / 2.0) * 0.1;
                double y2 = depthData[i][j + 1] * 0.05;

                double x3 = (j - width / 2.0) * 0.1;
                double z3 = (i + 1 - height / 2.0) * 0.1;
                double y3 = depthData[i + 1][j] * 0.05;

                // Второй треугольник
                double x4 = (j + 1 - width / 2.0) * 0.1;
                double z4 = (i + 1 - height / 2.0) * 0.1;
                double y4 = depthData[i + 1][j + 1] * 0.05;

                // Простые нормали
                glNormal3f(0, 1, 0);

                // Первый треугольник
                glVertex3f(x1, y1, z1);
                glVertex3f(x2, y2, z2);
                glVertex3f(x3, y3, z3);

                // Второй треугольник
                glVertex3f(x2, y2, z2);
                glVertex3f(x4, y4, z4);
                glVertex3f(x3, y3, z3);
            }
        }
        glEnd();
    }

    glutSwapBuffers();
}

void OpenGLVisualizer::keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': case 'W': rotationX += 5.0f; break;
    case 's': case 'S': rotationX -= 5.0f; break;
    case 'a': case 'A': rotationY += 5.0f; break;
    case 'd': case 'D': rotationY -= 5.0f; break;
    case 'r': case 'R': rotationX = rotationY = 0.0f; break;
    case 27: exit(0); break; // ESC
    }
    glutPostRedisplay();
}

void OpenGLVisualizer::specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:    rotationX += 5.0f; break;
    case GLUT_KEY_DOWN:  rotationX -= 5.0f; break;
    case GLUT_KEY_LEFT:  rotationY += 5.0f; break;
    case GLUT_KEY_RIGHT: rotationY -= 5.0f; break;
    }
    glutPostRedisplay();
}

void OpenGLVisualizer::reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGLVisualizer::setupLighting() {
    GLfloat lightPosition[] = { 2.0f, 5.0f, 2.0f, 1.0f };
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void OpenGLVisualizer::initialize(int argc, char** argv, const std::vector<std::vector<double>>& data) {
    depthData = data;

    // Инициализация GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Depth Map - Lab 3");

    // Настройка OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    setupLighting();

    // Устанавливаем функции обратного вызова
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    std::cout << "✅ OpenGL визуализация готова!" << std::endl;
}

void OpenGLVisualizer::run() {
    std::cout << "=== Управление ===" << std::endl;
    std::cout << "Стрелки Вверх/Вниз - вращение по X" << std::endl;
    std::cout << "Стрелки Влево/Вправо - вращение по Y" << std::endl;
    std::cout << "R - сброс вращения" << std::endl;
    std::cout << "ESC - выход" << std::endl;

    glutMainLoop();
}