#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int main() {
    FILE *file;
    wchar_t symbol;

    // Открываем файл для записи в UTF-8 кодировке
    file = fopen("sample.txt", "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Заполняем файл символами в диапазоне от U+00A0 до U+00FF
    for (symbol = 0x0000; symbol <= 0x00FF; symbol++) {
        fputwc(symbol, file);
    }

    // Закрываем файл
    fclose(file);

    printf("Файл заполнен символами Юникода.\n");
    return 0;
}
