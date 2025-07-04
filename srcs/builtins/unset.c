#include "../../minishell.h"

static void	delete_env_var(t_env *current, t_env **head)
{
	if (current->prev == NULL)
	{
		*head = current->next;
		if (current->next)
			current->next->prev = NULL;
	}
	else if (current->next == NULL)
	{
		current->prev->next = NULL;
	}
	else
	{
		current->next->prev = current->prev;
		current->prev->next = current->next;
	}
	free(current->key);
	if (current->value)
		free(current->value);
	free(current);
}

int	builtin_unset(char **args, t_env **envl)
{
	t_env	*current;
	int		i;

	if (!args[1])
		return (SUCCESS);
	i = 1;
	while (args[i])
	{
		current = *envl;
		while (current)
		{
			if (ft_strcmp(current->key, args[i]) == 0)
			{
				delete_env_var(current, envl);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	return (SUCCESS);
}
