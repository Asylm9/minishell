#include "minishell.h"

t_env	*init_env_list(char **env)
{
	t_env	*new_node;
	t_env	*head;
	char	*equal_pos;
	char	*value;
	int		i;
	int		level;

	if (!env)
		return (NULL);
	head = NULL;
	i = 0;
	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (!equal_pos)
			return (NULL);
		value = equal_pos + 1;
		equal_pos[0] = '\0';
		if (value && strcmp(env[i], "SHLVL") == 0)
		{
			level = atoi(value) + 1;
			new_node = create_node(ft_strdup(env[i]), ft_itoa(level));
		}
		else if (value)
			new_node = create_node(ft_strdup(env[i]), ft_strdup(value));
		else
			new_node = create_node(ft_strdup(env[i]), NULL);
		if (!new_node)
			return (NULL);
		equal_pos[0] = '=';
		head = add_back_node(new_node, head);
		i++;
	}
	return (head);
}

static int	init_minimal_shell(t_sh *shell)
{
	char	buffer[PATH_MAX];
	char	*pwd_var;

	shell->env = malloc(sizeof(char *) * 3);
	if (!shell->env)
		return (ERROR);
	if (!getcwd(buffer, sizeof(buffer)))
	{
		printf_fd(STDERR, "%s\n", strerror(errno));
		free(shell->env);
		return (ERROR);
	}
	pwd_var = ft_strjoin("PWD=", buffer);
	if (!pwd_var)
		return (free(shell->env), ERROR);
	shell->env[0] = ft_strdup("SHLVL=1");
	if (!shell->env[0])
		return (free(pwd_var), free(shell->env), ERROR);
	shell->env[1] = pwd_var;
	shell->env[2] = NULL;
	return (SUCCESS);
}

static int	init_env_array(t_sh *shell, char **envp)
{
	int	count;
	int	i;

	count = 0;
	while (envp[count])
		count++;
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
		return (ERROR);
	i = 0;
	while (envp[i])
	{
		shell->env[i] = ft_strdup(envp[i]);
		i++;
	}
	shell->env[i] = NULL;
	return (SUCCESS);
}

int	init_shell(t_sh *shell, char **envp)
{
	int	ret;

	if (!envp || !*envp)
		ret = init_minimal_shell(shell);
	else
		ret = init_env_array(shell, envp);
	if (ret != SUCCESS)
		return (ERROR);
	shell->in_pipeline = false;
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->exit_status = 0;
	shell->envl = init_env_list(shell->env);
	free_array(shell->env, -1);
	shell->env = NULL;
	if (shell->envl)
		return (ERROR);
	return (SUCCESS);
}
