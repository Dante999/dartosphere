PROJECT=dartosphere
CC_FLAGS = -Wall -Wextra -Werror -Wpedantic -I../thirdparty/libcutils/include
LD_FLAGS = -lSDL2_ttf

LIBS=`sdl2-config --cflags --libs`

all:
	${CC} ${CC_FLAGS} -g -o ${PROJECT} \
		main.c \
		screen.c \
		screen_game_on.c \
		screen_welcome.c \
		screen_player_selection.c \
		darts.c \
		${LIBS} \
		${LD_FLAGS}

clean:
	rm ${PROJECT}
