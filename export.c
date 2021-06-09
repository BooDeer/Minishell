/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 10:55:06 by hboudhir          #+#    #+#             */
/*   Updated: 2021/06/09 10:56:23 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		*find_strenv(char *str)
{
	int		index;

	index = find_env(str);
	if (g_env[index])
		return (g_env[index]);
	return (NULL);
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
	args = ft_split(args[0], '=');
	//printf("%s\n%s\n", args[0], args[1]);
	if (!args[0])
	{
		ft_builtin_env();
		return (1);
	}
	if (args[1] && args[2])
		return(error_msg("export: Too many arguments.", 2, 1));
	export_var(args[0], args[1]);
	ft_freestrarr(args);
	return (1);
}