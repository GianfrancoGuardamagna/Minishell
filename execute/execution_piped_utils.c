#include "../minishell.h"

int	create_pipe_if_needed(t_cmd *current, int *pipe_fd)
{
	if (current->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	return (0);
}

void	fd_redirections(int prev_fd, t_cmd	*current, int *pipe_fd)
{
	if (prev_fd != STDIN_FILENO)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
	}
	if (current->out_fd != STDOUT_FILENO && current->out_fd > 0)
	{
		dup2(current->out_fd, STDOUT_FILENO);
		close(current->out_fd);
	}
	if (current->in_fd != STDIN_FILENO && current->in_fd > 0)
	{
		dup2(current->in_fd, STDIN_FILENO);
		close(current->in_fd);
	}
}

void	wait_for_childs(t_shell *shell)
{
	t_cmd	*current;
	int		status;

	current = shell->commands;
	while (current)
	{
		wait(&status);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		current = current->next;
	}
}
