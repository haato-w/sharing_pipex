/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:25:17 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/05 21:51:51 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_2arrays(char **arr)
{
	size_t	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

bool	is_space(char c)
{
	return (c && ft_strchr(" \f\n\r\t\v", c) != NULL);
}

char **get_path(char **envp)
{
	size_t	i;

	if (!envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	return (ft_split(&envp[i][5], ':'));
}

char	*join_path(char *base, char *path)
{
	size_t		size;
	char		*ret;
	bool			has_slash_in_last;

	size = ft_strlen(base);
	has_slash_in_last = base[size - 1] == '/';
	if (has_slash_in_last)
		size += ft_strlen(path) + 1;
	else
		size += ft_strlen(path) + 2;
	ret = (char *)malloc(sizeof(char) * size);
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, base, size);
	if (!has_slash_in_last)
		ft_strlcat(ret, "/", size);
	ft_strlcat(ret, path, size);
	return (ret);
}
