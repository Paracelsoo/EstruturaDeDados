// ==== FILE: main.cpp ====
#include "avl.h"
#include "csv_reader.h"
#include "estatisticas.h"
#include <iostream>
#include <chrono>
#include <windows.h>
#include "remover.h"
#include "imprimir.h"
#include "contar.h"
#include <psapi.h>
#include "random_generator.h"
#include <stdio.h>
#include <vector>
#include <thread>
#include <functional>
#include <iostream>
#include <fstream>
#include "avl.h"
#include "csv_reader.h"
#include <iostream>
#include <string>
#include <cmath>
#include <atomic>
#include <mutex>
#include <functional>

//R13
class BatchProcessor {
public:
    explicit BatchProcessor(size_t batch_size = 100, int delay_ms = 200)
        : batch_size_(batch_size), delay_ms_(delay_ms) {}
    
    template<typename T, typename ProcessFunc>
    void process(std::vector<T>& data, ProcessFunc&& func) {
        for (size_t i = 0; i < data.size(); i += batch_size_) {
            size_t end = std::min(i + batch_size_, data.size());
            
            // Processa o lote atual
            for (size_t j = i; j < end; j++) {
                func(data[j]);
            }
            
            // Atraso artificial entre lotes
            if (end < data.size()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms_));
            }
        }
    }

private:
    size_t batch_size_;
    int delay_ms_;
};

class LowFrequencySimulator {
public:
    /// Construtor com frequência-alvo em GHz e modo verboso opcional
    explicit LowFrequencySimulator(double target_ghz = 1.0, bool verbose = false)
        : target_ghz(target_ghz), running(false), verbose(verbose) {
        calibrate_cpu_speed();
    }

    /// Destrutor garante que a thread será finalizada
    ~LowFrequencySimulator() {
        stop();
    }

    /// Executa uma função simulando frequência reduzida
    template<typename Func>
    void execute(Func&& func) {
        start();
        try {
            func();
        } catch (...) {
            stop();
            throw;
        }
        stop();
    }

private:
    double target_ghz;
    double estimated_ghz;
    std::atomic<bool> running;
    std::thread throttle_thread;
    std::mutex control_mutex;
    bool verbose;

    /// Inicia a simulação com throttling
    void start() {
        std::lock_guard<std::mutex> lock(control_mutex);
        if (running) return;

        running = true;
        if (verbose) {
            std::cout << "[LFS] Iniciando simulação com alvo de " << target_ghz << " GHz...\n";
        }

        throttle_thread = std::thread([this]() {
            while (running.load()) {
                auto start = std::chrono::high_resolution_clock::now();

                simulate_work();

                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                auto target_time = static_cast<long long>(elapsed.count() * (estimated_ghz / target_ghz));
                auto remaining_time = std::chrono::microseconds(target_time) - elapsed;

                if (remaining_time.count() > 0) {
                    std::this_thread::sleep_for(remaining_time);
                }
            }
        });
    }

    /// Para a thread de simulação
    void stop() {
        std::lock_guard<std::mutex> lock(control_mutex);
        if (!running) return;

        running = false;
        if (throttle_thread.joinable()) {
            throttle_thread.join();
        }

        if (verbose) {
            std::cout << "[LFS] Simulação encerrada.\n";
        }
    }

    /// Estima a velocidade da CPU (aproximadamente)
    void calibrate_cpu_speed() {
        constexpr double reference_ghz = 3.0;
        volatile double dummy = 0;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < 1000000; ++i) {
            dummy += std::sqrt(i);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        estimated_ghz = reference_ghz * (1000000.0 / elapsed);
        if (verbose) {
            std::cout << "[LFS] Estimativa de GHz atual: " << estimated_ghz << " GHz\n";
        }
    }

    /// Simula trabalho pesado da CPU
    void simulate_work() {
        volatile double dummy = 0;
        for (int i = 1; i < 10000; ++i) {
            dummy += std::sqrt(i) * std::log(i + 1);
        }
    }
};


