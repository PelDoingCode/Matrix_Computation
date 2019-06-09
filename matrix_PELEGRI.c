#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#define MAX_TAILLE 1000


/* Julien Pelegri  */

struct matrice {
    int m,          /* nombre de lignes de la matrice */
        n ;         /* nombre de colonnes de la matrice */
    double *v_ptr; /* valeurs contenues dans la matrice */
};

long convertir(const char *str, int *status) /* but verifier que l'on a bien des entiers */
{
	char *endptr ;	/* Permet de détecter les erreurs lors de l'utilisation de la fonction strtol (cf page de manuel) */
	long val ;	/* Le résultat de la conversion. Attention au type ! */


	errno = 0 ;
	val = strtol(str, &endptr, 10) ;	/* conversion en base 10. */

	/* A priori, ça devrait bien se passer */
	*status = 1 ;

	/* Y a-t-il eu des erreurs rapportées par la fonction de conversion ? */
	if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	|| (errno != 0 && val == 0) ) {
		*status = 0 ;	/* on signale que la conversion n'a pas pu se faire */
		return 0 ;
	}

	/* Y a-t-il des caractères non traités ? */
	if ( *endptr != '\0' ) {
		*status = 0 ;	/* On signale que la conversion n'est pas valide */
		return 0 ;
	}

	/* Si on arrive ici, c'est que tout s'est bien passé, il suffit de
	 * retourner la valeur obtenue.
	 */
	return val ;
}


void affiche_matrice(struct matrice *M_ptr)
{
	printf("\n");


    int i,j;
    double a;

    // on donne la taille avant de l'afficher

    printf("La taille de la matrice est : ");
    printf("%d ", M_ptr->m);
    printf("%d\n",  M_ptr->n);
    printf("\n");

    // affichage de la matrice dans le terminal :
    // on veut afficher en mettant les | et -


    printf("\n.- ");

    for (i=0; i < M_ptr->n; i++)
    {
    	printf("           ");
    }
    printf("-.\n");

    // on a fait le haut
    for (i=0; i < M_ptr->m; i++)
    {
    	int b = i*M_ptr->n;
    	a = M_ptr->v_ptr[b];
	    printf("|% 10g", (float)a);

        for (j=1; j< M_ptr->n -1; j++)
        {
            int b = i*M_ptr->n + j;
            a = M_ptr->v_ptr[b];
	    	printf(" % 10g", (float)a);
	    }
        a = M_ptr->v_ptr[b + M_ptr->n -1];
        printf(" % 10g    |\n", (float)a);
    }
    // on a fait les cotes en ecrivant la matrice

    printf("._ ");



    for (i=0; i < M_ptr->n; i++)
    {
    	printf("           ");
    }
    printf("_.\n");
    // on termine par le bas
}




// on extrait la matrice du fichier que l'on stocke dans M :

int lire_matrice(struct matrice *M_ptr, char *nom_fichier)
{
    char string[MAX_TAILLE] = "";
    FILE *fichier = NULL;
    fichier = fopen(nom_fichier, "r");

    int a, b;  // pour acceder à la taille de la matrice
    int i=0, m, n;

    int sret ;

    if (fichier != NULL)
    {
        printf("L'ouverture du fichier %s a reussi. \n", nom_fichier);
        a = fgetc(fichier);

        // on passe les commentaires en debut de fichier
        while(a == '#')
        {
            fgets(string, MAX_TAILLE, fichier);
            a = fgetc(fichier) ;
        }

        ungetc(a, fichier);
        sret = fscanf(fichier, "%d %d", &a, &b );

        m = a;
        n = b;

       /* on verifie que les tailles sont coherentes */

        if ( sret != 2 )
        {
        fprintf(stderr, "Impossible de trouver la taille de la matrice.\n") ;
        fclose(fichier) ;
        return 0 ;
        }


        if( m <= 0 || n <= 0)
        {
            printf(" Les données du ficher sont erronnées !");
            return EXIT_FAILURE;
        }

        M_ptr->m = m;
        M_ptr->n = n;

        // allocation dynamique
        M_ptr->v_ptr = NULL ;
        M_ptr->v_ptr = malloc((m*n)* sizeof(double));


        if (M_ptr->v_ptr == NULL)
        	{
            	printf("L'ordinateur n'a pas assez de mémoire!\n");

            	exit(0);
        	}
        	else
        	{
            	printf("Allocation en memoire de la matrice de stokage reussie !\n");
        	}

    for ( i = 0 ; i < M_ptr->n*M_ptr->m ; i++ )
        {
            sret = fscanf(fichier, "%lf", &(M_ptr->v_ptr[i]));
            if ( sret != 1 )
            {
                fprintf(stderr, "Erreur de lecture dans le fichier" " à la position %d.\n", i) ;
                fclose(fichier) ;

                return 0 ;
            }
            //printf("%f\n", M_ptr->v_ptr[i] );
            //printf("%d\n", i);
        }

		fclose(fichier);
        return 1;
    }
    else
    {
        printf("Il y a eu un probleme..., le fichier n'existe pas !! \n");
        fclose(fichier);
        return 0;
    }
    return 1;
}

