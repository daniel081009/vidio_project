#include "display.h"
#include <conio.h>
#include <locale.h>
#include <stdarg.h>

void clearScreen() {
    system("@cls||clear");
}

void print_select_view(int idx, wchar_t array[][50], int size) {
    for (int i = 0; i < size; i++) {
        if (i == idx) {
            wprintf(L"*  %ls\n", array[i]);
        } else {
            wprintf(L"   %ls\n", array[i]);
        }
    }
}

int select_view(wchar_t title[50], wchar_t array[][50], int size) {
    int ma = 0;
    int max = size - 1;
    char c;
    while (1) {
        clearScreen();
        wprintf(L"%ls\n", title);
        print_select_view(ma, array, size);
        c = _getwch();
        if (c == -32 || c == 0) {
            c = _getwch();
            switch (c) {
                case UP:
                    if (ma > 0) ma--;
                    else ma = max;
                    break;
                case DOWN:
                    if (ma < max) ma++;
                    else ma = 0;
                    break;
                default:
                    break;
            }
        } else if (c == 13) { 
            return ma;
        }
    }
}

void input_View(wchar_t prompt[100], wchar_t *output, size_t size) {
    wprintf(L"%ls : ", prompt);
    
    fgetws(output, size, stdin);

    size_t len = wcslen(output);
    if (len > 0 && output[len - 1] == L'\n') {
        output[len - 1] = L'\0';
    }
}

void input_View_Char(wchar_t prompt[100], char *output) {
    wprintf(L"%ls : ", prompt);
    *output = getchar();
    getchar();
}

void input_View_Int(wchar_t prompt[100], int *output) {
    wprintf(L"%ls : ", prompt);
    wscanf(L"%d", output);
    wchar_t buffer;
    while ((buffer = getwchar()) != '\n' && buffer != WEOF) {}
}