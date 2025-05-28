#include "files_processing.h"

void ReadBMP(const Parcer& parced_args, TMatrix<RGB>& ret) {
    FILE* f = std::fopen(parced_args.GetInputFile(), "rb");
    if (!f) {
        throw std::runtime_error("Unable to open the file");
    }
    // Считываем заголовок файла
    BMPHeader file_header;
    size_t reading_res = std::fread(&file_header, 1, sizeof(BMPHeader), f);
    if (reading_res != sizeof(BMPHeader) || file_header.file_type != FILE_TYPE || file_header.bit_count != BIT_COUNT) {
        std::fclose(f);
        throw std::runtime_error("Invalid file format");
    }
    ret.Resize(static_cast<size_t>(file_header.width), static_cast<size_t>(file_header.height), RGB{0, 0, 0});
    int bytes_in_row = 3 * file_header.width + file_header.width % 4;
    unsigned char* tmp_buf = new unsigned char[bytes_in_row * file_header.height];  // временный буфер для чтения
    // Считываем данные о пикселях
    reading_res = std::fread(tmp_buf, 1, bytes_in_row * file_header.height, f);
    std::fclose(f);
    if (reading_res != bytes_in_row * file_header.height) {
        throw std::runtime_error("Not enough pixel data in file");
    }
    for (int y = 0; y < file_header.height; ++y) {
        unsigned char* row_pt = tmp_buf + y * bytes_in_row;
        for (int x = 0; x < file_header.width; ++x) {
            ret.At(ret.GetRowsNum() - y - 1, x).rgbRed = *(row_pt + 2);
            ret.At(ret.GetRowsNum() - y - 1, x).rgbGreen = *(row_pt + 1);
            ret.At(ret.GetRowsNum() - y - 1, x).rgbBlue = *row_pt;
            row_pt += 3;
        }
    }
    delete[] tmp_buf;
}

void SaveBMP(const Parcer& parced_args, TMatrix<RGB>& image) {
    int colls = static_cast<int>(image.GetCollsNum());
    int rows = static_cast<int>(image.GetRowsNum());
    BMPHeader bh;  // заполним зааголовок файла, sizeof(BMPheader) = 54
    bh.file_type = FILE_TYPE;
    // Найдем кол-во байт в строке, включая округление вверх до кратного 4:
    int bytes_in_row = colls * 3 + (4 - (colls * 3) % 4) % 4;
    uint32_t filesize = HEADER_SIZE + rows * bytes_in_row;
    bh.file_size = filesize;
    bh.reserved = 0;
    bh.planes = 1;
    bh.size = INFO_SIZE;
    bh.offset_data = HEADER_SIZE;
    bh.width = colls;
    bh.height = rows;
    bh.bit_count = BIT_COUNT;
    bh.compression = 0;
    FILE* f = std::fopen(parced_args.GetOutputFile(), "wb");
    if (!f) {
        throw std::runtime_error("Unable to open the file");
    }
    std::fwrite(&bh, 1, sizeof(BMPHeader), f);

    // Временный буфер для последующей записи
    unsigned char* tmp_buf = new unsigned char[rows * bytes_in_row];
    // Перенос данных о пискселях
    for (int y = rows - 1; y >= 0; y--) {
        unsigned char* row_ptr = tmp_buf + bytes_in_row * y;
        for (int x = 0; x < colls; ++x) {
            *(row_ptr + 2) = image(image.GetRowsNum() - y - 1, x).rgbRed;
            *(row_ptr + 1) = image(image.GetRowsNum() - y - 1, x).rgbGreen;
            *row_ptr = image(image.GetRowsNum() - y - 1, x).rgbBlue;
            row_ptr += 3;
        }
    }
    // запишем tmp_buf в файл
    std::fwrite(tmp_buf, 1, rows * bytes_in_row, f);
    std::fclose(f);
    delete[] tmp_buf;
}
