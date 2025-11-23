#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
double clamp(double x, double lower, double upper) {
    return min(upper, max(x, lower));
}

double smoothstep(double edge0, double edge1, double x) {
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return x * x * (3 - 2 * x);
}


void createRockDepthMap(int width, int height, vector<vector<double>>& depthMap) {
    depthMap.resize(height, vector<double>(width, 0.0));

    srand(42);
    double centerX = width / 2.0;
    double centerY = height / 2.0;


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double dx = (j - centerX) / centerX;
            double dy = (i - centerY) / centerY;
            double distance = sqrt(dx * dx + dy * dy);
            double angle = atan2(dy, dx);

            // Неровный контур со всех сторон
            double irregularShape = 1.0;

            // Большие неровности (4 стороны)
            irregularShape += 0.25 * sin(angle * 4.0);
            // Средние неровности (8 сторон)
            irregularShape += 0.15 * sin(angle * 8.0 + 1.5);
            // Мелкие неровности (12 сторон)
            irregularShape += 0.08 * sin(angle * 12.0 + 2.3);
            // Случайные выступы
            irregularShape += 0.1 * sin(j * 0.4) * cos(i * 0.35);

            // Базовая форма - но с неровным контуром
            double effectiveDistance = distance / irregularShape;

            if (effectiveDistance < 1.0) {
                // Основная форма горы - неровная сверху
                double baseHeight = 35.0 * (1.0 - pow(effectiveDistance, 1.3));

                // Несколько вершин разной высоты
                double peaks = 0.0;
                peaks += 20.0 * exp(-effectiveDistance * 12.0); // Главная вершина

                // Боковые вершины в случайных местах
                peaks += 15.0 * exp(-((dx - 0.2) * (dx - 0.2) + (dy - 0.25) * (dy - 0.25)) * 30.0);
                peaks += 12.0 * exp(-((dx + 0.3) * (dx + 0.3) + (dy + 0.15) * (dy + 0.15)) * 25.0);
                peaks += 10.0 * exp(-((dx - 0.1) * (dx - 0.1) + (dy + 0.35) * (dy + 0.35)) * 35.0);
                peaks += 8.0 * exp(-((dx + 0.4) * (dx + 0.4) + (dy - 0.3) * (dy - 0.3)) * 20.0);

                // Скальные выступы и обрывы
                double cliffs = 0.0;
                cliffs += 6.0 * sin(angle * 6.0) * (1.0 - effectiveDistance);
                cliffs += 4.0 * cos(angle * 9.0 + 2.0) * (1.0 - effectiveDistance);

                // Грубая текстура скалы
                double rockTexture = 0.0;
                for (int octave = 1; octave <= 5; octave++) {
                    double freq = pow(2.0, octave);
                    double amp = pow(0.35, octave);
                    rockTexture += amp * (sin(freq * dx * 15.0 + octave * 2.0) +
                        cos(freq * dy * 13.0 + octave * 3.0));
                }

                // Овраги и расщелины
                double ravines = 5.0 * sin(dx * 25.0) * cos(dy * 22.0) * (1.0 - effectiveDistance);

                // Неровная верхняя поверхность
                double bumpyTop = 8.0 * sin(dx * 18.0 + 1.0) * cos(dy * 16.0 + 2.0);

                // Плавный переход к основанию с неровным краем
                double edgeTransition = 1.0;
                if (effectiveDistance > 0.6) {
                    // Делаем край неровным
                    double edgeRoughness = 0.3 * sin(angle * 10.0 + j * 0.2) *
                        cos(angle * 8.0 + i * 0.15);
                    edgeTransition = pow(1.0 - effectiveDistance, 1.1) * (3.0 + edgeRoughness);
                }

                // "Дыры" и впадины в скале
                double holes = 0.0;
                // Большая впадина с северо-востока
                holes -= 8.0 * exp(-((dx - 0.4) * (dx - 0.4) + (dy + 0.35) * (dy + 0.35)) * 40.0);
                // Впадина с юго-запада
                holes -= 6.0 * exp(-((dx + 0.45) * (dx + 0.45) + (dy - 0.25) * (dy - 0.25)) * 35.0);
                // Несколько мелких впадин
                holes -= 4.0 * exp(-((dx - 0.15) * (dx - 0.15) + (dy - 0.4) * (dy - 0.4)) * 50.0);
                holes -= 3.0 * exp(-((dx + 0.2) * (dx + 0.2) + (dy + 0.45) * (dy + 0.45)) * 45.0);

                depthMap[i][j] = (baseHeight + peaks + cliffs + rockTexture * 6.0 +
                    ravines + bumpyTop + holes) * edgeTransition;

                // Добавляем случайные скальные выступы по всей поверхности
                if (effectiveDistance < 0.9 && rand() % 100 < 15) {
                    double randomBump = (rand() % 100) / 25.0; // 0-4 единицы высоты
                    depthMap[i][j] += randomBump;
                }
            }
            else {
                // Область вокруг горы - холмистая местность
                double distanceFromEdge = (effectiveDistance - 1.0) * 2.0;
                if (distanceFromEdge < 1.0) {
                    double hills = 2.0 * sin(j * 0.7 + 1.0) * cos(i * 0.6 + 2.0);
                    hills += 1.5 * sin(distance * 12.0);
                    depthMap[i][j] = hills * (1.0 - distanceFromEdge);
                }
            }
        }
    }

  
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            if (depthMap[i][j] > 10.0) {
                // Ищем резкие перепады высот для создания обрывов
                double diffX = depthMap[i][j + 1] - depthMap[i][j - 1];
                double diffY = depthMap[i + 1][j] - depthMap[i - 1][j];
                double gradient = sqrt(diffX * diffX + diffY * diffY);

                if (gradient > 15.0 && rand() % 100 < 30) {
                    // Создаем обрыв
                    depthMap[i][j] += 3.0;
                }
            }
        }
    }
}

void generateTestData() {
    setlocale(LC_ALL, "Russian");
    int width = 64;
    int height = 64;

    vector<vector<double>> depthMap;
    createRockDepthMap(width, height, depthMap);

  
    remove("depthmap.dat");

    ofstream out("depthmap.dat", ios::binary | ios::trunc); 
    if (!out.is_open()) {
        cerr << "Ошибка: Не удалось создать файл depthmap.dat" << endl;
        return;
    }

    out.write((char*)&height, sizeof(height));
    out.write((char*)&width, sizeof(width));

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            out.write((char*)&depthMap[i][j], sizeof(double));
        }
    }

    out.close();
    cout << " Карта глубины: depthmap.dat" << endl;
    cout << " Размер: " << width << "x" << height << endl;

   
    system("sync"); 
}