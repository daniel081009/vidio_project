# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I. -I./db -I./db/db -I./reg -I./ui -I./util

SRC = main.c \
      $(wildcard util/*.c) \
      $(wildcard db/db/*.c) \
      $(wildcard db/*.c) \
      $(wildcard reg/*.c) \
      $(wildcard ui/*.c)

# 오브젝트 파일 설정
OBJ = $(SRC:.c=.o)

# 출력 실행 파일 이름
TARGET = vidio_project.exe
# 기본 빌드 규칙
all: $(TARGET)

# 실행 파일 빌드 규칙
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# 개별 오브젝트 파일 빌드 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 클린 규칙
clean:
	del /Q *.o db\\*.o db\\db\\*.o reg\\*.o ui\\*.o util\\*.o vidio_project.exe
