#include "../db.h"

int vidio_last_uqid() {
    Vidio *current = vidioList;
    int max_id = 0;
    while (current) {
        if (current->id > max_id) {
            max_id = current->id;
        }
        current = current->next;
    }
    return max_id;
}

int lenvidio() {
    int index =0;
    Vidio *current = vidioList;
    while (current) {
        index++;
        current = current->next;
    }
    return index;
}

int createVidio(wchar_t *name, wchar_t *desc, Position pos) {
    int id = vidio_last_uqid() + 1;
    Vidio *vidio = (Vidio *)malloc(sizeof(Vidio));
    if (!vidio) return -1;
    vidio->id = id;
    wcsncpy(vidio->name, name, 100);
    vidio->name[100] = L'\0';
    wcsncpy(vidio->desc, desc, 1000);
    vidio->desc[1000] = L'\0';
    vidio->pos = pos;
    vidio->create_date = time(NULL);
    vidio->next = vidioList;
    vidioList = vidio;

    Position *position = (Position *)malloc(sizeof(Position));
    if (position) {
        *position = pos;
        position->vidio_id = id;
        position->next = positionList;
        positionList = position;
    }
    return id;
}

Vidio *readVidio(int id) {
    Vidio *current = vidioList;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updateVidio(Vidio *vidio) {
    Vidio *existingVidio = readVidio(vidio->id);
    if (!existingVidio) {
        wprintf(L"비디오 id: %d는 존재하지 않음\n", vidio->id);
        return -1;
    }
    wcscpy(existingVidio->name, vidio->name);
    wcscpy(existingVidio->desc, vidio->desc);
    existingVidio->create_date = vidio->create_date;
    existingVidio->pos = vidio->pos;
    Position *position = positionList;
    while (position) {
        if (position->vidio_id == vidio->id) {
            position->horror = vidio->pos.horror;
            position->comedy = vidio->pos.comedy;
            position->action = vidio->pos.action;
            position->sf = vidio->pos.sf;
            position->fantasy = vidio->pos.fantasy;
            position->romance = vidio->pos.romance;
            position->family = vidio->pos.family;
            break;
        }
        position = position->next;
    }
    return 0;
}

int deleteVidio(int id) {
    Vidio *current = vidioList;
    Vidio *prev = NULL;
    while (current) {
        if (current->id == id) {
            Real_Vidio *realVidio = realVidioList;
            while (realVidio) {
                if (realVidio->vidio_id == id) {
                    wprintf(L"id %d는 지울 수 없음 실제 비디오에 참조 되어있음\n", id);
                    return -1;
                }
                realVidio = realVidio->next;
            }
            deletePosition(id);
            if (prev) {
                prev->next = current->next;
            } else {
                vidioList = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    wprintf(L"비디오 id %d를 찾을 수 없음\n", id);
    return -1;
}