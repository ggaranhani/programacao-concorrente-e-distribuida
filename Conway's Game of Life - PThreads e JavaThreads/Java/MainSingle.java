
public class Main{
	public static void main(String args[]){

		int MAX_THREADS = 1;
		int dim = 2048;
		
		System.out.println(MAX_THREADS + " THREADS");
	    	Tabuleiro atual = new Tabuleiro(dim);
	   	Tabuleiro novo = new Tabuleiro(dim);
	    	Jogo jogo = new Jogo(atual, novo, 1, MAX_THREADS, dim);
	
		atual.Inicia();
	   	//atual.print();
		jogo.simula();
		//novo.print();
		
		System.out.println("VIVOS: " + novo.contaVivos());
		Thread[] thread;
		
		
	}
}