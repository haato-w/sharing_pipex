/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 22:29:45 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/08 20:41:41 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

static char*	find_path(char *cmd, char **path)
{
	char	*ret;

	if (cmd && ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return ft_strdup(cmd);
		perror(cmd);
	}
	else
	{
		while (cmd && path && *path)
		{
			ret = join_path(*path, cmd);
			if (*ret && access(ret, X_OK) == 0)
				return (ret);
			free(ret);
			path++;
		}
		if (cmd)
			ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	ret = NULL;
	return (ret);
}

static int	dup_fd(int input_fd, int output_fd)
{
	if (dup2(input_fd, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		close(input_fd);
		close(output_fd);
		return (EXIT_FAILURE);
	}
	if (dup2(output_fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		close(input_fd);
		close(output_fd);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void	execute_cmd(char *cmd, int input_fd, int output_fd, char **env_path, char **envp)
{
	char	*cmd_path;
	char	**args;

	if (dup_fd(input_fd, output_fd) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	args = cmd_parse(cmd);
	if (args == NULL)
	{
		close(input_fd);
		close(output_fd);
		exit(EXIT_FAILURE);
	}
	cmd_path = find_path(args[0], env_path);
	if (!cmd_path)
	{
		close(input_fd);
		close(output_fd);
		free_2arrays(args);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	execute_process(size_t idx, int *pipe_fd, int prev_fd, t_input_vars *input, t_internal_vars *internal)
{
	// The file discriptor for reading of pipe isn't needed by child process because reading is executed on prev_fd.
	close(pipe_fd[0]);
	if (idx != input->cmd_num - 1)
		execute_cmd(input->cmds[idx], prev_fd, pipe_fd[1], internal->paths, input->envp);
	else
		execute_cmd(input->cmds[idx], prev_fd, internal->outfile, internal->paths, input->envp);
}
