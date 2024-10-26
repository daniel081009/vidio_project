#include "vidio_ui.h"

void retrygetint(wchar_t *s,int *output,int min,int max) {
    while (1) {
        input_View_Int(s, output);
        if (min <= output <=max) break;
    }
}

void printcur(Vidio *current) {
    wchar_t formatted_date[100];
    print_formatted_date(current->create_date, formatted_date, 100);

    wprintf(L"ID: %d\n이름: %ls\n설명: %ls\n생성 날짜: %ls\n", 
            current->id, current->name, current->desc, formatted_date);
    int rv_len = 0;
    int rv_not_ret =0;
    Real_Vidio *rv=realVidioList;
    while (rv) {
        if (rv->vidio_id == current->id && rv->useing==1) rv_not_ret++;
        else if (rv->vidio_id== current->id)rv_len++;
        rv= rv->next;
    }

    wprintf(L"재고: %d\n대여중인 재고: %d\n",rv_len,rv_not_ret);
    wprintf(L"장르:\n");
    wprintf(L"  공포: %d\n", current->pos.horror);
    wprintf(L"  코미디: %d\n", current->pos.comedy);
    wprintf(L"  액션: %d\n", current->pos.action);
    wprintf(L"  SF: %d\n", current->pos.sf);
    wprintf(L"  판타지: %d\n", current->pos.fantasy);
    wprintf(L"  로맨스: %d\n", current->pos.romance);
    wprintf(L"  가족: %d\n", current->pos.family);
    wprintf(L"----\n\n");
}

void vidio_vidio_ui() {
    while(1) {
        wchar_t title[50] = L"비디오 데이터 관리";
        wchar_t sel[][50] = {
            L"추가",
            L"수정",
            L"삭제",
            L"비디오 리스트",
            L"검색",
            L"뒤로가기",
        };
        int s = select_view(title, sel, 6);
        if (s == 0) { // 비디오 추가
            clearScreen();
            wchar_t name[100];
            wchar_t desc[1000];
            Position pos = {0};

            input_View(L"비디오 이름", name, 100);
            input_View(L"비디오 설명", desc, 1000);

            wprintf(L"장르 선택 (0~10 사이의 숫자)\n");
            
            retrygetint(L"공포(horror)", (int *)&pos.horror,0,10);
            retrygetint(L"코미디(comedy)", (int *)&pos.comedy,0,10);
            retrygetint(L"액션(action)", (int *)&pos.action,0,10);
            retrygetint(L"SF(sf)", (int *)&pos.sf,0,10);
            retrygetint(L"판타지(fantasy)", (int *)&pos.fantasy,0,10);
            retrygetint(L"로맨스(romance)", (int *)&pos.romance,0,10);
            retrygetint(L"가족(family)", (int *)&pos.family,0,10);

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
            wchar_t formatted_date[100];
            print_formatted_date(vidio->create_date, formatted_date, 100);

            if (vidio) {
                wprintf(L"현재 정보:\n");
                wprintf(L"이름: %ls\n", vidio->name);
                wprintf(L"설명: %ls\n", vidio->desc);
                wprintf(L"생성 날짜: %ls\n",formatted_date);
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
        }else if (s==3){
            clearScreen();
            Vidio *current = vidioList;

            if (current == NULL) {
                wprintf(L"비디오 리스트가 비어 있습니다.\n");
                wprintf(L"아무 키나 누르세요.");
                getchar();
            } else {
                while (current) {
                    printcur(current);
                    current = current->next;
                }
                wprintf(L"아무 키나 누르세요.");
                getchar();
            }
        }else if (s==4){
            clearScreen();
            wchar_t name[1000];
            input_View(L"검색어", name, 50);
            int reslen = 0;
            VidioSearchResult *res = search_vidio_all(name, vidioList, &reslen);
            qsort(res, reslen, sizeof(VidioSearchResult), compare_results);

            wprintf(L"\n\n");
            int displayed = 0;
            while (displayed < 5 && displayed < reslen) {
                printcur(res[displayed].u);
                displayed++;
            }

            wprintf(L"아무 키나 누르세요.");
            getchar();

        }else{
            break;
        }
    }
    
}