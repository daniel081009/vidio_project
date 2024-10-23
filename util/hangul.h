#ifndef hangul_h
#define hangul_h

#define HANGUL_START 0xAC00
#define HANGUL_END 0xD7A3
#define CHOSUNG_COUNT 19
#define JUNGSUNG_COUNT 21
#define JONGSUNG_COUNT 28

#include <wctype.h>

extern wchar_t chosung[CHOSUNG_COUNT];
extern wchar_t jungsung[JUNGSUNG_COUNT];
extern wchar_t jongsung[JONGSUNG_COUNT];

void decompose_hangul_char(wchar_t input, wchar_t *output, int *index);
void decompose_hangul_string(const wchar_t *input, wchar_t *output);
void extract_chosung_string(const wchar_t *input, wchar_t *output);
int jamo_set_similarity(const wchar_t *name, const wchar_t *keyword);

#endif