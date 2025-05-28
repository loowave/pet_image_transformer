#include "application.h"

int Application::Run() {
    TMatrix<RGB> image{0, 0, {0, 0, 0}};
    ReadBMP(parced_args_, image);
    pipeline_.Apply(image);
    SaveBMP(parced_args_, image);
    return 0;
}
