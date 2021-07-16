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

int		dup_fd(t_cmd *cmd, int fds[2], int input, int fd_zero)
{
	input++;
	if (cmd->is_piped == 0 && cmd->next->is_piped == 1)
		dup2(fds[1], 1);
	else if (cmd->is_piped && cmd->next && cmd->next->is_piped)
	{
		dup2(fd_zero, 0);
		dup2(fds[1], 1);
	}
	else
		dup2(fd_zero, 0);
	if (cmd->is_piped && cmd->next && cmd->next->is_piped)
		return(2);
	return (0);
}

int			close_all(int input, int fd1, int fd2, int pos)
{
	if (input)
		close (input);
	close(fd2);
	if (pos == 2)
		close(fd1);
	input = fd1;
	return (input);
}

void		pipes(t_cmd *cmd)
{
	t_cmd 	*head;
	int		fds[2];
	int		pid;
	int		input  = 0;
	int		pos = 0;
	int		fd_zero = 0;
	head = cmd;
	while (cmd)
	{
		if (cmd->next)
			pipe(fds);
		pid = fork();
		if (!pid)
		{
			pos = dup_fd(cmd, fds, input, fd_zero);
			close(fds[1]);
			close(fds[0]);
			close(fd_zero);
			exit(exec_bin(cmd->args));
		}
		if (fd_zero)
			close(fd_zero);
		fd_zero = fds[0];
		//input = close_all(input, fds[0], fds[1], pos);
		if (!cmd->next || !cmd->next->is_piped)
			break ;
		cmd = cmd->next;
		close(fds[1]);
	}
	wait(&pid);
	
}
void		append_lst(t_cmd **cmd, t_cmd *src)
{
	t_cmd	*tmp;

	if (*cmd == NULL)
		*cmd = src;
	else
	{
		tmp = *cmd;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = src;
	}
}

t_cmd		*fill_dummy(t_cmd **cmd, char **cmd_0, char **cmd_1)
{
	int		i;
	t_cmd	*dummy;
	i = 0;
	while (i < 2)
	{
		dummy = (t_cmd *)malloc(sizeof(t_cmd));
		dummy->pos = i;
		switch (i)
		{
		case 0:
			dummy->is_piped = 0;
			dummy->previous = NULL;
			dummy->next = NULL;
			dummy->args = cmd_0;
			break;
		case 1:
			dummy->is_piped = 1;
			dummy->next = NULL;
			dummy->args = cmd_1;
		default:
			break;
		}
		append_lst(cmd ,dummy);
		i++;
	}
	return (*cmd);
}

void		append_lstk(t_token **cmd, t_token *src)
{
	t_token	*tmp;

	if (*cmd == NULL)
		*cmd = src;
	else
	{
		tmp = *cmd;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = src;
	}
}

/*
*
*	arr ==> ["main.c", "test.c", NULL]
*	type ==> "aowwwa"  a = append, o = override,  w = write
*/

void		fill_token(t_token **head, char **arr, char* type)
{

	t_token  *token;
	int			i = -1;

	while (arr[++i])
	{
		token = (t_token *)malloc(sizeof(t_token));
		token->value = ft_strdup(arr[i]);
		token->type = type[i];
		token->next = NULL;
		append_lstk(head, token);
	}
}

void		output_token(t_token *token)
{
	while(token)
	{
		printf("%s====>%c\n", token->value, token->type);
		token = token->next;
	}
}

int			main(int argc, char **argv, char **env)
{
	//printf("\n\n");
	t_cmd	*test = NULL;					// testing linked ist
	t_token *token = NULL;
	char	*red[] =  {"main.c", "test.c", "main.c", NULL};
	fill_token(&token, red, "awa");
	output_token(token);
	init_env(argc, argv, env);
	char	*cmd[] = {"echo" , "dev/random", NULL};
	//char	*cmd[] = {"cat" , "main.c", NULL};
	//exec_builtin(cmd);
	char	*cmd1[] = {"wc",NULL};
	//exec_builtin(cmd1);
	//exec_bin(cmd);
	//printf("%s\n", getenv("PWD"));
	//char	*cmd2[] = {"cd",NULL};
	//exec_builtin(cmd2);
	test = fill_dummy(&test, cmd, cmd1);
	//printf("===>%d\n===>%s\n", test->is_piped, test->args[0]);
	pipes(test);
	//test[0].is_piped = 1;
	//test[0].args = cmd;
	//test[0].next = &test[1];
	//test[1].is_piped = 0;
	//test[1].args = cmd1;
	//test[1].next = NULL;





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
