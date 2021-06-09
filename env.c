/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 10:50:27 by hboudhir          #+#    #+#             */
/*   Updated: 2021/06/09 10:54:32 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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