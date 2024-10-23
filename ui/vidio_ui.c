#include "vidio_ui.h"

void vidio_ui() {
    while(1) {
        wchar_t title[50] = L"비디오 관리";
        wchar_t sel[][50] = {
            L"추가",
            L"수정",
            L"삭제",
            L"비디오 리스트",
            L"재고 관리",
            L"대여 기록",
            L"뒤로가기",
        };
        int s = select_view(title, sel, 7);

        if (s == 0) { // 비디오 추가
            clearScreen();
            wchar_t name[100];
            wchar_t desc[1000];
            Position pos = {0};

            input_View(L"비디오 이름", name, 100);
            input_View(L"비디오 설명", desc, 1000);

            wprintf(L"장르 선택 (0~10 사이의 숫자)\n");
            input_View_Int(L"공포(horror)", (int *)&pos.horror);
            input_View_Int(L"코미디(comedy)", (int *)&pos.comedy);
            input_View_Int(L"액션(action)", (int *)&pos.action);
            input_View_Int(L"SF(sf)", (int *)&pos.sf);
            input_View_Int(L"판타지(fantasy)", (int *)&pos.fantasy);
            input_View_Int(L"로맨스(romance)", (int *)&pos.romance);
            input_View_Int(L"가족(family)", (int *)&pos.family);

            int id = createVidio(name, desc, pos);
            if (id != -1) {
                wprintf(L"비디오가 추가되었습니다. ID = %d\n", id);
                saveData();
            } else {
                wprintf(L"비디오 추가에 실패하였습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (s == 1) { // 비디오 수정
            clearScreen();
            int vidio_id;
            input_View_Int(L"수정할 비디오 ID 입력", &vidio_id);
            Vidio *vidio = readVidio(vidio_id);
            if (vidio) {
                wprintf(L"현재 정보:\n");
                wprintf(L"이름: %ls\n", vidio->name);
                wprintf(L"설명: %ls\n", vidio->desc);
                wprintf(L"생성 날짜: %ld\n", vidio->create_date);
                wprintf(L"장르:\n");
                wprintf(L"  공포: %d\n", vidio->pos.horror);
                wprintf(L"  코미디: %d\n", vidio->pos.comedy);
                wprintf(L"  액션: %d\n", vidio->pos.action);
                wprintf(L"  SF: %d\n", vidio->pos.sf);
                wprintf(L"  판타지: %d\n", vidio->pos.fantasy);
                wprintf(L"  로맨스: %d\n", vidio->pos.romance);
                wprintf(L"  가족: %d\n", vidio->pos.family);

                wchar_t name[100];
                wchar_t desc[1000];
                Position pos = vidio->pos;

                input_View(L"새 이름 (공백 시 기존 값 유지)", name, 100);
                input_View(L"새 설명 (공백 시 기존 값 유지)", desc, 1000);

                wprintf(L"장르 수정 (1: 예, 0: 아니오, -1: 변경 없음)\n");
                int temp;
                input_View_Int(L"공포(horror)", &temp);
                if (temp != -1) pos.horror = temp;
                input_View_Int(L"코미디(comedy)", &temp);
                if (temp != -1) pos.comedy = temp;
                input_View_Int(L"액션(action)", &temp);
                if (temp != -1) pos.action = temp;
                input_View_Int(L"SF(sf)", &temp);
                if (temp != -1) pos.sf = temp;
                input_View_Int(L"판타지(fantasy)", &temp);
                if (temp != -1) pos.fantasy = temp;
                input_View_Int(L"로맨스(romance)", &temp);
                if (temp != -1) pos.romance = temp;
                input_View_Int(L"가족(family)", &temp);
                if (temp != -1) pos.family = temp;

                if (wcslen(name) > 0) {
                    wcscpy(vidio->name, name);
                }
                if (wcslen(desc) > 0) {
                    wcscpy(vidio->desc, desc);
                }
                vidio->pos = pos;

                updateVidio(vidio);
                saveData();
                wprintf(L"비디오 정보가 수정되었습니다.\n아무 키나 누르세요.");
                getchar();
            } else {
                wprintf(L"ID %d인 비디오를 찾을 수 없습니다.\n아무 키나 누르세요.", vidio_id);
                getchar();
            }

        } else if (s == 2) { // 비디오 삭제
            clearScreen();
            int vidio_id;
            input_View_Int(L"삭제할 비디오 ID 입력", &vidio_id);
            int result = deleteVidio(vidio_id);
            if (result == 0) {
                saveData();
                wprintf(L"비디오가 삭제되었습니다.\n");
            } else {
                wprintf(L"비디오를 삭제할 수 없습니다.\n");
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (s == 3) { // 비디오 리스트
            clearScreen();
            Vidio *current = vidioList;
            while (current) {
                wprintf(L"ID: %d\n이름: %ls\n설명: %ls\n생성 날짜: %ld\n", 
                        current->id, current->name, current->desc, current->create_date);
                wprintf(L"장르:\n");
                wprintf(L"  공포: %d\n", current->pos.horror);
                wprintf(L"  코미디: %d\n", current->pos.comedy);
                wprintf(L"  액션: %d\n", current->pos.action);
                wprintf(L"  SF: %d\n", current->pos.sf);
                wprintf(L"  판타지: %d\n", current->pos.fantasy);
                wprintf(L"  로맨스: %d\n", current->pos.romance);
                wprintf(L"  가족: %d\n", current->pos.family);
                wprintf(L"----\n\n");
                current = current->next;
            }
            wprintf(L"아무 키나 누르세요.");
            getchar();

        } else if (s == 4) { // 재고 관리
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
                    wprintf(L"Real_Vidio ID: %d\n비디오 ID: %d\n생성 날짜: %ld\n사용 중: %s\n",
                            current->id, current->vidio_id, current->create_date, current->useing ? L"예" : L"아니오");
                    wprintf(L"----\n\n");
                    current = current->next;
                }
                wprintf(L"아무 키나 누르세요.");
                getchar();
            } else {
                // 뒤로가기
            }
        }else if(s==5){

        } else {
            break; // 뒤로가기
        }
    }
}
