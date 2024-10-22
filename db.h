#ifndef db_h
#define db_h

#define DB_PATH "./db/"
#define DB_User_File "user.txt"
#define DB_Vidio_File "vidio.txt"
#define DB_Real_Vidio_File "real_vidio.txt"
#define DB_Real_Vidio_History_File "real_vidio_history.txt"
#define DB_Vidio_Position_File "vidio_Position.txt"

#include <wchar.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct User {
    int uqid; // Auto인크리먼트 and pk
    wchar_t name[50];
    wchar_t address[200];
    char man; // 0 or 1
    int birth_year;
    struct User *next;
} User;

typedef struct Position {
    int vidio_id; // PK and 포링키 Vidio(id)
    char horror;
    char comedy;
    char action;
    char sf;
    char fantasy;
    char romance;
    char family;
    struct Position *next;
} Position;

typedef struct Vidio {
    int id; // Auto인크리먼트 and pk
    wchar_t name[100];
    wchar_t desc[1000];
    Position pos;
    time_t create_date;
    struct Vidio *next;
} Vidio;

typedef struct Real_Vidio {
    int id; // Auto인크리먼트 and pk
    int vidio_id; // 포링키 Vidio(id)
    time_t create_date;
    char useing; // 0: 대여 가능 1:대여불가
    struct Real_Vidio *next;
} Real_Vidio;

typedef struct Real_Vidio_History {
    int id; // Auto인크리먼트 and pk
    int use_user_id; // 포링키 User(uqid)
    int real_vidio_id; // 포링키 Real_Vidio(id)
    char return_bool; // 0 or 1
    char good; // 0~10
    time_t borrowed_date;
    time_t return_date;
    struct Real_Vidio_History *next;
} Real_Vidio_History;

extern User *userList;
extern Vidio *vidioList;
extern Real_Vidio *realVidioList;
extern Real_Vidio_History *historyList;
extern Position *positionList;

void loadData();
void saveData();

int user_last_uqid();
int vidio_last_uqid();
int realvidio_last_uqid();
int realvidiohistory_last_id();

int lenUser();
int createUser(wchar_t *name, wchar_t *address, char man, int birth_year);
User *readUser(int uqid);
int updateUser(User *user);
int deleteUser(int uqid);

int createVidio(wchar_t *name, wchar_t *desc, Position pos);
Vidio *readVidio(int id);
int updateVidio(Vidio *vidio);
int deleteVidio(int id);

int createRealVidio(int vidio_id);
Real_Vidio *readRealVidio(int id);
int updateRealVidio(Real_Vidio *realVidio);
int deleteRealVidio(int id);

int createRealVidioHistory(int use_user_id, int real_vidio_id, char return_bool, char good, time_t borrowed_date, time_t return_date);
Real_Vidio_History *readRealVidioHistory(int id);
int updateRealVidioHistory(Real_Vidio_History *history);
int deleteRealVidioHistory(int id);

Position *createPosition(int vidio_id, char horror, char comedy, char action, char sf, char fantasy, char romance, char family);
Position *readPosition(int vidio_id);
int updatePosition(Position *position);
int deletePosition(int vidio_id);

void freeAllData();

#endif
