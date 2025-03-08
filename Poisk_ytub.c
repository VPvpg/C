#include <stdio.h>         // Подключаем библиотеку для ввода-вывода (printf, fgets)
#include <stdlib.h>        // Подключаем библиотеку для функций system и exit
#include <string.h>        // Подключаем библиотеку для работы со строками (strlen)
#include <windows.h>       // Подключаем библиотеку Windows API для работы с кодировками

// Функция для конвертации CP866 в UTF-8
void cp866ToUtf8(char *dest, const char *src, size_t destSize) {
    wchar_t wideStr[256];  // Создаем буфер для промежуточного хранения Unicode-строки
    MultiByteToWideChar(866, 0, src, -1, wideStr, 256);  // Преобразуем CP866 в Unicode
    WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, dest, destSize, NULL, NULL);  // Преобразуем Unicode в UTF-8
}

// Функция для URL-кодирования
void urlEncode(char *dest, const char *src, size_t destSize) {
    size_t i = 0, j = 0;   // Инициализируем индексы для исходной и результирующей строки
    while (src[i] && j < destSize - 4) {  // Пока есть символы и хватает места в буфере
        if ((src[i] >= 'A' && src[i] <= 'Z') ||  // Если символ - латинская буква
            (src[i] >= 'a' && src[i] <= 'z') ||  // или строчная латинская буква
            (src[i] >= '0' && src[i] <= '9')) {  // или цифра
            dest[j++] = src[i];  // Копируем символ как есть
        } else if (src[i] == ' ') {  // Если символ - пробел
            dest[j++] = '+';  // Заменяем пробел на "+"
        } else {  // Для всех остальных символов (включая кириллицу)
            sprintf(dest + j, "%%%02X", (unsigned char)src[i]);  // Кодируем в %XX
            j += 3;  // Увеличиваем индекс на 3 (длина %XX)
        }
        i++;  // Переходим к следующему символу исходной строки
    }
    dest[j] = '\0';  // Добавляем завершающий нуль в конец строки
}

void start() {
    SetConsoleCP(866);       // Устанавливаем кодовую страницу ввода CP866
    SetConsoleOutputCP(866); // Устанавливаем кодовую страницу вывода CP866

    // Выводим "Ищет в ютубе за сегодня." в кодировке CP866
    printf("\x88\x99\xa5\xe2 \xa2 \xed\xe2\xae\xa1\xa5 \xa7\xa0 \xe1\xa5\xa3\xae\xa4\xad\xef.\n");
    // Выводим "Что ищем? " в кодировке CP866
    printf("\x87\xe2\xae \xa8\x99\xa5\xac?\x20");

    char searchQuery[256];   // Создаем буфер для хранения введенного запроса
    fgets(searchQuery, sizeof(searchQuery), stdin);  // Считываем строку ввода пользователя

    size_t len = strlen(searchQuery);  // Получаем длину введенной строки
    if (len > 0 && searchQuery[len - 1] == '\n') {  // Если строка не пустая и заканчивается на \n
        searchQuery[len - 1] = '\0';  // Удаляем символ новой строки
    }

    char utf8Query[768];  // Создаем буфер для строки в UTF-8
    cp866ToUtf8(utf8Query, searchQuery, sizeof(utf8Query));  // Конвертируем CP866 в UTF-8

    char encodedQuery[768];  // Создаем буфер для URL-кодированной строки
    urlEncode(encodedQuery, utf8Query, sizeof(encodedQuery));  // Кодируем строку для URL

    char link[1024];  // Создаем буфер для полного URL
    // Формируем URL с запросом и фильтром "за сегодня"
    snprintf(link, sizeof(link), 
             "https://www.youtube.com/results?search_query=%s&sp=CAISBAgCEAE%%253D", 
             encodedQuery);

    char command[1024];  // Создаем буфер для команды запуска браузера
    // Формируем команду для открытия URL в браузере
    snprintf(command, sizeof(command), "start \"\" \"%s\"", link);
    system(command);  // Выполняем команду, открывая браузер
}

int main() {
    start();  // Вызываем основную функцию программы
    return 0;  // Завершаем программу с кодом успешного выполнения
}

// из тут: https://grok.com/share/bGVnYWN5_7ef3274d-4af3-4834-b63b-e7e9ea26332a
