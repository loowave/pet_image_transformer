// Шаблонная версия matrix
// Когда мы делаем шаблонную заготовку, пишем все в h файле

#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class TMatrix {
public:
    //ожидаемые (стандартные) для типов данных производнных от T

    using ValueType = T;
    using Pointer = T*;
    using ConstPointer = const T*;
    using Reference = ValueType&;

    //Представляет абстракцию над строчкой матрицы,
    //самодостаточную и компактную для копирования по значению
    class Row {
        friend class TMatrix<T>;

    public:
        T& operator[](size_t colnum) const {
            return matrix_->operator()(rownum_, colnum);
        }

        size_t Size() const {
            return matrix_->GetCollsNum();
        }

    private:
        // Разрешаем доступ к конструктору только матрице
        Row(TMatrix<T>* m, size_t rownum) : rownum_(rownum), matrix_(m) {
        }

    private:
        size_t rownum_;
        TMatrix<T>* matrix_;
    };

    class ConstRow {
        friend class TMatrix<T>;

    public:
        /*const int&*/ T& operator[](size_t colnum) const {
            return matrix_->operator()(rownum_, colnum);
        }

        size_t Size() const {
            return matrix_->GetCollsNum();
        }

    private:
        // Разрешаем доступ к конструктору только матрице
        ConstRow(const TMatrix<T>* m, size_t rownum) : rownum_(rownum), matrix_(m) {
        }

    private:
        size_t rownum_;
        const TMatrix<T>* matrix_;
    };

    class Col {
        friend class Matrix;

    public:
        T& operator[](size_t rownum) const {
            return matrix_->operator()(rownum, colnum_);
        }

        size_t Size() const {
            return matrix_->GetRowsNum();
        }

    private:
        // Разрешаем доступ к конструктору только матрице
        Col(TMatrix<T>* m, size_t colnum) : colnum_(colnum), matrix_(m) {
        }

    private:
        size_t colnum_;
        TMatrix<T>* matrix_;
    };

    class ConstCol {
        friend class TMatrix<T>;

    public:
        /*const int&*/ T& operator[](size_t rownum) const {
            return matrix_->operator()(rownum, colnum_);
        }

        size_t Size() const {
            return matrix_->GetRowsNum();
        }

    private:
        // Разрешаем доступ к конструктору только матрице
        ConstCol(const TMatrix<T>* m, size_t colnum) : colnum_(colnum), matrix_(m) {
        }

    private:
        size_t colnum_;
        const TMatrix<T>* matrix_;
    };

public:
    // Если мы явным образом не создали никаких конструкторов, то будет создан конструктор по умолчанию,
    // который делает ничего.

    // Matrix() = delete; Такая запись позволяет удалить дефолтные, создаваемые компилятором,
    // члены класса.

    // Конструктор по умолчанию

    TMatrix() : rows_num_{0}, cols_num_{0}, table_(nullptr) {
    }

    TMatrix(TMatrix<T>& other) = delete;  // удаляем, потому что делает не так,
                                          // как хотим (не удаляем потому что нужно :( )

    // Конструктор создаёт матрицу размера m x n и задаёт значение элементов по умолчанию
    // Если один из параметров не равен 0, то другой тоже. Иначе кидаем исключительную ситуацию
    explicit TMatrix(size_t rows_num = 0, size_t cols_num = 0, T def = T{})  // теперь T default constractable
    {
        if (rows_num == 0 && cols_num == 0) {
            rows_num_ = 0;
            cols_num_ = 0;
            table_ = nullptr;
            return;
        }

        if (rows_num == 0 || cols_num == 0) {
            throw std::invalid_argument("Can't make matrix with 0 rows or columns");
        }
        rows_num_ = rows_num;
        cols_num_ = cols_num;
        table_ = AllocateMatrix(rows_num_, cols_num_);

        // Нежизненно:
        for (size_t i = 0; i < rows_num; ++i) {
            for (size_t j = 0; j < cols_num; ++j) {
                this->operator()(i, j) = def;
            }
        }
    }

    ~TMatrix() {
        Clear();
    }

    // Возвращает i,j -й элемент матрицы
    T operator()(size_t i, size_t j) const {
        // return table_[i * cols_num_ + j];
        return GetIJEl(table_, i, j, cols_num_);
    }

    T& operator()(size_t i, size_t j) {
        // return table_[i * cols_num_ + j];
        return GetIJEl(table_, i, j, cols_num_);
    }

    // Возвращает i-ю строку, обёрнутую в абстракцию
    Row operator[](size_t i) {
        // return Row(this, i);
        /*typename*/ TMatrix<T>::Row ri(this, i);
        return ri;
    }

    ConstRow operator[](size_t i) const {
        return ConstRow(this, i);
    };

    T At(size_t i, size_t j) const {  // нужно подставлять наиболее подходяшее: const & или значение
        if (i < rows_num_ && j < cols_num_) {
            return this->operator()(i, j);
        }
        throw std::out_of_range("IOJ (matrix).");
    }

    // T& At(size_t i, size_t j);
    Reference At(size_t i, size_t j) {
        if (i < rows_num_ && j < cols_num_) {
            return this->operator()(i, j);
        }
        throw std::out_of_range("IOJ (matrix).");
    }

