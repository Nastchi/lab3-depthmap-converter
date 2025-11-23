#include <iostream>
#include <string>
#include "depth_reader.h"
#include "obj_writer.h"
#include "test_data_generator.h"
#include "opengl_visualizer.h"

using namespace std;

void printUsage() {
    cout << "Использование:\n";
    cout << " Lab3CityModel [опции]\n";
    cout << "Опции:\n";
    cout << " -i <файл> Входной файл карты глубины\n";
    cout << " -o <файл> Выходной OBJ файл\n";
    cout << " -s <scale> Масштаб модели\n";
    cout << " -v Визуализация в OpenGL\n";
    cout << " -h Справка\n";
}

int main(int argc, char** argv) {
    string inputFile = "depthmap.dat";
    string outputFile = "model.obj";
    double scale = 1.0;
    bool visualize = false;

    // Генерация тестовых данных карты глубины
    generateTestData();

    // Обработка аргументов командной строки
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-i" && i + 1 < argc) {
            inputFile = argv[++i];
        }
        else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "-s" && i + 1 < argc) {
            scale = stod(argv[++i]);
        }
        else if (arg == "-v") {
            visualize = true;
        }
        else if (arg == "-h") {
            printUsage();
            return 0;
        }
    }

    cout << "=== Лабораторная работа №3 ===\n";
    cout << "Вариант 10: C++ -> .obj формат\n\n";

    // Чтение карты глубины из файла
    DepthReader reader;
    if (!reader.readDepthMap(inputFile)) {
        cerr << "Ошибка при чтении карты глубины!" << endl;
        return -1;
    }

    reader.printInfo();

    // Экспорт 3D модели в OBJ формат
    OBJWriter writer;
    if (!writer.writeToOBJ(outputFile, reader.getDepthData(), scale)) {
        cerr << "Ошибка при экспорте в OBJ формат!" << endl;
        return -1;
    }

    cout << "Конвертация завершена успешно!\n";
    cout << "Файл: " << outputFile << endl;

    // Визуализация 3D модели в OpenGL
    if (visualize) {
        cout << "Запуск OpenGL визуализации...\n";
        OpenGLVisualizer::initialize(argc, argv, reader.getDepthData());
        OpenGLVisualizer::run();
    }

    return 0;
}