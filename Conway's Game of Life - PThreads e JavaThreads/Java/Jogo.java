
public class Jogo implements Runnable{
    Tabuleiro atual;
    Tabuleiro novo;
    int N;
    int MAX_THREADS;
    int dim;
    
    public Jogo(Tabuleiro atual, Tabuleiro novo, int N, int MAX_THREADS, int dim){
        this.atual = atual;
        this.novo = novo;
        this.N = N;
        this.MAX_THREADS = MAX_THREADS;
        this.dim = dim;
    }
    
    public void simula(){
	    int vivos;

    	for(int i = N; i < dim+1; i+=MAX_THREADS){
    		for(int j = 1; j < dim+1; j++){
    			vivos = getNeighbors(i, j); 
    			if(vivos < 2){
    				novo.grid[i][j] = 0;
    			}
    			else if(vivos >= 4){
    				novo.grid[i][j] = 0;
    			}
    			else if(vivos == 3 && atual.grid[i][j] == 0){
    				novo.grid[i][j] = 1;	
    			}			
    			else if(atual.grid[i][j] == 1){
    				novo.grid[i][j] = 1;
    			}
    		}
    	}
    }
    
    public int getNeighbors(int i, int j){
    	int vivos = 0;

    	if(atual.grid[i][j+1] == 1)
    		vivos++;
    	if(atual.grid[i][j-1] == 1)
    		vivos++;
    	if(atual.grid[i+1][j+1] == 1)
    		vivos++;
    	if(atual.grid[i+1][j-1] == 1)
    		vivos++;
    	if(atual.grid[i-1][j] == 1)
    		vivos++;
    	if(atual.grid[i+1][j] == 1)
    		vivos++;
    	if(atual.grid[i-1][j-1] == 1)
    		vivos++;
    	if(atual.grid[i-1][j+1] == 1)
    		vivos++;
    
    	return vivos;
    }
    
	
    public void copia(){
        for(int i = N; i < dim+1; i+=MAX_THREADS){
    		for(int j = 0; j < dim+1; j++){
    		    atual.grid[i][j] = novo.grid[i][j];
    		}
        }
    }
    public void run(){
	    simula();
	    //copia();
	}
    
}