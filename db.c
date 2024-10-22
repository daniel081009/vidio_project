#include "db.h"

User *userList = NULL;
Vidio *vidioList = NULL;
Real_Vidio *realVidioList = NULL;
Real_Vidio_History *historyList = NULL;
Position *positionList = NULL;

void loadData() {
    FILE *file = _wfopen(DB_PATH L"" DB_User_File, L"r, ccs=UTF-8");
    if (file) {
        wchar_t line[1024];
        while (fgetws(line, sizeof(line)/sizeof(wchar_t), file)) {
            User *user = (User *)malloc(sizeof(User));
            int man;
            if (swscanf(line, L"%d,%50[^,],%200[^,],%d,%d", &user->uqid, user->name, user->address, &man, &user->birth_year) == 5) {
                user->man = (char)man;
                user->next = userList;
                userList = user;
            } else {
                free(user);
            }
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Vidio_Position_File, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            Position *position = (Position *)malloc(sizeof(Position));
            if (sscanf(line, "%d,%hhd,%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
                       &position->vidio_id,
                       &position->horror,
                       &position->comedy,
                       &position->action,
                       &position->sf,
                       &position->fantasy,
                       &position->romance,
                       &position->family) == 8) {
                position->next = positionList;
                positionList = position;
            } else {
                free(position);
            }
        }
        fclose(file);
    }

    file = _wfopen(DB_PATH L"" DB_Vidio_File, L"r, ccs=UTF-8");
    if (file) {
        wchar_t line[2048];
        while (fgetws(line, sizeof(line)/sizeof(wchar_t), file)) {
            Vidio *vidio = (Vidio *)malloc(sizeof(Vidio));
            if (swscanf(line, L"%d,%100[^,],%1000[^,],%ld", &vidio->id, vidio->name, vidio->desc, &vidio->create_date) == 4) {
                Position *pos = positionList;
                while (pos) {
                    if (pos->vidio_id == vidio->id) {
                        vidio->pos = *pos;
                        break;
                    }
                    pos = pos->next;
                }
                vidio->next = vidioList;
                vidioList = vidio;
            } else {
                free(vidio);
            }
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Real_Vidio_File, "r");
    if (file) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            Real_Vidio *realVidio = (Real_Vidio *)malloc(sizeof(Real_Vidio));
            if (sscanf(line, "%d,%d,%ld,%hhd", &realVidio->id, &realVidio->vidio_id, &realVidio->create_date, &realVidio->useing) == 4) {
                realVidio->next = realVidioList;
                realVidioList = realVidio;
            } else {
                free(realVidio);
            }
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Real_Vidio_History_File, "r");
    if (file) {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            Real_Vidio_History *history = (Real_Vidio_History *)malloc(sizeof(Real_Vidio_History));
            if (sscanf(line, "%d,%d,%d,%hhd,%hhd,%ld,%ld",
                       &history->id,
                       &history->use_user_id,
                       &history->real_vidio_id,
                       &history->return_bool,
                       &history->good,
                       &history->borrowed_date,
                       &history->return_date) == 7) {
                history->next = historyList;
                historyList = history;
            } else {
                free(history);
            }
        }
        fclose(file);
    }
}

