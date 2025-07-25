/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:12:16 by agaland           #+#    #+#             */
/*   Updated: 2025/07/25 16:56:20 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_list(t_env **new_node, char *var, t_sh *shell, t_ast *ast)
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
		*new_node = create_node(x_strdup(var, shell, ast, NULL), ft_itoa(level),
				shell, NULL);
	}
	else if (value)
		*new_node = create_node(x_strdup(var, shell, ast, NULL), x_strdup(value,
					shell, ast, NULL), shell, NULL);
	else
		*new_node = create_node(x_strdup(var, shell, ast, NULL), NULL, shell,
				NULL);
	equal_pos[0] = '=';
}

t_env	*init_env_list(char **env, t_sh *shell, t_ast *ast)
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
		fill_list(&new_node, env[i], shell, ast);
		if (!new_node)
			return (NULL);
		head = add_back_node(new_node, head);
		i++;
	}
	return (head);
}

static t_env	*init_minimal_list(t_sh *shell)
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
	new_node1 = create_node(ft_strdup("PWD"), ft_strdup(buffer), shell, NULL);
	head = add_back_node(new_node1, head);
	new_node2 = create_node(ft_strdup("SHLVL"), ft_strdup("1"), shell, NULL);
	head = add_back_node(new_node2, head);
	return (head);
}

int	init_shell(t_sh *shell, char **envp, t_ast *ast)
{
	if (!envp || !*envp)
		shell->envl = init_minimal_list(shell);
	else
		shell->envl = init_env_list(envp, shell, ast);
	if (!shell->envl)
		return (ERROR);
	shell->in_pipeline = false;
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->exit_status = 0;
	return (SUCCESS);
}
