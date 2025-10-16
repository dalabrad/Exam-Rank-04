/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sandbox.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalabrad <dalabrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:25:10 by dalabrad          #+#    #+#             */
/*   Updated: 2025/10/16 19:28:39 by dalabrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


static void	do_nothing(int sig)
{
	(void)sig;
}

int	sandbox(void (*f)(void), unsigned int timeout, bool verbose)
{
	pid_t pid = fork();

	if (pid < 0)
		return (-1);
	if (pid == 0) //Child
	{
		alarm(timeout);
		f();
		exit(0);
	}

	struct sigaction	sa = {0};

	sa.sa_handler = do_nothing;
	sa.sa_flags = 0;
	if (sigaction(SIGALRM, &sa, NULL))
		return (-1);
	
	alarm(timeout);

	int 	status;
	pid_t	r = waitpid(pid, &status, 0);

	if (r < 0)
	{
		if (errno = EINTR)
		{
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
			return (0);
		}
		return (-1);
	}

	alarm(0); //Anular alarm()

	if (WIFEXITED(status))  // Check if child exited and its exit code.
	{
		int code = WEXITSTATUS(status);
		if (code == 0)
		{
			if (verbose)
				printf("Nice function!\n");
			return (1);
		}
		if (verbose)
			printf("Bad function; exited with code %d\n", code);
		return (0);
	}

	if (WIFSIGNALED(status)) //check if child finished by signal.
	{
		int sig = WTERMSIG(status);
		if (sig == SIGALRM)
		{
			if (verbose)
				printf("Bad function: timed out after %u seconds\n", timeout);
		}
		else
		{
			if (verbose)
			{
				printf("Bad function: %s\n", strsignal(sig));
			}
		}
		return (0);
	}
	return (-1); // Caso error
}
