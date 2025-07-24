/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sorting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:14:49 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 22:25:30 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	count_elements(t_env *envl)
{
	t_env	*current;
	int		n;

	current = envl;
	n = 0;
	while (current)
	{
		current = current->next;
		n++;
	}
	return (n);
}

void	print_exp_list(t_sh *shell, t_ast *root)
{
	t_env	**ptr_array;
	int		count;
	int		i;

	count = count_elements(shell->envl);
	ptr_array = init_temp_array(count, shell, root);
	if (!ptr_array)
		malloc_exit(shell, root);
	sort_env_list(ptr_array, count);
	i = 0;
	while (i < count)
	{
		if (!ptr_array[i]->value)
			printf_fd(STDOUT_FILENO, "export %s\n", ptr_array[i]->key);
		else
			printf_fd(STDOUT_FILENO, "export %s=\"%s\"\n", ptr_array[i]->key,
				ptr_array[i]->value);
		i++;
	}
	free(ptr_array);
}

t_env	**init_temp_array(int count, t_sh *shell, t_ast *root)
{
	t_env	**ptr_array;
	int		i;

	ptr_array = (t_env **) malloc(sizeof(t_env *) * count);
	if (!ptr_array)
		malloc_exit(shell, root);
	i = 0;
	while (i < count)
	{
		ptr_array[i] = shell->envl;
		shell->envl = shell->envl->next;
		i++;
	}
	return (ptr_array);
}

void	sort_env_list(t_env **array, int count)
{
	int		i;
	int		j;
	int		min_index;
	t_env	*temp;

	i = 0;
	while (i < count - 1)
	{
		min_index = i;
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(array[j]->key, array[min_index]->key) < 0)
				min_index = j;
			j++;
		}
		if (min_index != i)
		{
			temp = array[i];
			array[i] = array[min_index];
			array[min_index] = temp;
		}
		i++;
	}
}
