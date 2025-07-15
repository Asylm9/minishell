/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agaland <agaland@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 20:11:26 by agaland           #+#    #+#             */
/*   Updated: 2025/07/16 01:30:37 by agaland          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1
# define BUILTIN_ERR 2
# define STDERR 2
# define CMD_NOT_FOUND 127
# define EXECVE_ERR 126

# define PATH_MAX 4096
# define EMPTY ""

typedef struct s_redirect		t_redirect;
typedef struct s_command		t_command;
typedef struct s_sh				t_sh;
typedef struct s_token			t_lst;
typedef struct s_env			t_env;
typedef struct s_ast			t_ast;

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
}								t_token_type;

typedef enum e_expand
{
	EXPAND,
	NO_EXPAND
}								t_expand;

typedef struct s_token
{
	char						*value;
	int							hd_fd;
	t_expand					expand;
	t_token_type				type;
	t_lst						*next;
}								t_lst;

typedef struct s_exp
{
	char						*result;
	char						*buffer;
	char						*tmp;
	char						*expanded;
	int							end;
	int							start;
	char						quote;
}								t_exp;

typedef struct s_redirect
{
	t_token_type				type;
	char						*target;
	int							fd;
	t_redirect					*next;
}								t_redirect;

typedef struct s_command
{
	char						*cmd_name;
	char						**args;
	int							argc;
	t_redirect					*redirections;
}								t_command;

typedef struct s_env
{
	char						*key;
	char						*value;
	t_env						*next;
	t_env						*prev;
}								t_env;

typedef struct s_sh
{
	t_env						*envl;
	bool						in_pipeline;
	int							saved_stdin;
	int							saved_stdout;
	int							exit_status;
	t_lst						*tok_lst;
	t_lst						*exp_lst;
}								t_sh;

typedef struct s_ast
{
	t_token_type				type;
	t_command					*cmd;
	t_ast						*left;
	t_ast						*right;
}								t_ast;

extern volatile sig_atomic_t	g_sig;

/**************************		Parsing		*****************************/

void							print_token(t_lst *tok_lst);


/* Initialization */
int								init_shell(t_sh *shell, char **envp);

/* Tokenizer */
int								is_empty(char *input);
char							next_input(char *input);
int								check_quotes(const char *input, int *i,
									int *size, t_sh *shell);
int								check_ampersand(const char *input, int i,
									t_sh *shell);
int								check_special_delim(char delim, int size,
									t_sh *shell);

int								check_input(char *input, t_sh *shell);
int								create_list_node(t_lst **tok_lst);
int								skip_spaces(const char *input, int *start);
int								find_end_of_token(const char *input, int *end);
void							set_token_type(t_lst *tok_lst,
									const char *input, int *start, int *end);
int								set_value(t_lst *tok_lst, const char *input,
									int *start, int *end);
int								tokenize_input(t_lst *tok_lst,
									const char *input);
int								is_env_var(char *str);

/* Expander */
void							init_exp(t_exp *exp);
int								is_pipe_redir(char *str);
size_t							count_nb_words(char const *s, char c);
int								check_validity(t_lst *exp_lst, t_sh *shell);
void							input_error(int error, t_sh *shell,
									t_token_type tok);
int								expand_xcode(char **result, t_sh *shell);
int								expand_var(char *input, char **result,
									t_env *envl);
char							*expand_token(char *input, t_sh *shell);
void							handle_unclosed_pipes(t_lst *tok_lst,
									t_lst *exp_lst, t_sh *shell);
void							match_quotes(char *input, int *end,
									char *quote);
char							*trim_quotes(char *input);
int								is_pipe_redir(char *str);
int								expand_list(t_lst *tok_lst, t_lst *exp_lst,
									t_sh *shell);
int								process_heredoc(t_lst *tok_lst, t_lst *exp_lst,
									t_sh *shell);
void							process_expand(t_lst *tok_lst, t_lst *exp_lst,
									char *expanded);
int								process_lst_node(t_lst *tok_lst, t_lst *exp_lst,
									t_sh *shell, int *advance);
int								process_lst(t_lst *tok_lst, t_lst *exp_lst,
									t_sh *shell, int advance);

