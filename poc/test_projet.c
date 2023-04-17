#include "test_projet.h"
#include <stdint.h>
#include <stdio.h>
#include "string.h"

#define MAX 250

 struct trame {
	uint8_t boucle;
	uint8_t nb_iter;
	uint8_t timer_court;
	uint8_t timer_moyen;
	uint8_t timer_long;
	uint8_t taille_msg;
	char msg_trame[MAX];
	char fin_trame;
	} ;

unsigned char msg[] = { 1, 0, 10, 50, 100, 14, 'C', 'o', 'u', 'c', 'o','t', 'u', ' ', 'c', 'o', 'p', 'a', 'i', 'n', '\n' };

struct trame retour;
void affiche_trame(struct trame);

int main(void)
{
	struct trame trame_recue;
	trame_recue = decode_trame(msg);
	affiche_trame(trame_recue);
}

struct trame decode_trame(char* msg)
{
	retour.boucle = msg[0];
	retour.nb_iter = msg[1];
	retour.timer_court = msg[2];
	retour.timer_moyen = msg[3];
	retour.timer_long = msg[4];
	retour.taille_msg = msg[5];

	for (int j = 0; j < retour.taille_msg; j++)
	{
		retour.msg_trame[j] = msg[j+6];
	}
	retour.fin_trame = '\n';

	return retour;
}

void affiche_trame(struct trame trame_recue)
{
	printf("boucle = %d\n", trame_recue.boucle);
	printf("itération = %d\n", trame_recue.nb_iter);
	printf("timer 1 = %d\n", trame_recue.timer_court);
	printf("timer 2 = %d\n", trame_recue.timer_moyen);
	printf("timer 3= %d\n", trame_recue.timer_long);
	printf("taille msg = %d\n", trame_recue.taille_msg);
	printf("msg = %s\n", &trame_recue.msg_trame);
	printf("fin = %c\n", trame_recue.fin_trame);

}