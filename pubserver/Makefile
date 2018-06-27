CC	= gcc
NAME 	= my_ftl
SRC	= air_shed.c \
	container.c \
	system_control.c \
	system_repair.c \
	my_string.c \
	main.c

OBJ 	= $(SRC:%.c=%.o)
CFLAGS	= -W -Wall -Werror

$(NAME):$(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

all:	$(NAME)

.PHONY:	clean

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM)	$(NAME)

re:	fclean all
