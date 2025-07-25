/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:14:25 by agaland           #+#    #+#             */
/*   Updated: 2025/07/25 15:35:18 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*set_new_path(char **args, t_sh *shell, t_ast *root)
{
	char	*new_path;
	int		argc;

	new_path = NULL;
	argc = args_count(args);
	if (argc > 2)
		return (printf_fd(STDERR, "minishell: cd: too many arguments\n"), NULL);
	if (argc == 1)
		new_path = get_envl_var("HOME", shell, root);
	else if (argc == 2)
	{
		if (args[1][0] == '~')
			new_path = get_envl_var("HOME", shell, root);
		else if (args[1][0] == '-')
			new_path = get_envl_var("OLDPWD", shell, root);
		else
			new_path = x_strdup(args[1], shell, root, NULL);
	}
	return (new_path);
}

int	validate_path(char **args, char *new_path)
{
	int	argc;

	argc = args_count(args);
	if (!new_path)
	{
		if (argc == 1)
			printf_fd(STDERR, "minishell: cd: HOME not set\n");
		else if (args[1][0] == '-')
			printf_fd(STDERR, "minishell: cd: OLDPWD not set\n");
		return (ERROR);
	}
	if (access(new_path, F_OK) < 0)
	{
		printf_fd(STDERR, "cd: %s: No such file or directory\n", new_path);
		return (ERROR);
	}
	if (access(new_path, X_OK) < 0)
	{
		printf_fd(STDERR, "minishell: %s: Not a directory\n", args[1]);
		return (ERROR);
	}
	return (SUCCESS);
}

int	update_pwds(t_sh *shell, char *curr_dir, t_ast *root)
{
	set_envl_var("OLDPWD", curr_dir, shell, root);
	if (!getcwd(curr_dir, PATH_MAX))
		return (perror("getcwd"), ERROR);
	set_envl_var("PWD", curr_dir, shell, root);
	return (SUCCESS);
}

int	builtin_cd(char **args, t_sh *shell, t_ast *root)
{
	char	curr_dir[PATH_MAX];
	char	*new_path;

	if (!getcwd(curr_dir, PATH_MAX))
		return (perror("getcwd"), BUILTIN_ERR);
	new_path = set_new_path(args, shell, root);
	if (validate_path(args, new_path) != SUCCESS)
		return (ERROR);
	if (chdir(new_path) < 0)
		return (BUILTIN_ERR);
	free(new_path);
	if (update_pwds(shell, curr_dir, root) != 0)
		return (BUILTIN_ERR);
	return (SUCCESS);
}
