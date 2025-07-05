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

static t_env	*init_minimal_list(t_sh *shell)
{
	t_env	*head;
	t_env	*new_node1;
	t_env	*new_node2;
	char	buffer[PATH_MAX];

	head = NULL;
	shell->envl = malloc(sizeof(t_env *));
	if (!shell->envl)
		return (NULL);
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
		shell->envl = init_minimal_list(shell);
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
