#include "application.h"
#include <iostream>

int main(int argc, char** argv) {
    try {
        Application app{argc, argv};
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 2;
    }
    return 0;
}
