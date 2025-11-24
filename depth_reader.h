#pragma once

#include <vector>
#include <string>

class DepthReader {
public:
    DepthReader();
    bool readDepthMap(const std::string& filename);
    const std::vector<std::vector<double>>& getDepthData() const;
    int getWidth() const;
    int getHeight() const;
    void printInfo() const;

private:
    std::vector<std::vector<double>> depthData;
    int width, height;
};