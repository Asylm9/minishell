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

void	handle_sigquit(int sig)
{
	write(1, "exit\n", 5);
	exit(3);
	(void)sig;
}

//set_signals()
