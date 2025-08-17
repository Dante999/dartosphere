PROJECT=dartosphere
CC_FLAGS = -Wall -Wextra -Werror -Wpedantic -I../thirdparty/libcutils/include
LD_FLAGS = -lSDL2_ttf

LIBS=`sdl2-config --cflags --libs`

all:
	${CC} ${CC_FLAGS} -g -o ${PROJECT} \
		main.c \
		screen.c \
		player.c \
		game_x01.c \
		config.c \
		game_x01_screen.c \
		game_screen.c \
		screen_welcome.c \
		screen_player_selection.c \
		screen_game_selection.c \
		screen_utils.c \
		darts.c \
		${LIBS} \
		${LD_FLAGS}

clean:
	rm ${PROJECT}
