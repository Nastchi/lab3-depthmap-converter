#include "depth_reader.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

DepthReader::DepthReader() : width(0), height(0) {}

bool DepthReader::readDepthMap(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return false;
    }

    // Получаем размер файла
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Читаем заголовок
    file.read(reinterpret_cast<char*>(&height), sizeof(height));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));

    // Если размеры некорректны, определяем автоматически
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        size_t dataSize = fileSize - 8; // минус 8 байт заголовка
        size_t totalPoints = dataSize / sizeof(double);

        // Используем квадратный размер
        width = static_cast<int>(sqrt(totalPoints));
        height = width;

        // Пропускаем некорректный заголовок
        file.seekg(8, std::ios::beg);
    }

    std::cout << "Размер карты глубины: " << width << " x " << height << std::endl;

    // Проверяем корректность размеров
    if (width <= 0 || height <= 0) {
        std::cerr << "Ошибка: Некорректные размеры карты глубины" << std::endl;
        file.close();
        return false;
    }

    // Читаем данные
    depthData.resize(height, std::vector<double>(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!file.read(reinterpret_cast<char*>(&depthData[i][j]), sizeof(double))) {
                std::cerr << "Ошибка чтения данных" << std::endl;
                file.close();
                return false;
            }
        }
    }

    file.close();
    std::cout << "Карта глубины загружена успешно" << std::endl;
    return true;
}

const std::vector<std::vector<double>>& DepthReader::getDepthData() const {
    return depthData;
}

int DepthReader::getWidth() const { return width; }
int DepthReader::getHeight() const { return height; }

void DepthReader::printInfo() const {
    std::cout << "=== Информация о карте глубины ===" << std::endl;
    std::cout << "Размер: " << width << " x " << height << std::endl;

    if (!depthData.empty()) {
        double minDepth = depthData[0][0];
        double maxDepth = depthData[0][0];

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