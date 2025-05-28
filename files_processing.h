#pragma once

#include "parcer.h"
#include "tmatrix.h"
#include "RGB.h"

void ReadBMP(const Parcer& parced_args, TMatrix<RGB>& ret);

void SaveBMP(const Parcer& parced_args, TMatrix<RGB>& image);
