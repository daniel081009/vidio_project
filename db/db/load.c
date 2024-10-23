#include "../db.h"

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