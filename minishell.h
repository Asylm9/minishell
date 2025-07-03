/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 20:11:26 by agaland           #+#    #+#             */
/*   Updated: 2025/07/03 20:19:43 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//-lreadline
//-lhistory
# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1
# define BUILTIN_ERR 2
# define CMD_NOT_FOUND 127
# define EXECVE_ERR 126 // commande trouvee mais pas executable

# define STDERR 2

/* #ifndef PATH_MAX */
# define PATH_MAX 4096 // most common value on Linux systems
# define EMPTY ""

typedef struct s_redirect	t_redirect;
typedef struct s_command	t_command;
typedef struct s_sh			t_sh;
typedef struct s_token		t_token;
typedef struct s_env		t_env;
typedef struct s_ast		t_ast;

typedef enum e_token_type
{
	NOT_SET,
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	CMD
}							t_token_type;

typedef enum e_expand
{
	EXPAND,
	NO_EXPAND
}							t_expand;

typedef struct s_token
{
	char					*value;
	int						hd_fd;
	t_expand				expand;
	t_token_type			type;
	t_token					*next;
}							t_token;

typedef struct s_redirect
{
	t_token_type			type;
	char					*target;
	int						fd;
	t_redirect				*next;
}							t_redirect;

typedef struct s_command
{
	char					*cmd_name;
	char					**args;
	int						argc;
	t_redirect				*redirections;
}							t_command;

typedef struct s_env
{
	char					*key;
	char					*value;
	int						index;
	t_env					*next;
	t_env					*prev;
}							t_env;

typedef struct s_sh
{
	char					**env;
	t_env					*envl;
	bool					in_pipeline;
	char					*current_dir;
	int						saved_stdin;
	int						saved_stdout;
	int						exit_status;
}							t_sh;

typedef struct s_ast
{
	t_token_type			type;
	t_command				*cmd;
	t_ast					*left;
	t_ast					*right;
}							t_ast;

/**************************		Parsing		*****************************/

void						print_token(t_token *tok_lst);

/* Tokenizer */

int							check_input(char *input, t_sh *shell);
int							create_token_list(t_token **tok_lst);
int							create_token_node(t_token **tok_lst);
int							skip_spaces(const char *input, int *start);
int							find_end_of_token(const char *input, int *start,
								int *end);
void						set_token_type(t_token *tok_lst, const char *input,
								int *start, int *end);
int							set_value(t_token *tok_lst, const char *input,
								int *start, int *end);
int							tokenize_input(t_token *tok_lst, const char *input);
int							is_env_var(char *str);

/* Expander */
int							expand_xcode(char **result, t_sh *shell);
int							expand_var(char *input, char **result, t_env *envl);
char						*expand_token(char *input, t_sh *shell);
char						*trim_quotes(char *input);
int							is_pipe_redir(char *str);
int							expand_list(t_token *tok_lst, t_token *exp_lst,
								t_sh *shell);

/* Parser */
int							create_node_pipe(t_ast **ast);
t_command					*create_node_cmd(t_token **exp_lst);
int							parse_ast(t_token *tok_lst, t_ast **ast);
void						print_ast(t_ast *ast);

/**************************		Execution	*****************************/

int							handle_heredoc(char *delimiter, t_sh *shell);

/* Execution */
int							execute_ast(t_ast *ast, t_sh *shell);
int							execute_command(t_command *cmd, t_sh *shell);
int							execute_pipeline(t_ast *ast, t_sh *shell);
int							process_wait_status(int status);
int							execute_binary(t_command *cmd, t_sh *shell);

/* Exec utils */
pid_t						process_left_child(t_ast *ast, t_sh *shell,
								int *pfd);
pid_t						process_right_child(t_ast *ast, t_sh *shell,
								int *pfd);
int							handle_builtin(t_command *cmd, t_sh *shell);
void						handle_binary_pipeline(t_command *cmd, t_sh *shell);
int							fork_single_binary(t_command *cmd, t_sh *shell);

/* Path and environment handling */
char						*get_env_var(char *name, char **env);
int							set_env_var(char *name, char **env, char *path);
char						**get_paths(t_command *cmd, t_env *envl);
char						*find_cmd_path(char **paths, char *cmd_name);

/* Redirections */
int							save_or_restore_fds(t_sh *shell, char flag);
int							redirect_in(t_redirect *redir);
int							redirect_out(t_redirect *redir);
int							apply_redirections(t_command *cmd);

/* Builtin commands */
int							args_count(char **args);
bool						is_builtin(char *cmd_name);
int							execute_builtin(t_command *cmd, t_sh *shell);

/* Builtin implementations */
int							builtin_echo(char **args);

int							builtin_cd(char **args, t_sh *shell);

int							builtin_pwd(void);

int							builtin_export(char **args, t_env **envl);
t_env						**init_temp_array(t_env *envl, int count);
void						sort_env_list(t_env **array, int count);
int							count_elements(t_env *envl);

int							builtin_unset(char **args, t_env **envl);

int							builtin_env(t_sh *shell);

int							builtin_exit(char **args, t_sh *shell);

/* Env utils */
char						**convert_envl_to_env(t_env *envl);
t_env						*init_env_list(char **env);
int							add_new_entry(char *key, char *value, t_env **envl);
char						*get_envl_var(char *name, t_env *envl);
int							set_envl_var(char *name, t_env **envl, char *value);
/* char	*get_env_var(char *name, char **env);
int	set_env_var(char *name, char **env, char *value); */

/* Utils */
int							ft_strcmp(const char *s1, const char *s2);
char						*ft_charjoin(char const *s1, char const *s2,
								char c);
// void	error_message(const char *msg);

/* List utils */
int							list_size(t_env *envl);
void						print_env_list(t_env *envl);
void						print_exp_list(t_env *envl);
t_env						*create_node(char *key, char *value);
t_env						*find_last_node(t_env *head);
t_env						*add_back_node(t_env *new_node, t_env *head);

/* Resources */
// void						free_pipes(int **pipes, int i);
/* Free */
void						free_tok_lst(t_token **list);
void						free_redir(t_redirect *redirection);
void						free_cmd(t_command *cmd);
void						free_ast(t_ast *ast);
void						free_array(char **array, int i);
void						free_envl(t_env **head);
void						cleanup_shell(t_sh *shell);

/* Testing */
void						print_env_array(char **env);
void						init_redir(t_redirect *redir);
void						init_cmd_struct(t_command *cmd, char **av,
								t_redirect *redir);
int							init_minimal_shell(t_sh *shell);
void						init_shell_struct(t_sh *shell, char **envp);
void	handle_here_sig(int sig);
void	handle_sigint(int sig);
extern volatile sig_atomic_t g_sig;
#endif
