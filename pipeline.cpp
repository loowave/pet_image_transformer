#include "pipeline.h"

void Pipeline::AddFilter(Filter* filter_ptr) {
    filters_.push_back(filter_ptr);
}

void Pipeline::Apply(TMatrix<RGB>& image) const {
    for (Filter* filter_ptr : filters_) {
        filter_ptr->Apply(image);
    }
}

void CreatePipeline(Pipeline& pipeline, const FilterCreatorsMap& map, const std::vector<FilterDescriptor>& fd_list) {
    for (const FilterDescriptor& fd : fd_list) {
        if (map.find(fd.GetName()) == map.end()) {
            throw std::invalid_argument("Invalid filter name");
        }
        pipeline.AddFilter(map.at(fd.GetName())(fd));
    }
}
