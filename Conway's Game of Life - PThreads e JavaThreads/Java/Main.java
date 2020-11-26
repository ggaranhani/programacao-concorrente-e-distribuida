
public class Main{
	public static void main(String args[]){
		int MAX_ITER = 2000;
		int MAX_THREADS = 2;
		int dim = 2048;
		
		System.out.println(MAX_THREADS + " THREADS");

	    	Tabuleiro atual = new Tabuleiro(dim);
	   	Tabuleiro novo = new Tabuleiro(dim);		
	    	Jogo[] jogo = new Jogo[MAX_THREADS];
		Thread[] thread = new Thread[MAX_THREADS];
		
		for(int i = 0; i < MAX_THREADS; i++){
 			jogo[i] = new Jogo(atual, novo, i+1, MAX_THREADS, dim);
		}
	
		atual.Inicia();
		System.out.println("CELULAS VIVAS GERACAO ZERO: " + atual.contaVivos());

		//System.out.println("GERACAO ZERO");
	   	//atual.print();
		//jogo.simula();
		//novo.print();
		long start = System.currentTimeMillis();
		for(int k = 0; k < MAX_ITER; k++){
			//System.out.println("GERACAO " + (k+1));
			for(int i = 0; i < MAX_THREADS; i++){
				thread[i] = new Thread(jogo[i]);
				thread[i].start();
			}
			try{
        		    for(int i = 0; i < MAX_THREADS; i++)
        			thread[i].join();
     			}
     	   		catch(Exception e){
            			System.out.println("Exception " + e);
        		}
			for(int i = 1; i < dim+1; i++){
    				for(int j = 1; j < dim+1; j++){
    			    		atual.grid[i][j] = novo.grid[i][j];
    				}
        		}
			atual.Bordas();

			//atual.print();
			//novo.print();
		}
		long end = System.currentTimeMillis() - start;
		System.out.println("Tempo gasto dentro do loop: "+ end/1000);
		System.out.println("CELULAS VIVAS DEPOIS DE " + MAX_ITER + " GERAÇÕES: " + novo.contaVivos());
	}
}