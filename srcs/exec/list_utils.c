#include "../../minishell.h"

int	list_size(t_env *envl)
{
	int	size;

	size = 0;
	while (envl != NULL)
	{
		size++;
		envl = envl->next;
	}
	return (size);
}

t_env	*create_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = (t_env *) malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	if (value)
		new_node->value = value;
	else
		new_node->value = NULL;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_env	*find_last_node(t_env *head)
{
	t_env	*current;
	t_env	*last_node;

	if (!head)
		return (NULL);
	current = head;
	while (current->next != NULL)
		current = current->next;
	last_node = current;
	return (last_node);
}

t_env	*add_back_node(t_env *new_node, t_env *head)
{
	t_env	*last_node;

	if (!head)
		return (new_node);
	else
	{
		last_node = find_last_node(head);
		last_node->next = new_node;
		new_node->prev = last_node;
	}
	return (head);
}
