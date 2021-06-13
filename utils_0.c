/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_0.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboudhir <hboudhir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 16:08:59 by hboudhir          #+#    #+#             */
/*   Updated: 2021/06/12 14:10:32 by hboudhir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		*safe_malloc(size_t size)  // equal to ft_memalloc() | To modify later
{
	void	*ptr;

	if (!(ptr = malloc(size + 1)))
		return (NULL);
	ft_bzero(ptr, size + 1);
	return (ptr);
}

int		error_msg(char *error_msg, int fd, int exit_code)
{
	write(fd, error_msg, ft_strlen(error_msg));
	return (exit_code);
}

char	*ft_strjoinch(char const *s1, char c)
{
	char	*new_str;
	size_t	i;
	size_t	s1_len;

	if (!s1 || !c)
		return (NULL);
	s1_len = ft_strlen(s1);
	new_str = safe_malloc(s1_len + 1);
	if (!new_str)
		return (NULL);
	i = -1;
	while (++i < s1_len)
		*(new_str + i) = *(s1 + i);
	*(new_str + i) = c;
	return (new_str);
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