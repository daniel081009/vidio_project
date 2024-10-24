#include "vidio_ui.h"
void vidio_ui() {
    while(1) {
        wchar_t title[50] = L"비디오 관리";
        wchar_t sel[][50] = {
            L"비디오 데이터 관리",
            L"재고 관리",
            L"대여 관리",
            L"뒤로가기",
        };
        int s = select_view(title, sel, 4);
        if (s==0){
            vidio_vidio_ui();
        }else if (s == 1) {
            real_vidio_ui();
        }else if(s==2) {
            real_vidio_history_ui();
        } else {
            break;
        }
    }
}
