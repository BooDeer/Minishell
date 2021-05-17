#include "minishell.h"

int		main(int argc, char **argv, char  **env)
{
	(void )argc;
	(void )argv;
	int i = -1;

	while (env[++i])
		printf("%s\n",  env[i]);

}