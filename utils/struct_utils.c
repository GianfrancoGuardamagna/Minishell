#include "../minishell.h"

void	init_shell(t_shell *shell)
{
	if (!shell)
		return ;
	shell->env = manage_env(NULL, 0, NULL);
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->exit_status = 0;
	shell->stdin_copy = dup(STDIN_FILENO);
	shell->stdout_copy = dup(STDOUT_FILENO);
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->tokens)
		free_tokens(&shell->tokens);
	if (shell->commands)
		free_commands(&shell->commands);
	if (shell->stdin_copy > 2)
		close(shell->stdin_copy);
	if (shell->stdout_copy > 2)
		close(shell->stdout_copy);
}

void	check_struct(t_shell *shell)
{
	int i;

	i = 0;
	printf("Stdin: %d\n", shell->stdin_copy);
	printf("Stdout: %d\n", shell->stdout_copy);
	printf("Exit Status: %d\n", shell->exit_status);
	printf("/////////////////////Commands///////////////////////////\n");
	while(shell->commands)
	{
		printf("FDdin: %d\n", shell->commands->in_fd);
		printf("FDout: %d\n", shell->commands->out_fd);
		printf("Pipe 0: %d\n", shell->commands->pipe[0]);
		printf("Pipe 1: %d\n", shell->commands->pipe[1]);
		printf("Is Built in: %d\n", shell->commands->is_builtin);
		printf("Stdin: %d\n", shell->stdin_copy);
		printf("Stdout: %d\n", shell->stdout_copy);
		i = 0;
		while(shell->commands->av[i])
		{
			printf("As %d: %s\n", i,shell->commands->av[i]);
			i++;
		}
		printf("////////////////////////////////////////////////\n");
		shell->commands = shell->commands->next;
	}
}