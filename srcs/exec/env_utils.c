#include "../../minishell.h"

char	**convert_envl_to_env(t_env *envl)
{
	char	**env;
	t_env	*current;
	int		i;

	env = malloc(sizeof(char *) * (list_size(envl) + 1));
	if (!env)
		return (NULL);
	current = envl;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			env[i] = ft_charjoin(current->key, current->value, '=');
		}
		else
			env[i] = ft_strdup(current->key);
		if (!env[i])
			return (free_array(env, i), NULL);
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

int	add_new_entry(char *key, char *value, t_env **envl)
{
	t_env	*new_node;
	char	*key_copy;
	char	*value_copy;

	if (!key)
		return (ERROR);
	key_copy = ft_strdup(key);
	if (!key_copy)
		return (ERROR);
	value_copy = NULL;
	if (value)
	{
		value_copy = ft_strdup(value);
		if (!value_copy)
			return (free(key), ERROR);
	}
	new_node = create_node(key_copy, value_copy);
	if (!new_node)
		return (ERROR);
	*envl = add_back_node(new_node, *envl);
	return (SUCCESS);
}

char	*get_envl_var(char *name, t_env *envl)
{
	t_env	*current;

	if (!name || !envl)
		return (NULL);
	current = envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

int	set_envl_var(char *name, t_env **envl, char *value)
{
	t_env	*current;

	if (!name || !envl)
		return (ERROR);
	current = *envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			if (value && !current->value)
				return (ERROR);
			return (SUCCESS);
		}
		current = current->next;
	}
	return (add_new_entry(name, value, envl));
}
