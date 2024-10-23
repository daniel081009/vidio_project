#ifndef search_h
#define search_h

#include "db.h"

typedef struct {
    User *u;
    int distance;
} UserSearchResult;

int compare_results(const void *a, const void *b);
UserSearchResult *search_user_all(wchar_t keyword[1000], User *head, int *result_count);

#endif
