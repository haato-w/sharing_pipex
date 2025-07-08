/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 20:59:38 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/08 20:32:38 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <stdio.h>

# include "submodules/printf/libft/libft.h"
# include "submodules/printf/ft_printf.h"
# include "submodules/get_next_line/get_next_line_bonus.h"

typedef struct s_input_vars
{
  char *in;
  char **cmds;
  size_t cmd_num;
  char *out;
  bool here_doc;
  char *limiter;
  char **envp;
} t_input_vars;

typedef struct s_internal_vars
{
  char **paths;
  int infile;
  int outfile;
} t_internal_vars;

int		pipex(t_input_vars *vars);
void	execute_process(size_t idx, int *pipe_fd, int prev_fd, t_input_vars *input, t_internal_vars *internal);
int		pipex(t_input_vars *input);
char	**cmd_parse(char *cmd);
int		here_doc_fd(char *limiter);
void	free_2arrays(char **arr);
bool	is_space(char c);
char	**get_path(char **envp);
char	*join_path(char *base, char *path);

#endif
