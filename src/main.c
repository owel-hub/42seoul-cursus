/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyospark <hyospark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 20:30:10 by hyospark          #+#    #+#             */
/*   Updated: 2021/11/26 03:12:43 by hyospark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **start_sh(char **env, char *input)
{
	t_bundle	*bundles;
	int			i;
	int			result;

	if (is_space_str(input)) // check space input
		return (NULL);
	bundles = split_bundle(env, input); //별개의 명령어별 bundle 생성 및 우선순위 setting
	if (parsing_token(bundles) == FAIL) // bundle 별 cmd 연결리스트 parsing
	{
		free_bundle(bundles);
		return (NULL);
	}
	i = 0;
	while (bundles[i].cmd_line) // 우선순위체크 및 cmd 실행
	{
		result = execute_cmd(&bundles[i]);
		// if (result < 0)
		// 	child_exit(bundles, 1);
		i++;
		if (bundles[i].cmd_line != NULL && 
		(result == SUCCESS && bundles[i].priority == P_OR) \
		|| (result == FAIL && bundles[i].priority == P_AND))
		{
			i++;
		}
	}
	return (bundles->env);
}

char	**dup_envp(char **envp)
{
	char	**env;
	int		len;
	int		i;

	len = 0;
	while (envp[len])
		len++;
	env = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

void sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("%c[K\n", 27);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		printf("zsh: quit           %s", "a.out");
		rl_replace_line("               ", 0);
		rl_redisplay();
		printf("%c\n", 27);
		exit(0);
	}
}

void	loop(char **env, char **av)
{
	char	*input;
	char	**dup_env;
	// signal(SIGINT, sig_handler);
	// signal(SIGQUIT, sig_handler);

	dup_env = dup_envp(env);
	while(TRUE)
	{
		input = readline("minishell$ ");
		add_history(input);
		dup_env = start_sh(env, input);
		free(input);
		if (env == NULL)
			env = dup_env;
	}
	free(env);
}

int main(int argc, char **av, char **envp)
{
	char **dup_env;
	char **dup_av;

	dup_env = dup_envp(envp);
	dup_av = dup_envp(av);
	loop(dup_env, dup_av);
	return 0;
}
