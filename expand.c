#include <minishell.h>

char	*expand_exit_status(t_sh *shell, t_token token)
{
	char *exp_var;

	if (token->type == TOKEN_EXIT_STATUS)
	{
		exp_var = ft_itoa(shell->exit_status);
		return (exp_var);
	}
}
