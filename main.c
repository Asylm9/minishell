/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:10:00 by magoosse          #+#    #+#             */
/*   Updated: 2025/07/05 17:00:16 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

void	print_token(t_token *tok_lst)
{
	int	i;

	i = 1;
	while (tok_lst)
	{
		if (tok_lst->value != NULL)
			printf("Token %d type : %d value : |%s|\n", i, tok_lst->type,
				tok_lst->value);
		if (tok_lst->expand == EXPAND && tok_lst->value != NULL)
			printf("Token %d expand : EXPAND\n", i);
		else if (tok_lst->value != NULL)
			printf("Token %d expand : NO_EXPAND\n", i);
		tok_lst = tok_lst->next;
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_token	*tok_lst;
	t_token	*expanded;
	t_ast	*ast;
	t_sh	shell;

	ast = NULL;
	tok_lst = NULL;
	expanded = NULL;
	if (ac > 1)
	{
		fprintf(stderr, "Usage: %s\n", av[0]);
		return (1);
	}
	init_shell(&shell, envp);
	set_main_signals();
	while (1)
	{
		input = readline("\033[0;34m\033[1mMinishell> \033[0m");
		if (is_empty(input))
			add_history(input);
		if (!input)
		{
			free_ast(ast);
			printf("exit\n");
			exit(SUCCESS);
		}
		if (g_sig == SIGINT)
		{
			shell.exit_status = 128 + g_sig;
			g_sig = 0;
		}
		if (check_input(input, &shell) == SUCCESS)
		{
			if (create_token_node(&tok_lst) == ERROR)
				free(input);
			else if (tokenize_input(tok_lst, input) == ERROR
				|| (tok_lst->value == NULL && tok_lst->next == NULL))
			{
				if (tok_lst)
					free_tok_lst(&tok_lst);
				free(input);
			}
			else
			{
				free(input);
				if (create_token_node(&expanded) == ERROR)
				{
					free_tok_lst(&tok_lst);
					tok_lst = NULL;
				}
				else if (expand_list(tok_lst, expanded, &shell) == SUCCESS)
				{
					ast = malloc(sizeof(t_ast));
					if (!ast)
					{
						perror("malloc");
						free(tok_lst);
						tok_lst = NULL;
					}
					else
					{
						ast->cmd = malloc(sizeof(t_command));
						ast->left = NULL;
						ast->right = NULL;
						if (parse_ast(expanded, &ast, &shell) == SUCCESS)
							execute_ast(ast, &shell);
						free_ast(ast);
						// free_tok_lst(tok_lst);
						// free_tok_lst(expanded);
						ast = NULL;
						tok_lst = NULL;
						expanded = NULL;
					}
				}
				else
				{
					free_tok_lst(&tok_lst);
				}
			}
		}
		else
			free(input);
	}
}

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