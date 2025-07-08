#include "../../minishell.h"

void	free_array(char **array, int i)
{
	if (!array)
		return ;
	if (i < 0)
	{
		i = 0;
		while (array[i])
			free(array[i++]);
	}
	else
	{
		while (i >= 0)
			free(array[--i]);
	}
	free(array);
}

void	free_envl(t_env **head)
{
	t_env	*tmp;

	if (!head || !*head)
		return ;
	while (*head)
	{
		tmp = (*head)->next;
		if ((*head)->key)
			free((*head)->key);
		if ((*head)->value)
			free((*head)->value);
		free(*head);
		*head = tmp;
	}
	*head = NULL;
}

void	cleanup_shell(t_sh *shell, t_ast *ast)
{
	free_envl(&shell->envl);
	free_tok_lst(&shell->tok_lst);
	free_tok_lst(&shell->exp_lst);
	free_ast(ast);
	rl_clear_history();
}

void	clean_exit(t_sh *shell, t_ast *ast)
{
	cleanup_shell(shell, ast);
	exit(shell->exit_status);
}
