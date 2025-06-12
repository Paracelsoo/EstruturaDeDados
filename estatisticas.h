#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include "csv.h"

typedef struct {
    double media;
    double desvio;
} Estatistica;

Estatistica calcular_stats(BikeHour** dados, int count);
void classificar_picos(BikeHour** dados, int count);

#endif

