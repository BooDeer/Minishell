#include "minishell.h"
# define true 1
char **g_env;

void		*safe_malloc(size_t size)  // equal to ft_memalloc() | To modify later
{
	void	*ptr;

	if (!(ptr = malloc(size + 1)))
		return (NULL);
	ft_bzero(ptr, size + 1);
	return (ptr);
}

int		error_msg(char *error_msg, int fd, int exit_code)
{
	write(fd, error_msg, ft_strlen(error_msg));
	return (exit_code);
}
char		*find_env(char *str)
{
	int		index;

	index = find_env(str);
	if (g_env[index])
		return (g_env[index]);
	return (NULL);
}
char		**realloc_env(int size)
{
	char	**env;
	int		i;

	env = (char**)safe_malloc(sizeof(char*) * (size + 1));
	i = -1;
	while (g_env[++i] && i < size)
	{
		env[i] = ft_strdup(g_env[i]);
		free(g_env[i]);
	}
	free(g_env);
	return(env);
}

char	*ft_strjoinch(char const *s1, char c)
{
	char	*new_str;
	size_t	i;
	size_t	s1_len;

	if (!s1 || !c)
		return (NULL);
	s1_len = ft_strlen(s1);
	new_str = safe_malloc(s1_len + 1);
	if (!new_str)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		*(new_str + i) = *(s1 + i);
	*(new_str + i) = c;
	return (new_str);
}

int			ft_strstartw(char *s1, char *s2)
{
	int		i;

	i = -1;
	while (s2[++i])
		if (s1[i] != s2[i])
			return (0);
	return (1);
}
//void		shell_loop(void)
//{
//	int		status = 1;
//	char	*instruction;
//	char	buf[10];
//	while (1)
//	{
//		write(1,"> ", 2);
//		memset(buf, 0, 9);
//		read(0, buf, 9);
//		printf("Buf: |%s|\n", buf);
//		if (!status)
//			break ;
//	}
//}
int			ft_builtin_env(void)
{
	int			i;

	i = -1;
	while (g_env[++i])
	{
		write(1, g_env[i], ft_strlen(g_env[i]));
		write(1, "\n", 1);
	}
	return (1);
}

//int	pwd(int fd)
//{
//	char	buff[1024];

//	if (getcwd(buff, sizeof(buff)) == NULL)
//		return (1); // Failed
//	write(fd, &buff, length(buff));
//	write(fd, "\n", 1);
//	return (0); // SUCCESS
//}

int			ft_builtin_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)) == NULL)
		return (-1);
	write(1, &buf, ft_strlen(buf));
	write(1, "\n", 1);
	return (0);
}


int			find_env(char *value)
{
	int		i;
	char	*tmp;

	i = -1;
	tmp = safe_malloc(1);
	while (g_env[++i])
	{
		tmp = ft_strjoinch(value, '=');
		if (ft_strstartw(g_env[i], tmp))
		{
			free(tmp);
			return(i);
		}
		free(tmp);
	}
	return (i);
}

void		export_var(char *var, char *str)
{
	int		index;
	char	*tmp;


	index = find_env(var);
	tmp   = ft_strjoin("=", str);
	if (g_env[index])
	{
		free(g_env[index]);
		if (str)
			g_env[index] = ft_strjoin(var, tmp);
		else
			g_env[index] = ft_strjoin(var, "=");
	}
	else
	{
		g_env = realloc_env(index + 1);
		if (str)
			g_env[index] = ft_strjoin(var, tmp);
		else
			g_env[index] = ft_strjoin(var, "=");
	}
	free(tmp);
}

int			ft_builtin_export(char **args)  // make sure to split w/ ("=") b4 passing the args
{
	//args = ft_split(args[0], '=');
	printf("%s\n%s\n", args[0], args[1]);
	if (!args[0])
	{
		ft_builtin_env();
		return (1);
	}
	if (args[1] && args[2])
		return(error_msg("export: Too many arguments.", 2, 1));
	export_var(args[0], args[1]);
	//free(args);
	return (1);
}

