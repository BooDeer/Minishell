/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 15:41:54 by hboudhir          #+#    #+#             */
/*   Updated: 2021/04/12 15:46:15 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


int		builtin_cd(char **args)
{
	char	*home_p;

	home_p = getenv("HOME");
	if (!args[0])
		change_directoy
}