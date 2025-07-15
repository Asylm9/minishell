#include "../minishell.h"

void	handle_sigint_exec(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	handle_here_sig(int sig)
{
	g_sig = sig;
	rl_replace_line("", 0);
	rl_on_new_line();
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

int	set_subprocess_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	return (0);
}
