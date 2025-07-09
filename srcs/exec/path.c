#include "../../minishell.h"

char	**get_paths(t_command *cmd, t_env *envl)
{
	char	*env_path;
	char	**paths;

	if (!cmd->cmd_name)
		return (NULL);
	// verifier utilite
	if (cmd->cmd_name[0] == '\0')
	{
		paths = malloc(sizeof(char *) * 2);
		paths[0] = ft_strdup("");
		paths[1] = NULL;
		return (paths);
	}
	env_path = get_envl_var("PATH", envl);
	if (!env_path)
	{
		printf_fd(2, "minishell: %s: No such file or directory\n",
			cmd->cmd_name);
		return (NULL);
	}
	paths = ft_split(env_path, ':');
	free(env_path);
	return (paths);
}

static bool	is_absolute_or_relative(char *cmd)
{
	return (ft_strchr(cmd, '/') || cmd[0] == '.');
}

char	*find_cmd_path(char **paths, char *cmd_name)
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
			return (NULL);
		if (access(test_path, F_OK | X_OK) == 0)
			return (test_path);
		free(test_path);
		i++;
	}
	return (NULL);
}

int	check_file_type(char *path)
{
	struct stat	file_stat;

	stat(path, &file_stat);
	if (S_ISDIR(file_stat.st_mode))
		return (1);
	return (0);
}

int	execute_binary(t_ast *ast, t_sh *shell, t_ast *root)
{
	char	**paths;
	char	*cmd_path;
	char	**env;

	if (!ast->cmd || !shell->envl)
		return (1);
	if (is_absolute_or_relative(ast->cmd->cmd_name))
	{
		cmd_path = ft_strdup(ast->cmd->cmd_name);
		if (access(cmd_path, F_OK) < 0 || ft_strcmp("..", cmd_path) == 0)
		{
			if (access(cmd_path, X_OK) < 0)
			{
				printf_fd(STDERR, "minishell: %s: %s\n", cmd_path,
					strerror(errno));
			}
			else
				printf_fd(STDERR, "minishell: %s: command not found\n",
					ast->cmd->cmd_name);
			free(cmd_path);
			return (CMD_NOT_FOUND);
		}
	}
	else
	{
		paths = get_paths(ast->cmd, shell->envl);
		if (!paths)
			return (1);
		cmd_path = find_cmd_path(paths, ast->cmd->cmd_name);
		free_array(paths, -1);
		if (!cmd_path)
		{
			printf_fd(STDERR, "minishell: %s: command not found\n",
			ast->cmd->cmd_name);
			return (cleanup_shell(shell, root), CMD_NOT_FOUND);
		}
	}
	env = convert_envl_to_env(shell->envl);
	execve(cmd_path, ast->cmd->args, env);

	if (check_file_type(cmd_path) == 1)
		printf_fd(STDERR, "minishell: %s: Is a directory\n", cmd_path);
	else
		printf_fd(STDERR, "minishell: %s: %s\n", cmd_path, strerror(errno));
	free(cmd_path);
	free_array(env, -1);
	return (cleanup_shell(shell, root), EXECVE_ERR);
}
