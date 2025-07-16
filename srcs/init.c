/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:12:16 by agaland           #+#    #+#             */
/*   Updated: 2025/07/16 13:12:18 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_list(t_env **new_node, char *var)
{
	char	*equal_pos;
	char	*value;
	int		level;

	equal_pos = ft_strchr(var, '=');
	if (!equal_pos)
		return ;
	value = equal_pos + 1;
	equal_pos[0] = '\0';
	if (value && strcmp(var, "SHLVL") == 0)
	{
		level = atoi(value) + 1;
		*new_node = create_node(ft_strdup(var), ft_itoa(level));
	}
	else if (value)
		*new_node = create_node(ft_strdup(var), ft_strdup(value));
	else
		*new_node = create_node(ft_strdup(var), NULL);
	equal_pos[0] = '=';
	if (!*new_node)
		return ;
}

t_env	*init_env_list(char **env)
{
	t_env	*new_node;
	t_env	*head;
	int		i;

	new_node = NULL;
	if (!env)
		return (NULL);
	head = NULL;
	i = 0;
	while (env[i])
	{
		fill_list(&new_node, env[i]);
		if (!new_node)
			return (NULL);
		head = add_back_node(new_node, head);
		i++;
	}
	return (head);
}

static t_env	*init_minimal_list(void)
{
	t_env	*head;
	t_env	*new_node1;
	t_env	*new_node2;
	char	buffer[PATH_MAX];

	head = NULL;
	if (!getcwd(buffer, sizeof(buffer)))
	{
		printf_fd(STDERR, "%s\n", strerror(errno));
		return (NULL);
	}
	new_node1 = create_node(ft_strdup("PWD"), ft_strdup(buffer));
	if (!new_node1)
		return (NULL);
	head = add_back_node(new_node1, head);
	new_node2 = create_node(ft_strdup("SHLVL"), ft_strdup("1"));
	if (!new_node2)
		return (NULL);
	head = add_back_node(new_node2, head);
	return (head);
}

int	init_shell(t_sh *shell, char **envp)
{
	if (!envp || !*envp)
		shell->envl = init_minimal_list();
	else
		shell->envl = init_env_list(envp);
	if (!shell->envl)
		return (ERROR);
	shell->in_pipeline = false;
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->exit_status = 0;
	return (SUCCESS);
}
