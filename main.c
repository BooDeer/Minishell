#include "minishell.h"

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

//int			exec_cmd(char **cmd)
//{
//	int		pid;

//	pid = fork();
//	if (!pid)
//	{

//	}
//	else if (pid < 0)
//	{

//	}
//	else
//	{

//	}

//}

int			main(void)
{
	//char	**cmd;
	//exec_cmd(cmd);
	execve("/bin/ls", "-la", NULL);
	return (0);
}
