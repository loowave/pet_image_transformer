#pragma once

#include "filter.h"

// класс для последовательного применения фильтров
class Pipeline {
public:
    Pipeline() = default;

    ~Pipeline() {
        for (size_t i = 0; i < filters_.size(); ++i) {
            delete filters_[i];
        }
    }

    void AddFilter(Filter* filter_ptr);

    void Apply(TMatrix<RGB>& image) const;

protected:
    std::vector<Filter*> filters_;
};

// функция, изменяющая объект класса pipeline по коллекции фильтров и FilterCreatorsMap
void CreatePipeline(Pipeline& pipeline, const FilterCreatorsMap& map, const std::vector<FilterDescriptor>& fd_list);
