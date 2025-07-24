/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:01 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 22:20:24 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**convert_envl_to_env(t_sh *shell, t_ast *root)
{
	char	**env;
	t_env	*current;
	int		i;

	env = malloc(sizeof(char *) * (list_size(shell->envl) + 1));
	if (!env)
		malloc_exit(shell, root);
	current = shell->envl;
	i = 0;
	while (current)
	{
		if (current->value)
			env[i] = ft_charjoin(current->key, current->value, '=');
		else
			env[i] = ft_strdup(current->key);
		if (!env[i])
		{
			free_array(env, i);
			malloc_exit(shell, root);
		}
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

bool	key_exists(char *key, t_env *envl)
{
	t_env	*current;

	current = envl;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (true);
		current = current->next;
	}
	return (false);
}

int	add_new_entry(char *key, char *value, t_sh *shell, t_ast *root)
{
	t_env	*new_node;
	char	*key_copy;
	char	*value_copy;

	if (!key)
		return (ERROR);
	key_copy = ft_strdup(key);
	if (!key_copy)
		malloc_exit(shell, root);
	value_copy = NULL;
	if (value)
	{
		value_copy = ft_strdup(value);
		if (!value_copy)
		{
			free(key_copy);
			malloc_exit(shell, root);
		}
	}
	new_node = create_node(key_copy, value_copy, shell, root);
	if (!new_node)
		return (ERROR);
	shell->envl = add_back_node(new_node, shell->envl); //
	return (SUCCESS);
}

char	*get_envl_var(char *name, t_sh *shell, t_ast *root)
{
	t_env	*current;
	char	*value;

	if (!name || !shell->envl)
		return (NULL);
	current = shell->envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			if (current->value)
			{
				value = ft_strdup(current->value);
				if (!value)
					malloc_exit(shell, root);
			}
			else 
				value = NULL;
			return (value);
		}
		current = current->next;
	}
	return (NULL);
}

int	set_envl_var(char *name, char *value, t_sh *shell, t_ast *root)
{
	t_env	*current;

	if (!name || !shell->envl)
		return (ERROR);
	current = shell->envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			if (value)
			{
				current->value = ft_strdup(value);
				if (!current->value)
					malloc_exit(shell, root);
			}
			else
				current->value = NULL;
			if (value && !current->value)
				return (ERROR);
			return (SUCCESS);
		}
		current = current->next;
	}
	return (add_new_entry(name, value, shell, root));
}
