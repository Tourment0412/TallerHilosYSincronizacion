#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::queue<int> cola;
std::mutex mtx;
std::condition_variable cv;
bool produccion_terminada = false;

int tiempoProductor;
int tiempoConsumidor;

int elementos_consumidos = 0;

class Semaphore {
public:
    Semaphore(int count = 0) : count_(count) {}

    void notify() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            cv_.wait(lock);
        }
        --count_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

Semaphore semaforo(0);

void productor() {
    for (int i = 1; i <= 30; ++i) {
        {
            cola.push(i);
            std::cout << "Productor: generado elemento " << i << std::endl;
        }
        semaforo.notify(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempoProductor)); 
    } 
    produccion_terminada = true;
    semaforo.notify();
}

void consumidor() {
    while (true) {
        std::cout << "Consumidor: Esperando elemento" << std::endl;
        semaforo.wait();
        if (produccion_terminada && cola.empty()) {
            break;
        }

        if (cola.empty()) {
            throw std::runtime_error("Cola sin elementos");
        }

        int elemento = cola.front();
        cola.pop();
        std::cout << "Consumidor: consumido elemento " << elemento << std::endl;

        ++elementos_consumidos;
        std::this_thread::sleep_for(std::chrono::milliseconds(tiempoConsumidor)); 
    }
}

int main() {
    std::cout<<"Ingrese el tiempo de productor (milisegundos): ";
    std::cin >> tiempoProductor;
    std::cout<<"Ingrese el tiempo de consumidor (milisegundos): ";
    std::cin >> tiempoConsumidor;
    std::cout << "\n";
    std::thread productor_thread(productor);
    std::thread consumidor_thread(consumidor);

    productor_thread.join();
    consumidor_thread.join();

    return 0;
}
