#include "../../minishell.h"

char	**convert_envl_to_env(t_env *envl)
{
	char	**env;
	t_env	*current;
	int		i;

	env = malloc(sizeof(char *) * (list_size(envl) + 1));
	if (!env)
		return (NULL);
	current = envl;
	i = 0;
	while (current)
	{
		if (current->value)
		{
			env[i] = ft_charjoin(current->key, current->value, '=');
		}
		else
			env[i] = ft_strdup(current->key);
		if (!env[i])
			return (free_array(env, i), NULL);
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}

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

int	add_new_entry(char *key, char *value, t_env **envl)
{
	t_env	*new_node;
	char	*key_copy;
	char	*value_copy;

	// uniquement appele depuis set_envl_var pour l'instant:pas besoin de reverifier !key,!value!,env
	key_copy = ft_strdup(key);
	if (!key_copy)
		return (ERROR);
	value_copy = NULL;
	if (value)
	{
		value_copy = ft_strdup(value);
		if (!value_copy)
			return (free(key), ERROR);
	}
	new_node = create_node(key_copy, value_copy);
	if (!new_node)
		return (ERROR);
	*envl = add_back_node(new_node, *envl);
	return (SUCCESS);
}

char	*get_envl_var(char *name, t_env *envl)
{
	t_env	*current;

	if (!name || !envl)
		return (NULL);
	current = envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (NULL);
}

int	set_envl_var(char *name, t_env **envl, char *value)
{
	t_env	*current;

	if (!name || !envl)
		return (ERROR);
	current = *envl;
	while (current)
	{
		if (ft_strcmp(current->key, name) == 0)
		{
			free(current->value);
			if (value)
				current->value = ft_strdup(value);
			else
				current->value = NULL;
			if (value && !current->value)
				return (ERROR);
			return (SUCCESS);
		}
		current = current->next;
	}
	return (add_new_entry(name, value, envl));
}

int	init_minimal_shell(t_sh *shell)
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
	shell->in_pipeline = false;
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->exit_status = 0;
	shell->envl = init_env_list(shell->env);
	free_array(shell->env, -1);
	shell->env = NULL;
	return (SUCCESS);
}

void	init_shell_struct(t_sh *shell, char **envp)
{
	int	count;
	int	i;

	count = 0;
	while (envp[count])
		count++;
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
		return ;
	i = 0;
	while (envp[i])
	{
		shell->env[i] = ft_strdup(envp[i]);
		i++;
	}
	shell->env[i] = NULL;
	shell->in_pipeline = false;
	shell->saved_stdin = -1;
	shell->saved_stdout = -1;
	shell->exit_status = 0;
	shell->envl = init_env_list(shell->env);
	free_array(shell->env, -1);
	shell->env = NULL;
}

/* void	print_env_array(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
} */
