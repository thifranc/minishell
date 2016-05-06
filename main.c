/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thifranc <thifranc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/06 14:22:14 by thifranc          #+#    #+#             */
/*   Updated: 2016/05/06 17:48:05 by thifranc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int		wk_dir(void)
{
	char	*cur;
	int		i;

	cur = malloc(PATH_MAX);
	cur = getcwd(cur, PATH_MAX);
	i = 0;
	while (cur[i])
		i++;
	while (cur[i] != '/')
		i--;
	ft_putstr(ft_ptrf("%s $> ", cur + i + 1));
	ft_memdel((void**)&cur);
	return (1);
}

void	get_home(char **tab)
{
	int		i;
	char	*tmp;

	if (!tab[0])
	{
		ft_putstr("Your command must contain alphanumeric characters.\n");
		return ;
	}
	i = 1;
	while (tab[i])
	{
		if (tab[i][0] == '~')
		{
			tmp = ft_strdup(tab[i]);
			free(tab[i]);
			tab[i] = ft_ptrf("%s/%s", ft_getenv("HOME", 0), tmp + 2);
			free(tmp);
		}
		i++;
	}
}

void	do_cmd(void)
{
	char	**tmp;
	char	*epur;
	int		i;

	i = 0;
	while (g_cmd_list)
	{
		epur = g_cmd_list->name;
		while (epur[i])
		{
			if (epur[i] == '"' || epur[i] == '\t')
				epur[i] = ' ';
			i++;
		}
		tmp = ft_strsplit((char*)(g_cmd_list)->name, ' ');
		get_home(tmp);
		route_me(tmp);
		g_cmd_list = (g_cmd_list)->next;
		ft_deltab((void**)tmp);
	}
}

t_list	*get_cmd(char **tmp)
{
	int		i;
	t_list	*cmd_list;

	i = 0;
	cmd_list = NULL;
	while (tmp[i])
	{
		new_in_list(tmp[i], &cmd_list, &classic_node);
		i++;
	}
	return (cmd_list);
}

int		main(int ac, char **av, char **env)
{
	int		fd;
	char	**tmp;
	char	*line;

	if (ac > 1 || av[1])
		return (write(1, "No args please\n", 15));
	g_envi = list_env(env);
	g_bin = list_binaries();
	fd = open(".sh_h", O_CREAT | O_RDWR | O_APPEND,
	S_IRWXU | S_IRWXG | S_IRWXO);
	while (wk_dir() && ft_gnl(0, &line))
	{
		ft_write_fd(line, fd);
		tmp = ft_strsplit(line, ';');
		ft_memdel((void**)&line);
		g_cmd_list = get_cmd(tmp);
		ft_deltab((void**)tmp);
		do_cmd();
	}
	return (0);
}