import java.util.concurrent.Semaphore;
import java.lang.Math;

public class Main{
	public static void main(String[] args) {
	    int N = 100000;
	    int MAX_THREADS = 8;
	    System.out.println("THREADS: "+MAX_THREADS);
	    Numero soma = new Numero(0);
	    
		double[] A = new double[N];
		double[] B = new double[N];
        double ha = 0;
		
		Thread[] thread = new Thread[MAX_THREADS];
		SomaParcial[] parcial = new SomaParcial[MAX_THREADS];
		
		for(int i = 0; i < N; i++) {
      		A[i] = (double)(Math.random()*100);
			B[i] = (double)(Math.random()*100);
			ha = ha+A[i]+B[i];
    	}
        System.out.println("Média: "+ha/N);
        long start = System.currentTimeMillis();
    	for(int i = 0; i < MAX_THREADS; i++){
    	    parcial[i] = new SomaParcial(A, B, MAX_THREADS, soma, i, N);
    	    thread[i] = new Thread(parcial[i]);
    	    thread[i].start();
    	}
    	try{
    	    for(int i = 0; i < MAX_THREADS; i++)
			    thread[i].join();
    	}
    	catch(Exception e){
            System.out.println("Excessão: "+e);
        }
        // parcial[0] = new SomaParcial(A, B, 1, soma, 0, N);
        // parcial[0].run();
        long end = System.currentTimeMillis() - start;
        System.out.println("Soma total: "+soma.getNumero());
            
        System.out.println("Processamento finalizado em "+end+" milissegundos");
	}
}

class Numero{
    private double num = 0;
    public Numero(double n){
        this.num = n;
    }
    public synchronized void Soma(double n){
        this.num += n;
    }
    public double getNumero(){
        return num;
    }
    public void setNumero(double n){
        this.num = n;
    }
}

class SomaParcial implements Runnable{
    private double[] A;
    private double[] B;
    private int MAX_THREADS;
    private Numero soma;
    private int iter;
    private int N;
    private int parcial = 0;
    private int n = 0;
    
    static Semaphore lock = new Semaphore(1);

    
    public SomaParcial(double[] A, double[] B, int MAX_THREADS, Numero soma, int iter, int N){
        this.A = A;
        this.B = B;
        this.MAX_THREADS = MAX_THREADS;
        this.soma = soma;
        this.iter = iter;
        this.N = N;
    }
    public void run(){
        for(int i = iter; i < N; i += MAX_THREADS){
            parcial += A[i]*B[i];
            n += 1;
        }

        //System.out.println("Soma parcial da thread "+iter+": "+parcial+" N: "+n);
        
        soma.Soma(parcial); 
    }
}
