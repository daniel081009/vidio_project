#include "../db.h"

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