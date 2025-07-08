/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:24:57 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/05 21:06:34 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_string(t_string *string)
{
	size_t	initial_buffer_size;

	initial_buffer_size = 1024;
	string->buf = (char *)malloc(sizeof(char) * initial_buffer_size);
	string->buf_size = initial_buffer_size;
	string->len = 0;
}

static void	append_str(t_string *string, char *line)
{
	size_t		line_len;
	char		*tmp;

	line_len = ft_strlen(line);
	if (string->len + line_len + 1 > string->buf_size)
	{
		tmp = (char *)malloc(sizeof(char) * string->buf_size * 2);
		if (!tmp)
		{
			free(string->buf);
			free(line);
			string->buf = NULL;
			return ;
		}
		ft_memcpy(tmp, string->buf, string->len);
		free(string->buf);
		string->buf = tmp;
		string->buf_size *= 2;
	}
	ft_memcpy(string->buf + string->len, line, line_len);
	string->len += line_len;
}

static char	*get_line(t_string	*string)
{
	char	*line;

	ft_putstr_fd("> ", 1);
	line = get_next_line(STDIN_FILENO);
	if (!line)
	{
		free(string->buf);
		return (NULL);
	}
	return (line);
}

static char	*here_doc(char *limiter)
{
	t_string		result;
	char			*line;

	init_string(&result);
	if (!result.buf)
		return (NULL);
	while (true)
	{
		line = get_line(&result);
		if (line == NULL)
			return (NULL);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
			break ;
		append_str(&result, line);
		if (result.buf == NULL)
			return (NULL);
		free(line);
	}
	free(line);
	result.buf[result.len] = '\0';
	return (result.buf);
}

int	here_doc_fd(char *limiter)
{
	char	*input_str;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	input_str = here_doc(limiter);
	if (input_str != NULL)
	{
		ft_putstr_fd(input_str, pipe_fd[1]);
		free(input_str);
	}
	else
	{
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		return (-1);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
