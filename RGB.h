#pragma once

#include <cstdint>

const uint16_t FILE_TYPE = 0x4D42;  // нужный тип файла
const uint16_t BIT_COUNT = 24;      // кол-во бит на пиксель
const uint32_t HEADER_SIZE = 54;
const uint32_t INFO_SIZE = 40;  // т.к. BITMAPINFOHEADER
const int MAXCOL = 255;         // максимальное значение цвета
const int MINCOL = 0;           // минимальное значение цвета

// cтруктура для хранения данных заголовочного файла
struct BMPHeader {
    uint16_t file_type{0};
    uint32_t file_size{0};
    uint32_t reserved{0};
    uint32_t offset_data{0};
    uint32_t size{0};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{0};
    uint16_t bit_count{0};
    uint32_t compression{0};
    uint32_t size_image{0};
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
} __attribute__((packed));  // указание компилятору не добавлять лишних байтов

// структура для хранения данных о цвете пикселя и работы с ними
struct RGB {
    int rgbRed;
    int rgbGreen;
    int rgbBlue;

    // поцветовое сложение
    RGB operator+(const RGB other) const {
        RGB ret{0, 0, 0};
        ret.rgbRed = rgbRed + other.rgbRed;
        ret.rgbGreen = rgbGreen + other.rgbGreen;
        ret.rgbBlue = rgbBlue + other.rgbBlue;
        return ret;
    }

    void operator+=(const RGB other) {
        rgbRed += other.rgbRed;
        rgbGreen += other.rgbGreen;
        rgbBlue += other.rgbBlue;
    }

    // домножение на целочисленный коэффициент
    RGB operator*(const int coef) const {
        return {rgbRed * coef, rgbGreen * coef, rgbBlue * coef};
    }

    void Correct() {
        if (rgbRed > MAXCOL) {
            rgbRed = MAXCOL;
        }
        if (rgbGreen > MAXCOL) {
            rgbGreen = MAXCOL;
        }
        if (rgbBlue > MAXCOL) {
            rgbBlue = MAXCOL;
        }
        if (rgbRed < MINCOL) {
            rgbRed = MINCOL;
        }
        if (rgbGreen < MINCOL) {
            rgbGreen = MINCOL;
        }
        if (rgbBlue < MINCOL) {
            rgbBlue = MINCOL;
        }
    }
};

// структура для работы с нецелыми величинами
struct DoubleRGB {
    static const double constexpr MAXDOUBLE = 1.0;
    static const double constexpr MINDOUBLE = 0.0;
    double rgbRed;
    double rgbGreen;
    double rgbBlue;

    // поцветовое сложение
    DoubleRGB operator+(const DoubleRGB &other) const {
        DoubleRGB ret{0.0, 0.0, 0.0};
        ret.rgbRed = rgbRed + other.rgbRed;
        ret.rgbGreen = rgbGreen + other.rgbGreen;
        ret.rgbBlue = rgbBlue + other.rgbBlue;
        return ret;
    }

    void operator+=(const DoubleRGB &other) {
        rgbRed += other.rgbRed;
        rgbGreen += other.rgbGreen;
        rgbBlue += other.rgbBlue;
    }

    // домножение на коэффициент
    DoubleRGB operator*(const double &coef) const {
        return {rgbRed * coef, rgbGreen * coef, rgbBlue * coef};
    }

    void Correct() {
        if (rgbRed > MAXDOUBLE) {
            rgbRed = MAXDOUBLE;
        }
        if (rgbGreen > MAXDOUBLE) {
            rgbGreen = MAXDOUBLE;
        }
        if (rgbBlue > MAXDOUBLE) {
            rgbBlue = MAXDOUBLE;
        }
        if (rgbRed < MINDOUBLE) {
            rgbRed = MINDOUBLE;
        }
        if (rgbGreen < MINDOUBLE) {
            rgbGreen = MINDOUBLE;
        }
        if (rgbBlue < MINDOUBLE) {
            rgbBlue = MINDOUBLE;
        }
    }
};
