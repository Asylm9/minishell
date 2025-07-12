#include "../../minishell.h"

int	builtin_echo(t_ast *ast, t_sh *shell)
{
	int		i;
	int		j;
	bool	newline;

	if (!ast->cmd->args || !ast->cmd->args[0])
		return (BUILTIN_ERR);
	newline = true;
	i = 1;
	while (ast->cmd->args[i] && (ft_strncmp(ast->cmd->args[i], "-n", 2)) == 0)
	{
		j = 2;
		while (ast->cmd->args[i][j] == 'n')
			j++;
		if (ast->cmd->args[i][j] != '\0')
			break ;
		newline = false;
		i++;
	}
	while (ast->cmd->args[i])
	{
		printf("%s", ast->cmd->args[i]);
		if (g_sig == SIGPIPE)
		{
			cleanup_shell(shell, ast);
			g_sig = 0;
		}
		if (ast->cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}
