CC_FLAGS = -Wall -Wextra -Werror -Wpedantic -I ../thirdparty/libcutils/include

all:
	${CC} ${CC_FLAGS} -o dartosphere_tests \
		main.c \
		test_game_x01.c \
		../code/game_x01.c \
		../code/player.c

clean:
	rm dartosphere_tests
