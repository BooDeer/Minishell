CC 		= gcc
FLAGS	= -Wall -Werror -Wextra 
AR		= ar -rc
SRC		= main.c
PROGRAM = minishell
NAME	= minishell.a
MAKE 	= make -C
all:	$(NAME)

%.o:	%.c
	$(CC) -c $(FLAGS) $<

$(NAME): $(SRC:.c=.o)
	@echo "Creating the program."
	$(MAKE) libft
	@$(AR) $(NAME) $(SRC:.c=.o)
	ranlib $(NAME)
	$(CC) $(FLAGS) -o $(PROGRAM) $(NAME)

clean:
	@rm -rf $(SRC:.c=.o) $(NAME)
fclean: clean
	@rm -rf $(PROGRAM)

re:	fclean all
