import java.util.Random;

public class Tabuleiro{
	int[][] grid;
	int dim;
	Random rand = new Random(1985);
	
	public Tabuleiro(int dim){
		this.dim = dim;
		grid = new int[dim+2][dim+2];
	}

	public void Inicia(){
		for(int i = 1; i < dim+1; i++){
			for(int j = 1; j < dim+1; j++){
				this.grid[i][j] = rand.nextInt(2);
			}
		}
		this.Bordas();
	}

	public void Bordas(){
		for(int i = 1; i < dim+1; i++){
			grid[0][i] = grid[dim][i];
			grid[dim+1][i] = grid[1][i];
			grid[i][0] = grid[i][dim];
			grid[i][dim+1] = grid[i][1];
		}
		grid[0][0] = grid[dim][dim];
		grid[dim+1][dim+1] = grid[1][1];
		grid[0][dim+1] = grid[dim][1];
		grid[dim+1][0] = grid[1][dim];
	}
	
	public int contaVivos(){
	    int count = 0;
	    for(int i = 1; i < dim+1; i++){
	        for(int j = 1; j < dim+1; j++){
	            if(grid[i][j] == 1){
	                count++;
	            }
	        }
	    }
	    return count;
	}
	
	public void print(){
        System.out.println(); 
	    for(int i = 0; i < dim+2; i++){
	        System.out.print(i+": ");
			for(int j = 0; j < dim+2; j++){
				System.out.print(grid[i][j] + " ");
			}
			System.out.println();
		}
	}

}
	