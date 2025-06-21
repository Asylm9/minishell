#include "minishell.h"

void	free_pipes(int **pipes, int i)
{
	if (!pipes)
		return;
	while (i >= 0)
		free(pipes[i--]);
	free(pipes);
}

void	free_array(char **array, int i)
{
	if (!array)
		return;
	if (i < 0)
	{
		i = 0;
		while (array[i])
			free(array[i++]);
	}
	else 
	{
		while (i >= 0)
			free(array[i--]);
	}
	free(array);
}

void	free_envl(t_env **head)
{
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		if ((*head)->key)
			free((*head)->key);
		if ((*head)->value)
			free((*head)->value);
		free(*head);
		*head = tmp;
	}
	*head = NULL;
}

void	cleanup_shell(t_sh *shell)
{
	if (shell && shell->env)
	{
		free_array(shell->env, -1);
		shell->env = NULL;
	}
	if (!shell->envl)
		return;
	free_envl(&shell->envl);
}
