#include "minishell.h"

int g_exit_status = 0;

static void	evaluate_struct(t_shell *shell)
{
	if (!shell || !shell->commands)
		return ;
	if (!shell->commands->next)
	{
		if(shell->commands->is_builtin)
			execute_builtin(shell);
		else
			just_execute_it_man(shell);
	}
	else if (shell->commands->next)
		execute_pipeline(shell);
}

static void	process_input(t_shell	*shell, char *input)
{
	add_history(input);
	shell->tokens = tokenize(input);
	if (shell->tokens)
	{
		expand_variables(shell, shell->tokens);
		shell->commands = parse_tokens(shell->tokens);
		if (shell->commands)
			evaluate_struct(shell);
	}
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;

	init_shell(&shell, envp);
	init_signals();
	while(1)
	{
		cwd = getcwd(NULL, 0);
		prompt = format_cwd(cwd);
		input = readline(prompt);
		if (!input)
			null_input(cwd, input);
		else if (*input)
			process_input(&shell, input);
		shell.exit_status = g_exit_status;
		free(input);
		free(cwd);
		cleanup_shell(&shell);
	}
	return (0);
}
