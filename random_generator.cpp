#include "random_generator.h"
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <iostream>

Registro criarRegistroAleatorio() {
    Registro r;
    std::srand(std::time(nullptr));

    r.instant = std::rand() % 10000;
    
    // Gerar data no formato "AAAA-MM-DD"
    int year = 2011 + (std::rand() % 2); // 2011 ou 2012
    int month = 1 + (std::rand() % 12);
    int day = 1 + (std::rand() % 28);
    
    std::ostringstream oss;
    oss << year << "-" 
        << std::setw(2) << std::setfill('0') << month << "-" 
        << std::setw(2) << std::setfill('0') << day;
    
    
    r.season = 1 + (std::rand() % 4);
    r.yr = (year == 2011) ? 0 : 1; // 0=2011, 1=2012 (como no CSV original)
    r.mnth = month;
    r.hr = std::rand() % 24;
    r.holiday = std::rand() % 2;
    r.weekday = std::rand() % 7;
    r.workingday = std::rand() % 2;
    r.weathersit = 1 + (std::rand() % 4);
    r.temp = static_cast<float>(std::rand() % 1001) / 1000.0f;
    r.atemp = static_cast<float>(std::rand() % 1001) / 1000.0f;
    r.hum = static_cast<float>(std::rand() % 101) / 100.0f;
    r.windspeed = static_cast<float>(std::rand() % 1001) / 1000.0f;
    r.casual = std::rand() % 100;
    r.registered = std::rand() % 500;
    r.cnt = r.casual + r.registered;
	//MOSTRAR O DADO cnt--> std::cout << "\n\n[dado novo] Nó com instant=" << r.instant ;
    return r;
}


#include <random>
#include <stdexcept>

int gerar_numero_aleatorio() {
    // Inicializa o gerador de números aleatórios
    static std::random_device rd;  // Dispositivo de entropia
    static std::mt19937 gen(rd()); // Motor Mersenne Twister
    
    // Define a distribuição para o intervalo [1, 17379]
    static std::uniform_int_distribution<int> dist(1, 17379);
    
    return dist(gen);
}

// Versão alternativa com seed configurável (para testes reproduzíveis)
int gerar_numero_aleatorio(unsigned int seed) {
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<int> dist(1, 17379);
    return dist(gen);
}