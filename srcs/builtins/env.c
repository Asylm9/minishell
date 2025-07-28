/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:14:37 by agaland           #+#    #+#             */
/*   Updated: 2025/07/28 20:44:49 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_env_list(t_env *envl)
{
	t_env	*current;

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
	{
		printf_fd(STDERR_FILENO, "Environment list is NULL\n");
		return (BUILTIN_ERR);
	}
	print_env_list(shell->envl);
	return (SUCCESS);
}
