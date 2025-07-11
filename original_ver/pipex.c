/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:24:54 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/05 22:31:00 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	init_infile_fd(t_input_vars *input)
{
	int fd;

	if (input->here_doc)
	{
		fd = here_doc_fd(input->limiter);
		if (fd < 0)
			perror("here_doc");
	}
	else
	{
		fd = open(input->in, O_RDONLY);
		if (fd < 0)
			perror(input->in);
	}
	return (fd);
}

static int	init_io_fd(t_input_vars *input, t_internal_vars *internal)
{
	internal->infile = init_infile_fd(input);
	if (input->here_doc)
		internal->outfile = open(input->out, O_WRONLY | O_CREAT | O_APPEND, 
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		internal->outfile = open(input->out, O_WRONLY | O_CREAT | O_TRUNC, 
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (internal->infile < 0 || internal->outfile < 0)
	{
		if (internal->infile >= 0)
			close(internal->infile);
		if (internal->outfile >= 0)
			close(internal->outfile);
		else
			perror(input->out);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	create_pipe(int *read, int *write, t_internal_vars *internal, int prev_fd)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		if (prev_fd != internal->infile)
			close(prev_fd);
		close(internal->infile);
		close(internal->outfile);
		return (EXIT_FAILURE);
	}
	*read = pipe_fd[0];
	*write = pipe_fd[1];
	return (EXIT_SUCCESS);
}

static void	final_process(t_input_vars *input, t_internal_vars *internal, int prev_fd)
{
	size_t	i;

	close(prev_fd);
	close(internal->outfile);
	i = 0;
	while (i < input->cmd_num)
	{
		wait(NULL);
		i++;
	}
	free_2arrays(internal->paths);
}

int	pipex(t_input_vars *input)
{
	t_internal_vars	internal;
	int							prev_fd;
	int							pipe_fd[2];
	size_t					i;

	if (init_io_fd(input, &internal) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	internal.paths = get_path(input->envp);
	i = 0;
	prev_fd = internal.infile;
	while (i < input->cmd_num)
	{
		if (create_pipe(&pipe_fd[0], &pipe_fd[1], &internal, prev_fd))
			return (EXIT_FAILURE);
		if (create_fork(&internal, prev_fd) == 0) // child
			execute_process(i, pipe_fd, prev_fd, input, &internal);
		close(pipe_fd[1]);
		close(prev_fd);
		prev_fd = pipe_fd[0];
		i++;
	}
	final_process(input, &internal, prev_fd);
	return (EXIT_SUCCESS);
}
