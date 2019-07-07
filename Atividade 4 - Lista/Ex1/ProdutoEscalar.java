package ProdutoEscalar;
import java.util.concurrent.Semaphore;
import java.util.Random;
public class ProdutoEscalar implements Runnable{
	
	static final int MAX_THREADS = 2, N = 1000000;
	private int threadID;
	private Semaphore sem;
	private double[] ProdutoEscalar_calc, A ,B;
	private static double ProdutoEscalar_f;

	public ProdutoEscalar(int threadID, double[] ProdutoEscalar_calc, double[] A, double[] B, double ProdutoEscalar_f, Semaphore sem){
		this.threadID = threadID;
		this.ProdutoEscalar_calc = ProdutoEscalar_calc;
		this.A = A;
		this.B = B;
		ProdutoEscalar.ProdutoEscalar_f = ProdutoEscalar_f;
		this.sem = sem;
	}
	public void run(){
		int i=0;
		for(i = this.threadID; i < N; i+= MAX_THREADS){
			ProdutoEscalar_calc[this.threadID] += A[i]*B[i];}
		try{
			sem.acquire();
			ProdutoEscalar_f += ProdutoEscalar_calc[this.threadID];
			sem.release();
		}catch (InterruptedException ie){
			System.out.println("Excecao");}
	}
	
	public static void main(String[] args) throws InterruptedException{
		int i=0;
		double[] A, B, ProdutoEscalar_aux;
		Semaphore sem = new Semaphore(1, true);
		Random value = new Random();
		Thread[] threads = new Thread[MAX_THREADS];
		ProdutoEscalar_aux = new double[MAX_THREADS];
		A = new double[N];
		B = new double[N];
		for(i = 0; i < N; i++){
			A[i] = value.nextDouble();
			B[i] = value.nextDouble();}
		long startTime = System.currentTimeMillis();
		for(i = 0; i < threads.length; i++){
			threads[i] = new Thread(new ProdutoEscalar(i, ProdutoEscalar_aux, A, B, ProdutoEscalar_f, sem));
			threads[i].start();
			}
		for(i = 0; i < threads.length; i++){
			threads[i].join();}
		long totalTime = System.currentTimeMillis() - startTime;
		System.out.println("Tempo decorrido região paralela: " + totalTime + "ms");
		System.out.println("Produto Escalar:" + ProdutoEscalar_f);
	}
}
