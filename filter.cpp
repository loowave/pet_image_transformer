#include <cstring>
#include <cmath>
#include <algorithm>
#include "filter.h"

// Для каждого фильтра описываем метод Apply и функцию, создающую фильтр
void CropFilter::Apply(TMatrix<RGB>& image) const {
    if (static_cast<size_t>(width_) > image.GetCollsNum() || static_cast<size_t>(height_) > image.GetRowsNum()) {
        return;
    } else {
        image.Resize(width_, height_, {0, 0, 0});
    }
}

Filter* CreateCropFilter(const FilterDescriptor& fd) {
    static const size_t CROP_PARAMS_NUM = 2;
    if (std::strcmp(fd.GetName() + 1, "crop") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != CROP_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for crop filter");
    }
    int x = std::atoi(fd.GetParams()[0]);
    int y = std::atoi(fd.GetParams()[1]);
    if (x < 0 || y < 0) {
        throw std::invalid_argument("crop can't apply negative parameters");
    }
    CropFilter* filter = new CropFilter{x, y};
    return filter;
}

void GrayScaleFilter::Apply(TMatrix<RGB>& image) const {
    static const double REDCOEFF = 0.299;
    static const double GREENCOEFF = 0.587;
    static const double BLUECOEFF = 0.114;
    for (size_t i = 0; i < image.GetRowsNum(); ++i) {
        for (size_t j = 0; j < image.GetCollsNum(); ++j) {
            double red = REDCOEFF * static_cast<double>(image.At(i, j).rgbRed);
            double green = GREENCOEFF * static_cast<double>(image.At(i, j).rgbGreen);
            double blue = BLUECOEFF * static_cast<double>(image.At(i, j).rgbBlue);
            int col = static_cast<int>(std::round(red + green + blue));
            image.At(i, j).rgbRed = col;
            image.At(i, j).rgbGreen = col;
            image.At(i, j).rgbBlue = col;
            image.At(i, j).Correct();
        }
    }
}

Filter* CreateGrayScaleFilter(const FilterDescriptor& fd) {
    static const size_t GS_PARAMS_NUM = 0;
    if (std::strcmp(fd.GetName() + 1, "gs") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != GS_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for grayscale filter");
    }
    GrayScaleFilter* filter = new GrayScaleFilter;
    return filter;
}

void NegativeFilter::Apply(TMatrix<RGB>& image) const {
    for (size_t i = 0; i < image.GetRowsNum(); ++i) {
        for (size_t j = 0; j < image.GetCollsNum(); ++j) {
            image.At(i, j).rgbRed = MAXCOL - image.At(i, j).rgbRed;
            image.At(i, j).rgbGreen = MAXCOL - image.At(i, j).rgbGreen;
            image.At(i, j).rgbBlue = MAXCOL - image.At(i, j).rgbBlue;
        }
    }
}

Filter* CreateNegativeFilter(const FilterDescriptor& fd) {
    static const size_t NEGATIVE_PARAMS_NUM = 0;
    if (std::strcmp(fd.GetName() + 1, "neg") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != NEGATIVE_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for negative filter");
    }
    NegativeFilter* filter = new NegativeFilter;
    return filter;
}

void SharpeningFilter::Apply(TMatrix<RGB>& image) const {
    static const int CENTRCOEFF = 5;
    TMatrix<RGB> new_image{image.GetRowsNum(), image.GetCollsNum(), {0, 0, 0}};
    for (int i = 0; i < image.GetRowsNum(); ++i) {
        for (int j = 0; j < image.GetCollsNum(); ++j) {
            new_image.At(i, j) += image.At(std::max(0, i - 1), j) * (-1);
            new_image.At(i, j) += image.At(i, std::max(0, j - 1)) * (-1);
            new_image.At(i, j) += image.At(std::min<size_t>(i + 1, image.GetRowsNum() - 1), j) * (-1);
            new_image.At(i, j) += image.At(i, std::min<size_t>(j + 1, image.GetCollsNum() - 1)) * (-1);
            new_image.At(i, j) += image.At(i, j) * CENTRCOEFF;
            new_image.At(i, j).Correct();
        }
    }
    image.CopyContent(new_image);
}

Filter* CreateSharpeningFilter(const FilterDescriptor& fd) {
    static const size_t SHARP_PARAMS_NUM = 0;
    if (std::strcmp(fd.GetName() + 1, "sharp") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != SHARP_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for sharpening filter");
    }
    SharpeningFilter* filter = new SharpeningFilter;
    return filter;
}

