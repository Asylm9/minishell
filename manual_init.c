#include "minishell.h"

/* void init_redir(t_redirect *redir)
{
	redir->type = IN;
	redir->target = "/home/agaland/19Cursus/rank3/test_shell/file.txt";
	redir->fd = -1;
} */

void	init_cmd_struct(t_command *cmd, char **av, t_redirect *redir)
{
	cmd->cmd_name = av[0];
	cmd->args = av;
	cmd->redirections = redir;
}

void	init_shell_struct(t_sh *shell, char **envp)
{
	int	count;
	int	i;

	count = 0;
	while(envp[count])
		count++;
	shell->env = malloc(sizeof(char *) * (count + 1));
	if (!shell->env)
		return;
	i = 0;
	while (envp[i])
	{
		shell->env[i] = ft_strdup(envp[i]);
		i++;
	}
	shell->env[i] = NULL;
	shell->exit_status = 0;

	shell->envl = init_env_list(shell->env);
}
