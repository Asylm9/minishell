#ifndef EXEC_H
# define EXEC_H

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
# include <unistd.h>

# define SUCCESS 0
# define ERROR 1
# define BUILTIN_ERR 2

# define CMD_NOT_FOUND 127
# define EXECVE_ERR 126

/* #ifndef PATH_MAX */
# define PATH_MAX 4096 // valeur courante sur la plupart des systèmes Linux

typedef struct s_redirect	t_redirect;
typedef struct s_command	t_command;
typedef struct s_sh			t_sh;
typedef struct s_token		t_token;
typedef struct s_ast		t_ast;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	COMMAND,
	EMPTY
}							t_token_type;

typedef enum e_redir_type
{
	IN,     // <
	OUT,    // >
	APPEND, // >>
	HEREDOC // <<
}							t_redir_type;

typedef enum e_expand
{
	EXPAND,
	NO_EXPAND
}							t_expand;

typedef struct s_token
{
	char					*value;
	t_expand				expand;
	t_token_type			type;
	t_token					*next;
}							t_token;

typedef struct s_ast
{
	t_token_type			type;
	// char				**cmd;
	t_command				*cmd;
	t_ast					*left;
	t_ast					*right;
}							t_ast;

typedef struct s_redirect
{
	t_redir_type			type;
	char *target; // fichier ou delimiteur si heredoc
	int fd;       // pour heredoc
	t_redirect				*next;
}							t_redirect;

typedef struct s_command
{
	char					*cmd_name;
	char					**args;
	int						argc;
	t_redirect				*redirections;
	t_command				*next;
}							t_command;

typedef struct s_sh
{
	char					**env;
	//t_command				*cmd_list;
	// t_ast 				*ast; // AST pour l'exécution
	// int					nb_pipes; // nombre de pipes
	char					*current_dir;
	int						exit_status;
}							t_sh;

typedef struct s_exec
{
	//int						**pipes;
	//int						nb_pipes;
	pid_t					*pids;
	t_command				*current;
	int						i;
	int						status;
}							t_exec;

/**************************		Parsing		*****************************/

int							is_env_var(char *str);
void						print_token(t_token *tok_lst);
void						free_tok_lst(t_token *list);
// Tokenizer

int							create_token_list(t_token **tok_lst);
int							create_token_node(t_token **tok_lst);
int							skip_spaces(const char *input, int *start);
int							find_end_of_token(const char *input, int *start,
								int *end);
void						set_token_type(t_token *tok_lst, const char *input,
								int *end);
int							set_value(t_token *tok_lst, const char *input,
								int *start, int *end);
int							tokenize_input(t_token *tok_lst, const char *input);
int							is_env_var(char *str);

// Expander

int							expand_var(char *input, char **result, t_sh *shell);
char						*expand_token(char *input, t_sh *shell);
char						*trim_quotes(char *input);
int							is_pipe_redir(char *str);
int							expand_list(t_token *tok_lst, char **env,
								t_token *exp_lst, t_sh *shell);

// Parser
int							create_node_pipe(t_ast **ast);
int							create_node_cmd(t_token **exp_lst, t_command **cmd);
int							parse_ast(t_token *tok_lst, t_ast **ast);
void						print_ast(t_ast *ast);

/**************************		Execution	*****************************/

int							execute(t_command *cmd_list, t_sh *shell);
// transformer en execute_ast probablement :v
int							execute_command(t_command *cmd, t_sh *shell);
int							execute_pipeline(t_command *cmds, t_sh *shell);
int							execute_binary(t_command *cmd, char **env);

/* Path and environment handling */
char						*get_env_var(char *name, char **env);
char						**get_paths(char **env);
char						*find_cmd_path(char **paths, char *cmd_name);

/* Redirections */
int							redirect_in(int fd, t_redirect *redir);
int							apply_redirections(t_command *cmd);
int							setup_pipes_redirections(int **pipes, int nb_pipes,
								int i);
int	setup_heredoc(t_redirect *redir); // parsing ou exec?

/* Pipe handling */
void						get_pipe_count(t_command *cmd_list, int *nb_pipes);
void						close_pipes(int **pipes, int nb_pipes);
int							init_pipes(int **pipes, int nb_pipes);
int							**create_pipes(int nb_pipes);

/* Builtin commands */
bool						is_builtin(char *cmd_name);
int							execute_builtin(t_command *cmd, t_sh *shell);

/* Builtin implementations */
int							builtin_echo(char **args, t_sh *shell);
int							builtin_cd(char **args, t_sh *shell);
int							builtin_pwd(t_sh *shell);
int							builtin_export(char **args, t_sh *shell);
int							builtin_unset(char **args, t_sh *shell);
int							builtin_env(t_sh *shell);
int							builtin_exit(char **args, t_sh *shell);

/* Utils */
void						error_message(const char *msg);
int							ft_strcmp(const char *s1, const char *s2);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
char						*ft_pathjoin(char const *s1, char const *s2);
// char	*ft_strdup(const char *s);
// char	**ft_split(const char *s, char c);
// char	*ft_fstrjoin(const char *s1, const char *s2);
// size_t	ft_strlen(const char *s);
// void	ft_putendl_fd(const char *s, int fd);

/* Resources */
void						free_pipes(int **pipes, int i);
void						free_array(char **array, int i);

/* Testing */
void						init_redir(t_redirect *redir);
void						init_cmd_struct(t_command *cmd, char **av,
								t_redirect *redir);
void						init_shell_struct(t_sh *shell, char **envp);

#endif