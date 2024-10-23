#include "../db.h"

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