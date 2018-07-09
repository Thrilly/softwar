CC		= gcc
NAME 	= server
SRC		= main.c

OBJ 	= $(SRC:%.c=%.o)
CFLAGS	= -W -Wall -Werror
LIB		= -lzmq -lczmq

$(NAME):$(OBJ)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME) $(LIB)

all:	$(NAME)

.PHONY:	clean

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM)	$(NAME)

re:	fclean all
