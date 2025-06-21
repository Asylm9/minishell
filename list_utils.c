#include "minishell.h"

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

void	print_exp_list(t_env *envl)
{
	t_env	**ptr_array;
	int		count;
	int		i;

	count = count_elements(envl);
	ptr_array = init_temp_array(envl, count);
	if (!ptr_array)
		return;
	sort_env_list(ptr_array, count);
	i = 0;
	while (i < count)
	{
		if (!ptr_array[i]->value)
			printf("export %s\n", ptr_array[i]->key);
		else
			printf("export %s=\"%s\"\n", ptr_array[i]->key, ptr_array[i]->value); // gestion "" = sparadrap :v
		i++;
	}
	free(ptr_array);
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
