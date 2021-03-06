/*
 * gamecard.c
 *
 *  Created on: 17 jun. 2020
 *      Author: utnso
 */

#include "gamecard.h"

t_log* logger = NULL;
t_config* config = NULL;
t_bitarray* bitarray = NULL;
char* ruta_punto_montaje = NULL;
uint32_t tiempo_reconexion = 0;

int main() {

	char* logfile;

	int init = 0;

	config = leer_config();

	logfile = config_get_string_value(config, "LOG_FILE");
	ruta_punto_montaje = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	tiempo_reconexion = config_get_int_value(config, "TIEMPO_DE_REINTENTO_CONEXION");
	process_id = config_get_int_value(config, "PROCESS_ID");

	logger = iniciar_logger(logfile);

	pthread_mutex_init(&mx_open, NULL);
	pthread_mutex_init(&mx_bitmap, NULL);
	pthread_mutex_init(&mx_metadata_blocks, NULL);

	bitarray = iniciar_bitarray();

	mx_dict = dictionary_create();

	escucharAlGameboy();
	suscribirGameCardAlBroker();

	return 1;
}

t_log* iniciar_logger(char* logfile) {
	return log_create(logfile, "GameCard", true, LOG_LEVEL_INFO);
}

t_config* leer_config(void) {
	return config_create("gamecard.config");
}

t_bitarray* iniciar_bitarray(void) {


	char* bitmap_path = NULL;
	t_bitarray* array = NULL;
	//struct stat statbuf;
	size_t bitmap_size;
	char* buffer = NULL;
	char* metadata_path;
	t_config* metadata;

	bitmap_path = concat_string(ruta_punto_montaje, "/Metadata/Bitmap.bin");
	metadata_path = concat_string(ruta_punto_montaje, "/Metadata/Metadata.bin");

	metadata = config_create(metadata_path);

	bitmap_size = config_get_int_value(metadata, "BLOCKS");

	config_destroy(metadata);

	//stat(bitmap_path, &statbuf);

	int file = open(bitmap_path, O_RDWR);

	if(file == -1){
	    FILE* f = fopen(bitmap_path, "wb");

	    for(int i = 0; i < bitmap_size/8; i++){
	      uint8_t bit = 0;
	      fwrite(&bit, sizeof(uint8_t),1 ,f);
	    }

	    fflush(f);
	    fclose(f);

	    file = open(bitmap_path, O_RDWR);
	 }
	//fread(buffer, 1, bitmap_size, file);
	//fclose(file);

	buffer = mmap(NULL, bitmap_size, PROT_WRITE | PROT_READ, MAP_SHARED, file, 0);

	array = bitarray_create_with_mode(buffer, bitmap_size, MSB_FIRST);

	free(bitmap_path);
	free(metadata_path);

	return array;
}

