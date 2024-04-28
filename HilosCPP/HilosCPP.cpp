//Inclusión de librerias estandár para el programa
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

//Declaracion de variables globales del programa
std::queue<int> cola;
std::mutex mtx;
std::condition_variable cv;
bool produccion_terminada = false;
int tiempoProductor;
int tiempoConsumidor;
int elementos_consumidos = 0;

//Declaracion de la clase semaforo para la sincronizacion
class Semaphore {
/* Declaracion de sus elementos publicos que seran accesibles
 * por las diferentes partes del programa
 */
public:
    /* Constructor de la clase, recibe un entero que en caso de no enviarse
     * se asignara como un cero
     * */
    Semaphore(int count = 0) : count_(count) {}

    /* Funcion que se usa para notificar la posibilidad de continuar
     * la ejecucion de algun hilo que esta esperando
     * */
    void notify() {
        std::unique_lock<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }
    /* Funcion que se usa para hacer esperar a un hilo con la instruccion wait
     * En dado caso que no hayan instrucciones de que se puede ejecutar
     */
    void wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            cv_.wait(lock);
        }
        --count_;
    }
//Declaracion de los elementos privados de la clase Semaphore
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

Semaphore semaforo(0);

/* Metodo encargado de producir los elementos y notificar posibilidad de ejecucion
 * al hilo consumidor cuando hayan productos disponibles
 */
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
/* Metodo que consumira los productos siempre y cuando la cola no este vacia
 * esto se logra mediante la sincroninzacion con el producto.
 */
void consumidor() {
    while (true) {
        std::cout << "Consumidor: Esperando elemento" << std::endl;
        semaforo.wait();
        std::cout << "Consumidor: Espera terminada"<< std::endl;
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
/* Metodo main donde se inicializan los hilos y
 * se espera a que finalicen su ejecucion
 */
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
