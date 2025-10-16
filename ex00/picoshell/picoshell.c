/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picoshell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalabrad <dalabrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:17:26 by dalabrad          #+#    #+#             */
/*   Updated: 2025/10/16 11:58:15 by dalabrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int	picoshell(char **cmds[])
{
	int		status;
	int		in_fd = 0;
	int		fd[2];
	pid_t	pid;
	int		ret = 0;

	int i = 0;
	while (cmds[i])
	{
		if (cmds[i + 1])
		{
			if (pipe(fd) < 0) //pipe if there's a next command
				return (1);
		}
		else
		{
			fd[0] = -1;
			fd[1] = -1;
		}
		pid = fork();
		if (pid < 0) //Error fork
		{
			if(fd[0] != -1)
				close(fd[0]);
			if(fd[0] != -1)
				close(fd[1]);
			if (in_fd != 0)
				close (in_fd);
			return (1);
		}
		if (pid == 0) //Child
		{
			if (in_fd != 0)
			{
				if (dup2(in_fd, STDIN_FILENO) < 0)
					exit(1);
				close(in_fd);
			}
			if (fd[1] != -1)
			{
				if (dup2(fd[1], STDOUT_FILENO) < 0)
					exit(1);
				close(fd[0]);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		else //Parent
		{
			if (in_fd != 0)
				close(in_fd);
			if (fd[1] != -1)
				close(fd[1]);
			in_fd = fd[0];
			i++;
		}
	}
	while(wait(&status) > 0)
	{
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			ret = 1;
		if (!WIFEXITED(status))
			ret = 1;
	}
	if(fd[0] != -1)
		close(fd[0]);
	if(fd[0] != -1)
		close(fd[1]);
	if (in_fd != 0 && in_fd != -1)
		close (in_fd);
	return (ret);
}
