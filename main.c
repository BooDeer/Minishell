#include "minishell.h"
# define true 1










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
	char	**path;

	path = ft_split(find_strenv("HOME"), '=');
	if (!path[1])
		return (error_msg("cd: HOME not set\n", 2, 1));
	if (*path[1] == '\0')
		path[1] = getcwd(NULL, 1024);
	else
		path[1] = ft_strdup(path[1]);
	if (chdir(path[1]) == -1)
		return (path_error(path[1]));
	export_var(ft_strdup("OLDPWD"), ft_strdup(find_strenv("PWD")));
	export_var(ft_strdup("PATH"), path[1]);
	ft_freestrarr(path);
	return (0);
}
char		*find_path(char *str)
{
	if (ft_strcmp(str, "-"))
		return (find_strenv("OLDPWD"));
	else if (ft_strcmp(str, "~"))
		return (find_strenv("HOME"));
	else if (*str == '~' && ft_strcmp((str + 1), find_strenv("USER")))
		return (find_strenv("HOME"));
	return (str);
}

int			ft_builtin_cd(char **args)
{
	char	*path;

	if (!args[0])
		return (home_run());
	path = find_path(args[1]);
	if (chdir(path) == -1)
		return (path_error(ft_strdup(path)));
	path = getcwd(NULL, 1024);
	export_var(ft_strdup("OLDPWD"), ft_strdup(find_strenv("PWD")));
	export_var(ft_strdup("PWD"), path);
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






int			main(int argc, char **argv, char **env)
{
	//printf("\n\n");
	init_env(argc, argv, env);
	//printf("%s\n", getenv("PWD"));
	char	*cmd[] = {"export", "pepepopo=test" ,NULL};
	exec_builtin(cmd);
	char	*cmd1[] = {"env" ,NULL};
	exec_builtin(cmd1);
	//exec_bin(cmd);
	//printf("%s\n", getenv("PWD"));
	char	*cmd2[] = {"cd",NULL};
	exec_builtin(cmd2);






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
