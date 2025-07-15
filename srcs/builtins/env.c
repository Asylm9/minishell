#include "../../minishell.h"

void	print_env_list(t_env *envl)
{
	t_env	*current;

	if (!envl)
	{
		printf_fd(STDERR_FILENO, "Environment list is NULL\n");
		return ;
	}
	current = envl;
	while (current)
	{
		if (current->value != NULL)
			printf_fd(STDOUT_FILENO, "%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int	builtin_env(t_sh *shell)
{
	if (!shell || !shell->envl)
		return (BUILTIN_ERR);
	print_env_list(shell->envl);
	return (SUCCESS);
}
