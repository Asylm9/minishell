#include "../../minishell.h"

char	*get_env_var(char *name, char **env)
{
	int		i;
	int		len;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}

/* char	**get_paths(char **env)
{
	char	*env_path;
	char	**paths;

	env_path = get_env_var("PATH", env);
	if (!env_path)
		return (NULL);
	paths = ft_split(env_path, ':');
	free(env_path);
	return (paths);
} */

char	**get_paths(t_command *cmd, t_env *envl)
{
	char	*env_path;
	char	**paths;

	if (!cmd->cmd_name)
		return (NULL);
	if (cmd->cmd_name[0] == '\0')
	{
		paths = malloc(sizeof(cmd->cmd_name) + 1);
		paths[0] = ft_strdup("");
		return (paths);
	}
	env_path = get_envl_var("PATH", envl);
	if (!env_path)
	{
		printf_fd(2, "minishell: %s: No such file or directory\n", cmd->cmd_name);
		return (NULL);
	}
	paths = ft_split(env_path, ':');
	return (paths);
}

static char	*is_absolute_or_relative(char *cmd)
{
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd)); //pour ne pas risquer de free directement cmd->name plus tard
		return (NULL);
	}
	return (NULL);
}

char	*find_cmd_path(char **paths, char *cmd_name)
{
	char	*test_path;
	char	*abs_path;
	int		i;

	if (!paths || *paths[0] == '\0')
		return (NULL);
	abs_path = is_absolute_or_relative(cmd_name);
	if (abs_path)
		return (abs_path);
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

int	execute_binary(t_command *cmd, t_env *envl)
{
	char	**paths;
	char	*cmd_path;
	char	**env;

	if (!cmd || !envl)
		return (1);
	paths = get_paths(cmd, envl);
	if (!paths)
		return (1);
	cmd_path = find_cmd_path(paths, cmd->cmd_name);
	free_array(paths, -1);
	if (!cmd_path)
	{
		printf_fd(STDERR,"minishell: %s: command not found\n", cmd->cmd_name);
		return (CMD_NOT_FOUND);
	}
	env = convert_envl_to_env(envl);
	execve(cmd_path, cmd->args, env);
	perror("execve");
	free(cmd_path);
	return (EXECVE_ERR);
}

/* int	main(int ac, char **av, char **envp)
{
	t_command	cmd;
	t_redirect	redir;
	int			status;


	status = 0;
	if (ac < 2)
		return (0);
	init_redir(&redir);
	init_cmd_struct(&cmd, &av[1], &redir);
	status = execute_binary(&cmd, envp);
	return (status);
} */
