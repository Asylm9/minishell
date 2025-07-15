/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:31:53 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/15 16:32:51 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_env_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	expand_var(char *input, char **result, t_env *envl)
{
	int		i;
	char	*var;

	i = 1;
	if (input[i] >= '0' && input[i] <= '9')
		var = ft_substr(input, 1, 1);
	else
	{
		while (ft_isalnum(input[i]) || input[i] == '_')
			i++;
		var = ft_substr(input, 1, i - 1);
	}
	(*result) = get_envl_var(var, envl);
	free(var);
	if ((*result) == NULL)
		return (1);
	return (0);
}

int	expand_xcode(char **result, t_sh *shell)
{
	*result = ft_itoa(shell->exit_status);
	if (!(*result))
		return (1);
	return (0);
}

void	init_exp(t_exp *exp)
{
	exp->result = ft_calloc(1, 1);
	exp->buffer = NULL;
	exp->tmp = NULL;
	exp->end = 0;
	exp->start = 0;
	exp->quote = ' ';
}

void	match_quotes(char *input, int *end, char *quote)
{
	if (*quote == '\'' || *quote == '"')
	{
		while (input[(*end)] && input[(*end)] != *quote)
			(*end)++;
		*quote = ' ';
	}
	else
	{
		while (input[(*end)] != '\'' && input[(*end)] != '"' && input[(*end)])
			(*end)++;
		*quote = input[(*end)];
	}
}
