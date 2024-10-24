#include "vidio_ui.h"

void real_vidio_history_ui() {
    while (1) {
        clearScreen();
        wchar_t history_title[50] = L"대여 관리";
        wchar_t history_sel[][50] = {
            L"비디오 대여",
            L"비디오 반납",
            L"전체 대여 기록",
            L"개인 대여 기록",
            L"뒤로가기",
        };
        int history_s = select_view(history_title, history_sel, 5);
        
        if(history_s==0){
            clearScreen();
            int vidio_id;
            int user_id;

            input_View_Int(L"\n빌릴 비디오 ID 입력", &vidio_id);
            Real_Vidio *head_vidio_list = realVidioList;
            while(head_vidio_list) {
                if (head_vidio_list->useing==0 && head_vidio_list->vidio_id == vidio_id) {
                    break;
                }
                head_vidio_list = head_vidio_list->next;
            }
            if (head_vidio_list==NULL) {
                wprintf(L"빌릴수 있는 재고가 없습니다.\n아무 키나 누르세요.");
                getchar();
                continue;
            }

            input_View_Int(L"빌릴 유저 ID 입력", &user_id);
            User *u = readUser(user_id);
            if (u==NULL) {
                wprintf(L"존재하지 않는 유저입니다.\n아무 키나 누르세요.");
                getchar();
                continue;
            }
            int id = createRealVidioHistory(user_id,head_vidio_list->id,vidio_id,0);
            head_vidio_list->useing=1;

            if (id != -1) {
                saveData();
                wprintf(L"기록되었습니다 Real_Vidio_History ID = %d\n", id);
            } else {
                wprintf(L"대여에 실패하였습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else if(history_s==1){
            clearScreen();
            int vidio_id;
            int user_id;
            int good;

            input_View_Int(L"\n반납할 유저 ID 입력", &user_id);
            User *u = readUser(user_id);
            if (u==NULL) {
                wprintf(L"존재하지 않는 유저입니다.\n아무 키나 누르세요.");
                getchar();
                continue;
            }
            input_View_Int(L"빌린 비디오 ID 입력", &vidio_id);
            Vidio *v = readVidio(vidio_id);
            if (v==NULL) {
                wprintf(L"존재하지 않는 비디오입니다.\n아무 키나 누르세요.");
                getchar();
                continue;
            }

            Real_Vidio_History *rvh = historyList;
            while(rvh) {
                if (rvh->use_user_id == u->uqid && rvh->vidio_id == vidio_id && rvh->return_bool==0) break;
                rvh = rvh->next;
            }
            if (rvh==NULL) {
                wprintf(L"대여하지 않았습니다.\n아무 키나 누르세요.");
                getchar();
                continue;
            }
            input_View_Int(L"평점(0~10)",&good);

            Real_Vidio *rv =readRealVidio(rvh->real_vidio_id);
            rv->useing=0;

            rvh->return_bool=1;
            rvh->return_date= time(NULL);
            rvh->good=good;
            wprintf(L"반납하였습니다 ID = %d\n",rvh->id);

            saveData();
            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else if (history_s == 2) { // 전체 기록 확인
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

                    wprintf(L"기록 ID: %d\n사용자 ID: %d\n비디오 ID: %d\n리얼비디오 ID: %d\n대여일: %ls\n반납일: %ls\n", 
                            current->id, current->use_user_id, current->vidio_id, current->real_vidio_id, formatted_borrowed_date, formatted_return_date);
                    wprintf(L"반납 여부: %s\n평점: %d\n", 
                            current->return_bool ? L"반납됨" : L"미반납", current->good);
                    wprintf(L"----\n\n");
                    current = current->next;
                }
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();
        } else if (history_s == 3) { // 개인 유저 기록 확인
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

                    wprintf(L"기록 ID: %d\n리얼비디오 ID: %d\n대여일: %ls\n반납일: %ls\n", 
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