/* Parser */
int								create_node_pipe(t_ast **ast);
int								count_args(t_lst *exp_lst);
char							**fill_args(t_lst **exp_lst, int argc,
									char **cmd_name);
t_redirect						*add_redirection(t_redirect *redir,
									t_lst *exp_lst);
t_command						*create_node_cmd(t_lst **exp_lst);
int								parse_ast(t_lst *exp_lst, t_ast **ast,
									t_sh *shell);
void							print_ast(t_ast *ast);

/* Heredoc */
int								handle_heredoc(char *delimiter, t_sh *shell);
int								read_heredoc_content(char *del, t_sh *shell, char **buffer);
char							*process_heredoc_line(char *input, char *del, t_sh *shell);

/**************************		Execution	*****************************/

/* Execution */
int								execute_ast(t_ast *ast, t_sh *shell,
									t_ast *root);
int								execute_command(t_ast *ast, t_sh *shell,
									t_ast *root);
int								execute_pipeline(t_ast *ast, t_sh *shell,
									t_ast *root);
int								process_wait_status(int status);
int								execute_binary(t_ast *ast, t_sh *shell,
									t_ast *root);

/* Exec utils */
pid_t							process_left_child(t_ast *ast, t_sh *shell,
									int *pfd, t_ast *root);
pid_t							process_right_child(t_ast *ast, t_sh *shell,
									int *pfd, t_ast *root);
int								handle_builtin(t_ast *ast, t_sh *shell,
									t_ast *root);
void							handle_binary_pipeline(t_ast *ast, t_sh *shell,
									t_ast *root);
int								fork_single_binary(t_ast *ast, t_sh *shell,
									t_ast *root);

/* Path and environment handling */
char							*get_env_var(char *name, char **env);
int								set_env_var(char *name, char **env, char *path);
char							**get_paths(t_command *cmd, t_env *envl);
char							*find_cmd_path(char **paths, char *cmd_name);

/* Redirections */
int								save_or_restore_fds(t_sh *shell, char flag);
int								redirect_in(t_redirect *redir);
int								redirect_out(t_redirect *redir);
int								apply_redirections(t_command *cmd);

/* Exec builtins */
int								args_count(char **args);
bool							is_builtin(char *cmd_name);
int								execute_builtin(t_ast *ast, t_sh *shell);

/* Builtin implementations */
int								builtin_echo(t_ast *ast, t_sh *shell);
int								builtin_cd(char **args, t_sh *shell);
int								builtin_pwd(char **args);
int								builtin_export(char **args, t_env **envl);
t_env							**init_temp_array(t_env *envl, int count);
void							sort_env_list(t_env **array, int count);
int								count_elements(t_env *envl);
int								builtin_unset(char **args, t_env **envl);
int								builtin_env(t_sh *shell);
int								builtin_exit(t_ast *ast, t_sh *shell);
bool							is_numeric(char *arg);

/* Env utils */
char							**convert_envl_to_env(t_env *envl);
t_env							*init_env_list(char **env);
bool							key_exists(char *key, t_env *envl);
int								add_new_entry(char *key, char *value,
									t_env **envl);
char							*get_envl_var(char *name, t_env *envl);
int								set_envl_var(char *name, t_env **envl,
									char *value);

/* Utils */
int								ft_strcmp(const char *s1, const char *s2);
char							*ft_charjoin(char const *s1, char const *s2,
									char c);

/* List utils */
int								list_size(t_env *envl);
void							print_env_list(t_env *envl);
void							print_exp_list(t_env *envl);
t_env							*create_node(char *key, char *value);
t_env							*find_last_node(t_env *head);
t_env							*add_back_node(t_env *new_node, t_env *head);

/* Resources *//* Free */
void							free_tok_lst(t_lst **list);
void							free_redir(t_redirect *redirection);
void							free_cmd(t_command *cmd);
void							free_ast(t_ast *ast);
void							free_array(char **array, int i);
void							free_envl(t_env **head);
void							cleanup_shell(t_sh *shell, t_ast *ast);
void							fd_clean_exit(t_sh *shell, int *pfd, int exit_code);
void							close_all_fds(int fd);

/* Signals */
void							handle_here_sig(int sig);
void							handle_sigint(int sig);
void							handle_sigint_exec(int sig);
int								set_main_signals(void);
int								set_subprocess_signals(void);

#endif
