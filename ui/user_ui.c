#include "user_ui.h"

typedef struct {
    Vidio *u;
    double good;
} Vidio_good;

static int compare_results_good(const void *a, const void *b) {
    Vidio_good *vg1 = (Vidio_good *)a;
    Vidio_good *vg2 = (Vidio_good *)b;
    return vg2->good - vg1->good; 
}

void user_ui() {
    while(1) {
        wchar_t title[50]=L"사용자 관리";
        wchar_t sel[][50] = {
            L"등록",
            L"편집",
            L"검색",
            L"삭제",
            L"사용자 리스트",
            L"선호 비디오 추천",
            L"뒤로가기",
        };

        int s =  select_view(title,sel,7);
        if (s==0) {
            clearScreen();
            wchar_t name[100];
            wchar_t address[100];
            char man;
            int birth_year;
            
            input_View(L"이름", name,sizeof(name) / sizeof(wchar_t));
            input_View(L"주소", address, sizeof(address) / sizeof(wchar_t));
            input_View_Char(L"성별(0 또는 1)", &man);
            input_View_Int(L"출생 연도", &birth_year);

            int id = createUser(name,address,man,birth_year);
            wprintf(L"정상적으로 생성되었습니다. id=%d\n아무키나 누르세요",id);
            saveData();
            getchar();
        }else if(s==1){
            clearScreen();
            int uqid;
            input_View_Int(L"수정할 사용자 ID 입력", &uqid);
            User *user = readUser(uqid);
            if(user) {
                wprintf(L"현재 정보:\n");
                wprintf(L"이름: %ls\n", user->name);
                wprintf(L"주소: %ls\n", user->address);
                wprintf(L"성별: %c\n", user->man);
                wprintf(L"출생 연도: %d\n", user->birth_year);

                wchar_t name[100];
                wchar_t address[100];
                char man;
                int birth_year;

                input_View(L"새 이름 (공백 시 기존 값 유지)", name, 100);
                input_View(L"새 주소 (공백 시 기존 값 유지)", address, 100);
                input_View_Char(L"새 성별(0 또는 1, 2입력시 기존 값 유지)", &man);
                input_View_Int(L"새 출생 연도 (0 입력 시 기존 값 유지)", &birth_year);

                if(wcslen(name) > 0) {
                    wcscpy(user->name, name);
                }
                if(wcslen(address) > 0) {
                    wcscpy(user->address, address);
                }
                if(man != '\n') {
                    user->man = man;
                }
                if(birth_year != 0) {
                    user->birth_year = birth_year;
                }

                updateUser(user);
                saveData();
                wprintf(L"사용자 정보가 수정되었습니다.\n아무 키나 누르세요.");
                getchar();
            } else {
                wprintf(L"ID %d인 사용자를 찾을 수 없습니다.\n아무 키나 누르세요.", uqid);
                getchar();
            }
        }else if(s==2) { // 검색
            clearScreen();
            wchar_t name[50];
            input_View(L"검색어",name,50);
            int reslen = 0;
            UserSearchResult *res = search_user_all(name,userList,&reslen);
            qsort(res, reslen, sizeof(UserSearchResult), compare_results);

            wprintf(L"\n\n");
            int displayed = 0;
            while(displayed < 5 && displayed < reslen) {
                wprintf(L"id: %d\n이름: %ls\n주소:%ls\n성별:%c\n생년:%d\n\n----\n", 
                        res[displayed].u->uqid,
                        res[displayed].u->name,
                        res[displayed].u->address,
                        res[displayed].u->man,
                        res[displayed].u->birth_year);
                displayed++;
            }

            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else if (s==3) {
            clearScreen();
            int uqid;
            input_View_Int(L"삭제할 사용자 ID 입력", &uqid);
            int result = deleteUser(uqid);
            if(result == 0) {
                saveData();
                wprintf(L"사용자가 삭제되었습니다.\n");
            } else {
                wprintf(L"사용자를 삭제할 수 없습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else if (s==4) {
            clearScreen();
            User *current = userList;
            int max_uqid = 0;
            while (current) {
                wprintf(L"id: %d\n이름: %ls\n주소: %ls\n성별: %c\n출생 연도: %d\n----\n\n",current->uqid,current->name,current->address,current->man,current->birth_year);
                current = current->next;
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else if (s==5) {
            clearScreen();
            int uqid;
            input_View_Int(L"사용자 ID 입력", &uqid);

            int *user_see_vidio_id_list = NULL;
            int *user_see_vidio_good_list = NULL;
            int capacity = 10; // 초기 용량
            int count = 0;

            user_see_vidio_id_list = (int *)malloc(capacity * sizeof(int));
            user_see_vidio_good_list = (int *)malloc(capacity * sizeof(int));

            if (user_see_vidio_id_list == NULL || user_see_vidio_good_list == NULL) {
                wprintf(L"메모리 할당 실패\n");
                return;
            }

            Real_Vidio_History *rvh = historyList;
            while (rvh) {
                if (rvh->use_user_id == uqid && rvh->return_bool == 1) {
                    if (count >= capacity) {
                        capacity *= 2;
                        int *temp = realloc(user_see_vidio_id_list, capacity * sizeof(int));
                        if (temp == NULL) {
                            free(user_see_vidio_id_list);
                            free(user_see_vidio_good_list);
                            wprintf(L"메모리 재할당 실패\n");
                            return;
                        }
                        user_see_vidio_id_list = temp;

                        temp = realloc(user_see_vidio_good_list, capacity * sizeof(int));
                        if (temp == NULL) {
                            free(user_see_vidio_id_list);
                            free(user_see_vidio_good_list);
                            wprintf(L"메모리 재할당 실패\n");
                            return;
                        }
                        user_see_vidio_good_list = temp;
                    }
                    user_see_vidio_id_list[count] = rvh->vidio_id;
                    user_see_vidio_good_list[count] = rvh->good;
                    count++;
                }
                rvh = rvh->next;
            }

            if (count+1 < 8) {
                wprintf(L"예측하기 위해선 대여 기록이 8개 이상 필요합니다. 현재: %d개\n", count+1);
                wprintf(L"아무 키나 누르세요.");
                getchar();
                continue;
            }

            Position_pre *data = (Position_pre *)malloc(count * sizeof(Position_pre));
            if (data == NULL) {
                free(user_see_vidio_id_list);
                free(user_see_vidio_good_list);
                wprintf(L"메모리 할당 실패\n");
                return;
            }

            for (int i = 0; i < count; i++) {
                Vidio *v = readVidio(user_see_vidio_id_list[i]);
                data[i] = pos_to_pre(v->pos, user_see_vidio_good_list[i]);
            }

            double coefficients[8] = {0};
            multiple_linear_regression(data, count, coefficients);

            int total_vidio_count = lenvidio();
            Vidio_good *results = (Vidio_good *)malloc(total_vidio_count * sizeof(Vidio_good));
            if (results == NULL) {
                free(user_see_vidio_id_list);
                free(user_see_vidio_good_list);
                free(data);
                wprintf(L"메모리 할당 실패\n");
                return;
            }

            int i = 0;
            Vidio *v = vidioList;
            while (v) {
                results[i].u = v;
                results[i].good = predict_rating(&v->pos, coefficients);
                v = v->next;
                i++;
            }

            qsort(results, i, sizeof(Vidio_good), compare_results_good);

            int display = 0;
            while (display < 3 && display < i) {
                char sk=0;
                for (int i = 0; i < count; i++) {
                    if (results[display].u->id ==user_see_vidio_id_list[i]) {
                        sk=1;
                        break;
                    }
                }
                if (sk==1) {display++;continue;};
                wprintf(L"\n예측 평점: %.2f\n", results[display].good);
                printcur(results[display].u);
                display++;
            }

            free(user_see_vidio_id_list);
            free(user_see_vidio_good_list);
            free(data);
            free(results);

            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else {
            break;
        }
    }
}
