#include "minishell.h"

void	reset_readline(void)
{
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_here_sig(int sig)
{
	g_sig = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

void	handle_sigint(int sig)
{
	g_sig = sig;
	reset_readline();
}

void	handle_sigint_exec(int sig)
{
	(void)sig;
	write(1, "\n", 1);
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

int	set_subprocess_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (0);
}
