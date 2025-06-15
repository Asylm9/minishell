#include "minishell.h"

int	init_struct_pipe(t_ast *first_node)
{
	t_command	*cmd;
	t_command	*cmd_second;
	t_ast		*cmd_ast;
	t_ast		*cmd_second_ast;

	if (!first_node)
		return (1);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (1);
	cmd->cmd_name = "ls";
	cmd->args = malloc(3 * sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (1);
	}
	cmd->args[0] = "ls";
	cmd->args[1] = "-l";
	cmd->args[2] = NULL;
	cmd->argc = 2; // Set argc to the number of arguments
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd_second = malloc(sizeof(t_command));
	if (!cmd_second)
	{
		free(cmd);
		return (1);
	}
	cmd_second->cmd_name = "wc";
	cmd_second->args = malloc(3 * sizeof(char *));
	if (!cmd_second->args)
	{
		free(cmd);
		free(cmd_second);
		return (1);
	}
	cmd_second->args[0] = "wc";
	cmd_second->args[1] = "-l";
	cmd_second->args[2] = NULL;
	cmd_second->argc = 2; // Set argc to the number of arguments
	cmd_second->redirections = NULL;
	cmd_second->next = NULL;
	cmd_ast = malloc(sizeof(t_ast));
	if (!cmd_ast)
	{
		free(cmd);
		free(cmd_second);
		return (1);
	}
	cmd_ast->type = CMD;
	cmd_ast->cmd = cmd;
	cmd_ast->left = NULL;
	cmd_ast->right = NULL;
	cmd_second_ast = malloc(sizeof(t_ast));
	if (!cmd_second_ast)
	{
		free(cmd);
		free(cmd_second);
		free(cmd_ast);
		return (1);
	}
	cmd_second_ast->type = CMD;
	cmd_second_ast->cmd = cmd_second;
	cmd_second_ast->left = NULL;
	cmd_second_ast->right = NULL;
	first_node->type = PIPE;
	first_node->cmd = NULL;
	first_node->left = cmd_ast;
	first_node->right = cmd_second_ast;
	return (0);
}

int	init_struct_double_pipe(t_ast *first_node)
{
	t_command	*cmd;
	t_command	*cmd_second;
	t_command	*cmd_third;
	t_ast		*cmd_first_ast;
	t_ast		*cmd_second_ast;
	t_ast		*cmd_third_ast;
	t_ast		*second_node;

	if (!first_node)
		return (1);
	// First cmd
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (1);
	cmd->cmd_name = "ls";
	cmd->args = malloc(3 * sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (1);
	}
	cmd->args[0] = "ls";
	cmd->args[1] = "-l";
	cmd->args[2] = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	// Second cmd
	cmd_second = malloc(sizeof(t_command));
	if (!cmd_second)
	{
		free(cmd);
		return (1);
	}
	cmd_second->cmd_name = "grep";
	cmd_second->args = malloc(3 * sizeof(char *));
	if (!cmd_second->args)
	{
		free(cmd);
		free(cmd_second);
		return (1);
	}
	cmd_second->args[0] = "grep";
	cmd_second->args[1] = "Makefile";
	cmd_second->args[2] = NULL;
	cmd_second->redirections = NULL;
	cmd_second->next = NULL;
	// Third cmd
	cmd_third = malloc(sizeof(t_command));
	if (!cmd_third)
	{
		free(cmd);
		free(cmd_second);
		return (1);
	}
	cmd_third->cmd_name = "wc";
	cmd_third->args = malloc(3 * sizeof(char *));
	if (!cmd_third->args)
	{
		free(cmd);
		free(cmd_second);
		free(cmd_third);
		return (1);
	}
	cmd_third->args[0] = "wc";
	cmd_third->args[1] = "-l";
	cmd_third->args[2] = NULL;
	cmd_third->redirections = NULL;
	cmd_third->next = NULL;
	// First cmd ast
	cmd_first_ast = malloc(sizeof(t_ast));
	if (!cmd_first_ast)
	{
		free(cmd);
		free(cmd_second);
		return (1);
	}
	cmd_first_ast->type = CMD;
	cmd_first_ast->cmd = cmd;
	cmd_first_ast->left = NULL;
	cmd_first_ast->right = NULL;
	// Second cmd ast
	cmd_second_ast = malloc(sizeof(t_ast));
	if (!cmd_second_ast)
	{
		free(cmd);
		free(cmd_second);
		free(cmd_first_ast);
		return (1);
	}
	cmd_second_ast->type = CMD;
	cmd_second_ast->cmd = cmd_second;
	cmd_second_ast->left = NULL;
	cmd_second_ast->right = NULL;
	// Third cmd ast
	cmd_third_ast = malloc(sizeof(t_ast));
	if (!cmd_third_ast)
	{
		free(cmd);
		free(cmd_second);
		free(cmd_first_ast);
		free(cmd_second_ast);
		return (1);
	}
	cmd_third_ast->type = CMD;
	cmd_third_ast->cmd = cmd_third;
	cmd_third_ast->left = NULL;
	cmd_third_ast->right = NULL;
	// Second pipe node
	second_node = malloc(sizeof(t_ast));
	if (!second_node)
	{
		free(cmd);
		free(cmd_second);
		free(cmd_first_ast);
		free(cmd_second_ast);
		free(cmd_third_ast);
		return (1);
	}
	second_node->type = PIPE;
	second_node->cmd = NULL;
	second_node->left = cmd_first_ast;
	second_node->right = cmd_second_ast;
	// First pipe node
	first_node->type = PIPE;
	first_node->cmd = NULL;
	first_node->left = second_node;
	first_node->right = cmd_third_ast;
	return (0);
}

