#include "depth_reader.h"
#include <iostream>
#include <fstream>

DepthReader::DepthReader() : width(0), height(0) {}

// Чтение карты глубины из бинарного файла
bool DepthReader::readDepthMap(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return false;
    }

    // Чтение размеров карты глубины
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));

    // Выделение памяти и чтение данных глубины
    depthData.resize(height, std::vector<double>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            file.read(reinterpret_cast<char*>(&depthData[i][j]), sizeof(double));
        }
    }

    file.close();
    return true;
}

const std::vector<std::vector<double>>& DepthReader::getDepthData() const {
    return depthData;
}

int DepthReader::getWidth() const { return width; }
int DepthReader::getHeight() const { return height; }

// Вывод информации о карте глубины
void DepthReader::printInfo() const {
    std::cout << "=== Информация о карте глубины ===" << std::endl;
    std::cout << "Размер: " << width << " x " << height << std::endl;

    if (!depthData.empty()) {
        double minDepth = depthData[0][0];
        double maxDepth = depthData[0][0];

        // Поиск минимального и максимального значения глубины
        for (const auto& row : depthData) {
            for (double depth : row) {
                if (depth < minDepth) minDepth = depth;
                if (depth > maxDepth) maxDepth = depth;
            }
        }

        std::cout << "Минимальная глубина: " << minDepth << std::endl;
        std::cout << "Максимальная глубина: " << maxDepth << std::endl;
    }
}