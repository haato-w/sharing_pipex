/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:25:11 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/05 21:54:44 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static bool is_here_doc(char **argv)
{
  if (ft_strncmp(argv[1], "here_doc", 9) == 0)
    return (true);
  return (false);
}

static void parse_vars(int argc, char **argv, char **envp, t_input_vars *vars)
{
  vars->here_doc = is_here_doc(argv);
  if (!vars->here_doc)
  {
    vars->in = argv[1];
    vars->cmds = argv + 2;
    vars->cmd_num = argc - 3;
    vars->out = argv[argc - 1];
    vars->limiter = NULL;
  }
  else
  {
    vars->in = NULL;
    vars->cmds = argv + 3;
    vars->cmd_num = argc - 4;
    vars->out = argv[argc - 1];
    vars->limiter = argv[2];
  }
	vars->envp = envp;
}

int main(int argc, char **argv, char **envp)
{
  t_input_vars vars;

  if (argc - 1 < 4)
  {
    ft_putendl_fd("Error", 2);
    return (EXIT_FAILURE);
  }
  parse_vars(argc, argv, envp, &vars);
  return pipex(&vars);
}
