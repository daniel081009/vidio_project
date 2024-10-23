#include "db.h"

User *userList = NULL;
Vidio *vidioList = NULL;
Real_Vidio *realVidioList = NULL;
Real_Vidio_History *historyList = NULL;
Position *positionList = NULL;

void freeAllData() {
    User *userCurrent = userList;
    while (userCurrent) {
        User *temp = userCurrent;
        userCurrent = userCurrent->next;
        free(temp);
    }
    userList = NULL;

    Position *posCurrent = positionList;
    while (posCurrent) {
        Position *temp = posCurrent;
        posCurrent = posCurrent->next;
        free(temp);
    }
    positionList = NULL;

    Vidio *vidioCurrent = vidioList;
    while (vidioCurrent) {
        Vidio *temp = vidioCurrent;
        vidioCurrent = vidioCurrent->next;
        free(temp);
    }
    vidioList = NULL;

    Real_Vidio *realVidioCurrent = realVidioList;
    while (realVidioCurrent) {
        Real_Vidio *temp = realVidioCurrent;
        realVidioCurrent = realVidioCurrent->next;
        free(temp);
    }
    realVidioList = NULL;

    Real_Vidio_History *historyCurrent = historyList;
    while (historyCurrent) {
        Real_Vidio_History *temp = historyCurrent;
        historyCurrent = historyCurrent->next;
        free(temp);
    }
    historyList = NULL;
}
