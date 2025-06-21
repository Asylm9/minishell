# include "../../minishell.h"

int	builtin_pwd(void)
{
	char	buffer[PATH_MAX]; //normalement present dans limits.h
	if (!getcwd(buffer, sizeof(buffer)))
	{
		perror("getcwd");
		return (BUILTIN_ERR);
	}
	ft_putendl_fd(buffer, 1);
	return (SUCCESS);
}
