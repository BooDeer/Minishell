#include "minishell.h"
# define true 1
char **g_env;
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

//int			exec_cmd(char **cmd)// ["cd", ...]
//{
//	if (ft_strcmp(cmd[0], "echo"))
//		return (ft_builtin_echo(cmd + 1));
//	else if (ft_strcmp(cmd[0], "cd"))
//		return (ft_builtin_cd(cmd + 1));
//	else if (ft_strcmp(cmd[0], "pwd"))
//		return (ft_builtin_pwd(cmd + 1));
//	else if (ft_strcmp(cmd[0], "export"))
//		return (ft_builtin_export(cmd + 1));
//	else if (ft_strcmp(cmd[0], "unset"))
//		return (ft_builtin_unset(cmd + 1));
//	else if (ft_strcmp(cmd[0], "env"))
//		return (ft_builtin_env(cmd + 1));
//	else if (ft_strcmp(cmd[0], "exit"))
//		return (ft_builtin_exit(cmd + 1));

//}

int		exit_program(char *error_msg, int fd, int exit_code)
{
	write(fd, error_msg, ft_strlen(error_msg));
	return (exit_code);
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
int			ft_strstartw(char *s1, char *s2)
{
	int		i;

	i = -1;
	while (s2[++i])
		if (s1[i] != s2[i])
			return (0);
	return (1);
}

int		ft_strcmp(const char *s1, const char *s2)
{
	int		i;

	i = 0;
	while (*(s1 + i) && *(s1 + i) == *(s2 + i))
		i++;
	return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
}

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
	printf("\n%s\n", exec_path);
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

int			is_exec(char *exec_path, struct stat stats, char **cmd)
{
	if (stats.st_mode & S_IFREG)
	{
		if (stats.st_mode & S_IXUSR)
			return (run_cmd(exec_path, cmd));
		else
			exit_program("Permission denied!\n", 2, 0);
		free(exec_path);
		return (1);
	}
	free(exec_path);
	return (0);
}

int			exec_cmd(char **cmd)
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


void		*safe_malloc(size_t size)
{
	void	*ptr;

	if (!(ptr = malloc(size)))
		return (NULL);
	ft_bzero(ptr, size);
	return (ptr);
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
			exit_program("A memory allocation failed!\n", 2, 0);
	}
}

int			main(int argc, char **argv, char **env)
{

	init_env(argc, argv, env);
	char	*cmd[] = {"ping", "google.com", NULL};
	exec_cmd(cmd);
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
