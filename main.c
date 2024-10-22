#include "display.h"
#include "db.h"
#include "search.h"

#include <locale.h>
#include <windows.h>

void user_control() {
    while(1) {
        wchar_t title[50]=L"사용자 관리";
        wchar_t sel[][50] = {
            L"등록",
            L"편집",
            L"검색",
            L"삭제",
            L"사용자 리스트",
            L"뒤로가기",
        };

        int s =  select_view(title,sel,6);
        if (s==0) {
            clearScreen();
            wchar_t name[100];
            wchar_t address[100];
            char man;
            int birth_year;

            input_View(L"이름", name, L"%ls");
            input_View(L"주소", address, L"%ls");
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
            wchar_t name[50];
            input_View(L"검색어: ",name,50);
            int reslen = 0;
            UserSearchResult *res = search_user_all(name,userList,&reslen);
            
            qsort(res, reslen, sizeof(UserSearchResult), compare_results);

            wprintf(L"\n\n");
            int displayed = 0;
            while(displayed < 5 && res[displayed]== NULL) { //TODO: 이거알아서나중에작성 이제자러감
                wprintf(L"%d\n이름: %ls\n주소:%ls\n성별:%c\n\n생년:%d\n - %ls\n", res[displayed].u->uqid,res[displayed].u->name,
                                                                                res[displayed].u->address,res[displayed].u->man,res[displayed].u->birth_year);
                displayed++;
            }
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
        }else {
            break;
        }
    }
}
void vidio_control() {

}
void statistics_control() {

}


int main() {
    setlocale(LC_ALL, "korean");
    _wsetlocale(LC_ALL, L"korean");
    SetConsoleOutputCP(949);
    SetConsoleCP(949);



    loadData();

    wchar_t title[50]=L"Home";
    wchar_t sel[][50] = {
        L"사용자 관리",
        L"비디오 관리",
        L"통계",
        L"종료",
    };
    while(1) {
        int s =  select_view(title,sel,4);
        if (s==0) {
            user_control();
        }else if(s==1){
            vidio_control();
        }else if(s==2) {
            statistics_control();
        }else {
            break;
        }
    }

    return 0;
}