/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:10:00 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/16 02:42:28 by magoosse         ###   ########.fr       */
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
// int	init_ast(t_ast *ast, t_sh )
// {
// 	ast = malloc(sizeof(t_ast));
// 	if (!ast)
// 	{
// 		free_tok_lst(&shell.tok_lst);
// 		free_tok_lst(&shell.exp_lst);
// 		shell.tok_lst = NULL;
// 		shell.exp_lst = NULL;
// 	}
// 	else
// 	{
// 		ast->cmd = NULL;
// 		ast->left = NULL;
// 		ast->right = NULL;
// 	}
// }

int	tokenize(char *input, t_sh *shell)
{
	if (create_list_node(&shell->tok_lst) == ERROR)
	{
		free(input);
		return (ERROR);
	}
	else if (tokenize_input(shell->tok_lst, input) == ERROR)
	{
		free_tok_lst(&shell->tok_lst);
		free(input);
		return(ERROR);
	}
	return (SUCCESS);
}
int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_ast	*ast;
	t_sh	shell;

	input = NULL;
	if (ac > 1)
	{
		fprintf(stderr, "Usage: %s\n", av[0]);
		return (1);
	}
	if (init_minishell(&shell, envp) == ERROR)
		return (ERROR);
	set_main_signals();
	while (1)
	{
		get_input(&input, &shell, ast);
		if (check_input(input, &shell) == SUCCESS)
		{
			if (tokenize(input, &shell) == ERROR)
				return (ERROR);
			if (create_list_node(&shell.exp_lst) == ERROR)
			{
				free_tok_lst(&shell.tok_lst);
				return (ERROR);
			}
			if (expand_list(shell.tok_lst, shell.exp_lst,
					&shell) == SUCCESS)
			{
				ast = malloc(sizeof(t_ast));
				if (!ast)
				{
					free_tok_lst(&shell.tok_lst);
					free_tok_lst(&shell.exp_lst);
					shell.tok_lst = NULL;
					shell.exp_lst = NULL;
				}
				else
				{
					ast->cmd = NULL;
					ast->left = NULL;
					ast->right = NULL;
					if (parse_ast(shell.exp_lst, &ast, &shell) == SUCCESS)
						execute_ast(ast, &shell, ast);
					signal(SIGINT, handle_sigint);
					free_ast(ast);
					free_tok_lst(&shell.tok_lst);
					free_tok_lst(&shell.exp_lst);
					ast = NULL;
					shell.tok_lst = NULL;
					shell.exp_lst = NULL;
				}
			}
			else
			{
				free_tok_lst(&shell.tok_lst);
				free_tok_lst(&shell.exp_lst);
			}
		}
		free(input);
	}
	
}


// void	print_token(t_lst *tok_lst)
// {
// 	int	i;

// 	i = 1;
// 	while (tok_lst)
// 	{
// 		if (tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d type : %d value : |%s|\n", i, tok_lst->type,
// 				tok_lst->value);
// 		if (tok_lst->expand == EXPAND && tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d expand : EXPAND\n", i);
// 		else if (tok_lst->value != NULL)
// 			printf_fd(STDOUT_FILENO, "Token %d expand : NO_EXPAND\n", i);
// 		tok_lst = tok_lst->next;
// 		i++;
// 	}
// }

// CHECK SET TOKEN VALUE FOR QUOTES BUGGED

// execve("/bin/cat", args, NULL);

// export plusieurs variables d'environnement d'un coup
// pour les binaires 1 string par flag

/*
	Parse()
		Readline()
		Tokenize()
			Create_tok_lst()
				Init_token()
				Add_token()
			Fill_token()
		Expand_tok_lst()
			Expand_token()
				Expand_var()
		Create_AST()
			Count_pipes()
			Create_pipes_nodes()
				Init_pipe_node()
			Create_cmd_nodes()
				Init_cmd_node()
				Fill_cmd()

	EXPAND CASES :
		- $VAR
		- ${VAR}
		- $?
		- $$
		- $VAR1$VAR2
		- $VAR1$VAR2$VAR3
		- $VAR1$VAR2$VAR3$
		- $VAR1$VAR2$VAR3$$
		- $VAR1$VAR2$VAR3${VAR4}
		- $'string'
		- $"string"
*/