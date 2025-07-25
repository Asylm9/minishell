/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:36 by agaland           #+#    #+#             */
/*   Updated: 2025/07/25 16:59:16 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	is_absolute_or_relative(char *cmd)
{
	return (ft_strchr(cmd, '/') || (cmd[0] == '.' && ft_strchr(cmd, '/'))
		|| (cmd[0] == '.' && cmd[1] == '.'));
}

char	*resolve_direct_path(t_ast *ast, t_sh *shell, t_ast *root)
{
	char	*cmd_path;

	cmd_path = x_strdup(ast->cmd->cmd_name, shell, root, NULL);
	if (access(cmd_path, F_OK) < 0 || ft_strcmp("..", cmd_path) == 0)
	{
		if (access(cmd_path, X_OK) < 0)
		{
			printf_fd(STDERR, "minishell: %s: %s\n", cmd_path, strerror(errno));
		}
		else
			printf_fd(STDERR, "minishell: %s: command not found\n",
				ast->cmd->cmd_name);
		free(cmd_path);
		return (NULL);
	}
	return (cmd_path);
}

static int	check_file_type(char *path)
{
	struct stat	file_stat;

	stat(path, &file_stat);
	if (S_ISDIR(file_stat.st_mode))
		return (1);
	return (0);
}

static int	execve_error(char *cmd_path, char **env, t_sh *shell, t_ast *root)
{
	if (check_file_type(cmd_path) == 1)
		printf_fd(STDERR, "minishell: %s: Is a directory\n", cmd_path);
	else
		printf_fd(STDERR, "minishell: %s: %s\n", cmd_path, strerror(errno));
	free(cmd_path);
	free_array(env, -1);
	return (cleanup_shell(shell, root), EXECVE_ERR);
}

int	execute_binary(t_ast *ast, t_sh *shell, t_ast *root)
{
	char	*cmd_path;
	char	**env;

	if (!ast->cmd || !ast->cmd->cmd_name || !shell->envl)
		return (cleanup_shell(shell, root), 0);
	if (is_absolute_or_relative(ast->cmd->cmd_name))
	{
		cmd_path = resolve_direct_path(ast, shell, root);
		if (!cmd_path)
			return (cleanup_shell(shell, root), CMD_NOT_FOUND);
	}
	else
		cmd_path = resolve_path(ast, shell, root);
	if (!cmd_path)
	{
		printf_fd(STDERR, "minishell: %s: command not found\n",
			ast->cmd->cmd_name);
		return (cleanup_shell(shell, root), CMD_NOT_FOUND);
	}
	env = convert_envl_to_env(shell, root, ast);
	set_subprocess_signals();
	execve(cmd_path, ast->cmd->args, env);
	return (execve_error(cmd_path, env, shell, root));
}
