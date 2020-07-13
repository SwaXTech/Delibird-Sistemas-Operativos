#include "team.h"

t_log* logger = NULL;
t_config* config = NULL;

int main() {
	char* logfile;

	config = leer_config();

	logfile = config_get_string_value(config, "LOG_FILE");
	process_id = config_get_int_value(config, "PROCESS_ID");

	logger = iniciar_logger(logfile);

	inicializar_team();

	log_debug(logger, "El team finaliza satisfactoriamente");
	terminar_programa(logger, config);
	return 1;
}


/*
 * Iniciarlizar el proceso team y que quede corriendo
 */

void inicializar_team(void) {
	inciarListasMensajes();
	iniciarPlanificador();
	cargarEntrenadores(); // Tambien se define el objetivo global aca
	enviarGetsAlBroker();
	escucharAlGameboy();
	suscribirseAlBroker();	// Tiene que estar a lo ultimo ya que tiene los wait ara que los entrenadores finalicen
}

t_log* iniciar_logger(char* logfile)
{
	return log_create(logfile, "Team", true, LOG_LEVEL_DEBUG);
}

t_config* leer_config(void) {
	return config_create("team.config");
}

void terminar_programa(t_log* logger, t_config* config) {
	if(logger != NULL){
		log_destroy(logger);
	}
	if(config != NULL){
		config_destroy(config);
	}
	exit(1);
}
