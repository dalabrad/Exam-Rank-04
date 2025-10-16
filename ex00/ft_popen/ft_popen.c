/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_popen.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalabrad <dalabrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:16:31 by dalabrad          #+#    #+#             */
/*   Updated: 2025/10/16 11:11:40 by dalabrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const *argv, char type)
{
    int		fd[2];
    pid_t	pid;
    
    if (!file || !argv || !argv[0] || !type || (type != 'r' && type != 'w'))
        return (-1);
    if (pipe(fd) < 0)
        return (-1);
    pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0) //Child Process
	{
		if (type == 'r')
		{
			close(fd[0]);
			if(dup2(fd[1], STDOUT_FILENO) < 0)
			{
				close(fd[1]);
				exit (-1);
			}
			close(fd[1]);
		}
		else
		{
			close(fd[1]);
			if(dup2(fd[0], STDIN_FILENO) < 0)
			{
				close(fd[0]);
				exit (-1);
			}
			close(fd[0]);
		}
		execvp(file, argv);
		exit(-1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else
	{
		close(fd[0]);
		return (fd[1]);
	}
}

/* #include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd1 = open("hola.txt", O_RDONLY);

	char buf[1];
	while(read(fd1, buf, 1))
		write(1, buf, 1);

	close(fd1);

	int fd2 = ft_popen("ls", (char *const[]){"ls", "-la", NULL}, 'r');

	while(read(fd2, buf, 1))
		write(1, buf, 1);

	close(fd2);

	int fd_file = open("hola.txt", O_RDONLY);
	int fd3 = ft_popen("grep", (char *const[]){"grep", "hola mundo!", NULL}, 'w');

	while(read(fd_file, buf, 1))
		write(fd3, buf, 1);

	close(fd3);
	close(fd_file);
	return (0);
}
 */