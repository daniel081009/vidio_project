#include "hangul.h"

wchar_t chosung[CHOSUNG_COUNT] = {L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ',
                                  L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
                                  L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};
wchar_t jungsung[JUNGSUNG_COUNT] = {L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ',
                                    L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
                                    L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ',
                                    L'ㅡ', L'ㅢ', L'ㅣ'};
wchar_t jongsung[JONGSUNG_COUNT] = {L' ', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ',
                                    L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ',
                                    L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ',
                                    L'ㅄ', L'ㅅ', L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
                                    L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};

int jamo_set_similarity(const wchar_t *name, const wchar_t *keyword) {
    int name_len = wcslen(name);
    int keyword_len = wcslen(keyword);
    int match_count = 0;

    for (int i = 0; i < keyword_len; i++) {
        for (int j = 0; j < name_len; j++) {
            if (keyword[i] == name[j]) {
                match_count++;
                break;
            }
        }
    }

    int distance = keyword_len - match_count;
    return distance;
}

void decompose_hangul_char(wchar_t input, wchar_t *output, int *index) {
    if (input >= HANGUL_START && input <= HANGUL_END) {
        int syllable_index = input - HANGUL_START;
        wchar_t cho = chosung[syllable_index / (JUNGSUNG_COUNT * JONGSUNG_COUNT)];
        wchar_t jung = jungsung[(syllable_index % (JUNGSUNG_COUNT * JONGSUNG_COUNT)) / JONGSUNG_COUNT];
        wchar_t jong = jongsung[syllable_index % JONGSUNG_COUNT];

        output[(*index)++] = cho;
        output[(*index)++] = jung;
        if (jong != L' ') {
            output[(*index)++] = jong;
        }
    } else {
        output[(*index)++] = input;
    }
}

void decompose_hangul_string(const wchar_t *input, wchar_t *output) {
    int index = 0;
    for (int i = 0; input[i] != L'\0'; i++) {
        decompose_hangul_char(input[i], output, &index);
    }
    output[index] = L'\0';
}

void extract_chosung_string(const wchar_t *input, wchar_t *output) {
    int index = 0;
    for (int i = 0; input[i] != L'\0'; i++) {
        if (input[i] >= HANGUL_START && input[i] <= HANGUL_END) {
            int syllable_index = input[i] - HANGUL_START;
            wchar_t cho = chosung[syllable_index / (JUNGSUNG_COUNT * JONGSUNG_COUNT)];
            output[index++] = cho;
        } else {
            output[index++] = input[i];
        }
    }
    output[index] = L'\0';
}
