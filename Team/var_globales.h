/*
 * var_globales.h
 *
 *  Created on: 1 may. 2020
 *      Author: utnso
 */

#ifndef VAR_GLOBALES_H_
#define VAR_GLOBALES_H_

#include <pthread.h>
#include <commons/config.h>
#include <commons/log.h>
#include <commons/collections/list.h>
#include <stdint.h>

typedef struct {
	t_list* lista;
	pthread_mutex_t mutex;
} t_cola_planificacion;

extern t_config* config;
extern t_log* logger;
extern t_log* logger_extra;
extern uint32_t process_id;

extern t_cola_planificacion* entrenadores_new; // lista de t_tcb*
extern t_cola_planificacion* entrenadores_ready; // lista de t_tcb*
extern t_cola_planificacion* entrenadores_blocked_idle; // lista de t_tcb* que se bloquea sin tareas
extern t_cola_planificacion* entrenadores_blocked_waiting_caught; // acá van los que se bloquean esperando a recibir un caught
extern t_cola_planificacion* entrenadores_blocked_full; // Bloqueados por no poder agarrar mas pokemones, pero no cumplen su objetivo
extern t_cola_planificacion* entrenadores_blocked_waiting_trade;
extern t_cola_planificacion* entrenadores_exit; // lista de t_tcb*

extern t_list* actuales_global;
extern t_list* pokemones_planificados;

// mutexs

extern pthread_mutex_t mutex_entrenadores_blocked_full;	// SACAR
extern pthread_mutex_t mutex_actuales_global;
extern pthread_mutex_t mutex_pokemones_planificados;

// logs
extern int log_cpus_totales;
extern int log_cambios_contexto;
extern int log_deadlocks_producidos;

#endif /* VAR_GLOBALES_H_ */
