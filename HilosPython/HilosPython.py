import threading
import time
import queue

RESET="\033[0m"
RED="\033[31m"
GREEN="\033[32m"
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
        print(GREEN+"Fabrica: generando producto "+str(i)+RESET)
        semaforo.release()
        time.sleep(tiempoProductor)
    produccion_terminada=True
    semaforo.release()
    
def consumidor():
    global tiempoConsumidor
    global elementos_consumidos
    global semaforo
    while(True):
        print(RED+"Cliente: Esperando productos"+RESET)
        semaforo.acquire()
        print(RED+"Cliente: Espera terminada"+RESET)

        if (produccion_terminada and cola.empty()):
            break

        if (cola.empty()):
            raise ValueError("Estanteria sin productos")
        
        elemento=cola.get()
        print(RED+"Cliente: producto obtenido "+str(elemento)+RESET)
        elementos_consumidos+=1
        time.sleep(tiempoConsumidor)
        



def main():
    global tiempoProductor
    global tiempoConsumidor
    tiempoProductor=float(input("Ingrese el tiempo de produccion de la fabrica (milisegundos): "))/1000
    tiempoConsumidor=float(input("Ingrese el tiempo de consumo del cliente (milisegundos): "))/1000
    t1 = threading.Thread(target=productor)
    t2 = threading.Thread(target=consumidor)
    t1.start()
    t2.start()
    t1.join()
    t2.join()

main()
