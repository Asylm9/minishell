#include "../../minishell.h"

static bool	is_numeric(char *arg)
{
	int	i;

	i = 0;
	while (ft_isspace(arg[i]))
		i++;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	if (!arg[i])
		return (false);
	while (arg[i])
	{
		if (!ft_isdigit((arg[i])))
			return (false);
		i++;
	}
	return (true);
}

int	builtin_exit(t_ast *ast, t_sh *shell)
{
	printf_fd(1, "exit\n");
	if (ast->cmd->args && ast->cmd->args[1])
	{
		if (!is_numeric(ast->cmd->args[1]))
		{
			printf_fd(STDERR,
						"minishell: exit:\
				%s: numeric argument required\n",
						ast->cmd->args[1]);
			shell->exit_status = BUILTIN_ERR;
		}
		else if (ast->cmd->args[2])
		{
			printf_fd(STDERR, "minishell: exit: too many arguments\n");
			return (ERROR);
		}
		else
			shell->exit_status = ft_atoi(ast->cmd->args[1]);
	}
	clean_exit(shell, ast);
	return (SUCCESS);
}
