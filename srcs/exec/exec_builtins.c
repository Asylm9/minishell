/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:13:10 by agaland           #+#    #+#             */
/*   Updated: 2025/07/26 23:12:30 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	args_count(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

bool	is_builtin(char *cmd_name)
{
	if (!cmd_name)
		return (false);
	if (ft_strcmp(cmd_name, "echo") == 0 || ft_strcmp(cmd_name, "cd") == 0
		|| ft_strcmp(cmd_name, "pwd") == 0 || ft_strcmp(cmd_name, "export") == 0
		|| ft_strcmp(cmd_name, "unset") == 0 || ft_strcmp(cmd_name, "env") == 0
		|| ft_strcmp(cmd_name, "exit") == 0)
		return (true);
	return (false);
}

int	execute_builtin(t_ast *ast, t_sh *shell, t_ast *root)
{
	if (!ast->cmd || !shell)
		return (ERROR);
	if (ft_strcmp(ast->cmd->cmd_name, "echo") == 0)
		shell->exit_status = builtin_echo(ast, shell);
	else if (ft_strcmp(ast->cmd->cmd_name, "cd") == 0)
		shell->exit_status = builtin_cd(ast->cmd->args, shell, root);
	else if (ft_strcmp(ast->cmd->cmd_name, "pwd") == 0)
		shell->exit_status = builtin_pwd(ast->cmd->args);
	else if (ft_strcmp(ast->cmd->cmd_name, "export") == 0)
		shell->exit_status = builtin_export(ast->cmd->args, shell, root);
	else if (ft_strcmp(ast->cmd->cmd_name, "unset") == 0)
		shell->exit_status = builtin_unset(ast->cmd->args, &shell->envl);
	else if (ft_strcmp(ast->cmd->cmd_name, "env") == 0)
		shell->exit_status = builtin_env(shell);
	else if (ft_strcmp(ast->cmd->cmd_name, "exit") == 0)
		shell->exit_status = builtin_exit(ast, shell, root);
	return (shell->exit_status);
}
