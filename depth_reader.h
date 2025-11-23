#pragma once
#include <vector>
#include <string>

// Класс для чтения карты глубины из бинарного файла
class DepthReader {
public:
    DepthReader();
    bool readDepthMap(const std::string& filename);
    const std::vector<std::vector<double>>& getDepthData() const;
    int getWidth() const;
    int getHeight() const;
    void printInfo() const;

private:
    std::vector<std::vector<double>> depthData; // 2D массив значений глубины
    int width, height; // Размеры карты глубины
};