#include "search.h"
#include "hangul.h"

const int usernamelen = 50;

typedef struct {
    User *u;
    int distance;
} UserSearchResult;


#include "search.h"
#include "hangul.h"

const int usernamelen = 50;


UserSearchResult *search_user_all(wchar_t keyword[1000], User *head, int *result_count) {
    int max = lenUser();
    UserSearchResult *results = (UserSearchResult *)malloc(max * sizeof(UserSearchResult));
    if (results == NULL) {
        *result_count = 0;
        return NULL;
    }

    wchar_t decomposed_keyword[usernamelen * 3];
    decompose_hangul_string(keyword, decomposed_keyword);

    int i = 0;
    User *current = head;
    
    while (current != NULL && i < max) {
        wchar_t decomposed_name[usernamelen * 3];
        decompose_hangul_string(current->name, decomposed_name);

        int distance_name = jamo_set_similarity(decomposed_name, decomposed_keyword);

        results[i].u = current;
        results[i].distance = distance_name;
        i++;
        current = current->next;
    }

    *result_count = i;
    return results;
}