void		delete_env(int	index)
{
	int		i;
	int		j;

	free(g_env[index]);
	g_env[index] = NULL;
	i = index;
	j = index + 1;
	while (g_env[i + 1])
	{
		g_env[i] = ft_strdup(g_env[i + 1]);
		free(g_env[i + 1]);
		i++;
		j++;
	}
	g_env = realloc_env(j - 1);
}

int			ft_builtin_unset(char **args)
{
	int		i;
	int		index;

	printf("%s\n\n", args[0]);
	if (!args[0])
		return(error_msg("Error!\n too few arguments.", 2, 1));
	i = -1;
	while (args[++i])
	{
		index	= find_env(args[i]);
		if (g_env[index])
			delete_env(index);
	}
	return (1);
}

int			ft_builtin_exit(char **args)
{
	(void)args; // TODO: check what to do w/ exit

	return (-1);
}
int			valid_option(char *str)
{
	int		i;

	i = 0;
	if(str[i] == '-' && str[++i] == 'n')
	{
		while (str[++i] == 'n')
			continue ;
		if (str[i] == '\0')
			return (1);
		return (0);
	}
	return (0);
}

void		check_n(char **args, int *i, int *j)
{
	while (args[++(*i)])
	{
		if (valid_option(args[*i]))
			(*j)++;
		else
			break ;
	}
}
void		exec_echo(char **args, int i)
{
	int		c;

	c = 0;
	while (args[++i])
	{
		if (args[i][0] == '\0')
			continue ;
		if (c)
			write(1, " ", 1);
		write(1, args[i], ft_strlen(args[i]));
		c = 1;
	}
}
int			ft_builtin_echo(char **args)
{
	int		option;
	int		index;

	index = 0;
	option = 0;
	check_n(args, &index, &option); // modify the return value and give it to the var option
	exec_echo(args, index - 1);
	if (!option)
		write(1, "\n", 1);
	return (0);
}

int			path_error(char *path)
{
	error_msg("cd: ", 2, 1);
	error_msg(path, 2, 1);
	error_msg(": No such file or directory\n", 2, 1);
	free(path);
	return (1);
}
int			home_run(void)
{
	char	*path;

	path = getenv("HOME");
	if (!path)
		return (error_msg("cd: HOME not set\n", 2, 1));
	if (*path == '\0')
		path = getcwd(NULL, 1024);
	else
		path = ft_strdup(path);
	if (chdir(path) == -1)
		return (path_error(path));
	export_var(ft_strdup("OLDPWD"), ft_strdup(getenv("PWD")));
	export_var(ft_strdup("PATH"), path);
	return (0);
}
char		*find_path(char *str)
{
	if (ft_strcmp(str, "-"))
		return ()
}

int			ft_builtin_cd(char **args)
{
	char	*path;

	if (!args[0])
		return (home_run());
	path = find_path(args[1]);
	return (0);
}

int			exec_builtin(char **cmd)// ["cd", ...]
{
	if (!ft_strcmp(cmd[0], "echo"))
		return (1);
	else if (!ft_strcmp(cmd[0], "cd"))
		return (ft_builtin_cd(cmd + 1));
	else if (!ft_strcmp(cmd[0], "pwd"))
		return (ft_builtin_pwd());
	else if (!ft_strcmp(cmd[0], "export"))
		return (ft_builtin_export(cmd + 1));
	else if (!ft_strcmp(cmd[0], "unset"))
		return (ft_builtin_unset(cmd + 1));
	else if (!ft_strcmp(cmd[0], "env"))
		return (ft_builtin_env());
	else if (!ft_strcmp(cmd[0], "exit"))
		return (ft_builtin_exit(cmd + 1));
	return (0);
}


void		ft_freestrarr(char **arr)
{
	int		i;

	i = -1;
	if (arr)
	{
		while (arr[++i])
			free(arr[i]);
		free(arr);
	}
	arr = NULL;
}


//int		ft_strcmp(const char *s1, const char *s2)
//{
//	int		i;

//	i = 0;
//	while (*(s1 + i) && *(s1 + i) == *(s2 + i))
//		i++;
//	return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
//}

