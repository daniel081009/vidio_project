#include <locale.h>
#include <windows.h>

#include "util/display.h"
#include "db/db.h"
#include "db/search.h"

#include "ui/user_ui.h"
#include "ui/vidio_ui.h"

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
        L"비디오 대여",
        L"종료",
    };
    while(1) {
        int s =  select_view(title,sel,4);
        if (s==0) {
            user_ui();
        }else if(s==1){
            vidio_ui();
        }else if(s==2){
            int vidio_id;
            int user_id;

            input_View_Int(L"빌릴 비디오 ID 입력", &vidio_id);
            Real_Vidio *head = realVidioList;
            while(head) {
                if (head->useing==0) {
                    

                }
                head = head->next;
            }

            input_View_Int(L"빌릴 유저 ID 입력", &user_id);
        }else {
            break;
        }
    }

    return 0;
}