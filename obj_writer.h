#pragma once
#include <vector>
#include <string>

// Класс для экспорта 3D модели в формат OBJ
class OBJWriter {
public:
    OBJWriter();
    bool writeToOBJ(const std::string& filename,
        const std::vector<std::vector<double>>& depthData,
        double scale = 1.0);

private:
    void writeHeader(std::ofstream& file); // Запись заголовка файла
    void writeVertices(std::ofstream& file, // Запись вершин
        const std::vector<std::vector<double>>& depthData,
        double scale);
    void writeFaces(std::ofstream& file, // Запись полигонов
        const std::vector<std::vector<double>>& depthData);
};