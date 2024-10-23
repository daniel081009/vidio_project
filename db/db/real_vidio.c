#include "../db.h"

int realvidio_last_uqid() {
    Real_Vidio *current = realVidioList;
    int max_id = 0;
    while (current) {
        if (current->id > max_id) {
            max_id = current->id;
        }
        current = current->next;
    }
    return max_id;
}

int createRealVidio(int vidio_id) {
    if (!readVidio(vidio_id)) {
        wprintf(L"비디오 id %d는 존재하지 않음\n", vidio_id);
        return -1;
    }

    Real_Vidio *realVidio = (Real_Vidio *)malloc(sizeof(Real_Vidio));
    if (!realVidio) return -1;
    realVidio->id = realvidio_last_uqid() + 1;
    realVidio->vidio_id = vidio_id;
    realVidio->create_date = time(NULL);
    realVidio->useing = 0; // Available
    realVidio->next = realVidioList;
    realVidioList = realVidio;
    return realVidio->id;
}

Real_Vidio *readRealVidio(int id) {
    Real_Vidio *current = realVidioList;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updateRealVidio(Real_Vidio *realVidio) {
    Real_Vidio *existingRealVidio = readRealVidio(realVidio->id);
    if (!existingRealVidio) {
        wprintf(L"오프라인 비디오 id %d는 존재하지 않음\n", realVidio->id);
        return -1;
    }
    if (!readVidio(realVidio->vidio_id)) {
        wprintf(L"비디오 id %d는 존재하지 않음\n", realVidio->vidio_id);
        return -1;
    }
    existingRealVidio->vidio_id = realVidio->vidio_id;
    existingRealVidio->create_date = realVidio->create_date;
    existingRealVidio->useing = realVidio->useing;
    return 0;
}

int deleteRealVidio(int id) {
    Real_Vidio *current = realVidioList;
    Real_Vidio *prev = NULL;
    while (current) {
        if (current->id == id) {
            Real_Vidio_History *history = historyList;
            while (history) {
                if (history->real_vidio_id == id) {
                    wprintf(L"오프라인 비디오 id %d는 삭제할 수 없음\n", id);
                    return -1;
                }
                history = history->next;
            }
            if (prev) {
                prev->next = current->next;
            } else {
                realVidioList = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    wprintf(L"Real_Vidio ID %d를 찾을 수 없음\n", id);
    return -1;
}
