#include "vidio_ui.h"

void real_vidio_history_ui() {
    while (1) {
        clearScreen();
        wchar_t history_title[50] = L"대여 기록 관리";
        wchar_t history_sel[][50] = {
            L"전체 기록",
            L"개인 유저 기록",
            L"뒤로가기",
        };
        int history_s = select_view(history_title, history_sel, 3);

        if (history_s == 0) { // 전체 기록 확인
            clearScreen();
            Real_Vidio_History *current = historyList;
            if (current == NULL) {
                wprintf(L"대여 기록이 비어 있습니다.\n");
            } else {
                while (current) {
                    wchar_t formatted_borrowed_date[100];
                    wchar_t formatted_return_date[100];
                    print_formatted_date(current->borrowed_date, formatted_borrowed_date, 100);
                    if (current->return_date > 0) {
                        print_formatted_date(current->return_date, formatted_return_date, 100);
                    } else {
                        wcscpy(formatted_return_date, L"반납되지 않음");
                    }

                    wprintf(L"기록 ID: %d\n사용자 ID: %d\n비디오 ID: %d\n대여일: %ls\n반납일: %ls\n", 
                            current->id, current->use_user_id, current->real_vidio_id, formatted_borrowed_date, formatted_return_date);
                    wprintf(L"반납 여부: %s\n평점: %d\n", 
                            current->return_bool ? L"반납됨" : L"미반납", current->good);
                    wprintf(L"----\n\n");
                    current = current->next;
                }
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (history_s == 1) { // 개인 유저 기록 확인
            clearScreen();
            int user_id;
            input_View_Int(L"확인할 사용자 ID 입력", &user_id);
            Real_Vidio_History *current = historyList;
            int found = 0;
            while (current) {
                if (current->use_user_id == user_id) {
                    wchar_t formatted_borrowed_date[100];
                    wchar_t formatted_return_date[100];
                    print_formatted_date(current->borrowed_date, formatted_borrowed_date, 100);
                    if (current->return_date > 0) {
                        print_formatted_date(current->return_date, formatted_return_date, 100);
                    } else {
                        wcscpy(formatted_return_date, L"반납되지 않음");
                    }

                    wprintf(L"기록 ID: %d\n비디오 ID: %d\n대여일: %ls\n반납일: %ls\n", 
                            current->id, current->real_vidio_id, formatted_borrowed_date, formatted_return_date);
                    wprintf(L"반납 여부: %s\n평점: %d\n", 
                            current->return_bool ? L"반납됨" : L"미반납", current->good);
                    wprintf(L"----\n\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                wprintf(L"ID %d인 사용자의 대여 기록이 없습니다.\n", user_id);
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else {
            break; // 뒤로가기
        }
    }
}
