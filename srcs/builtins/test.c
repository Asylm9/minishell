#include "../../minishell.h"

/* REFACTORISATION: Cette fonction reste identique à l'original */
static int	validate_format_export(char **args, int i)
{
	int		j;

	if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
	{
		if (args[i][0] == '-')
		{
			printf_fd(2, "minishell: export: `%s': invalid option\n", args[i]);
			return (BUILTIN_ERR);
		}
		printf_fd(2, "minishell: export: `%s': not a valid identifier\n",
			args[i]);
		return (ERROR);
	}
	j = 1;
	while (args[i][j] && args[i][j] != '=')
	{
		if (args[i][j] == '+' && args[i][j + 1] == '=')
			break ;
		if (!ft_isalnum(args[i][j]) && args[i][j] != '_')
		{
			printf_fd(2, "minishell: export: `%s': not a valid identifier\n",
				args[i]);
			return (ERROR);
		}
		j++;
	}
	return (SUCCESS);
}

/* FONCTION SUPPRIMÉE: Fusionnée avec process_append pour simplifier l'architecture */

/* NOUVELLE FONCTION: Extraite de process_export_arg pour gérer SHLVL et valeurs vides
 * Remplace ce bloc dans l'original:
 *   if ((ft_strcmp(args[i], "SHLVL") == 0 && (*value == '-' || is_numeric(value) == false)))
 *   {
 *       shlvl_value = "0";
 *       ret = set_envl_var(args[i], envl, shlvl_value);
 *   }
 *   else if (ft_strlen(value) == 0)
 *       ret = set_envl_var(args[i], envl, EMPTY);
 *   else
 *       ret = set_envl_var(args[i], envl, value);
 */
static int	handle_shlvl_special_case(char *key, char *value, t_env **envl)
{
	if (ft_strcmp(key, "SHLVL") == 0 && (*value == '-' || is_numeric(value) == false))
		return (set_envl_var(key, envl, "0"));
	if (ft_strlen(value) == 0)
		return (set_envl_var(key, envl, EMPTY));
	return (set_envl_var(key, envl, value));
}

/* NOUVELLE FONCTION: Extraite de process_export_arg pour gérer les assignations normales
 * Remplace ce bloc dans l'original:
 *   *equal_pos = '\0';
 *   value = equal_pos + 1;
 *   if ((ft_strcmp(args[i], "SHLVL") == 0 && (*value == '-' || is_numeric(value) == false)))
 *   {
 *       shlvl_value = "0";
 *       ret = set_envl_var(args[i], envl, shlvl_value);
 *   }
 *   else if (ft_strlen(value) == 0)
 *       ret = set_envl_var(args[i], envl, EMPTY);
 *   else
 *       ret = set_envl_var(args[i], envl, value);
 *   *equal_pos = '=';
 */
static int	process_assignment(char *arg, char *equal_pos, t_env **envl)
{
	char	*value;
	int		ret;

	*equal_pos = '\0';
	value = equal_pos + 1;
	ret = handle_shlvl_special_case(arg, value, envl);
	*equal_pos = '=';
	return (ret);
}

/* FONCTION FUSIONNÉE: Combine process_append + handle_append_export (24 lignes ≤ 25)
 * AVANT: Deux fonctions séparées (8 + 16 lignes)
 * APRÈS: Une seule fonction qui gère tout le processus d'append
 * 
 * LOGIQUE:
 * 1. Parse l'argument (extraction de la valeur après +=)
 * 2. Récupère la valeur actuelle de la variable
 * 3. Concatène ou crée selon le cas
 * 4. Restaure le format original de l'argument
 */
static int	process_append(char *arg, char *plus_pos, t_env **envl)
{
	char	*new_value;
	char	*current_value;
	char	*joined_value;
	int		ret;

	*plus_pos = '\0';
	new_value = plus_pos + 2;

	current_value = get_envl_var(arg, *envl);
	if (!current_value)
		ret = set_envl_var(arg, envl, new_value);
	else
	{
		joined_value = ft_strjoin(current_value, new_value);
		free(current_value);
		if (!joined_value)
		{
			*plus_pos = '+';
			return (ERROR);
		}
		ret = set_envl_var(arg, envl, joined_value);
		free(joined_value);
	}
	*plus_pos = '+';
	return (ret);
}

/* FONCTION REFACTORISÉE: Simplifiée de 43 lignes → 12 lignes
 * AVANT: Une grosse fonction avec toute la logique mélangée
 * APRÈS: Délègue aux fonctions spécialisées selon le type d'assignation
 * 
 * CHANGEMENTS:
 * - Suppression de la variable 'shlvl_value'
 * - Extraction des blocs if/else complexes vers des fonctions dédiées
 * - Logique simplifiée avec delegation claire
 */
static int	process_export_arg(char **args, int i, t_env **envl)
{
	char	*equal_pos;
	char	*plus_pos;

	plus_pos = ft_strnstr(args[i], "+=", ft_strlen(args[i]));
	equal_pos = ft_strchr(args[i], '=');
	if (!equal_pos)
	{
		if (!get_envl_var(args[i], *envl))
			return (add_new_entry(args[i], NULL, envl));
		return (SUCCESS);
	}
	if (plus_pos)
		return (process_append(args[i], plus_pos, envl));
	return (process_assignment(args[i], equal_pos, envl));
}

/* FONCTION PRINCIPALE: Reste identique, aucune modification nécessaire
 * Déjà conforme à la norme (23 lignes < 25)
 */
int	builtin_export(char **args, t_env **envl)
{
	int		i;
	int		ret;
	int		status;

	if (!envl)
		return (ERROR);
	if (args_count(args) == 1)
	{
		print_exp_list(*envl);
		return (SUCCESS);
	}
	ret = 0;
	status = 0;
	i = 0;
	while (args[++i])
	{
		ret = validate_format_export(args, i);
		if (ret != 0)
			status = ret;
		else if (process_export_arg(args, i, envl) != 0)
			status = ERROR;
	}
	return (status);
}