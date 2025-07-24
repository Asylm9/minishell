/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:48 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 22:17:20 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*find_cmd_path(char **paths, char *cmd_name, t_sh *shell, t_ast *root)
{
	char	*test_path;
	int		i;

	if (!paths || !paths[0] || paths[0][0] == '\0')
		return (NULL);
	i = 0;
	while (paths[i])
	{
		test_path = ft_charjoin(paths[i], cmd_name, '/');
		if (!test_path)
			malloc_exit(shell, root);
		if (access(test_path, F_OK | X_OK) == 0)
			return (test_path);
		free(test_path);
		i++;
	}
	return (NULL);
}

char	**get_paths(t_command *cmd, t_sh *shell, t_ast *root)
{
	char	*env_path;
	char	**paths;

	if (!cmd->cmd_name)
		return (NULL);
	if (cmd->cmd_name[0] == '\0')
	{
		paths = malloc(sizeof(char *) * 2);
		if (!paths)
			malloc_exit(shell, root);
		paths[0] = ft_strdup("");
		if (!paths[0])
		{
			free(paths);
			malloc_exit(shell, root);
		}
		paths[1] = NULL;
		return (paths);
	}
	env_path = get_envl_var("PATH", shell, root);
	if (!env_path)
	{
		printf_fd(STDERR, "minishell: %s: No such file or directory\n",
			cmd->cmd_name);
		return (NULL);
	}
	paths = ft_split(env_path, ':');
	free(env_path);
	if (!paths)
		malloc_exit(shell, root);
	return (paths);
}

char	*resolve_path(t_ast *ast, t_sh *shell, t_ast *root)
{
	char	**paths;
	char	*cmd_path;

	paths = get_paths(ast->cmd, shell, root);
	if (!paths)
	{
		cleanup_shell(shell, root);
		exit(CMD_NOT_FOUND);
	}
	cmd_path = find_cmd_path(paths, ast->cmd->cmd_name, shell, root);
	free_array(paths, -1);
	return (cmd_path);
}