void EdgeDetectionFilter::Apply(TMatrix<RGB>& image) const {
    static const int CENTRCOEFF = 4;
    static const int WHITE = 255;
    static const int BLACK = 0;
    GrayScaleFilter gs;
    gs.Apply(image);
    TMatrix<RGB> new_image{image.GetRowsNum(), image.GetCollsNum(), {0, 0, 0}};
    for (int i = 0; i < image.GetRowsNum(); ++i) {
        for (int j = 0; j < image.GetCollsNum(); ++j) {
            new_image.At(i, j) += image.At(std::max(0, i - 1), j) * (-1);
            new_image.At(i, j) += image.At(i, std::max(0, j - 1)) * (-1);
            new_image.At(i, j) += image.At(std::min<size_t>(i + 1, image.GetRowsNum() - 1), j) * (-1);
            new_image.At(i, j) += image.At(i, std::min<size_t>(j + 1, image.GetCollsNum() - 1)) * (-1);
            new_image.At(i, j) += image.At(i, j) * CENTRCOEFF;
            if (static_cast<double>(new_image.At(i, j).rgbRed) / static_cast<double>(MAXCOL) > threshold_) {
                new_image.At(i, j).rgbRed = WHITE;
                new_image.At(i, j).rgbGreen = WHITE;
                new_image.At(i, j).rgbBlue = WHITE;
            } else {
                new_image.At(i, j).rgbRed = BLACK;
                new_image.At(i, j).rgbGreen = BLACK;
                new_image.At(i, j).rgbBlue = BLACK;
            }
        }
    }
    image.CopyContent(new_image);
}

Filter* CreateEdgeDetectionFilter(const FilterDescriptor& fd) {
    static const size_t SHARP_PARAMS_NUM = 1;
    if (std::strcmp(fd.GetName() + 1, "edge") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != SHARP_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for edge filter");
    }
    double x = std::stod(fd.GetParams()[0]);
    if (x < 0.0 || x > 1.0) {
        throw std::invalid_argument("Edge parameter be from 0.0 to 1.0");
    }
    EdgeDetectionFilter* filter = new EdgeDetectionFilter{x};
    return filter;
}

void GaussianBlurFilter::Apply(TMatrix<RGB>& image) const {
    static const double SQRT = 0.5;
    int core_r = std::ceil(3 * sigma_);
    std::vector<double> coefs;
    for (int i = 0; i < core_r + 1; ++i) {
        double coord = static_cast<double>(i);
        coefs.push_back(std::pow(M_E, -coord * coord / (2 * sigma_ * sigma_)) /
                        std::pow(2 * M_PI * sigma_ * sigma_, SQRT));
    }
    TMatrix<DoubleRGB> new_image{image.GetRowsNum(), image.GetCollsNum(), {0.0, 0.0, 0.0}};
    for (int i = 0; i < image.GetRowsNum(); ++i) {
        for (int j = 0; j < image.GetCollsNum(); ++j) {
            for (int x = j - core_r; x <= j + core_r; ++x) {
                size_t real_x = std::min<size_t>(image.GetCollsNum() - 1, std::max(0, x));
                new_image.At(i, j).rgbRed +=
                    static_cast<double>(image.At(i, real_x).rgbRed) * coefs[std::abs(j - x)] / MAXCOL;
                new_image.At(i, j).rgbGreen +=
                    static_cast<double>(image.At(i, real_x).rgbGreen) * coefs[std::abs(j - x)] / MAXCOL;
                new_image.At(i, j).rgbBlue +=
                    static_cast<double>(image.At(i, real_x).rgbBlue) * coefs[std::abs(j - x)] / MAXCOL;
            }
            new_image.At(i, j).Correct();
        }
    }
    for (int i = 0; i < image.GetRowsNum(); ++i) {
        for (int j = 0; j < image.GetCollsNum(); ++j) {
            image.At(i, j) = {0, 0, 0};
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            for (int y = i - core_r; y <= i + core_r; ++y) {
                size_t real_y = std::min<size_t>(image.GetRowsNum() - 1, std::max(0, y));
                r += new_image.At(real_y, j).rgbRed * coefs[std::abs(i - y)];
                g += new_image.At(real_y, j).rgbGreen * coefs[std::abs(i - y)];
                b += new_image.At(real_y, j).rgbBlue * coefs[std::abs(i - y)];
            }
            image.At(i, j).rgbRed = static_cast<int>(r * MAXCOL);
            image.At(i, j).rgbGreen = static_cast<int>(g * MAXCOL);
            image.At(i, j).rgbBlue = static_cast<int>(b * MAXCOL);
            image.At(i, j).Correct();
        }
    }
}

Filter* CreateGaussianBlurFilter(const FilterDescriptor& fd) {
    static const size_t SHARP_PARAMS_NUM = 1;
    if (std::strcmp(fd.GetName() + 1, "blur") != 0) {
        throw std::invalid_argument("incorrect filter name");
    }
    if (fd.GetParams().size() != SHARP_PARAMS_NUM) {
        throw std::invalid_argument("incorrect params for Gaussian blur filter");
    }
    double x = std::stod(fd.GetParams()[0]);
    if (x < 0.0) {
        throw std::invalid_argument("Gaussian blur sigma should be bigger than 0.0");
    }
    GaussianBlurFilter* filter = new GaussianBlurFilter{x};
    return filter;
}

FilterCreatorsMap InitProducers() {
    std::unordered_map<const char*, Ptr2FilterCreator> ret;
    ret.insert({"-crop", &CreateCropFilter});
    ret.insert({"-gs", &CreateGrayScaleFilter});
    ret.insert({"-neg", &CreateNegativeFilter});
    ret.insert({"-sharp", &CreateSharpeningFilter});
    ret.insert({"-edge", &CreateEdgeDetectionFilter});
    ret.insert({"-blur", &CreateGaussianBlurFilter});
    return ret;
}
