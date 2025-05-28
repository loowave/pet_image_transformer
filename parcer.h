#pragma once

#include <vector>
#include <stdexcept>

// Абстракция для описания фильтров
struct FilterDescriptor {
public:
    explicit FilterDescriptor(char**& arg) {
        if (**arg != '-') {
            throw std::invalid_argument("filter name written incorrectly");
        }
        name_ = *arg;
        ++arg;
        while (*arg != nullptr && **arg != '-') {
            params_.push_back(*arg);
            ++arg;
        }
    }

    const char* GetName() const {
        return name_;
    }

    std::vector<char*> GetParams() const {
        return params_;
    }

protected:
    const char* name_;
    std::vector<char*> params_;
};

// parser аргументов входной строки
class Parser {
public:
    Parser(int argc, char** argv) {
        if (argc < 3) {
            throw std::invalid_argument("not enough parameters");
        }
        program_name_ = *argv;
        ++argv;
        input_file_ = *argv;
        ++argv;
        output_file_ = *argv;
        ++argv;
        while (*argv != nullptr) {
            descriptors_.emplace_back(FilterDescriptor(argv));
        }
    }

    const char* GetProgramName() const {
        return program_name_;
    }

    const char* GetInputFile() const {
        return input_file_;
    }

    const char* GetOutputFile() const {
        return output_file_;
    }

    const std::vector<FilterDescriptor>& GetFilterDescriptors() const {
        return descriptors_;
    }

protected:
    const char* program_name_;
    const char* input_file_;
    const char* output_file_;
    std::vector<FilterDescriptor> descriptors_;
};
