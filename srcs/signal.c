#include "../minishell.h"

void	reset_readline(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint(int sig)
{
	g_sig = sig;
	reset_readline();
}

void	handle_sigpipe(int sig)
{
	g_sig = sig;
}

int	set_main_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, handle_sigpipe);
	return (0);
}

