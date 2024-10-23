#include "vidio_ui.h"

void real_vidio_ui() {
    while(1) {
        clearScreen();
        wchar_t inventory_title[50] = L"재고 관리";
        wchar_t inventory_sel[][50] = {
            L"재고 추가",
            L"재고 삭제",
            L"재고 리스트",
            L"뒤로가기",
        };
        int inv_s = select_view(inventory_title, inventory_sel, 4);
        if (inv_s == 0) { // 재고 추가
            int vidio_id;
            input_View_Int(L"재고를 추가할 비디오 ID 입력", &vidio_id);
            int real_vidio_id = createRealVidio(vidio_id);
            if (real_vidio_id != -1) {
                saveData();
                wprintf(L"재고가 추가되었습니다. Real_Vidio ID = %d\n", real_vidio_id);
            } else {
                wprintf(L"재고 추가에 실패하였습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (inv_s == 1) { // 재고 삭제
            int real_vidio_id;
            input_View_Int(L"삭제할 재고의 Real_Vidio ID 입력", &real_vidio_id);
            int result = deleteRealVidio(real_vidio_id);
            if (result == 0) {
                saveData();
                wprintf(L"재고가 삭제되었습니다.\n");
            } else {
                wprintf(L"재고를 삭제할 수 없습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (inv_s == 2) { // 재고 리스트
            Real_Vidio *current = realVidioList;
            while (current) {
                wchar_t formatted_date[100];
                print_formatted_date(current->create_date, formatted_date, 100);
                wprintf(L"ID: %d\n비디오 ID: %d\n생성 날짜: %ls\n대여여부: %s\n",
                        current->id, current->vidio_id, formatted_date, current->useing ? L"예" : L"아니오");
                wprintf(L"----\n\n");
                current = current->next;
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();
        } else {
            break;
        }
    }
}