
NAME	=	neuronne

SRC	=	main.cpp \
		neuron.cpp

CC	=	g++ -ggdb

OBJ	=	$(SRC:.cpp=.o)

CFLAGS =	-Wall -Wextra -ffast-math

LDFLAGS =	-lSDL -lm

$(NAME):	$(OBJ)
		$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LDFLAGS)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re:		fclean $(NAME)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@