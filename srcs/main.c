/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:10:00 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 15:04:41 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

volatile sig_atomic_t	g_sig = 0;

int	init_minishell(t_sh *shell, char **envp)
{
	shell->tok_lst = NULL;
	shell->exp_lst = NULL;
	if (init_shell(shell, envp) != SUCCESS)
	{
		printf_fd(2, "Shell initialization failed\n");
		free_envl(&shell->envl);
		return (ERROR);
	}
	return (SUCCESS);
}

void	get_input(char **input, t_sh *shell, t_ast *ast)
{
	(*input) = readline("\033[0;34m\033[1mMinishell> \033[0m");
	if (is_empty(*input))
		add_history(*input);
	if (!(*input))
	{
		cleanup_shell(shell, ast);
		printf_fd(STDOUT_FILENO, "exit\n");
		exit(SUCCESS);
	}
	if (g_sig == SIGINT)
	{
		shell->exit_status = 128 + g_sig;
		g_sig = 0;
	}
}

void	prepare_next_cmd(t_ast *ast, t_sh *shell, char *input)
{
	signal(SIGINT, handle_sigint);
	free_ast(ast);
	free_tok_lst(&shell->tok_lst);
	free_tok_lst(&shell->exp_lst);
	ast = NULL;
	shell->tok_lst = NULL;
	shell->exp_lst = NULL;
	free(input);
}

int	parse(char *input, t_sh *shell, t_ast **ast)
{
	if (tokenize(input, shell) == ERROR)
		return (cleanup_exit(shell, NULL), ERROR);
	if (create_list_node(&shell->exp_lst) == ERROR)
		return (cleanup_exit(shell, NULL), ERROR);
	if (expand_list(shell->tok_lst, shell->exp_lst, shell) == ERROR)
		return (cleanup_exit(shell, *ast), ERROR);
	if (init_ast(ast, shell) == ERROR)
		return (cleanup_exit(shell, *ast), ERROR);
	if (parse_ast(shell->exp_lst, ast, shell) == ERROR)
		return (cleanup_exit(shell, *ast), ERROR);
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_ast	*ast;
	t_sh	shell;

	input = NULL;
	ast = NULL;
	if (ac > 1)
	{
		fprintf(stderr, "Usage: %s\n", av[0]);
		return (ERROR);
	}
	if (init_minishell(&shell, envp) == ERROR)
		return (ERROR);
	set_main_signals();
	while (1)
	{
		get_input(&input, &shell, ast);
		if (check_input(input, &shell) == SUCCESS)
		{
			if (parse(input, &shell, &ast) == SUCCESS)
				if (execute_ast(ast, &shell, ast) == ERROR)
					cleanup_exit(&shell, ast);
		}
		prepare_next_cmd(ast, &shell, input);
	}
}
