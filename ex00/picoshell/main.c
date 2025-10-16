/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dalabrad <dalabrad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:48:22 by dalabrad          #+#    #+#             */
/*   Updated: 2025/10/16 11:59:53 by dalabrad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	picoshell(char **cmds[]);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s comando [args ...] \"|\" comando [args ...]\n", argv[0]);
        return 1;
    }

    // Contar cantidad de comandos (separados por "|")
    int num_cmds = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0)
            num_cmds++;
    }

    // Reservar espacio para los comandos
    char ***cmds = malloc((num_cmds + 1) * sizeof(char **));
    if (!cmds) {
        perror("malloc cmds");
        return 1;
    }

    int cmd_idx = 0;
    int start = 1;

    // Separar los argumentos por pipes
    for (int i = 1; i <= argc; i++) {
        if (i == argc || strcmp(argv[i], "|") == 0) {
            int cmd_argc = i - start;
            cmds[cmd_idx] = malloc((cmd_argc + 1) * sizeof(char *));
            if (!cmds[cmd_idx]) {
                perror("malloc cmds[i]");
                return 1;
            }

            for (int j = 0; j < cmd_argc; j++) {
                cmds[cmd_idx][j] = argv[start + j];
            }
            cmds[cmd_idx][cmd_argc] = NULL; // fin de args de este comando
            cmd_idx++;
            start = i + 1;
        }
    }
    cmds[cmd_idx] = NULL; // fin de lista de comandos

    // Mostrar resultado (solo para verificar)
    for (int i = 0; cmds[i]; i++) {
        printf("Comando %d:\n", i);
        for (int j = 0; cmds[i][j]; j++) {
            printf("  arg[%d] = '%s'\n", j, cmds[i][j]);
        }
    }


	printf("\n\n-------------------------------EJECUTANDO PICOSHELL-------------------------------\n\n");
	int	out = picoshell(cmds);
	printf("\npicoshell()'s OUTPUT : %d\n", out);
    // Liberar memoria
	printf("\nLIBERANDO MEMORIA...\n");
    for (int i = 0; cmds[i]; i++)
        free(cmds[i]);
    free(cmds);
	printf("\nMEMORIA LIBERADA...\n");

    return 0;
}