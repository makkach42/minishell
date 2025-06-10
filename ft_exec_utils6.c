# include <minishell.h>

void	ft_is_dir(char *s)
{
	struct stat buf;

	stat(s, &buf);
	if (S_ISDIR(buf.st_mode) != 0)
	{
		ft_putstr_fd(2, "minishell: ");
		ft_putstr_fd(2, s);
		ft_putstr_fd(2, ": is a directory\n");
		exit (126);
	}
}

void	ft_new_handler(int sig)
{
	(void)sig;
	printf("\n");
}

void	ft_new_handler_pip(int sig)
{
	(void)sig;
	// printf("\n");
	hide_terminal_control_chars();
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	display_terminal_control_chars(void)
{
	struct termios	terminos_p;

	tcgetattr(0, &terminos_p);
	terminos_p.c_lflag |= ECHOCTL;
	tcsetattr(0, TCSANOW, &terminos_p);
}

void	ft_word_handle(t_tree *tree, t_env **h, char **e, int *check)
{
	int id;

	display_terminal_control_chars();
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ft_exec(tree, *h, e);
	}
	else if (id < 0)
	{
		perror("fork");
		tree->status = 1;
		return ;
	}
	if (*check == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	if (waitpid(id, &(tree->status), 0) == -1)
		tree->status = 1;
	else if (WIFEXITED(tree->status))
	{
		tree->status = WEXITSTATUS(tree->status);
	}
	else if (WIFSIGNALED(tree->status))
	{
		tree->status = WTERMSIG(tree->status);
		if (tree->status == 2)
			global_status = SIGINT;
		else if (tree->status == 3)
			global_status = SIGQUIT;
		tree->status += 128;
	}
	if (*check)
		exit (tree->status);
}