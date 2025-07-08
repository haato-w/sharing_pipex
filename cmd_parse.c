/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:25:01 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/05 20:28:00 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static	size_t	count_token(char *cmd)
{
	size_t	i;
	size_t	cnt;

	cnt = 0;
	i = 0;
	while (cmd[i])
	{
		if (!is_space(cmd[i]) && (i == 0 || is_space(cmd[i - 1])))
			cnt++;
		i++;
	}
	return (cnt);
}

static	ssize_t	handle_quote(char **p, char *buf)
{
	char	quote;
	ssize_t	buf_i;

	quote = **p;
	p++;
	buf_i = 0;
	while (**p && **p != quote)
	{
		buf[buf_i] = **p;
		buf_i++;
		(*p)++;
	}
	if (**p == quote)
	{
		(*p) += buf_i + 2;
		return (buf_i);
	}
	return (-1);
}

static	char	*get_elem(char **p)
{
	char	*buf;
	ssize_t	buf_i;

	buf = (char *)malloc(ft_strlen(*p) + 1);
	if (buf == NULL)
		return (NULL);
	if (**p == '\'' || **p == '"')
	{
		buf_i = handle_quote(p, buf);
		if (buf_i == -1)
			return (NULL);
	}
	else
	{
		buf_i = 0;
		while (**p && !is_space(**p))
		{
			buf[buf_i] = **p;
			buf_i++;
			(*p)++;
		}
	}
	buf[buf_i] = '\0';
	return (buf);
}

static char	**get_token_arr(char *cmd)
{
	int		token_num;
	char	**tokens;

	token_num = count_token(cmd);
	tokens = (char **)malloc(sizeof(char *) * (token_num + 1));
	return (tokens);
}

char	**cmd_parse(char *cmd)
{
	char	**tokens;
	int		tok_i;
	char	*p;
	char	*elem;

	tokens = get_token_arr(cmd);
	tok_i = 0;
	p = cmd;
	while (*p)
	{
		while (is_space(*p))
			p++;
		if (*p == '\0')
			break ;
		elem = get_elem(&p);
		if (elem == NULL)
		{
			tokens[tok_i++] = NULL;
			free_2arrays(tokens);
			return (NULL);
		}
		tokens[tok_i++] = elem;
	}
	tokens[tok_i] = NULL;
	return (tokens);
}

// int main() {
//     char input[] = "echo 'hello world' \"foo bar\" baz";

//     char **tokens = cmd_parse(input);
//     for (int i = 0; tokens[i]; i++) {
//         printf("Token %d: [%s]\n", i, tokens[i]);
//         free(tokens[i]);
//     }
//     free(tokens);
//     return 0;
// }
