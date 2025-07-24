/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 13:15:04 by agaland           #+#    #+#             */
/*   Updated: 2025/07/24 18:20:02 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	welcome_to_the_void(void)
{
	printf_fd(STDERR, "pwd: error retrieving current directory: ");
	printf_fd(STDERR, "getcwd: cannot access parent directories: ");
	printf_fd(STDERR, "No such file or directory\n");
	printf_fd(STDERR, "\n*** Welcome to the void ***\n\n");
}

int	builtin_pwd(char **args)
{
	char	buffer[PATH_MAX];

	if (args[1] && args[1][0] == '-' && args[1][1])
	{
		if (!(args[1][1] == '-' && args[1][2] == '\0'))
		{
			printf_fd(STDERR, "minishell: pwd: %s: invalid option\n", args[1]);
			return (BUILTIN_ERR);
		}
	}
	if (!getcwd(buffer, sizeof(buffer)))
	{
		welcome_to_the_void();
		return (ERROR);
	}
	printf_fd(STDOUT_FILENO, "%s\n", buffer);
	return (SUCCESS);
}
