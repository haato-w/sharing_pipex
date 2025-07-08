/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haatwata <haatwata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:24:54 by haatwata          #+#    #+#             */
/*   Updated: 2025/07/08 20:39:15 by haatwata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
This function prepare input file discriptor.
*/
static int	init_infile_fd(t_input_vars *input)
{
	int fd;

	// Here Document is implemented here. But I suppose that you don't need to understand
	// all of here_doc code. Ordinary input file situation is much important.
	if (input->here_doc)
	{
		fd = here_doc_fd(input->limiter);
		if (fd < 0)
			perror("here_doc");
	}
	else
	{
		// Ordinary input file situation is here.
		fd = open(input->in, O_RDONLY);
		if (fd < 0)
			perror(input->in);
	}
	return (fd);
}

/*
This function prepare input and output file discriptor.
*/
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

/*
This function create "pipe" which is a buffer on memory to share data between different processes.
*/
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

/*
This function is for error of "fork" function. You don't need to take care about this.
*/
void fork_error(t_internal_vars *internal, int prev_fd)
{
	perror("fork");
	if (prev_fd != internal->infile)
		close(prev_fd);
	close(internal->infile);
	close(internal->outfile);
}

int	pipex(t_input_vars *input)
{
	t_internal_vars	internal;
	int							prev_fd;
	int							pipe_fd[2];
	size_t					idx;
	pid_t						pid;

	// Initial process that prepare data resources
	if (init_io_fd(input, &internal) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	internal.paths = get_path(input->envp);
	
	// Main loop that exexute each commands
	idx = 0;
	prev_fd = internal.infile;
	while (idx < input->cmd_num)
	{
		if (create_pipe(&pipe_fd[0], &pipe_fd[1], &internal, prev_fd))
			return (EXIT_FAILURE);

		pid = fork();
		if (pid == -1) // fork error
			fork_error(&internal, prev_fd);
		if (pid == 0) // child process
			execute_process(idx, pipe_fd, prev_fd, input, &internal);
		// parent process
		close(pipe_fd[1]);
		close(prev_fd);
		prev_fd = pipe_fd[0];
		idx++;
	}
	
	// Final process that clean data resources
	close(prev_fd);
	close(internal.outfile);
	idx = 0;
	while (idx < input->cmd_num)
	{
		wait(NULL); // Waiting for the ends of all child processes
		idx++;
	}
	free_2arrays(internal.paths);
	
	return (EXIT_SUCCESS);
}
