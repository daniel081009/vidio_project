#include "search.h"
#include "../util/hangul.h"

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
        
        wchar_t decomposed_address[usernamelen * 3];
        decompose_hangul_string(current->address, decomposed_address);

        int distance_name =jamo_set_similarity(decomposed_address,decomposed_keyword)+ jamo_set_similarity(decomposed_name, decomposed_keyword); //TODO 검색 알고리즘 고도화

        results[i].u = current;
        results[i].distance = distance_name;
        i++;
        current = current->next;
    }

    *result_count = i;
    return results;
}


VidioSearchResult *search_vidio_all(wchar_t keyword[1000], Vidio *head, int *result_count) {
    int max = lenvidio();
    VidioSearchResult *results = (VidioSearchResult *)malloc(max * sizeof(VidioSearchResult));
    if (results == NULL) {
        *result_count = 0;
        return NULL;
    }

    wchar_t decomposed_keyword[usernamelen * 3];
    decompose_hangul_string(keyword, decomposed_keyword);

    int i = 0;
    Vidio *current = head;
    
    while (current != NULL && i < max) {
        wchar_t decomposed_name[usernamelen * 3];
        decompose_hangul_string(current->name, decomposed_name);
        
        wchar_t decomposed_desc[usernamelen * 3];
        decompose_hangul_string(current->desc, decomposed_desc);

        int distance_name =jamo_set_similarity(decomposed_desc,decomposed_keyword)+ jamo_set_similarity(decomposed_name, decomposed_keyword); //TODO 검색 알고리즘 고도화

        results[i].u = current;
        results[i].distance = distance_name;
        i++;
        current = current->next;
    }

    *result_count = i;
    return results;
}

int compare_results(const void *a, const void *b) {
    UserSearchResult *result_a = (UserSearchResult *)a;
    UserSearchResult *result_b = (UserSearchResult *)b;
    return result_a->distance - result_b->distance;
}