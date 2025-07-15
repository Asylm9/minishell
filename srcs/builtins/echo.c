#include "../../minishell.h"

static void	sigpipe_intercepted(t_sh *shell, t_ast *ast)
{
	if (g_sig == SIGPIPE)
	{
		cleanup_shell(shell, ast);
		g_sig = 0;
	}
}

static int	skip_n_flags(char **args, bool *newline)
{
	int		i;
	int		j;

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

	if (!ast->cmd->args || !ast->cmd->args[0])
		return (BUILTIN_ERR);
	newline = true;
	i = skip_n_flags(ast->cmd->args, &newline);
	while (ast->cmd->args[i])
	{
		printf_fd(STDOUT_FILENO, "%s", ast->cmd->args[i]);
		sigpipe_intercepted(shell, ast);
		if (ast->cmd->args[i + 1])
			printf_fd(STDOUT_FILENO, " ");
		i++;
	}
	if (newline)
		printf_fd(STDOUT_FILENO, "\n");
	return (SUCCESS);
}
