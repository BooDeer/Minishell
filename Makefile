CC 	= gcc
FLAGS	= -Wall -Werror -Wextra 
AR	= ar -rc
SRC	= main.c
PROGRAM = minishell
NAME	= minishell.a

all:	$(NAME)

%.o:	%.c
	$(CC) -c $(FLAG) $<
$(NAME): $(SRC:.c=.o)
	@echo "Creating the program."
	@$(AR) $(NAME) $(SRC:.c=.o)
	ranlib $(NAME)
	$(CC) -o $(PROGRAM) $(NAME)

clean:
	@rm -rf $(SRC:.c=.o) $(NAME)
fclean: clean
	@rm -rf $(PROGRAM)

re:	fclean all