int ecrit_matrice(struct matrice *M_ptr, char *nom_fichier)
{
    int i, k;
    FILE* fichier = NULL;
    fichier = fopen(nom_fichier, "r+");
    char commentaire[1000]; // le commentaire ne doit pas dépasser 1000 caractère

    // le commentaire doit etre ecrit_comme_ca_avec_des_tirets


    while(fichier==NULL)
    {
        fclose(fichier);

        // On affiche un message d'erreur si on veut
        printf("Impossible d'ouvrir le fichier demandé : %s\n", nom_fichier);
        printf("Le fichier existe deja ou ce fichier n'existe pas, je vais vous demander de rerentrer un nom de fichier : ");  /* ici je pars du principe que l'on ne force pas la création du fichier, si le fichier n'est pas en memoire on redemande un nom de fichier */
        scanf("%s", nom_fichier);
        printf("\n");
        fichier = fopen(nom_fichier, "r+");

    }
    printf("On a reussit à ouvrir le fichier : %s\n", nom_fichier);


    // on a le bon fichier on peut lui rentrer la matrice
    if(fichier != NULL)
    {
        printf("\n");
        // On peut lire et écrire dans le fichier
        printf("L'ouverture de nom_fichier s'est bien déroulée.\n\n");

        // on introduit le commentaire
        printf("Quel est ton commentaire que tu veux ajouter en debut de ficher ?\n");

        //fgets(commentaire,100,stdin);
        scanf("%s", commentaire);

        fprintf(fichier, "#%s\n", commentaire);
        fprintf(fichier, "%d %d \n", M_ptr->m, M_ptr->n);
        // on rentre les valeurs dans le fichier
        for(i=0; i < M_ptr->m; i++)
        {

            for(k=0; k < M_ptr->n; k++)
            {
                fprintf(fichier, "%g",M_ptr->v_ptr[(M_ptr->n)*i+k]);
                fprintf(fichier, " ");
            }
            fprintf(fichier, "\n");
        }
        fclose(fichier);
        printf("Tout s'est bien passé.\n");
        return 1;
    }
    else
    {
        printf("Il y a eu un probleme...\n");
        return 0;
    }
}



struct matrice * transpose(struct matrice *M_ptr)
{
    int i,j ;
    struct matrice *TM;

    TM = malloc(sizeof(struct matrice));

    TM->m = M_ptr->n;
    TM->n  = M_ptr->m;
    //printf("%d\n", TM->n);
    //printf("%d\n", TM->m);


    TM->v_ptr = malloc(TM->n*TM->m*sizeof(double));

    if (TM->v_ptr == NULL)
        {
            printf("L'ordinateur n'a pas assez de mémoire!\n");
            exit(1);
        }
        else
        {
            printf("Allocation en memoire de la matrice de transposition reussie !\n");
        }

    // on place dans TM la Transposée de M_ptr

    for(i=0; i< M_ptr->m; i++)
    {
        for(j=0; j< M_ptr->n; j++)
        {
            TM->v_ptr[(TM->n)*(j)+i] = M_ptr->v_ptr[(M_ptr->n)*(i)+j];
            //printf("%f\n", TM->v_ptr[(TM->n)*(j)+i]);
        }
    }
    printf("\n");
    return TM;
}


struct matrice * additionne(struct matrice *M_ptr1, struct matrice * M_ptr2)
{
    int i, j;
    struct matrice *A;
    A = malloc(sizeof(struct matrice));

    // sur mac | = alt+maj.L

    if ( M_ptr1->m != M_ptr2->m || M_ptr1->n != M_ptr2->n)
    {
        printf(" Les tailles des matrices ne sont pas valides pour réaliser une addition. \n");
        exit(1);
    }

