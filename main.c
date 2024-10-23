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
        L"비디오 반납",
        L"종료",
    };
    while(1) {
        int s =  select_view(title,sel,5);
        if (s==0) {
            user_ui();
        }else if(s==1){
            vidio_ui();
        }else if(s==2){
            int vidio_id;
            int user_id;

            input_View_Int(L"빌릴 비디오 ID 입력", &vidio_id);
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
        }else if(s==3){
            int vidio_id;
            int user_id;
            int good;

            input_View_Int(L"반납할 유저 ID 입력", &user_id);
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
            
            Real_Vidio *rv = realVidioList;
            while(rv) {
                if (rv->vidio_id==vidio_id && rv->useing==1) // TODO: 이거 ㅡ냥 없
                rv = rv->next;
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

            rv->useing=0;
            rvh->return_bool=1;
            rvh->return_date= time(NULL);
            rvh->good=good;
            saveData();
            wprintf(L"반납하였습니다 ID = %d\n",rvh->id);

            wprintf(L"아무 키나 누르세요.");
            getchar();
        }else {
            break;
        }
    }
    return 0;
}