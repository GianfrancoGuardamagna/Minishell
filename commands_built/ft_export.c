#include "../minishell.h"

//Mostrar "declare -x VAR="var", en orden alfabetico y los valores enter ""

void	ft_export_env(t_shell *shell)
{
	int	i;

	i = 0;
	while(shell->env[i])
	{
		printf("declare -x ");
		printf("%s\n", shell->env[i]);
		i++;
	}
}