int main() {
	//lingua
	 setlocale(LC_ALL, "pt_BR");
    
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Só no Windows
    #endif
    	//ver RAM
	
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE hProcess = GetCurrentProcess();
	   	if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        // Exibe a quantidade de memória usada em bytes
        	std::cout << "Uso de memória RAM utilizada: " << pmc.WorkingSetSize << " bytes" << std::endl;
    	} else {
        std::cerr << "Erro ao obter informações de memória." << std::endl;
    	}

    // Libera o handle do processo
    CloseHandle(hProcess);
	//fim da ram
			std::cout << "Simulação CPU de 1 GHz\n";
   		std::cout << "TESTE INSERIR, BUSCA E REMOVER\n";
    	LowFrequencySimulator lfs(1.0, true);  // Modo verboso ligado
    	lfs.execute([]() {
	
		
		No* raiz = nullptr; // árvore inicial vazia
    	auto startinserir2 = std::chrono::high_resolution_clock::now();
		auto endinserir2 = std::chrono::high_resolution_clock::now();
 		std::cout << "====Carregando dados do CSV====\n";
		startinserir2 = std::chrono::high_resolution_clock::now();
    	std::vector<Registro> dados = lerCSV("hour.csv");
		for (const auto& r : dados) {
    	    raiz = inserir(raiz, r);
    	    }
    	endinserir2 = std::chrono::high_resolution_clock::now();
    	std::cout << "\n[INFO] Inserção concluída em " << std::chrono::duration<double>(endinserir2 - startinserir2).count() << "s\n\n";
    	std::cout << "Dados do CSV inseridos na árvore AVL.\n";
        std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
		auto startbuscar2 = std::chrono::high_resolution_clock::now();
		auto endbuscar2 = std::chrono::high_resolution_clock::now();
 	
		int chave = gerar_numero_aleatorio();
        std::cout << "\n Teste de Busca - Aleatória\n ";
		std::cout << "\n Buscar:" << chave;
		
        startbuscar2 = std::chrono::high_resolution_clock::now();
        buscar(raiz, chave);
   		endbuscar2 = std::chrono::high_resolution_clock::now();
    	std::cout << "[INFO] Busca concluída em " << std::chrono::duration<double>(endbuscar2 - startbuscar2).count() << "s\n\n";

		std::cout << "\n=========== TESTE remoção ===========\n" << std::endl;
		std::cout << "\nContagem antes de remover\n " << std::endl;
   		contarNos(raiz);
   		auto startremover2 = std::chrono::high_resolution_clock::now();
		auto endremover2 = std::chrono::high_resolution_clock::now();
 	  	std::cout << "\n Remover: " << chave;
		
		startremover2 = std::chrono::high_resolution_clock::now();
    	raiz = remover(raiz, chave);
		endremover2 = std::chrono::high_resolution_clock::now();
    	std::cout << "[INFO] Remoção concluída em " << std::chrono::duration<double>(endremover2 - startremover2).count() << "s\n\n";
    	buscar(raiz, chave);
 		std::cout << "\n\n[INFO] LATÊNCIA:" << std::chrono::duration<double>((endinserir2 - startinserir2 + endbuscar2 - startbuscar2 + endremover2 - startremover2)/3).count() << "s\n\n";
		std::cout << "\nContagem depois de remover\n " << std::endl;
    	contarNos(raiz);
 	    std::cout << "\n======Processo simulado concluído.\n";

    });
    std::cout << "\n\n SIMULAÇÃO com inserção artificial de processamento\n\n";
	std:vector<int> data(1);
    BatchProcessor batch_processor;
    batch_processor.process(data, [](int& item) {
        		
		No* raiz = nullptr; // árvore inicial vazia
    	auto startinserir2 = std::chrono::high_resolution_clock::now();
		auto endinserir2 = std::chrono::high_resolution_clock::now();
 		std::cout << "====Carregando dados do CSV====\n";
		startinserir2 = std::chrono::high_resolution_clock::now();
    	std::vector<Registro> dados = lerCSV("hour.csv");
		for (const auto& r : dados) {
    	    raiz = inserir(raiz, r);
    	    }
    	endinserir2 = std::chrono::high_resolution_clock::now();
    	std::cout << "\n[INFO] Inserção concluída em " << std::chrono::duration<double>(endinserir2 - startinserir2).count() << "s\n\n";
    	std::cout << "Dados do CSV inseridos na árvore AVL.\n";
        std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
		auto startbuscar2 = std::chrono::high_resolution_clock::now();
		auto endbuscar2 = std::chrono::high_resolution_clock::now();
 	
		int chave = gerar_numero_aleatorio();
        std::cout << "\n Teste de Busca - Aleatória\n ";
		std::cout << "\n Buscar:" << chave;
		
        startbuscar2 = std::chrono::high_resolution_clock::now();
        buscar(raiz, chave);
   		endbuscar2 = std::chrono::high_resolution_clock::now();
    	std::cout << "[INFO] Busca concluída em " << std::chrono::duration<double>(endbuscar2 - startbuscar2).count() << "s\n\n";

		std::cout << "\n=========== TESTE remoção ===========\n" << std::endl;
		std::cout << "\nContagem antes de remover\n " << std::endl;
   		contarNos(raiz);
   		auto startremover2 = std::chrono::high_resolution_clock::now();
		auto endremover2 = std::chrono::high_resolution_clock::now();
 	  	std::cout << "\n Remover: " << chave;
		
		startremover2 = std::chrono::high_resolution_clock::now();
    	raiz = remover(raiz, chave);
		endremover2 = std::chrono::high_resolution_clock::now();
    	std::cout << "[INFO] Remoção concluída em " << std::chrono::duration<double>(endremover2 - startremover2).count() << "s\n\n";
    	buscar(raiz, chave);
 		std::cout << "\n\n[INFO] LATÊNCIA:" << std::chrono::duration<double>((endinserir2 - startinserir2 + endbuscar2 - startbuscar2 + endremover2 - startremover2)/3).count() << "s\n\n";
		std::cout << "\nContagem depois de remover\n " << std::endl;
    	contarNos(raiz);
 	    std::cout << "\n======Processo simulado concluído.\n";
    });
        No* raiz = nullptr; // árvore inicial vazia
    auto startinserir = std::chrono::high_resolution_clock::now();
	auto endinserir = std::chrono::high_resolution_clock::now();
 	std::cout << "Carregando dados do CSV\n";
	startinserir = std::chrono::high_resolution_clock::now();
    std::vector<Registro> dados = lerCSV("hour.csv");
	for (const auto& r : dados) {
        raiz = inserir(raiz, r);
        }
    endinserir = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Inserção concluída em " << std::chrono::duration<double>(endinserir - startinserir).count() << "s\n\n";
    
	std::cout << "Dados do CSV inseridos na árvore AVL.\n";
	std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
    

	int opcao;

    do {
        std::cout << "\n=== MENU ===\n";
        std::cout << "1. Inserir dado manualmente\n";
        std::cout << "2. Tempo de busca aleatória\n";
        std::cout << "3. Buscar por chave (index)\n";
        std::cout << "4. Remover por index\n";
        std::cout << "5. Latência em teste aleatório\n";std::cout << "0. Sair\n";
        std::cout << "Escolha: ";
        std::cin >> opcao;

        if (opcao == 1) {
            Registro r;
            std::cout << "Digite o index: ";
            std::cin >> r.instant;
            std::cout << "Digite a data (dteday): ";
            std::cin >> r.dteday;
            std::cout << "Digite a hora (hr): ";
            std::cin >> r.hr;
            std::cout << "Digite a temperatura (temp): ";
            std::cin >> r.temp;
            std::cout << "Digite a umidade (hum): ";
            std::cin >> r.hum;
            std::cout << "Digite a velocidade do vento (windspeed): ";
            std::cin >> r.windspeed;
            std::cout << "Digite o tipo de clima (weathersit): ";
            std::cin >> r.weathersit;
            std::cout << "Digite o total de usuários (cnt): ";
            std::cin >> r.cnt;

            raiz = inserir(raiz, r);
            std::cout << "Registro inserido com sucesso!\n";

        } else if (opcao == 2) {
         std::cout << "Registro inserido com sucesso!\n";
   		std::cout << "\n\n Tempo médio para buscar um número aleatório:";
   		auto startlatALE = std::chrono::high_resolution_clock::now();
		auto endlatALE = std::chrono::high_resolution_clock::now();
  		startlatALE = std::chrono::high_resolution_clock::now();
  	
 		startlatALE = std::chrono::high_resolution_clock::now();
  	
 		for(int i=0;i<10;i++){
		int num = gerar_numero_aleatorio();
   
  	 	buscar(raiz, num);
   	
		}
		endlatALE = std::chrono::high_resolution_clock::now();
 		std::cout << "\n\n[INFO] Média de tempo de Busca Aleatória:" << std::chrono::duration<double>((endlatALE - startlatALE)/10).count() << "s\n\n";
	
        } else if (opcao == 3) {
            int chave;
            std::cout << "Digite o index para busca: ";
            std::cin >> chave;
            buscar(raiz, chave);

        } else if (opcao == 4) {
            int chave;
            std::cout << "Digite o index para remover: ";
            std::cin >> chave;
            remover(raiz,chave);
        
         } else if (opcao == 5) {
           	std::cout << "\nLatência em Buscar e Remover Aleatório + inserir inicial";
       		auto startlatALE3 = std::chrono::high_resolution_clock::now();
			auto endlatALE3 = std::chrono::high_resolution_clock::now();
  			int chave2 = gerar_numero_aleatorio();
    	    std::cout << "\n Teste de Busca - Aleatória\n ";
			std::cout << "\n Buscar e remover chave: " << chave2;
		
		  	startlatALE3 = std::chrono::high_resolution_clock::now();
  			buscar(raiz, chave2);
			raiz = remover(raiz, chave2);
			endlatALE3 = std::chrono::high_resolution_clock::now();
	
 			std::cout << "\n\n[INFO] LATÊNCIA:" << std::chrono::duration<double>(endlatALE3 - startlatALE3).count() << "s\n\n";


        } else if (opcao == 0) {
            std::cout << "Saindo...\n";
        } else {
            std::cout << "Opção inválida.\n";
        }

    } while (opcao != 0);
	
    return 0;
}
