# include "../../minishell.h"

static int	validate_format_export(char **args, int i)
{
	int		j;

	if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
	{
		if (args[i][0]== '-')
		{
			printf_fd(2, "minishell: export: `%s': invalid option\n", args[i]);
			return (BUILTIN_ERR);
		}
		printf_fd(2, "minishell: export: `%s': not a valid identifier\n", args[i]);
		return (ERROR);
	}
	j = 1;
	while (args[i][j] && args[i][j] != '=')
	{
		if (args[i][j] == '+' && args[i][j + 1] == '=')
			break ;
		if (!ft_isalnum(args[i][j]) && args[i][j] != '_')
		{
			printf_fd(2, "minishell: export: `%s': not a valid identifier\n",
				args[i]);
			return (ERROR);
		}
		j++;
	}
	return (SUCCESS);
}

static int	handle_append_export(char *key, char *new_value, t_env **envl)
{
	char	*value;
	char	*joined_value;
	int		ret;

	value = get_envl_var(key, *envl);
	if (!value)
		ret = set_envl_var(key, envl, new_value);
	else
	{
		joined_value = ft_strjoin(value, new_value);
		free(value);
		if (!joined_value)
			return (ERROR);
		ret = set_envl_var(key, envl, joined_value);
		free(joined_value);
	}
	return (ret);
}

static int	process_export_arg(char **args, int i, t_env **envl)
{
	char	*equal_pos;
	char	*plus_pos;
	char	*value;
	int		ret;

	ret = SUCCESS;
	plus_pos = ft_strnstr(args[i], "+=", ft_strlen(args[i]));
	equal_pos = ft_strchr(args[i], '=');
	if (!equal_pos)
	{
		if (!get_envl_var(args[i], *envl))
			ret = add_new_entry(args[i], NULL, envl);
	}
	else
	{
		if (plus_pos)
		{
			*plus_pos = '\0';
			value = plus_pos + 2;
			ret = handle_append_export(args[i], value, envl);
			*plus_pos = '+';
		}
		else
		{
			*equal_pos = '\0';
			value = equal_pos + 1;
			if (ft_strlen(value) == 0)
				ret = set_envl_var(args[i], envl, EMPTY);
			else
				ret = set_envl_var(args[i], envl, value);
			*equal_pos = '=';
		}
	}
	if (ret != 0)
		return (ERROR);
	return (SUCCESS);
}

int	builtin_export(char **args, t_env **envl)
{
	int		i;
	int		ret;
	int		status;

	if (!envl)
		return (ERROR); // verifier comportement
	if (args_count(args) == 1)
	{
		print_exp_list(*envl);
		return (SUCCESS);
	}
	ret = 0;
	status = 0;
	i = 0;
	while (args[++i])
	{
		ret = validate_format_export(args, i);
		if (ret != 0)
			status = ret;
		else if (process_export_arg(args, i, envl) != 0)
			status = ERROR;
	}
	return (status);
}
