CC 		= gcc
FLAGS	= -Wall -Werror -Wextra #-fsanitize=address -g
AR		= ar -rc
SRC		= main.c utils_0.c utils_1.c env.c pwd.c export.c unset.c
PROGRAM = minishell
NAME	= minishell.a
MAKE 	= make -C
all:	$(NAME)

%.o:	%.c
	@$(CC) -c $(FLAGS) $<

$(NAME): $(SRC:.c=.o)
	@echo "Creating the program."
	make -C libft
	@$(AR) $(NAME) $(SRC:.c=.o)
	ranlib $(NAME)
	$(CC) $(FLAGS) -o $(PROGRAM) $(NAME) libft.a

clean:
	make clean -C  libft
	@rm -rf $(SRC:.c=.o) $(NAME)
fclean: clean
	make fclean -C  libft
	@rm -rf $(PROGRAM)

re:	fclean all
