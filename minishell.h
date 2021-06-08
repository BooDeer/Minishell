#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <dirent.h>
# include <signal.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "libft/libft.h"

void		*safe_malloc(size_t size);
int			error_msg(char *error_msg, int fd, int exit_code);
char		*ft_strjoinch(char const *s1, char c);
int			ft_strstartw(char *s1, char *s2);
int			ft_strendw(char *s1, char *s2);


#endif

