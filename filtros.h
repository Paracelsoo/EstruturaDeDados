#ifndef FILTROS_H
#define FILTROS_H

#include "csv.h"

void filtrar_por_clima(BikeHour* dados, int total, int clima, BikeHour** saida, int* saida_count);
void ordenar_por_demanda(BikeHour** dados, int count);

#endif
