#include "../../minishell.h"

bool	is_numeric(char *arg)
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

int	valid_exit(char *code)
{
	int		i;
	int		sign;
	long	result;
	long	checker;

	i = 0;
	sign = 1;
	result = 0;
	checker = 0;
	while (code[i] == ' ' || (code[i] >= 9 && code[i] <= 13))
		i++;
	if (code[i] == '-' || code[i] == '+')
		if (code[i++] == '-')
			sign = -1;
	while (code[i] > 47 && code[i] < 58)
	{
		result = (result * 10) + (code[i] - '0');
		if (checker > result && ((result < 0 && sign == 1) || (result > 0
					&& sign == -1)))
			return (1);
		checker = result;
		i++;
	}
	return (0);
}

void	exit_arg_error(t_ast *ast)
{
	printf_fd(STDERR,
				"minishell: exit:\
				%s: numeric argument required\n",
				ast->cmd->args[1]);
}

int	builtin_exit(t_ast *ast, t_sh *shell)
{
	printf_fd(1, "exit\n");
	if (ast->cmd->args && ast->cmd->args[1])
	{
		if (!is_numeric(ast->cmd->args[1]))
		{
			exit_arg_error(ast);
			shell->exit_status = BUILTIN_ERR;
		}
		else if (ast->cmd->args[2])
		{
			printf_fd(STDERR, "minishell: exit: too many arguments\n");
			return (ERROR);
		}
		else if (valid_exit(ast->cmd->args[1]) == 0)
			shell->exit_status = ft_atoi(ast->cmd->args[1]);
		else
		{
			exit_arg_error(ast);
			shell->exit_status = 2;
		}
	}
	clean_exit(shell, ast);
	return (SUCCESS);
}