int			ft_strendw(char *s1, char *s2)
{
	int		i;
	
	i = -1;
	while (s1[++i])
		if (s1[i] == s2[0])
			if (ft_strcmp(s1 + i, s2) == 0)
				return (1);
	return (0);
}

char	*ft_strjoincl(char *s1, char *s2, int free_both)
{
	char	*new;

	if (!(new = ft_strjoin(s1, s2)))
		return (NULL);
	free(s1);
	s1 = NULL;
	if (free_both)
	{
		free(s2);
		s2 = NULL;
	}
	return (new);
}

char		*ft_pathjoin(char  *s1, char *s2)
{
	char	*tmp;
	if (!s1 || !s2)
		return (NULL);
	if (!ft_strendw(s1, "/"))
	{
		if (s2[0] == '/')
			return (ft_strjoin(s1, s2));
		else
		{
			tmp = ft_strjoincl(ft_strjoin(s1, "/"), s2, 0);
			return (tmp);
		}
	}
	else
	{
		if (s2[0] == '/')
			return (ft_strjoin(s1, s2 + 1));
		else
			return (ft_strjoin(s1, s2));
	}
}

int			run_cmd(char *exec_path, char **args)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execve(exec_path, args, g_env);
	else if (pid < 0)
	{
		free(exec_path);
		write(1, "Failed to create fork\n", ft_strlen("Failed to create fork\n"));
		return (-1);
	}
	wait(&pid);
	if (exec_path)
		free(exec_path);
	return (1);
}

int			is_exec(char *exec_path, struct stat stats, char **cmd) // check better this function
{
	if (stats.st_mode & S_IFREG)
	{
		if (stats.st_mode & S_IXUSR)
			return (run_cmd(exec_path, cmd));
		else
			error_msg("Permission denied!\n", 2, 0);
		free(exec_path);
		return (1);
	}
	free(exec_path);
	return (0);
}

int			exec_bin(char **cmd)		// equal to check_bin();
{
	char	**path = ft_split(getenv("PATH"), ':');
	int		i;
	char	*exec_path;
	struct stat stats;

	i = -1;
	while (path && path[++i])
	{
		if (ft_strstartw(cmd[0], path[i]))
			exec_path = ft_strdup(cmd[0]);
		else
			exec_path = ft_pathjoin(path[i], cmd[0]);
		if (lstat(exec_path, &stats) == -1)
			free(exec_path);
		else
		{
			ft_freestrarr(path);
			return (is_exec(exec_path, stats, cmd));
		}
	}
	ft_freestrarr(path);
	return (0);
	//int		pid;

	//pid = fork();
	//if (!pid)
	//{

	//}
	//else if (pid < 0)
	//{

	//}
	//else
	//{

	//}

}



int			env_len(char **env) // to modify later. (No need for J)
{
	int		i;
	int		j;
	
	i = -1;
	j = 0;
	while(env[++i])
		j++;
	return  (j);
}
void		init_env(int argc, char **argv, char **env)
{
	int		i;

	(void)argc;
	(void)argv;

	g_env = (char**)safe_malloc(sizeof(char *) * (env_len(env) + 1));
	i = -1;
	while (env[++i])
	{
		if (!(g_env[i] = ft_strdup(env[i])))
			error_msg("A memory allocation failed!\n", 2, 0);
	}
}

int			main(int argc, char **argv, char **env)
{
	//printf("\n\n");
	init_env(argc, argv, env);
	//printf("%s\n", getenv("PWD"));
	char	*cmd[] = {"cd" ,NULL};
	exec_builtin(cmd);
	//exec_bin(cmd);
	//printf("%s\n", getenv("PWD"));
	//char	*cmd1[] = {"env",NULL};
	//exec_builtin(cmd1);












	//int	i = -1;
	//while (g_env[++i])
	//	printf("%s\n", g_env[i]);
	//char **path = ft_split(getenv("PATH"), ':');
	//int i = -1;
	//printf("===================\n===================\n");
	//while (path[++i])
	//{
	//	printf("%s\n", path[i]);
	//	//execve(path[i],  NULL, NULL);
	//}
	//printf("===================\n===================\n");
	//char *arg[] = {"-C", NULL};
	//execve("/bin/ls", arg, NULL);
	return (0);
}
