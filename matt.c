/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:10:00 by magoosse          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/06/18 16:21:08 by magoosse         ###   ########.fr       */
=======
/*   Updated: 2025/06/18 15:55:32 by matthieu         ###   ########.fr       */
>>>>>>> 524e214f43c5203e32fa7d89b1dac41a78614e05
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_env_var(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

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

void	free_tok_lst(t_token *list)
{
	t_token	*temp;

	while (list->next)
	{
		temp = list;
		list = list->next;
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_token	*tok_lst;
	t_token	*temp;
	t_ast	*ast;
	t_sh	*shell;

	tok_lst = NULL;
	temp = NULL;
	if (ac > 1)
	{
		fprintf(stderr, "Usage: %s\n", av[0]);
		return (1);
	}
	while (1)
	{
		input = readline("\033[0;34m\033[1m   Minishell> \033[0m");
		add_history(input);
		printf("Input: %s\n", input);
		if (create_token_node(&tok_lst))
		{
			free(input);
			return (1);
		}
		printf("TOKENISATION\n");
		if (tokenize_input(tok_lst, input))
		{
			free(input);
			return (1);
		}
		printf("TOKENISATION SUCCESSFULL\n");
		free(input);
		print_token(tok_lst);
		if (create_token_node(&temp))
		{
			free_tok_lst(tok_lst);
			tok_lst = NULL;
			return (1);
		}
		printf("EXPANDING LIST\n");
		expand_list(tok_lst, envp, temp, shell);
		printf("LIST EXPANDED\n");
		print_token(temp);
		ast = malloc(sizeof(t_ast));
		if (!ast)
		{
			perror("malloc");
			free(tok_lst);
			tok_lst = NULL;
			return (1);
		}
		ast->cmd = NULL;
		ast->left = NULL;
		ast->right = NULL;
		ast->type = EMPTY;
		// parse_ast(temp, ast);
		// printf("AST:\n");
		// print_ast(ast);
		free_tok_lst(tok_lst);
		tok_lst = NULL;
		free_tok_lst(temp);
		temp = NULL;
	}
}

// execve("/bin/cat", args, NULL);

// export plusieurs variables d'environnement d'un coup
// pour les binaires 1 string par flag

/*
	Parse()
		Readline()
		Tokenize()
			Create_tok_lst()
				Init_token()			CHECK EXPAND DANS SET TOKEN TYPE
				Add_token()
			Fill_token()
		Expand_tok_lst()
			Expand_token()				CHANGER EN INT POUR REMONTER ERREUR ---- > CHECK ZONE TEST
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