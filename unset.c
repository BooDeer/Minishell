/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 10:58:45 by hboudhir          #+#    #+#             */
/*   Updated: 2021/06/09 10:59:04 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	//printf("%s\n\n", args[0]);
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