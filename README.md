Это консольное приложение на C++, которое применяет к изображениям последовательность фильтров.  
Проект создан как pet-проект для практики работы с изображениями, проектирования архитектуры и использования C++.


---

## Возможности

-  Чтение bmp-файлов
-  Применение настраиваемой последовательности фильтров:
    - Crop (-crop width height)
    - Grayscale (-gs)
    - Negative (-neg)
    - Sharpening (-sharp)
    - Edge Detection (-edge threshold)
    - Gaussian Blur (-blur sigma)
-  Сохранение результата в bmp-файл
-  Расширяемая архитектура: легко добавлять новые фильтры

---

##  Сборка и запуск

### Зависимости

- C++17
- CMake ≥ 3.10
### 📦 Сборка (на Linux)

```
git clone https://github.com/loowave/pet_image_transformer.git
cd pet_image_transformer
cmake --build cmake-build-debug/
./cmake-build-debug/untitled1 <input_image> <output_image> <filter> <filter_args>
```

### Пример
```
./cmake-build-debug/untitled1 ./input_files/lenna.bmp ./output_files/test_lenna.bmp -gs
```
