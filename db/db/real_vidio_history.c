#include "../db.h"

int realvidiohistory_last_id() {
    Real_Vidio_History *current = historyList;
    int max_id = 0;
    while (current) {
        if (current->id > max_id) {
            max_id = current->id;
        }
        current = current->next;
    }
    return max_id;
}

int createRealVidioHistory(int use_user_id, int real_vidio_id, char return_bool, char good, time_t borrowed_date, time_t return_date) {
    if (!readUser(use_user_id)) {
        wprintf(L"ID %d인 유저는 존재하지 않습니다.\n", use_user_id);
        return -1;
    }

    if (!readRealVidio(real_vidio_id)) {
        wprintf(L"ID %d인 리얼비디오는 존재하지 않습니다.\n", real_vidio_id);
        return -1;
    }

    Real_Vidio_History *history = (Real_Vidio_History *)malloc(sizeof(Real_Vidio_History));
    if (!history) return -1;
    history->id = realvidiohistory_last_id() + 1;
    history->use_user_id = use_user_id;
    history->real_vidio_id = real_vidio_id;
    history->return_bool = return_bool;
    history->good = good;
    history->borrowed_date = borrowed_date;
    history->return_date = return_date;
    history->next = historyList;
    historyList = history;
    return history->id;
}

Real_Vidio_History *readRealVidioHistory(int id) {
    Real_Vidio_History *current = historyList;
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updateRealVidioHistory(Real_Vidio_History *history) {
    Real_Vidio_History *existingHistory = readRealVidioHistory(history->id);
    if (!existingHistory) {
        wprintf(L"빌린 기록 ID %d는 존재하지 않습니다.\n", history->id);
        return -1;
    }
    if (!readUser(history->use_user_id)) {
        wprintf(L"ID %d인 유저는 존재하지 않습니다.\n",history-> use_user_id);
        return -1;
    }

    if (!readRealVidio(history->real_vidio_id)) {
        wprintf(L"ID %d인 리얼비디오는 존재하지 않습니다.\n",  history->real_vidio_id);
        return -1;
    }

    existingHistory->use_user_id = history->use_user_id;
    existingHistory->real_vidio_id = history->real_vidio_id;
    existingHistory->return_bool = history->return_bool;
    existingHistory->good = history->good;
    existingHistory->borrowed_date = history->borrowed_date;
    existingHistory->return_date = history->return_date;
    return 0;
}

int deleteRealVidioHistory(int id) {
    Real_Vidio_History *current = historyList;
    Real_Vidio_History *prev = NULL;
    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                historyList = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    wprintf(L"기록 ID %d를 찾을 수 없음\n", id);
    return -1;
}