#pragma once

#include <map>
#include <cstring>
#include "RGB.h"
#include "tmatrix.h"
#include "parcer.h"

// Базовый класс фильтров
class Filter {
public:
    Filter() = default;

    virtual ~Filter() = default;

    virtual void Apply(TMatrix<RGB>& image) const {};
};

class CropFilter : public Filter {
public:
    explicit CropFilter(int width = 0, int height = 0) : width_(width), height_(height) {
    }

    void Apply(TMatrix<RGB>& image) const override;

protected:
    int width_;
    int height_;
};

class GrayScaleFilter : public Filter {
public:
    GrayScaleFilter() = default;

    void Apply(TMatrix<RGB>& image) const override;
};

class NegativeFilter : public Filter {
public:
    NegativeFilter() = default;

    void Apply(TMatrix<RGB>& image) const override;
};

class SharpeningFilter : public Filter {
public:
    SharpeningFilter() = default;

    void Apply(TMatrix<RGB>& image) const override;
};

class EdgeDetectionFilter : public Filter {
public:
    explicit EdgeDetectionFilter(double threshold = 0.0) : threshold_(threshold) {
    }

    void Apply(TMatrix<RGB>& image) const override;

protected:
    double threshold_;
};

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma = 0.0) : sigma_(sigma) {
    }

    void Apply(TMatrix<RGB>& image) const override;

protected:
    double sigma_;
};

// Функция, создающаяя map сопоставляющую названию фильтра, фукнцию, создающую его

// сравнение ключей
struct CompareStr {
    bool operator()(char const* a, char const* b) const {
        return std::strcmp(a, b) < 0;
    }
};
using Ptr2FilterCreator = Filter* (*)(const FilterDescriptor&);
using FilterCreatorsMap = std::map<const char*, Ptr2FilterCreator, CompareStr>;
FilterCreatorsMap InitProducers();

// Функция для проверки присутствия в FiltercreatorsMap соответствующего ключа
size_t KeyCount(const FilterCreatorsMap& map, const char* key);
