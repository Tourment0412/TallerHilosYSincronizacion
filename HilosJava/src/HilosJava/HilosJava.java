package HilosJava;

import java.util.LinkedList;
import java.util.Queue;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

public class HilosJava extends Thread {
	public static final String RESET = "\033[0m";
	public static final String RED = "\033[0;31m";
	public static final String GREEN = "\033[0;32m";
	static int elementos_consumidos = 0;
	static double tiempoProductor = 0.0;
	static double tiempoConsumidor = 0.0;
	static boolean produccion_terminada = false;

	static Queue<Integer> cola = new LinkedList<>();
	static Semaphore semaphore = new Semaphore(0);


	public static void productor() {
		for (int i = 1; i <= 30; i++) {
			cola.add(i);
			System.out.println(GREEN+"Fabrica: generando producto " + i + RESET);
			try {
				semaphore.release();
				Thread.sleep((long) tiempoProductor);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		produccion_terminada = true;
		semaphore.release();

	}

	public static void consumidor() {
		while (true) {
			System.out.println(RED+"Cliente: Esperando productos" + RESET);
			try {
				semaphore.acquire();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println(RED+"Cliente: Espera terminarda" + RESET);
			if (produccion_terminada && cola.isEmpty()) {
				break;
			}

			if (cola.isEmpty()) {
				throw new IllegalStateException("Estanteria sin productos");
			}

			int elemento = cola.remove();
			System.out.println(RED+"Cliente: producto obtenido " + elemento + RESET);
			elementos_consumidos += 1;
			try {
				Thread.sleep((long) tiempoConsumidor);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}


	public static void main(String args[]) {
		try {
			Scanner scanner = new Scanner(System.in);

			System.out.print("Ingrese el tiempo de produccion de la fabrica (milisegundos): ");
			tiempoProductor = Double.parseDouble(scanner.nextLine());

			System.out.print("Ingrese el tiempo de consumo del cliente (milisegundos): ");
			tiempoConsumidor = Double.parseDouble(scanner.nextLine());
			Thread t1 = new Thread(HilosJava::productor);
			Thread t2 = new Thread(HilosJava::consumidor);

			t1.start();
			t2.start();

			t1.join();
			t2.join();
			
			scanner.close();
		} catch (NumberFormatException | InterruptedException e) {
			e.printStackTrace();
		}
	}

}