int	init_struct_redir_in(t_ast *first_node)
{
	t_command	*cmd;
	FILE		*file;

	if (!first_node)
		return (1);
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (1);
	cmd->cmd_name = "cat";
	cmd->args = malloc(2 * sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (1);
	}
	cmd->args[0] = "cat";
	cmd->args[1] = NULL;
	cmd->redirections = malloc(sizeof(t_redirect));
	if (!cmd->redirections)
	{
		free(cmd->args);
		free(cmd);
		return (1);
	}
	cmd->redirections->type = IN;             // Example type
	cmd->redirections->target = "file.txt"; // Example target
	cmd->redirections->fd = -1;               // Not used for this example
	cmd->redirections->next = NULL;
/* 	if ((file = fopen(cmd->redirections->target, "r")) == NULL)
	{
		perror("fopen");
		free(cmd->redirections);
		free(cmd->args);
		free(cmd);
		return (1);
	}
	cmd->redirections->fd = fileno(file); // Set the file descriptor
	cmd->cmd_name = "cat";
	cmd->args[0] = "cat";
	cmd->args[1] = NULL;
	cmd->redirections->next = NULL; */
	// Initialize the command structure
	cmd->next = NULL;
	first_node->type = CMD; // remplace REDIR_IN
	first_node->cmd = cmd;
	first_node->left = NULL;
	first_node->right = NULL;
	return (0);
}

void	init_shell(t_sh *shell, char **envp)
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

	shell->in_pipeline = false;
	shell->exit_status = 0;

	shell->envl = init_env_list(shell->env);
	
}

