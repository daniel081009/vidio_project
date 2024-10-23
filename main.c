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
        L"종료",
    };
    while(1) {
        int s =  select_view(title,sel,3);
        if (s==0) {
            user_ui();
        }else if(s==1){
            vidio_ui();
        }else {
            break;
        }
    }

    return 0;
}