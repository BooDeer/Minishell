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
void		ft_freestrarr(char **arr);
char		*ft_strjoincl(char *s1, char *s2, int free_both);
void		init_env(int argc, char **argv, char **env);
int			env_len(char **env);
int			ft_builtin_env(void);
int			ft_builtin_pwd(void);
char		*find_strenv(char *str);
int			find_env(char *value);
void		export_var(char *var, char *str);
char		**realloc_env(int size);
int			ft_builtin_export(char **args);
char		*ft_pathjoin(char  *s1, char *s2);
void		delete_env(int	index);
int			ft_builtin_unset(char **args);

char		**g_env;
#endif

