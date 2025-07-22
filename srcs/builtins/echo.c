/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:14:30 by agaland           #+#    #+#             */
/*   Updated: 2025/07/22 20:11:07 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	skip_n_flags(char **args, bool *newline)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && (ft_strncmp(args[i], "-n", 2)) == 0)
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ;
		*newline = false;
		i++;
	}
	return (i);
}

int	builtin_echo(t_ast *ast, t_sh *shell)
{
	int		i;
	bool	newline;

	(void)shell;
	newline = true;
	i = skip_n_flags(ast->cmd->args, &newline);
	while (ast->cmd->args[i])
	{
		printf_fd(STDOUT_FILENO, "%s", ast->cmd->args[i]);
		if (ast->cmd->args[i + 1])
			printf_fd(STDOUT_FILENO, " ");
		i++;
	}
	if (newline)
		printf_fd(STDOUT_FILENO, "\n");
	return (SUCCESS);
}
