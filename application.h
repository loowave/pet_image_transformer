#pragma once

#include "pipeline.h"
#include "files_processing.h"

class Application {
public:
    Application() = delete;

    Application(int argc, char** argv) : parced_args_(argc, argv) {
        prod_map_ = InitProducers();
        CreatePipeline(pipeline_, prod_map_, parced_args_.GetFilterDescriptors());
    }

    int Run();

protected:
    Parser parced_args_;
    FilterCreatorsMap prod_map_;
    Pipeline pipeline_;
};
