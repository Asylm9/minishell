#include "../../minishell.h"

int	builtin_pwd(void)
{
	char	buffer[PATH_MAX];

	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("getcwd");
		return (ERROR);
	}
	ft_putendl_fd(buffer, 1);
	return (SUCCESS);
}
