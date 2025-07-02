# include "../../minishell.h"

void	print_env_list(t_env *envl)
{
	t_env	*current;

	current = envl;
	while (current)
	{
		if (current->value != NULL) //skip les var ne devant etre affichees que par export
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int	builtin_env(t_sh *shell)
{
	if (!shell || !shell->envl) // Error: no environment variables found
		return (BUILTIN_ERR);
	print_env_list(shell->envl);
	return (SUCCESS);
}
