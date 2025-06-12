#ifndef HASH_FILTROS_H
#define HASH_FILTROS_H

#include "hash.h"

// Classifica horários de pico com base nas contagens (cnt).
// Retorna um array alocado com os registros ordenados decrescentemente por cnt.
// *out_size receberá o número de elementos retornados.
BikeData* classificar_horarios_pico(HashTable* table, size_t* out_size);

// Filtra os registros pelo código do clima (weathersit) e ordena por demanda (cnt) decrescente.
// Retorna um array alocado com os registros filtrados e ordenados.
// *out_size receberá o número de elementos retornados.
BikeData* filtrar_por_clima_ordenar_demanda(HashTable* table, int clima, size_t* out_size);

#endif
