#include "../minishell.h"

int	builtin_env(t_sh *shell)
{
	if (!shell || !shell->envl) // Error: no environment variables found
		return (BUILTIN_ERR);
	print_env_list(shell->envl);
	return (SUCCESS);
}
