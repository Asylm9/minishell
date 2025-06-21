/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: magoosse <magoosse@student.42.be>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 16:10:00 by magoosse          #+#    #+#             */
/*   Updated: 2025/06/21 20:41:24 by magoosse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_input(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			if (input[i + 1] == '|')
				return (ERROR);
		if (input[i] == '<')
		{
			if (input[i + 1] == '|' || input[i + 1] == '>')
				return (ERROR);
		}
		if (input[i] == '>')
		{
			if (input[i + 1] == '|' || input[i + 1] == '<')
				return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

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
	t_token	*expanded;
	t_ast	*ast;
	t_sh	shell;

	init_shell_struct(&shell, envp);
	tok_lst = NULL;
	expanded = NULL;
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
		if (check_input(input) == ERROR)
			return (1);
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
		if (create_token_node(&expanded))
		{
			free_tok_lst(tok_lst);
			tok_lst = NULL;
			return (1);
		}
		printf("EXPANDING LIST\n");
		expand_list(tok_lst, expanded, &shell);
		printf("LIST EXPANDED\n");
		print_token(expanded);
		ast = malloc(sizeof(t_ast));
		if (!ast)
		{
			perror("malloc");
			free(tok_lst);
			tok_lst = NULL;
			return (1);
		}
		ast->cmd = malloc(sizeof(t_command));
		ast->left = NULL;
		ast->right = NULL;
		// ast->type = EMPTY;
		printf("TEST BF PARSE\n");
		parse_ast(expanded, &ast);
		printf("AST:\n");
		print_ast(ast);
		execute_ast(ast, &shell);
		free_tok_lst(tok_lst);
		tok_lst = NULL;
		free_tok_lst(expanded);
		expanded = NULL;
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
				Fill_cmd()				CREER FONCTION DE CREATION DE NODE CMD POUR EVITER RELINK ET ECRASER L'ANCIENNE

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