void saveData() {
    FILE *file = _wfopen(DB_PATH L"" DB_User_File, L"w, ccs=UTF-8");
    if (file) {
        User *current = userList;
        while (current) {
            fwprintf(file, L"%d,%ls,%ls,%d,%d\n", current->uqid, current->name, current->address, current->man, current->birth_year);
            current = current->next;
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Vidio_Position_File, "w");
    if (file) {
        Position *current = positionList;
        while (current) {
            fprintf(file, "%d,%d,%d,%d,%d,%d,%d,%d\n",
                    current->vidio_id,
                    current->horror,
                    current->comedy,
                    current->action,
                    current->sf,
                    current->fantasy,
                    current->romance,
                    current->family);
            current = current->next;
        }
        fclose(file);
    }

    file = _wfopen(DB_PATH L"" DB_Vidio_File, L"w, ccs=UTF-8");
    if (file) {
        Vidio *current = vidioList;
        while (current) {
            fwprintf(file, L"%d,%ls,%ls,%ld\n", current->id, current->name, current->desc, current->create_date);
            current = current->next;
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Real_Vidio_File, "w");
    if (file) {
        Real_Vidio *current = realVidioList;
        while (current) {
            fprintf(file, "%d,%d,%ld,%d\n", current->id, current->vidio_id, current->create_date, current->useing);
            current = current->next;
        }
        fclose(file);
    }

    file = fopen(DB_PATH DB_Real_Vidio_History_File, "w");
    if (file) {
        Real_Vidio_History *current = historyList;
        while (current) {
            fprintf(file, "%d,%d,%d,%d,%d,%ld,%ld\n",
                    current->id,
                    current->use_user_id,
                    current->real_vidio_id,
                    current->return_bool,
                    current->good,
                    current->borrowed_date,
                    current->return_date);
            current = current->next;
        }
        fclose(file);
    }
}

int user_last_uqid() {
    User *current = userList;
    int max_uqid = 0;
    while (current) {
        if (current->uqid > max_uqid) {
            max_uqid = current->uqid;
        }
        current = current->next;
    }
    return max_uqid;
}
int lenUser() {
    User *current = userList;
    int index = 0;
    while (current) {
        index++;
        current = current->next;
    }
    return index;

}
int createUser(wchar_t *name, wchar_t *address, char man, int birth_year) {
    User *user = (User *)malloc(sizeof(User));
    if (!user) return -1;
    user->uqid = user_last_uqid() + 1;
    wcsncpy(user->name, name, 50);
    user->name[50] = L'\0';
    wcsncpy(user->address, address, 200);
    user->address[200] = L'\0';
    user->man = man;
    user->birth_year = birth_year;
    user->next = userList;
    userList = user;
    return user->uqid;
}

User *readUser(int uqid) {
    User *current = userList;
    while (current) {
        if (current->uqid == uqid) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updateUser(User *user) {
    User *existingUser = readUser(user->uqid);
    if (!existingUser) {
        wprintf(L"id: %d 유저를 찾을 수 없음\n", user->uqid);
        return -1;
    }
    wcscpy(existingUser->name, user->name);
    wcscpy(existingUser->address, user->address);
    existingUser->man = user->man;
    existingUser->birth_year = user->birth_year;
    return 0;
}

int deleteUser(int uqid) {
    User *current = userList;
    User *prev = NULL;
    while (current) {
        if (current->uqid == uqid) {
            Real_Vidio_History *history = historyList;
            while (history) {
                if (history->use_user_id == uqid && history->return_bool ==0) {
                    wprintf(L"%d 번의 사용자를 삭제할 수 없습니다. 비디오 테이프 반납 안함\n", uqid);
                    return -1;
                }
                history = history->next;
            }
            if (prev) {
                prev->next = current->next;
            } else {
                userList = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    wprintf(L"User with ID %d not found.\n", uqid);
    return -1;
}

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
        wprintf(L"Vidio with ID %d does not exist.\n", vidio->id);
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
                    wprintf(L"Cannot delete vidio with ID %d; it's referenced in Real_Vidio.\n", id);
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
    wprintf(L"Vidio with ID %d not found.\n", id);
    return -1;
}

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
        wprintf(L"Vidio with ID %d does not exist.\n", vidio_id);
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
        wprintf(L"Real_Vidio with ID %d does not exist.\n", realVidio->id);
        return -1;
    }
    if (!readVidio(realVidio->vidio_id)) {
        wprintf(L"Vidio with ID %d does not exist.\n", realVidio->vidio_id);
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
                    wprintf(L"Cannot delete Real_Vidio with ID %d; it's referenced in Real_Vidio_History.\n", id);
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
    wprintf(L"Real_Vidio with ID %d not found.\n", id);
    return -1;
}

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
        wprintf(L"User with ID %d does not exist.\n", use_user_id);
        return -1;
    }

    if (!readRealVidio(real_vidio_id)) {
        wprintf(L"Real_Vidio with ID %d does not exist.\n", real_vidio_id);
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
        wprintf(L"History with ID %d does not exist.\n", history->id);
        return -1;
    }
    // Verify foreign keys
    if (!readUser(history->use_user_id)) {
        wprintf(L"User with ID %d does not exist.\n", history->use_user_id);
        return -1;
    }

    if (!readRealVidio(history->real_vidio_id)) {
        wprintf(L"Real_Vidio with ID %d does not exist.\n", history->real_vidio_id);
        return -1;
    }

    // Update fields
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
    wprintf(L"History with ID %d not found.\n", id);
    return -1;
}

// Position CRUD operations

Position *createPosition(int vidio_id, char horror, char comedy, char action, char sf, char fantasy, char romance, char family) {
    if (readPosition(vidio_id)) {
        printf("Position for Vidio ID %d already exists.\n", vidio_id);
        return NULL;
    }
    if (!readVidio(vidio_id)) {
        printf("Vidio with ID %d does not exist.\n", vidio_id);
        return NULL;
    }

    Position *position = (Position *)malloc(sizeof(Position));
    if (!position) return NULL;
    position->vidio_id = vidio_id;
    position->horror = horror;
    position->comedy = comedy;
    position->action = action;
    position->sf = sf;
    position->fantasy = fantasy;
    position->romance = romance;
    position->family = family;
    position->next = positionList;
    positionList = position;

    // Update the position in the corresponding Vidio
    Vidio *vidio = readVidio(vidio_id);
    if (vidio) {
        vidio->pos = *position;
    }

    return position;
}

Position *readPosition(int vidio_id) {
    Position *current = positionList;
    while (current) {
        if (current->vidio_id == vidio_id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int updatePosition(Position *position) {
    Position *existingPosition = readPosition(position->vidio_id);
    if (!existingPosition) {
        printf("Position for Vidio ID %d does not exist.\n", position->vidio_id);
        return -1;
    }
    // Update fields
    existingPosition->horror = position->horror;
    existingPosition->comedy = position->comedy;
    existingPosition->action = position->action;
    existingPosition->sf = position->sf;
    existingPosition->fantasy = position->fantasy;
    existingPosition->romance = position->romance;
    existingPosition->family = position->family;

    // Update the position in the corresponding Vidio
    Vidio *vidio = readVidio(position->vidio_id);
    if (vidio) {
        vidio->pos = *existingPosition;
    }

    return 0;
}

int deletePosition(int vidio_id) {
    Position *current = positionList;
    Position *prev = NULL;
    while (current) {
        if (current->vidio_id == vidio_id) {
            // Remove the position from the Vidio
            Vidio *vidio = readVidio(vidio_id);
            if (vidio) {
                memset(&vidio->pos, 0, sizeof(Position));
            }
            // Remove from linked list
            if (prev) {
                prev->next = current->next;
            } else {
                positionList = current->next;
            }
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    printf("Position for Vidio ID %d not found.\n", vidio_id);
    return -1;
}

// Function to free all allocated memory
void freeAllData() {
    // Free Users
    User *userCurrent = userList;
    while (userCurrent) {
        User *temp = userCurrent;
        userCurrent = userCurrent->next;
        free(temp);
    }
    userList = NULL;

    // Free Positions
    Position *posCurrent = positionList;
    while (posCurrent) {
        Position *temp = posCurrent;
        posCurrent = posCurrent->next;
        free(temp);
    }
    positionList = NULL;

    // Free Vidios
    Vidio *vidioCurrent = vidioList;
    while (vidioCurrent) {
        Vidio *temp = vidioCurrent;
        vidioCurrent = vidioCurrent->next;
        free(temp);
    }
    vidioList = NULL;

    // Free Real_Vidios
    Real_Vidio *realVidioCurrent = realVidioList;
    while (realVidioCurrent) {
        Real_Vidio *temp = realVidioCurrent;
        realVidioCurrent = realVidioCurrent->next;
        free(temp);
    }
    realVidioList = NULL;

    // Free Real_Vidio_Histories
    Real_Vidio_History *historyCurrent = historyList;
    while (historyCurrent) {
        Real_Vidio_History *temp = historyCurrent;
        historyCurrent = historyCurrent->next;
        free(temp);
    }
    historyList = NULL;
}
