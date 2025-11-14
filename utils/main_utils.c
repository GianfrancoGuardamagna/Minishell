#include "../minishell.h"

void	null_input(char	*cwd, char	*input)
{
	printf("exit\n");
	free(cwd);
	free(input);
	exit(0);
}
void	init_signals(void)
{
	struct	sigaction sa;
	rl_catch_signals = 0;
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