public:
    void Resize(size_t new_col_num, size_t new_row_num, T def = T{}) {
        if (new_col_num == 0 && new_row_num == 0) {
            Clear();
            return;
        }
        if (new_row_num == 0 || new_col_num == 0) {
            throw std::invalid_argument("Can't make matrix with 0 rows or columns");
        }

        T* new_table = AllocateMatrix(new_row_num, new_col_num);
        for (size_t i = 0; i < new_row_num; ++i) {
            for (size_t j = 0; j < new_col_num; ++j) {
                if (i < rows_num_ && j < cols_num_) {
                    GetIJEl(new_table, i, j, new_col_num) = GetIJEl(table_, i, j, cols_num_);
                } else {
                    GetIJEl(new_table, i, j, new_col_num) = def;
                }
            }
        }
        delete[] table_;
        table_ = new_table;
        rows_num_ = new_row_num;
        cols_num_ = new_col_num;
    }

    void Clear() {
        if (!table_) {
            return;
        }
        delete[] table_;
        rows_num_ = 0;
        cols_num_ = 0;
    }

    void CopyContent(const TMatrix<T>& other) {
        if (rows_num_ != other.GetRowsNum() || cols_num_ != other.GetCollsNum()) {
            throw std::invalid_argument("incorrect matrix for copying");
        }
        for (size_t i = 0; i < rows_num_; ++i) {
            for (size_t j = 0; j < cols_num_; ++j) {
                this->operator()(i, j) = other(i, j);
            }
        }
    }

    size_t GetRowsNum() const {
        return rows_num_;
    }

    size_t GetCollsNum() const {
        return cols_num_;
    }

    // Неконстантная перегрузка позволяет изменять подлежащую память
    T* GetPtr() {
        return table_;
    }

    // Константная перегрузка позволяет только читать подлежащую память
    const T* GetPtr() const {
        return table_;
    }

    // help-ерная функция, которая по 4 параметрам позволяет получить правильное место эл-та 5-ого параметра
    // static T& GetIJEl(T* arr, size_t i, size_t j, size_t cols_num){
    static Reference GetIJEl(T* arr, size_t i, size_t j, size_t cols_num) {
        return arr[i * cols_num + j];
    }

private:
    // Метод аллоцирует память в кучу для заданных параметров n и m
    // Методу от this ничего не надо, поэтому он статический
    static T* AllocateMatrix(size_t rows_n, size_t cols_m) {
        T* ans = new T[rows_n * cols_m];
        return ans;
    }

private:
    // T* table_;
    Pointer table_;  // читается тяжелее чем T*, но так написано в стандарте
    size_t rows_num_;
    size_t cols_num_;
};
