/*
 * deadlock.c
 *
 *  Created on: 12 jul. 2020
 *      Author: utnso
 */

#include "deadlock.h"

void deteccionYCorreccionDeadlock(void) {
	while (list_size(entrenadores_blocked_full) >= 2) {
		corregirUnDeadlock();	// TODO ver como planificar varios deadlocks distintos
	}

	/*
	 * Si queda 1 solo entrenador, significa que se capturó un pokemon que no tendría que haberse capturado
	 */
}

void corregirUnDeadlock(void) {
	// Buscar un entrenador que necesite un pokemon que otro entrenador no necesite
	int cantidad_enrenadores_full = list_size(entrenadores_blocked_full);
	log_debug(logger, "Se lanza algoritmo de detección de deadlock");

	t_tcb* necesitado;
	t_tcb* buscado;
	char* pokemon_necesitado;
	char* pokemon_a_dar;

	t_intercambio* intercambio = malloc(sizeof(t_intercambio));

	for (int pos_necesitado = 0; pos_necesitado < cantidad_enrenadores_full; pos_necesitado++) {
		necesitado = list_get(entrenadores_blocked_full, pos_necesitado);
		for (int pos_buscado = 0; pos_buscado < cantidad_enrenadores_full; pos_buscado++) {

			if (pos_necesitado == pos_buscado)	// Si son el mismo salteo
				continue;

			buscado = list_get(entrenadores_blocked_full, pos_buscado);

			pokemon_necesitado = pokemonQueNoNecesiteYelOtroSi(buscado->entrenador, necesitado->entrenador);

			if (pokemon_necesitado != NULL) {
				log_debug(logger, "El entrenador %d necesita un %s y se lo va a dar el entrenador %d", necesitado->entrenador->id_entrenador, pokemon_necesitado, buscado->entrenador->id_entrenador);

				intercambio->tcb = buscado;
				intercambio->su_pokemon = pokemon_necesitado;

				// Trato de darle un pokemon que necesite, sino uno random
				pokemon_a_dar = pokemonQueNoNecesiteYelOtroSi(necesitado->entrenador, buscado->entrenador);
				if (pokemon_a_dar == NULL) {
					t_inventario* inv = (t_inventario*) list_get(pokemonesNoNecesariosDe(necesitado->entrenador), 0);
					pokemon_a_dar = inv->pokemon->name;
				}

				intercambio->mi_pokemon = pokemon_a_dar;

				necesitado->intercambio = intercambio;

				cambiarDeLista(buscado, entrenadores_blocked_full, entrenadores_blocked_waiting_trade);
				cambiarDeLista(necesitado, entrenadores_blocked_full, entrenadores_ready);
				return;
			}
		}
	}
}