    A->m = M_ptr1->m;
    A->n = M_ptr1->n;

    A->v_ptr = malloc(A->n*A->m*sizeof(double));

    if (A->v_ptr == NULL)
        {
            printf("L'ordinateur n'a pas assez de mémoire!\n");
            exit(1);
        }
        else
        {
            printf("Allocation en memoire de la matrice de transposition reussie !!\n");
        }


    for(i=0; i< A->m; i++)
    {
        for(j=0; j< A->n; j++)
        {
            A->v_ptr[(A->m)*(i)+j] = M_ptr1->v_ptr[(M_ptr1->m)*(i)+j] + M_ptr2->v_ptr[(M_ptr2->m)*(i)+j];
            //printf("%f\n", M_ptr1->v_ptr[(M_ptr1->m)*(i)+j]);
        }
    }
    printf("\n");
    return A;
}


struct matrice * multiplie_scalaire(struct matrice *M_ptr, double scalaire)
{

    int i, j;

    for(i=0; i< M_ptr->m; i++)
    {
        for(j=0; j< M_ptr->n; j++)
        {
            M_ptr->v_ptr[(M_ptr->m)*(i)+j] = scalaire * M_ptr->v_ptr[(M_ptr->m)*(i)+j];
        }
    }
    printf("\n");
    return M_ptr;
}

struct matrice * multiplie_matrice(struct matrice *M_ptr1, struct matrice * M_ptr2)
{
    // attention ici je tiens compte de la facon dont est rentrée la commande
    int i, j, k;
    struct matrice *M;
    M = malloc(sizeof(struct matrice));
    // sur max | = alt+maj.L

    if ( M_ptr1->n != M_ptr2->m )
    {
        printf(" Les tailles des matrices ne sont pas valides pour réaliser une multiplication. \n");
        exit(0);
    }

    M->m = M_ptr1->m;
    M->n  = M_ptr2->n;

    M->v_ptr = malloc(M->n*M->m*sizeof(double));

    if (M->v_ptr == NULL)
        {
            printf("L'ordinateur n'a pas assez de mémoire!\n");
            exit(0);
        }
        else
        {
            printf("Allocation en memoire de la matrice de transposition reussie !\n");
        }


    for(i=0; i< M->m; i++)
    {
        for(j=0; j< M->n; j++)
        {
            for(k=0; k< M_ptr1->n; k++)
            {
                M->v_ptr[(M->m)*(i)+j] = M->v_ptr[(M->m)*(i)+j] + M_ptr1->v_ptr[(M_ptr1->m)*(i)+k]*M_ptr2->v_ptr[(M_ptr2->m)*(k)+j];
            }
        }
    }
    printf("On a realiser l'addition avec succès \n");
    return M;
}




