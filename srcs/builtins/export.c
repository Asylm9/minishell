/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:14:57 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 01:01:31 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	validate_format_export(char **args, int i)
{
	int		j;

	if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
	{
		if (args[i][0] == '-')
			return (printf_fd(2, "minishell: export: `%s': invalid option\n"
					, args[i]), BUILTIN_ERR);
		printf_fd(2, "minishell: export: `%s': not a valid identifier\n",
			args[i]);
		return (ERROR);
	}
	j = 0;
	while (args[i][++j] && args[i][j] != '=')
	{
		if (args[i][j] == '+' && args[i][j + 1] == '=')
			break ;
		if (!ft_isalnum(args[i][j]) && args[i][j] != '_')
		{
			printf_fd(2, "minishell: export: `%s': not a valid identifier\n",
				args[i]);
			return (ERROR);
		}
	}
	return (SUCCESS);
}

static int	append_export(char *key, char *plus_pos, t_env **envl)
{
	char	*current_value;
	char	*new_value;
	char	*joined_value;
	int		ret;

	*plus_pos = '\0';
	new_value = plus_pos + 2;
	current_value = get_envl_var(key, *envl);
	if (!current_value)
		ret = set_envl_var(key, envl, new_value);
	else
	{
		joined_value = ft_strjoin(current_value, new_value);
		free(current_value);
		if (!joined_value)
			return (ERROR);
		ret = set_envl_var(key, envl, joined_value);
		free(joined_value);
	}
	return (ret);
}

static int	process_assignment(char *key, char *equal_pos, t_env **envl)
{
	char	*value;

	*equal_pos = '\0';
	value = equal_pos + 1;
	if ((ft_strcmp(key, "SHLVL") == 0 && (*value == '-' || !is_numeric(value))))
		return (set_envl_var(key, envl, "0"));
	else if (ft_strlen(value) == 0)
		return (set_envl_var(key, envl, EMPTY));
	return (set_envl_var(key, envl, value));
}

static int	process_export_arg(char **args, int i, t_env **envl)
{
	char	*equal_pos;
	char	*plus_pos;

	plus_pos = ft_strnstr(args[i], "+=", ft_strlen(args[i]));
	equal_pos = ft_strchr(args[i], '=');
	if (!equal_pos)
	{
		if (!key_exists(args[i], *envl))
			return (add_new_entry(args[i], NULL, envl));
		return (SUCCESS);
	}
	if (plus_pos)
		return (append_export(args[i], plus_pos, envl));
	return (process_assignment(args[i], equal_pos, envl));
}

int	builtin_export(char **args, t_env **envl)
{
	int		i;
	int		ret;
	int		status;

	if (!envl)
		return (ERROR);
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