void	print_ast(t_ast *ast)
{
	printf("Type:\n");
	if (!ast)
		return ;
	if (ast->type == CMD && ast->cmd)
	{
		if (ast->cmd->cmd_name == NULL)
			printf("	Command:\n		Name: NULL\n");
		else
			printf("	Command:\n		Name: %s\n", ast->cmd->cmd_name);
		if (ast->cmd->args)
		{
			printf("		Arguments count: %d\n", ast->cmd->argc);
			printf("		Arguments:\n");
			for (int i = 0; ast->cmd->args[i]; i++)
			{
				printf("			%s\n", ast->cmd->args[i]);
			}
		}
		else
			printf("No arguments\n");
	}
	else if (ast->type == PIPE)
		printf("	Pipe\n");
	else if (ast->type == REDIR_IN)
	{
		printf("	Redirect In\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("        Command:\n");
			if (ast->cmd->cmd_name == NULL)
				printf("		Name: NULL\n");
			else
				printf("		Name: %s\n", ast->cmd->cmd_name);
			if (ast->cmd->args)
			{
				printf("		Arguments count: %d\n", ast->cmd->argc);
				printf("		Arguments:\n");
				for (int i = 0; ast->cmd->args[i]; i++)
				{
					printf("			%s\n", ast->cmd->args[i]);
				}
			}
			else
				printf("No arguments\n");
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_OUT)
	{
		printf("	Redirect Out\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_APPEND)
	{
		printf("	Redirect Append\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else if (ast->type == REDIR_HEREDOC)
	{
		printf("	Redirect Heredoc\n");
		if (ast->cmd && ast->cmd->redirections)
		{
			printf("		Redirection Target: %s\n",
						ast->cmd->redirections->target);
			printf("		File Descriptor: %d\n", ast->cmd->redirections->fd);
		}
		else
			printf("No redirection target or file descriptor\n");
	}
	else
		printf("Unknown type\n");
	if (ast->left)
	{
		printf("Left child:\n");
		printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
		print_ast(ast->left);
		printf("LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
	}
	// else
	// 	printf("No left child\n");
	if (ast->right)
	{
		printf("Right child:\n");
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
		print_ast(ast->right);
		printf("RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n");
	}
	// else
	// 	printf("No right child\n");
}

int	main(int ac, char **av, char **envp)
{
	t_ast	*ast;
	t_ast	*ast_double_pipe;
	t_ast	*ast_redir;
	t_sh	shell;
	(void)ac;
	(void)av;

	ast = malloc(sizeof(t_ast));
	if (!ast)
		return (1);
	ast_double_pipe = malloc(sizeof(t_ast));
	if (!ast_double_pipe)
	{
		free(ast);
		return (1);
	}
	ast_redir = malloc(sizeof(t_ast));
	if (!ast_redir)
	{
		free(ast);
		free(ast_double_pipe);
		return (1);
	}

	if (init_struct_pipe(ast) != 0)
	{
		free(ast);
		return (1);
	}
	printf("---------------------------------------------------------------------------------------------------------------------\n");
	printf("First pipe AST:\n");
	print_ast(ast);

	init_shell(&shell, envp);

	printf("-------EXECUTE AST 1-------------------------------------------------------------------------------------------------\n");
	execute_ast(ast, &shell);
	printf("---------------------------------------------------------------------------------------------------------------------\n");


	if (init_struct_double_pipe(ast_double_pipe) != 0)
	{
		free(ast);
		free(ast_double_pipe);
		return (1);
	}
	printf("---------------------------------------------------------------------------------------------------------------------\n");
	printf("Double Pipe AST:\n");
	print_ast(ast_double_pipe);

	printf("-------EXECUTE AST 2-------------------------------------------------------------------------------------------------\n");
	execute_ast(ast_double_pipe, &shell);
	printf("---------------------------------------------------------------------------------------------------------------------\n");

	if (init_struct_redir_in(ast_redir) != 0)
	{
		free(ast);
		free(ast_double_pipe);
		free(ast_redir);
		return (1);
	}
	printf("---------------------------------------------------------------------------------------------------------------------\n");
	printf("Redirection AST:\n");
	print_ast(ast_redir);

		printf("-------EXECUTE AST 3-------------------------------------------------------------------------------------------------\n");
	shell.exit_status = execute_ast(ast_redir, &shell);
	printf("---------------------------------------------------------------------------------------------------------------------\n");

	// Free allocated memory
	if (ast->left && ast->left->cmd)
	{
		free(ast->left->cmd);
	}
	if (ast->right && ast->right->cmd)
	{
		free(ast->right->cmd);
	}
	free(ast->left);
	free(ast->right);
	free(ast);

	return (shell.exit_status);
}