int main (int argc, char *argv[])
{
	srand(time(NULL));
	printf("Convole, un outil de manipulation de matrices.\n\n");

    char optstring[] = "c:o:"; /* option permise : -c et -o dans ce qu'on rentre dans le terminal */

	// choix_operation nous donne acces a ce que rentre l'utilisateur
	char choix_operation;

    choix_operation = getopt(argc, argv, optstring);

    // si les donnees rentrees sont fausses
	if (choix_operation != 'c' && choix_operation != 'o')
	{
		printf("\n les commandes ne sont pas reconnues  \n");
        exit(1);
    }


    // transposée
    else if (choix_operation == 'c' && strcmp(optarg, "t") == 0 )
    {
    	struct matrice M_ptr;
    	struct matrice* T_M_ptr;
    	char *nom_fichier;
    	M_ptr.v_ptr = NULL;
    	T_M_ptr = NULL;

    	choix_operation = getopt(argc,argv,optstring);

    	// on affiche juste
    	if (choix_operation == EOF)
    	{
			nom_fichier = argv[optind];

			lire_matrice(&M_ptr, nom_fichier);

			T_M_ptr = transpose(&M_ptr);
			affiche_matrice(T_M_ptr);
		}

		// on met la transposéé dans un fichier
		else if (choix_operation == 'o')
		{
			char* nom_fichier_ecrire = optarg;
			choix_operation = getopt(argc,argv,optstring);

			if (choix_operation != EOF)
			{
				printf(" commande erronée, trop d'arguments  \n");
				exit(1);
			}

			nom_fichier = argv[optind];
			lire_matrice(&M_ptr, nom_fichier);

			T_M_ptr = transpose(&M_ptr);
			ecrit_matrice(T_M_ptr, nom_fichier_ecrire);
		}
	}

	// addition
	else if (choix_operation == 'c' && strcmp(optarg, "+") == 0 )
	{

		struct matrice M_ptr1;
		struct matrice M_ptr2;
    	struct matrice* A_M_ptr;

    	M_ptr1.v_ptr = NULL;
    	M_ptr2.v_ptr = NULL;
    	A_M_ptr = NULL;

		choix_operation = getopt(argc, argv, optstring);

		// on affiche que la matrice
		if (choix_operation == EOF)
		{
			char* nom_fichier1 = argv[optind];
			char* nom_fichier2 = argv[optind +1];

			lire_matrice(&M_ptr1, nom_fichier1);
			lire_matrice(&M_ptr2, nom_fichier2);

			A_M_ptr = additionne(&M_ptr1, &M_ptr2);
			affiche_matrice(A_M_ptr);
		}

		// on la met dans un fichier
		else if (choix_operation == 'o')
		{
			char* nom_fichier_ecrire = optarg;
			choix_operation = getopt(argc, argv, optstring);

			if (choix_operation != EOF)
			{
				printf(" commande erronée, trop d'arguments\n");
				exit(1);
			}
			// on stocke dans un fichier
			else
			{
				char* nom_fichier1 = argv[optind];
				char* nom_fichier2 = argv[optind +1];

				lire_matrice(&M_ptr1,nom_fichier1);
				lire_matrice(&M_ptr2,nom_fichier2);

				A_M_ptr = additionne(&M_ptr1,&M_ptr2);
				ecrit_matrice(A_M_ptr, nom_fichier_ecrire);
			}
		}
	}

	// multiplication
	else if (choix_operation == 'c' && strcmp(optarg, "x") == 0 )
	{
		struct matrice M_ptr;
    	struct matrice* M_M_ptr;

    	M_ptr.v_ptr = NULL;
    	M_M_ptr = NULL;

		choix_operation = getopt(argc, argv, optstring);

		// on affiche juste
		if (choix_operation == EOF)
		{
			int ok;
			double test = convertir(argv[optind], &ok);

			//on multiplie par un scalaire car le test est verifié
			if (ok)
			{
				char* nom_fichier = argv[optind+1];
				double scalaire = test;
				lire_matrice(&M_ptr, nom_fichier);
				M_M_ptr = multiplie_scalaire(&M_ptr, scalaire);
				affiche_matrice(M_M_ptr);
			}

			else
			{
				char* nom_fichier = argv[optind];
				int okk;
				double scalaire = convertir(argv[optind +1], &okk);

				if (okk)
				{
					lire_matrice(&M_ptr, nom_fichier);
					M_M_ptr = multiplie_scalaire(&M_ptr, scalaire);
					affiche_matrice(M_M_ptr);
				}

				else
				{
					struct matrice M_ptr1;
					struct matrice M_ptr2;

					char* nom_fichier1 = nom_fichier;
					char* nom_fichier2 = argv[optind +1];

					M_ptr1.v_ptr = NULL;
					M_ptr2.v_ptr = NULL;

					lire_matrice(&M_ptr1, nom_fichier1);
					lire_matrice(&M_ptr2, nom_fichier2);
					M_M_ptr = multiplie_matrice(&M_ptr1, &M_ptr2);
					affiche_matrice(M_M_ptr);
				}
			}
		}
		// on met dans un fichier
		else if (choix_operation == 'o')
		{
			char* nom_fichier_ecrire = optarg;
			choix_operation = getopt(argc, argv, optstring);

			if (choix_operation != EOF)
			{
				printf("commande erronée, trop d'arguments \n");
				exit(1);
			}


			else
			{
				int ok;
				double test = convertir(argv[optind], &ok);


				// encore une fois on regarde si on a un scalaire
				if (ok)
				{
					char* nom_fichier = argv[optind+1];
					double scalaire = test;

					lire_matrice(&M_ptr, nom_fichier);
					M_M_ptr = multiplie_scalaire(&M_ptr, scalaire);
					ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
				}

				else
				{
					char* nom_fichier = argv[optind];
					int okk;
					double scalaire = convertir(argv[optind +1], &okk);


					// si le scalaire est en deuxieme
					if (okk)
					{
						lire_matrice(&M_ptr, nom_fichier);
						M_M_ptr = multiplie_scalaire(&M_ptr, scalaire);
						ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
					}

					else
					{
						struct matrice M_ptr1;
						struct matrice M_ptr2;
						char* nom_fichier1 = nom_fichier;

						char* nom_fichier2 = argv[optind +1];
						M_ptr1.v_ptr = NULL;
						M_ptr2.v_ptr = NULL;

						lire_matrice(&M_ptr1, nom_fichier1);
						lire_matrice(&M_ptr2, nom_fichier2);
						M_M_ptr = multiplie_matrice(&M_ptr1, &M_ptr2);
						ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
					}
				}
			}
		}
	}






			// si on a pas mis -c
	else if (choix_operation == 'c' && (strcmp(optarg, "t") != 0 || strcmp(optarg, "+") != 0 || strcmp(optarg, "x") != 0 ))
	{
		printf(" commande eronée, ajout non possible à la commande -c  \n");
		exit(1);
	}


	// ici on traite le cas ou on a mis -c avant -o
	else if (choix_operation == 'o')
	{
		char* nom_fichier_ecrire = optarg;
		struct matrice M_ptr1;
		struct matrice M_ptr2;

		M_ptr1.v_ptr = NULL;
		M_ptr2.v_ptr = NULL;

		choix_operation = getopt(argc, argv, optstring);
		if (choix_operation != 'c')
		{
			printf("commande eronnée, il faut rentrer -c pour les calculs \n");
			exit(1);
		}


		else
		{
			// transposée
			if ( strcmp(optarg, "t") == 0 )
			{
				struct matrice * T_M_ptr;
				T_M_ptr = NULL;

				choix_operation = getopt(argc, argv, optstring);

				if (choix_operation != EOF)
				{
					printf(" commande eronnée, trop d'arguments  \n");
					exit(1);
				}

				else
				{
					char * nom_fichier = argv[optind];
					lire_matrice(&M_ptr1, nom_fichier);
					T_M_ptr = transpose(&M_ptr1);
					ecrit_matrice(T_M_ptr, nom_fichier_ecrire);
				}
			}


			// addition

			else if ( strcmp(optarg, "+" ) == 0)
			{
				struct matrice * A_M_ptr;
				A_M_ptr = NULL;

				choix_operation = getopt(argc, argv, optstring);

				if (choix_operation != EOF)
				{
					printf("commande eronnée, trop d'arguments \n");
					exit(1);
				}

				else
				{
					char* nom_fichier1 = argv[optind];
					char* nom_fichier2 = argv[optind +1];

					lire_matrice(&M_ptr1, nom_fichier1);
					lire_matrice(&M_ptr2, nom_fichier2);

					A_M_ptr = additionne(&M_ptr1, &M_ptr2);
					ecrit_matrice(A_M_ptr, nom_fichier_ecrire);

				}
			}


			// multiplication
			else if (strcmp(optarg, "x") == 0)
			{
				struct matrice * M_M_ptr;
				M_M_ptr = NULL;

				choix_operation = getopt(argc, argv, optstring);

				if (choix_operation != EOF)
				{
					printf(" commande eronée, trop d'arguments \n");
					exit(1);
				}

				else
				{
					int ok;
					double test = convertir(argv[optind], &ok);
					// si le scalaire est au debut

					if (ok)
					{
						char* nom_fichier = argv[optind+1];

						double scalaire = test;
						lire_matrice(&M_ptr1, nom_fichier);
						M_M_ptr = multiplie_scalaire(&M_ptr1, scalaire);
						ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
					}

					else
					{
						char* nom_fichier = argv[optind];
						int okk;
						double scalaire = convertir(argv[optind +1], &okk);

						// si le scalaire est le deuxieme argument
						if (okk)
						{
							lire_matrice(&M_ptr1, nom_fichier);
							M_M_ptr = multiplie_scalaire(&M_ptr1, scalaire);
							ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
						}

						else
						{
							char* nom_fichier1 = nom_fichier;
							char* nom_fichier2 = argv[optind +1];

							lire_matrice(&M_ptr1, nom_fichier1);
							lire_matrice(&M_ptr2, nom_fichier2);

							M_M_ptr = multiplie_matrice(&M_ptr1, &M_ptr2);
							ecrit_matrice(M_M_ptr, nom_fichier_ecrire);
						}
					}
				}
			}
			// on a fait tous les cas possibles on sort
			else
			{
				printf(" commande eronnée, ajout à -c non possible \n");
				exit(1);
			}
		}
	}
	return 0;
}


