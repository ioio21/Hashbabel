// Copyright 2022 <Tudor Ioana Octavia>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "utils.h"

typedef struct info_carte {
	Hashtable* carte;
	float rating;
	float sume_note;
	int imprumutat;  // imi spune daca cartea e imprumutata
	int nr_imprumuturi;
} info_carte;

typedef struct user {
	char nume_carte[41];
	char nume_user[21];
	int banat;
	int imprumutat;  // imi spune daca userul are deja o carte imprumutata
	int punctaj;
	int termen_limita;
} user;

typedef struct top_useri {
	char nume_user[21];
	int punctaj;
} top_useri;
// structura folosita la afisare in EXIT pt useri
typedef struct top_carte {
	char nume_carte[41];
	int nr_imprumuturi;
	float rating;
} top_carte;
// structura folosita la afisare in EXIT pt carti

int main()
{
    char comanda[40];
    char nume_carte[41];
	char user_nume[21];
    char linie[101];
	char* params;
	char key[21], value[21];
	char def_key[21];
	int n;
    int ok = 0;
	int nr_u = 0;
	Hashtable* h_bib = create_hashtable(1, hash, cmp);
	// creez hashtable care imi retine cartile
	// key = numele cartii val = structura info_carte
	Hashtable* h_users = create_hashtable(1, hash, cmp);
	// creez hashtable pentru useri
	// key = nume_user val = structura user
    while (ok != 1) {
        scanf("%s ", comanda);
        if (strcmp(comanda, "ADD_BOOK") == 0) {
            fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, "\"\n");
			n = atoi(params);
			info_carte* c = malloc(sizeof(info_carte));
			c->carte = create_hashtable(1, hash, cmp);
			// creez hashtable pentru o carte
			// def = definitie cuvant din carte , val = cuvant
			c->imprumutat = 0;
			c->nr_imprumuturi = 0;
			c->rating = 0.0;
			c->sume_note = 0.0;
			for (int i = 0; i < n; i++) {
				scanf("%s", key);
				scanf("%s", value);
				add_hashtable(&(c->carte), key, strlen(key) + 1, value,
					strlen(value) + 1);
				// adaug in hashtable-ul cartii cheia si val citita
			}
			// Verifica daca exista deja
			void* p = find_hashtable(h_bib, nume_carte,
						strlen(nume_carte) + 1);  // o structura info_carte
			if (p != NULL) {
				info_carte* cc  = p;
				free_hashtable(cc->carte);
				delete_hashtable(h_bib, nume_carte, strlen(nume_carte) + 1);
				// daca cartea exista o sterg din h_bib
			}


			add_hashtable(&h_bib, nume_carte, strlen(nume_carte) + 1, c,
								sizeof(info_carte));
			// adaug cartea creeata la hashtable-ul bibliotecii

			free(c);  // eliberez c pentru ca l-am mai alocat o data
					// in add_hashtable

        } else if (strcmp(comanda, "EXIT") == 0) {
			printf("Books ranking:\n");
			top_carte* c = malloc(h_bib->size * sizeof(top_carte));
			// vector care imi tine o structura care imi retine
			// numele cartii, rating si nr de imprumuturi
			// voi parcurge tot vectorul de liste si fiecare lista in parte
			// pentru a imi recalcula ratingul si a initializa c[l]
			// l numara cate elemente am in vector
			int l = 0;
			for (int k = 0; k < h_bib->size; k++) {
				List* lis = (List*)h_bib->buckets[k];
				if(lis->head != NULL) {
					Node* curent = (Node*)lis->head;
					for (int t =0; t < lis->size; t++) {
						Entry* e = (Entry*)curent->data;
						info_carte* book = e->val;
						if (book->nr_imprumuturi == 0) {
							book->rating = 0.0;
						} else {
							book->rating = (float)(book->sume_note /
												book->nr_imprumuturi);
						}
						strcpy(c[l].nume_carte, e->key);
						c[l].nr_imprumuturi = book->nr_imprumuturi;
						c[l].rating = book->rating;
						l++;
						curent = curent->next;
					}
				}
			}
			// sortez descrescator dupa cele 3 criterii prin interschimbare
			for (int t = 0; t < l - 1; t++) {
				for (int s = t + 1; s < l; s++) {
					if (c[t].rating < c[s].rating) {
						top_carte aux = c[t];
						c[t] = c[s];
						c[s] = aux;
					}
					if (c[t].rating == c[s].rating) {
						if (c[t].nr_imprumuturi < c[s].nr_imprumuturi) {
							top_carte aux = c[t];
							c[t] = c[s];
							c[s] = aux;
						} else if (c[t].nr_imprumuturi ==
									c[s].nr_imprumuturi) {
							if(strcmp(c[t].nume_carte, c[s].nume_carte) > 0) {
								top_carte aux = c[t];
								c[t] = c[s];
								c[s] = aux;
							}
						}
					}
				}
			}
			for (int i = 0; i < l; i++) {
				printf("%d. Name:%s Rating:%.3f Purchases:%d\n", i+1,
					c[i].nume_carte, c[i].rating, c[i].nr_imprumuturi);
			}
			free(c);  // eliberez vectorul folosit pentru a-mi afisa cartile


			printf("Users ranking:\n");
			top_useri* v = malloc(h_users->entries * sizeof(top_useri));
			// vector care imi tine o structura care imi retine
			// numele userului si punctaj
			// voi parcurge tot vectorul de liste si fiecare lista in parte
			// pentru a initializa v[d]
			// d numara cate elemente am in vector
			int d = 0;
			for (int k = 0; k < h_users->size; k++) {
				List* l = (List*)h_users->buckets[k];
				if (l->head != NULL) {
					Node* curent = (Node*)l->head;
					for (int t =0; t < l->size; t++) {
						Entry* e = (Entry*)curent->data;
						user* u = (user*)e->val;
						if (u->banat == 0) {
							strcpy(v[d].nume_user, u->nume_user);
							v[d].punctaj = u->punctaj;
							d++;
						}
						curent = curent->next;
					}
				}
			}
			// sortez descrescator dupa cele 2 criterii prin interschimbare
			for (int t = 0; t < d - 1; t++) {
				for (int s = t + 1; s < d; s++) {
					if (v[t].punctaj < v[s].punctaj) {
						top_useri aux = v[t];
						v[t] = v[s];
						v[s] = aux;
					}
					if (v[t].punctaj == v[s].punctaj) {
						if (strcmp(v[t].nume_user, v[s].nume_user) > 0) {
							top_useri aux = v[t];
							v[t] = v[s];
							v[s] = aux;
						}
					}
				}
			}
			for (int j = 0 ; j < d; j++) {
				printf("%d. Name:%s Points:%d\n", j+1, v[j].nume_user,
														v[j].punctaj);
			}
		    free(v);  // eliberez vectorul folosit pentru a-mi afisa userii
			ok = 1;
			int i;
			for (i = 0; i < h_bib->size; i++) {
				List* lista = (List*)h_bib->buckets[i];
				// trec prin fiecare lista din h_bib si o parcurg
				if (lista == NULL) {
					break;
				}
				if (lista->head != NULL) {
					Node* curent = (Node*)lista->head;
					int j;
					for (j = 0; j < lista->size; j++) {
						if (curent == NULL) {
							break;
						}
						Entry* e = (Entry*)curent->data;
					    info_carte*c = (info_carte*)e->val;
						Hashtable* h = (Hashtable*) c->carte;

						free_hashtable(h);
						// eliberes hashtable-ul cartii din val-ul
						// din fiecare data al unui nod
						curent = curent->next;
					}
				}
			}
			free_hashtable(h_bib);  // eliberez hashtable-ul biblioteca
			free_hashtable(h_users);  // eliberez hashtable-ul de useri
		} else if (strcmp(comanda, "GET_DEF") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, " \"\n");
			strcpy(def_key, params);
			// caut cartea
			void* p = find_hashtable(h_bib, nume_carte,
								strlen(nume_carte) + 1);
			if (p != NULL) {
				info_carte* c = p;
				// caut definitia in carte daca exista cartea
				void* p2 = find_hashtable(c->carte, def_key,
										strlen(def_key) + 1);
				if (p2 != NULL) {
					printf("%s\n", (char*)p2);
				} else {
					printf("The definition is not in the book.\n");
				}
			} else {
				printf("The book is not in the library.\n");
			}
		} else if (strcmp(comanda, "GET_BOOK") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			// imi gaseste cartea
			void* p = find_hashtable(h_bib, nume_carte,
								strlen(nume_carte) + 1);
			if (p == NULL) {
				printf("The book is not in the library.\n");
			} else {
				info_carte* c = p;
				// ii calculez rating
				if (c->nr_imprumuturi == 0) {
					c->rating = 0.0;
				} else {
					c->rating = (float)(c->sume_note / c->nr_imprumuturi);
				}
				printf("Name:%s Rating:%.3f Purchases:%d\n", nume_carte,
											c->rating, c->nr_imprumuturi);
			}
		} else if (strcmp(comanda, "RMV_BOOK") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			// caut cartea in biblioteca
			void* p = find_hashtable(h_bib, nume_carte,
						strlen(nume_carte) + 1);  // o structura info_carte
			if (p == NULL) {
				printf("The book is not in the  dfd library.\n");
			} else {
				// o scot si trebuie eliberat mai intai
				// hashtable-ul mic pentru carte
				info_carte* c  = p;
				free_hashtable(c->carte);
				delete_hashtable(h_bib, nume_carte, strlen(nume_carte) + 1);
			}
		} else if (strcmp(comanda, "ADD_DEF") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, " \n");
			strcpy(key, params);
			params = strtok(NULL, " \n");
			strcpy(value, params);
			// caut mai intai cartea
			void* p = find_hashtable(h_bib, nume_carte,
					strlen(nume_carte) + 1);  // o structura info_carte
			if (p == NULL) {
				printf("The book is not in the library.\n");

			} else {
				// adaug definitia si cuvantul la hashtable-ul cartii
				info_carte* c  = p;
				add_hashtable(&(c->carte), key, strlen(key) + 1, value,
														strlen(value) + 1);
			}
    	} else if (strcmp(comanda, "RMV_DEF") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, " \n");
			strcpy(key, params);
			// caut cartea in h_bib
			void* p = find_hashtable(h_bib, nume_carte,
									strlen(nume_carte) + 1);
			if (p == NULL) {
				printf("The book is not in the library.\n");
			} else {
				// caut definitia in c->carte
				info_carte* c = p;
				void* p2 = find_hashtable(c->carte, key, strlen(key) + 1);
				if (p2 != NULL) {
					// o elimin din hashtable-ul cartii
					delete_hashtable(c->carte, key, strlen(key) + 1);
				} else {
					printf("The definition is not in the book.\n");
				}
			}
		} else if (strcmp(comanda, "ADD_USER") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, " \"\n");
			strcpy(user_nume, params);
			// verific daca userul e deja in hashtable-ul pt useri
			void* p = find_hashtable(h_users, user_nume,
										strlen(user_nume) + 1);
			if (p != NULL) {
				printf("User is already registered.\n");
			} else {
				// initializez datele pentru un user nou
				// si apoi il adaug in h_users
				nr_u++;
				user* u = calloc(1, sizeof(user));
				u->banat = 0;
				u->imprumutat = 0;
				u->punctaj = 100;
				u->termen_limita = 0;
				strcpy(u->nume_user, user_nume);
				add_hashtable(&h_users, user_nume, strlen(user_nume) + 1, u,
																sizeof(user));
				free(u);  // mai aloc o data si in add_hashtable
			}
		} else if (strcmp(comanda, "BORROW") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, " \"\n");
			strcpy(user_nume, params);
			params = strtok(NULL, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, " \"\n");
			int nr = atoi(params);
			// verific daca exista userul in h_users
			void* p = find_hashtable(h_users, user_nume, strlen(user_nume) + 1);
			if (p == NULL) {
				printf("You are not registered yet.\n");
			} else {
				user* u = (user*)p;
				if (u->banat == 1) {
					printf("You are banned from this library.\n");  // e banat
				} else {
					if (u->imprumutat == 1) {  // daca deja a imprumutata una
						printf("You have already borrowed a book.\n");
					} else {
						// daca exista cartea in h_bib
						void* p2 = find_hashtable(h_bib, nume_carte,
												strlen(nume_carte) + 1);
						info_carte* c = (info_carte*)p2;
						if (p2 == NULL) {
							printf("The book is not in the library.\n");
						} else {
							if (c->imprumutat == 1) {  // carte deja imprumutat
								printf("The book is borrowed.\n");
							} else {
								c->imprumutat = 1;  // e imprumutata cartea
								u->imprumutat = 1;  // userul are deja o carte
								strcpy(u->nume_carte, nume_carte);
								strcpy(u->nume_user, user_nume);
								u->termen_limita = nr;  // zile pana la predat
							}
						}
					}
				}
			}
		} else if (strcmp(comanda, "RETURN") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, " \"\n");
			strcpy(user_nume, params);
			params = strtok(NULL, "\"\n");
			strcpy(nume_carte, params);
			params = strtok(NULL, " \"\n");
			int zile = atoi(params);
			params = strtok(NULL, " \"\n");
			int rating = atoi(params);
			// Se garanteaza ca dau return userii deja inregistrati in sistem
			void* p = find_hashtable(h_users, user_nume,
									strlen(user_nume) + 1);
			if (p != NULL) {
				user* u = (user*)p;
				if (u->banat == 1) {
					printf("You are banned from this library.\n");
				} else {
					if(strcmp(nume_carte, u->nume_carte) != 0 ||
											u->nume_carte == NULL) {
						printf("You didn't borrow this book.\n");
					} else {
						// calculez punctajul userului in functie de dupa cate
						// zile a returnat cartea
						if (u->termen_limita - zile >= 0){
							u->punctaj += (u->termen_limita - zile);
						} else {
							u->punctaj -= (zile - u->termen_limita) * 2;
						}
						if (u->punctaj < 0) {  // verific sa nu fi ajuns
							// punctajul negativ al userului
							u->banat = 1;
							nr_u--;
							printf("The user %s has been banned from this library.\n", u->nume_user);
						}
						// caut cartea in h_bib si modific campurile ei
						void* p2 = find_hashtable(h_bib, nume_carte,
												strlen(nume_carte) + 1);
						info_carte* c = (info_carte*)p2;
						c->imprumutat = 0;  // acum poate fi iar imprumutata
						c->sume_note += rating;
						u->imprumutat = 0;  // userul poate imprumuta iar
						c->nr_imprumuturi++;
					}
				}
			}
		} else if (strcmp(comanda, "LOST") == 0) {
			fgets(linie, 100, stdin);
			params = strtok(linie, " \"\n");
			strcpy(user_nume, params);
			params = strtok(NULL, "\"\n");
			strcpy(nume_carte, params);
			// gasesc userul in h_users
			void* p = find_hashtable(h_users, user_nume,
									strlen(user_nume) + 1);
			if (p == NULL) {
				printf("You are not registered yet.\n");
			} else {
				user* u = (user*)p;
				if (u->banat == 1) {
					printf("You are banned from this library.\n");
				} else {
					// gasesc cartea si o scot din h_bib
					void* p = find_hashtable(h_bib, nume_carte,
							strlen(nume_carte) + 1);  // o structura info_carte
					info_carte* c  = p;
					free_hashtable(c->carte);
					delete_hashtable(h_bib, nume_carte,
										strlen(nume_carte) + 1);
					// scad punctajul userului , refac campurile lui
					// si vad daca a ajuns la punctaj negativ
					u->punctaj = u->punctaj - 50;
					u->imprumutat = 0;
					u->termen_limita = 0;
					if (u->punctaj < 0) {
						printf("The user %s has been banned from this library.\n", u->nume_user);
						u->banat = 1;
					}
				}
			}
		}
	}
    return 0;
}
