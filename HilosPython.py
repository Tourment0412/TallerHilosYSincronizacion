import threading
import time
import queue

elementos_consumidos=0
tiempoProductor=0.0
tiempoConsumidor=0.0
produccion_terminada = False

cola = queue.Queue()
semaforo = threading.Semaphore(0)


def productor():
    global tiempoProductor
    global produccion_terminada
    global cola
    global semaforo
    for i in range (1,31):
        cola.put(i)
        print("\033[32mProductor: generado elemento "+str(i)+"\033[0m")
        semaforo.release()
        time.sleep(tiempoProductor)
    produccion_terminada=True
    semaforo.release()
    
def consumidor():
    global tiempoConsumidor
    global elementos_consumidos
    global semaforo
    while(True):
        print("\033[31mConsumidor: Esperando elemento\033[0m")
        semaforo.acquire()
        print("\033[31mConsumidor: Espera terminada\033[0m")

        if (produccion_terminada and cola.empty()):
            break

        if (cola.empty()):
            raise ValueError("Cola sin elementos")
        
        elemento=cola.get()
        print("\033[31mConsumidor: consumido elemento "+str(elemento)+"\033[0m")
        elementos_consumidos+=1
        time.sleep(tiempoConsumidor)
        



def main():
    global tiempoProductor
    global tiempoConsumidor
    tiempoProductor=float(input("Ingrese el tiempo de productor (milisegundos): "))/1000
    tiempoConsumidor=float(input("Ingrese el tiempo de consumidor (milisegundos): "))/1000
    t1 = threading.Thread(target=productor)
    t2 = threading.Thread(target=consumidor)
    t1.start()
    t2.start()
main()