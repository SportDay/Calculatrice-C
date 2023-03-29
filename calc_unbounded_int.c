#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "unbounded_int.h"

#define INPUT_CHAR "-i"
#define OUTPUT_CHAR "-o"
#define HELP_CHAR "-h"

#define ERROR_PREFIX "[ERROR]"
#define INFO_PREFIX "[INFO]"

typedef struct variable {
    char *nom;
    unbounded_int val;
    struct variable *suivant;
} variable;

typedef struct variableList {
    size_t len;
    variable *premier;
    variable *dernier;
} variableList;

typedef struct tmpVar {
    char nom;
    struct tmpVar *suivant;
} tmpVar;

typedef struct tmpList {
    size_t len;
    tmpVar *premier;
    tmpVar *dernier;
} tmpList;

static variable *makeVariable(char *nom, unbounded_int val) {
    variable *to_return = malloc(sizeof(variable));
    if (to_return == NULL) {
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    to_return->nom = malloc(sizeof(char) * strlen(nom));
    if (to_return->nom == NULL) {
        free(to_return);
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    to_return->nom = nom;
    to_return->val = val;
    to_return->suivant = NULL;
    return to_return;
}

static variableList *addLastVariableList(variableList *v, unbounded_int u, char *name) {
    if (v != NULL) {
        variable *ch = makeVariable(name, u);
        if (v->premier == NULL) {
            v->premier = ch;
        } else {
            v->dernier->suivant = ch;
        }
        v->dernier = ch;
        v->len++;
    }
    return v;
}

static variable *getVariableByString(variableList *u, char *c) {
    variable *tmp = u->premier;
    while (tmp != NULL) {
        if (strcmp(tmp->nom, c) == 0) {
            return tmp;
        }
        tmp = tmp->suivant;
    }
    return NULL;
}

static void destroy_variable(variable *c) {
    if (c != NULL) {
        destroy_variable(c->suivant);
        free(c);
    }
}

static void destroy_variableList(variableList *u) {
    destroy_variable(u->premier);
    u->premier = NULL;
    u->dernier = NULL;
}

static variableList *emptyVariableList() {
    variableList *e = malloc(sizeof(variableList));
    if (e == NULL) {
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    e->premier = NULL;
    e->dernier = NULL;
    e->len = 0;
    return e;
}

static void destroy_tmpVar(tmpVar *c) {
    if (c != NULL) {
        destroy_tmpVar(c->suivant);
        free(c);
    }
}

static void destroy_tmpList(tmpList *u) {
    destroy_tmpVar(u->premier);
    u->premier = NULL;
    u->dernier = NULL;
}

static tmpVar *maketmpVar(char nom) {
    tmpVar *to_return = malloc(sizeof(tmpVar));
    if (to_return == NULL) {
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    to_return->nom = nom;
    to_return->suivant = NULL;
    return to_return;
}

static tmpList *addLastTmpList(tmpList *u, char c) {
    if (u != NULL) {
        tmpVar *ch = maketmpVar(c);
        if (u->premier == NULL) {
            u->premier = ch;
        } else {
            u->dernier->suivant = ch;
        }
        u->dernier = ch;
        u->len++;
    }
    return u;
}

static tmpList *emptyTmpList() {
    tmpList *to_return = malloc(sizeof(tmpList));
    if (to_return == NULL) {
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    to_return->premier = NULL;
    to_return->dernier = NULL;
    to_return->len = 0;
    return to_return;
}

static char *tmpVar2String(tmpList i) {
    char *to_return = malloc((sizeof(char) * i.len) + 1);
    int start = 0;
    if (to_return == NULL) {
        fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
        exit(EXIT_FAILURE);
    }
    tmpVar *courant = i.premier;
    while (courant != NULL) {
        to_return[start] = courant->nom;
        courant = courant->suivant;
        start++;
    }
    to_return[start] = '\0';
    return to_return;
}


static int isOperator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return 1;
        default:
            return 0;
    }
}

static FILE *open(const char *arg, const char *mode) {
    FILE *flot = fopen(arg, mode);
    if (flot == NULL) {
        fprintf(stderr, "%s Le fichier %s est introuvable.\n", ERROR_PREFIX, arg);
        if (strcmp(mode, "w") == 0) {
            return stdout;
        } else if (strcmp(mode, "r") == 0) {
            return stdin;
        }
        return NULL;
    }
    return flot;
}


static void parseFile(FILE *fInput, FILE *fOutput) {

    char *op = "%s %s %s";

    tmpList *varName = emptyTmpList();
    tmpList *restString = emptyTmpList();
    variableList *varList = emptyVariableList();

    char *u1;
    char *ope;
    char *u2;

    int i;

    int newLine = 0;
    int varFound = 0;
    int printFound = 0;
    int firstAlpha = 0;
    int skip = 0;

    do {
        i = fgetc(fInput);
        if (i == '\n' || i == EOF) {
            if (restString->len != 0 && varName->len != 0) {

                if (newLine) {
                    free(u1);
                    free(u2);
                    free(ope);
                }

                tmpVar *start = restString->premier;
                tmpVar *end = maketmpVar('1');
                int len = 0;
                int addOne = 0;
                int findOper = 0;
                while (start != NULL) {
                    if (findOper || start->nom == '\n') {
                        break;
                    } else if (isOperator(start->nom) && addOne) {
                        findOper = 1;
                    } else if (!isspace((int) start->nom) &&
                               (isalnum((int) start->nom) || start->nom == '+' || start->nom == '-')) {
                        addOne = 1;
                        len += 1;
                    }
                    start = start->suivant;
                    end = start;
                }

                u1 = malloc(sizeof(char) * len);
                if (u1 == NULL) {
                    fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
                    exit(EXIT_FAILURE);
                }

                len = 0;
                addOne = 0;

                while (end != NULL) {
                    if (end->nom == '\n' || (addOne && isspace((int) end->nom))) {
                        break;
                    } else if (!isspace((int) end->nom) &&
                               (isalnum((int) end->nom) || end->nom == '+' || end->nom == '-')) {
                        addOne = 1;
                        len += 1;
                    }
                    end = end->suivant;
                }

                u2 = malloc(sizeof(char) * len);
                if (u2 == NULL) {
                    fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
                    exit(EXIT_FAILURE);
                }

                ope = malloc(sizeof(char));
                if (ope == NULL) {
                    fprintf(stderr, "%s Probleme avec l'allocation de la memoire.\n", ERROR_PREFIX);
                    exit(EXIT_FAILURE);
                }

                char *tt = tmpVar2String(*restString);
                char *varNom = tmpVar2String(*varName);
                int yo = sscanf(tt, op, u1, ope, u2);
                if (yo == 1) {
                    variable *tmpVari = getVariableByString(varList, varNom);
                    unbounded_int uu1;

                    if (isalpha((int) u1[0])) {
                        variable *tmpVari2 = getVariableByString(varList, u1);

                        if (tmpVari2 != NULL) {
                            uu1 = tmpVari2->val;
                        } else {
                            if (printFound) {
                                fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                            } else {
                                fprintf(stderr, "%s Probleme avec l'expression: %s = %s\n", ERROR_PREFIX, varNom, tt);
                            }
                            fprintf(stderr, "%s Car la variable: %s n'existe pas.\n", ERROR_PREFIX, u1);

                            newLine = 1;
                            free(tt);
                            destroy_tmpList(restString);
                            destroy_tmpList(varName);

                            restString = emptyTmpList();
                            varName = emptyTmpList();

                            varFound = 0;
                            printFound = 0;
                            firstAlpha = 0;
                            skip = 0;
                            continue;
                        }
                        if (printFound) {
                            fprintf(fOutput, "%s = %s\n", u1, unbounded_int2string(tmpVari2->val));
                        }
                    } else if (printFound) {
                        fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                        fprintf(stderr, "%s Car la variable: %s est incorrect.\n", ERROR_PREFIX, u1);
                    } else {
                        uu1 = string2unbounded_int(u1);
                        if (uu1.signe == '*') {
                            fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                            fprintf(stderr, "%s Car la valeur: %s est incorrect.\n", ERROR_PREFIX, u1);
                            newLine = 1;
                            free(tt);
                            destroy_tmpList(restString);
                            destroy_tmpList(varName);

                            restString = emptyTmpList();
                            varName = emptyTmpList();

                            varFound = 0;
                            printFound = 0;
                            firstAlpha = 0;
                            skip = 0;
                            continue;
                        }
                    }

                    if (!printFound) {
                        if (tmpVari != NULL) {
                            tmpVari->val = uu1;
                        } else {
                            addLastVariableList(varList, uu1, varNom);
                        }
                    }
                } else if (yo == 3 && !printFound) {
                    variable *tmpVari = getVariableByString(varList, varNom);
                    unbounded_int result;

                    unbounded_int uu1;
                    unbounded_int uu2;
                    if (isalpha((int) u1[0])) {
                        variable *tmpVari2 = getVariableByString(varList, u1);
                        if (tmpVari2 != NULL) {
                            uu1 = tmpVari2->val;
                        } else {
                            fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                            fprintf(stderr, "%s Car la variable: %s n'existe pas.\n", ERROR_PREFIX, u1);
                            newLine = 1;
                            free(tt);
                            destroy_tmpList(restString);
                            destroy_tmpList(varName);

                            restString = emptyTmpList();
                            varName = emptyTmpList();

                            varFound = 0;
                            printFound = 0;
                            firstAlpha = 0;
                            skip = 0;
                            continue;
                        }
                    } else {
                        uu1 = string2unbounded_int(u1);
                    }
                    if (isalpha((int) u2[0])) {
                        variable *tmpVari2 = getVariableByString(varList, u2);
                        if (tmpVari2 != NULL) {
                            uu2 = tmpVari2->val;
                        } else {
                            fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                            fprintf(stderr, "%s Car la variable: %s n'existe pas.\n", ERROR_PREFIX, u2);
                            newLine = 1;
                            free(tt);
                            destroy_tmpList(restString);
                            destroy_tmpList(varName);

                            restString = emptyTmpList();
                            varName = emptyTmpList();

                            varFound = 0;
                            printFound = 0;
                            firstAlpha = 0;
                            skip = 0;
                            continue;
                        }
                    } else {
                        uu2 = string2unbounded_int(u2);
                    }

                    if (uu1.signe == '*') {
                        fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                        fprintf(stderr, "%s Car la valeur: %s est incorrect.\n", ERROR_PREFIX, u1);
                        newLine = 1;
                        free(tt);
                        destroy_tmpList(restString);
                        destroy_tmpList(varName);

                        restString = emptyTmpList();
                        varName = emptyTmpList();

                        varFound = 0;
                        printFound = 0;
                        firstAlpha = 0;
                        skip = 0;
                        continue;
                    }
                    if (uu2.signe == '*') {
                        fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                        fprintf(stderr, "%s Car la valeur: %s est incorrect.\n", ERROR_PREFIX, u2);
                        newLine = 1;
                        free(tt);
                        destroy_tmpList(restString);
                        destroy_tmpList(varName);

                        restString = emptyTmpList();
                        varName = emptyTmpList();

                        varFound = 0;
                        printFound = 0;
                        firstAlpha = 0;
                        skip = 0;
                        continue;
                    }

                    if (strcmp(ope, "+") == 0) {
                        result = unbounded_int_somme(uu1, uu2);
                    } else if (strcmp(ope, "-") == 0) {
                        result = unbounded_int_difference(uu1, uu2);
                    } else if (strcmp(ope, "*") == 0) {
                        result = unbounded_int_produit(uu1, uu2);
                    } else if (strcmp(ope, "/") == 0) {
                        result = unbounded_int_quotient(uu1, uu2);
                    } else if (strcmp(ope, "%") == 0) {
                        result = unbounded_int_modulo(uu1, uu2);
                    }

                    if (tmpVari == NULL) {
                        addLastVariableList(varList, result, varNom);
                    } else {
                        tmpVari->val = result;
                    }
                } else {
                    fprintf(stderr, "%s Probleme avec l'expression: %s %s\n", ERROR_PREFIX, varNom, tt);
                }
                newLine = 1;
                free(tt);
            } else if (varName->len != 0 && restString->len == 0) {
                fprintf(stderr, "%s Probleme avec l'expression: %s\n", ERROR_PREFIX, tmpVar2String(*varName));
            }

            destroy_tmpList(restString);
            destroy_tmpList(varName);

            restString = emptyTmpList();
            varName = emptyTmpList();

            varFound = 0;
            printFound = 0;
            firstAlpha = 0;
            skip = 0;
        } else if (isspace(i) && !varFound && !printFound) {

        } else if (!firstAlpha) {
            if (isalpha(i)) {
                firstAlpha = 1;
            } else if (!skip) {
                skip = 1;
                fprintf(stderr, "%s La variable doit commencer par une lettre\n", ERROR_PREFIX);
            }
        }

        if (firstAlpha && !skip) {
            if (!varFound && !printFound && isalnum(i)) {
                addLastTmpList(varName, (char) i);
            } else if (!varFound && i == '=') {
                varFound = 1;
            } else if (!printFound && varName->len == 5 && strcmp("print", tmpVar2String(*varName)) == 0) {
                printFound = 1;
            } else {
               addLastTmpList(restString, (char) i);
            }
        }


    } while (i != EOF);

    destroy_tmpList(varName);
    destroy_tmpList(restString);

    destroy_variableList(varList);
}

int main(int nbrArg, char *args[]) {
    FILE *src = stdin;
    FILE *dest = stdout;

    int nameOpenFilePos = 0;
    int nameWriteFilePos = 0;

    if (nbrArg == 2) {
        if (strcmp(args[1], HELP_CHAR) == 0) {
            fprintf(stdout, "\nInformations sur le programme: calc_unbounded_int.\n\n"
                            "Options (facultative):\n"
                            "    -i fichier      Le nom du fichier texte qui contient une suite d’instructions. (Le fichier doit exister)\n"
                            "    -o fichier      Le nom du fichier ou il faut écrire les résultats.\n"
                            "    -h              Pour afficher ce message.\n"
                            "\nExemples:\n"
                            "    ./calc_unbounded_int -i src.txt -o dst.txt\n"
                            "    ./calc_unbounded_int -o dst.txt -i src.txt\n"
                            "    ./calc_unbounded_int -i src.txt\n"
                            "    ./calc_unbounded_int -o dst.txt\n"
                            "    ./calc_unbounded_int\n\n");
            exit(EXIT_SUCCESS);
        }
    } else if (nbrArg == 3) {
        if (strcmp(args[1], INPUT_CHAR) == 0) {
            nameOpenFilePos = 2;
            src = open(args[2], "r");
        } else if (strcmp(args[1], OUTPUT_CHAR) == 0) {
            nameWriteFilePos = 2;
            dest = open(args[2], "w");
        }
    } else if (nbrArg == 5) {
        if (strcmp(args[1], INPUT_CHAR) == 0) {
            nameOpenFilePos = 2;
            src = open(args[2], "r");
            if (strcmp(args[3], OUTPUT_CHAR) == 0) {
                nameWriteFilePos = 4;
                dest = open(args[4], "w");
            }
        } else if (strcmp(args[1], OUTPUT_CHAR) == 0) {
            nameWriteFilePos = 2;
            dest = open(args[2], "w");
            if (strcmp(args[3], INPUT_CHAR) == 0) {
                nameOpenFilePos = 4;
                src = open(args[4], "r");
            }
        }
    }

    if (src == stdin) {
        fprintf(stdout, "%s Le programme va utiliser l'entree standard.\n", INFO_PREFIX);
        fprintf(stdout, "%s (A) variable = entier_ou_variable op entier_ou_variable\n"
                        "           entier_ou_variable est soit un entier soit une variable. op est une de trois opérations :\n"
                        "           *, +, -. Le progranne effectue l’opération, le résultat de l’opération devient\n"
                        "           la valeur de la variable.\n"
                        "       (B) variable = entier\n"
                        "           entier devient la nouvelle valeur de la variable.\n"
                        "       (C) print variable\n"
                        "           Le programme écrira dans le fichier de sortie une ligne sous forme\n"
                        "           variable = valeur\n"
                        "           où valeur est la valeur de variable.\n", INFO_PREFIX);

    } else {
        fprintf(stdout, "%s Debut de la lecture du fichier: %s.\n", INFO_PREFIX, args[nameOpenFilePos]);
    }

    if (dest == stdout) {
        fprintf(stdout, "%s Le programme va utiliser la sortie standard pour afficher les donnes.\n", INFO_PREFIX);
    } else {
        fprintf(stdout, "%s Le programme va utiliser le fichier: %s pour sauvegarder les donnes.\n", INFO_PREFIX,
                args[nameWriteFilePos]);
    }

    parseFile(src, dest);


    if (feof(src) && src != stdin) {
        fprintf(stdout, "%s Le programme a finit de traiter le fichier: %s.\n", INFO_PREFIX, args[nameOpenFilePos]);
    }

    fclose(src);
    fclose(dest);

    return 0;
}
