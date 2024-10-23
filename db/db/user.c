#include "../db.h"

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
    wprintf(L"사용자 id %d 를 찾을 수 없음\n", uqid);
    return -1;
}