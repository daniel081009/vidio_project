#ifndef display_h
#define display_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <conio.h>

#define UP 72
#define DOWN 80

void clearScreen();

void print_select_view(int idx, wchar_t array[][50], int size);
int select_view(wchar_t title[50], wchar_t array[][50], int size);

void input_View(wchar_t prompt[100], wchar_t *output, size_t size); 
void input_View_Char(wchar_t prompt[100], char *output);
void input_View_Int(wchar_t prompt[100], int *output);

#endif