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

//R7
#include <chrono>
#include <thread>
#include <atomic>
#include <cmath>

class LowFrequencySimulator {
public:
    // Configura a velocidade desejada (em GHz)
    explicit LowFrequencySimulator(double target_ghz = 1.0) 
        : target_ghz(target_ghz), running(false) {
        // Estima a velocidade atual (aproximada)
        calibrate_cpu_speed();
    }

    ~LowFrequencySimulator() {
        stop();
    }

    void start() {
        if (running) return;
        running = true;
        
        // Thread que controla o throttling
        throttle_thread = std::thread([this]() {
            while (running) {
                auto start = std::chrono::high_resolution_clock::now();
                
                // Realiza trabalho controlado
                simulate_work();
                
                // Calcula o tempo que deveria levar na frequência alvo
                auto end = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                auto target_time = static_cast<long long>(elapsed.count() * (estimated_ghz / target_ghz));
                
                // Ajusta o tempo de espera
                auto remaining_time = std::chrono::microseconds(target_time) - elapsed;
                if (remaining_time.count() > 0) {
                    std::this_thread::sleep_for(remaining_time);
                }
            }
        });
    }

    void stop() {
        running = false;
        if (throttle_thread.joinable()) {
            throttle_thread.join();
        }
    }

    template<typename Func>
    void execute(Func&& func) {
        // Envolve a execução com o throttling
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

    void calibrate_cpu_speed() {
        // Mede o desempenho bruto da CPU por 100ms
        volatile double dummy = 0;
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 1000000; ++i) {
            dummy += std::sqrt(i);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        // Estimativa grosseira (baseada em experimentação)
        estimated_ghz = 3.0 * (1000000.0 / elapsed); // Ajuste conforme necessário
    }

    void simulate_work() {
        // Padrão de trabalho que consome CPU
        volatile double dummy = 0;
        for (int i = 0; i < 10000; ++i) {
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

//R7
	std::cout << "[INFO] Iniciando Teste com Redução da frequência do processador...\n";
	// Exemplo de uso:
    LowFrequencySimulator lfs(1.0); // Simula CPU de 1 GHz

    lfs.execute([]() {
        // Código que será executado como se estivesse em CPU de 1 GHz
         No* raiz = nullptr;
 	setlocale(LC_ALL, "pt_BR");
    
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Só no Windows
    #endif
    
    //definisdo o tempo para latencia
	auto startlatR7 = std::chrono::high_resolution_clock::now();
	auto endlatR7 = std::chrono::high_resolution_clock::now();
 		
	
 	
    std::cout << "[INFO] Iniciando leitura do CSV...\n";
	auto start = std::chrono::high_resolution_clock::now();
    std::vector<Registro> dados = lerCSV("hour.csv");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
    std::cout << "[INFO] Tempo de leitura: " << std::chrono::duration<double>(end - start).count() << "s\n\n";

    std::cout << "[INFO] Inserindo dados na árvore AVL...\n";
    //inicio latencia
    startlatR7 = std::chrono::high_resolution_clock::now();

	start = std::chrono::high_resolution_clock::now();
    for (const auto& reg : dados) {
        raiz = inserir(raiz, reg);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Inserção concluída em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
    
    std::cout << "[INFO] Cálculo de estatísticas por hora...\n";
    start = std::chrono::high_resolution_clock::now();
    calcularEstatisticas(raiz);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Estatísticas concluídas em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
	
    std::cout << "\n=========== TESTE DE BUSCA ===========\n" << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();

	No* resultado = buscar(raiz, 5110);  // exemplo de busca

	auto fim = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duracao = fim - inicio;
	std::cout << "[BUSCAR] Tempo de execução: " << duracao.count() << " segundos.\n";
 	std::cout << "\n=========== TESTE remoção ===========\n" << std::endl;
	std::cout << "\nContagem antes de remover\n " << std::endl;
    contarNos(raiz);
    
    raiz = remover(raiz, 120);  // REMOVA E ATUALIZE
	endlatR7 = std::chrono::high_resolution_clock::now();
 	std::cout << "\n\n[INFO] LETÊNCIA:" << std::chrono::duration<double>(endlatR7 - startlatR7).count() << "s\n\n";
	
	std::cout << "\nContagem depois de remover\n " << std::endl;
    
	contarNos(raiz);
    
    });//fim R7
	//fim da R7
	
	//NORMAL
	auto startlatNO = std::chrono::high_resolution_clock::now();
	auto endlatNO = std::chrono::high_resolution_clock::now();
	std::cout << "\n\n[INFO] Iniciando Teste sem Redução da frequência do processador...\n";
	
    No* raiz = nullptr;

    std::cout << "[INFO] Iniciando leitura do CSV...\n";
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Registro> dados = lerCSV("hour.csv");
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
    std::cout << "[INFO] Tempo de leitura: " << std::chrono::duration<double>(end - start).count() << "s\n\n";

    std::cout << "[INFO] Inserindo dados na árvore AVL...\n";
    startlatNO = std::chrono::high_resolution_clock::now();
	start = std::chrono::high_resolution_clock::now();
    for (const auto& reg : dados) {
        raiz = inserir(raiz, reg);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Inserção concluída em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
	//inserir com maximo
	for (const auto& reg : dados) {
        raiz = inserir(raiz, reg);
    }
    
    std::cout << "[INFO] Cálculo de estatísticas por hora...\n";
    start = std::chrono::high_resolution_clock::now();
    calcularEstatisticas(raiz);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Estatísticas concluídas em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
	
    std::cout << "\n=========== TESTE DE BUSCA ===========\n" << std::endl;
    auto inicio = std::chrono::high_resolution_clock::now();

	No* resultado = buscar(raiz, 5110);  // exemplo de busca

	auto fim = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duracao = fim - inicio;
	std::cout << "[BUSCAR] Tempo de execução: " << duracao.count() << " segundos.\n";
 	std::cout << "\n=========== TESTE remoção ===========\n" << std::endl;
	std::cout << "\nContagem antes de remover\n " << std::endl;
    contarNos(raiz);
    
    raiz = remover(raiz, 120);  // REMOVA E ATUALIZE
	std::cout << "\nContagem depois de remover\n " << std::endl;
    contarNos(raiz);
    endlatNO = std::chrono::high_resolution_clock::now();
 	std::cout << "\n\n[INFO] LETÊNCIA:" << std::chrono::duration<double>(endlatNO - startlatNO).count() << "s\n\n";

  	//FIM NORMAL
  	std::cout << "\n\n Tempo médio para buscar um número aleatório:";
   	auto startlatALE = std::chrono::high_resolution_clock::now();
	auto endlatALE = std::chrono::high_resolution_clock::now();
  	startlatALE = std::chrono::high_resolution_clock::now();
  	endlatALE = std::chrono::high_resolution_clock::now();
 	
 	startlatALE = std::chrono::high_resolution_clock::now();
  	
 	for(int i=0;i<10;i++){
	int num = gerar_numero_aleatorio();
   
   	buscar(raiz, num);
   	
	}
	endlatALE = std::chrono::high_resolution_clock::now();
 	std::cout << "\n\n[INFO] LETÊNCIA:" << std::chrono::duration<double>((endlatALE - startlatALE)/10).count() << "s\n\n";
	
    
    //escalabilidade
    std::cout << "\n\n TESTE ESCALABILIDADE, ainda não tem\n\n";
	
    //R13
    std::cout << "\n\n TESTE com inserção artificial de processamento\n\n";
	std:vector<int> data(1000);
    BatchProcessor batch_processor;
    batch_processor.process(data, [](int& item) {
        item = rand();
    });
    // Código que será executado como se estivesse em CPU de 1 GHz
        
 	setlocale(LC_ALL, "pt_BR");
    
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Só no Windows
    #endif
    
    //definisdo o tempo para latencia
	auto startlatR7 = std::chrono::high_resolution_clock::now();
	auto endlatR7 = std::chrono::high_resolution_clock::now();
 		
	
 	
    std::cout << "[INFO] Iniciando leitura do CSV...\n";
    std::cout << "[INFO] Total de dados lidos: " << dados.size() << "\n";
    std::cout << "[INFO] Tempo de leitura: " << std::chrono::duration<double>(end - start).count() << "s\n\n";

    std::cout << "[INFO] Inserindo dados na árvore AVL...\n";
    //inicio latencia
    startlatR7 = std::chrono::high_resolution_clock::now();

	start = std::chrono::high_resolution_clock::now();
	
    
	for (const auto& reg : dados) {
        raiz = inserir(raiz, reg);
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Inserção concluída em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
    
    std::cout << "[INFO] Cálculo de estatísticas por hora...\n";
    start = std::chrono::high_resolution_clock::now();
    calcularEstatisticas(raiz);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "[INFO] Estatísticas concluídas em " << std::chrono::duration<double>(end - start).count() << "s\n\n";
	
    std::cout << "\n=========== TESTE DE BUSCA ===========\n" << std::endl;
    

	
	auto start1 = std::chrono::high_resolution_clock::now();
	auto end1 = std::chrono::high_resolution_clock::now();
 		
 	
	
	std::cout << "[BUSCAR] Tempo de execução: " << duracao.count() << " segundos.\n";
 	std::cout << "\n=========== TESTE remoção ===========\n" << std::endl;
	std::cout << "\nContagem antes de remover\n " << std::endl;
    contarNos(raiz);
    start1 = std::chrono::high_resolution_clock::now();
    raiz = remover(raiz, 120);  // REMOVA E ATUALIZE
	end1 = std::chrono::high_resolution_clock::now();
	std::cout << "[INFO] Remoção concluída em " << std::chrono::duration<double>(end1 - start1).count() << "s\n\n";
	
	endlatR7 = std::chrono::high_resolution_clock::now();
	
 	std::cout << "\n\n[INFO] LETÊNCIA:" << std::chrono::duration<double>(endlatR7 - startlatR7).count() << "s\n\n";
	
	std::cout << "\nContagem depois de remover\n " << std::endl;
    
	contarNos(raiz);
    
    
}



