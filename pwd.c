/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 10:53:46 by hboudhir          #+#    #+#             */
/*   Updated: 2021/06/09 10:54:00 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			ft_builtin_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)) == NULL)
		return (-1);
	write(1, &buf, ft_strlen(buf));
	write(1, "\n", 1);
	return (0);
}