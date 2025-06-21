#include "../minishell.h"

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

int	builtin_exit(char **args, t_sh *shell)
{
	int	code;

	printf("exit\n");
	if (!args[1])
		code = shell->exit_status;
	else
	{
		if (!is_numeric(args[1]))
		{
			printf_fd(STDERR,
				"minishell: exit: %s: numeric argument required\n", args[1]);
			code = BUILTIN_ERR;
		}
		else if (args[2])
		{
			printf_fd(STDERR, "minishell: exit: too many arguments\n");
			return (ERROR);
		}
		else
			code = ft_atoi(args[1]);
	}
	exit(code);
	return (SUCCESS);
}
