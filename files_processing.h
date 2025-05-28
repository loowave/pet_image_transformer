#pragma once

#include "parcer.h"
#include "tmatrix.h"
#include "RGB.h"

void ReadBMP(const Parser& parced_args, TMatrix<RGB>& ret);

void SaveBMP(const Parser& parced_args, TMatrix<RGB>& image);
