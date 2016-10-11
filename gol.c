/***********************************************************           
 * gol.c -- A implementation in c of the Game of life      *
 *          from the mathematician Jonh Conway.            *   
 *                                                         *   
 * Author:  Jean-Hugo OUWE MISSI OUKEM                     *   
 *                                                         *   
 * Purpose:  Demonstration of the game of life algorithm   *
 *           and training with c pointers.                 *   
 *                                                         *   
 *                                                         *   
 ***********************************************************/  

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


/**
 * Count a cell's neighbours simulating a torique matrix
 * If we hit the size of the grid then we check the opposite side.
 */
int get_neighbours(int ** grid, int i, int j, int l_size, int c_size)
{
  int n = 0;
  
  n += grid[i][(j + 1) > (c_size - 1) ? 0 : (j + 1)];
  n += grid[i][(j - 1) >= 0 ? (j - 1) : (c_size - 1)];
  
  n += grid[(i + 1) > (l_size - 1) ? 0 : (i + 1)][j];
  n += grid[(i - 1) >= 0 ? (i - 1) : (l_size - 1)][j];
  
  n += grid[(i + 1) > (l_size - 1) ? 0 : (i + 1)][(j + 1) > (c_size - 1) ? 0 : (j + 1)];
  n += grid[(i + 1) > (l_size - 1) ? 0 : (i + 1)][(j - 1) >= 0 ? (j - 1) : (c_size - 1)];

  n += grid[(i - 1) >= 0 ? (i - 1) : (l_size - 1)][(j + 1) > (c_size - 1) ? 0 : (j + 1)];
  n += grid[(i - 1) >= 0 ? (i - 1) : (l_size - 1)][(j - 1) >= 0 ? (j - 1) : (c_size - 1)];
  
 
  return n;
}

/**
 * Swap two array of two dimensions.
 */
void swap_grid(int *** grid_old, int *** grid_new)
{
  int ** tmp = *grid_old;
  *grid_old = *grid_new;
  *grid_new = tmp;
}

/**
 * Simulate the world and calculate he next generation of cells.
 */
void update_grid(int *** grid_old, int *** grid_new, int l_size, int c_size)
{
  int i, j, n;
 
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
      n = get_neighbours(*grid_old, i, j, l_size, c_size);
      if((*grid_old)[i][j]){ // alive
	(*grid_new)[i][j] = (n == 3 || n == 2 );
      }else { // dead cell
	(*grid_new)[i][j] = (n == 3);
      }
    }
  }
  
  swap_grid(grid_old, grid_new);

}

/**
 * Display the grid, change the output here.
 */
void display_grid(int ** grid, int l_size, int c_size)
{
  int i, j;
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
      if(grid[i][j]){
	printf("O");
      } else {
	printf(".");
      }     
    }
    printf("\n");
  }  
}

/**
 * Initialise a random grid with the gived size.
 */
int ** init_grid(int l_size, int c_size, int alea_percent)
{
  int i, j;
  int ** grid = malloc(sizeof(*grid) * l_size);
  
  for (i = 0; i < l_size; i++){
    grid[i] = malloc(sizeof(**grid) * c_size);
  }

  srand(time(NULL));
  
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
      grid[i][j] = ((rand()%100+1) <= alea_percent);
    }
  }  
  return grid;
}


int main(int argc, char * argv[])
{
  int l_size, c_size, counter, wait_time, alea_percent;
  int ** grid, ** grid_tmp;

  counter = 0;
  wait_time = 100;
  alea_percent = 15;

  if(argc < 2){
    printf("Usage: %s [grid_size] or %s [line_size][columns_size][update_time][init_cells_apparition_percent] (the minimal size is 5)\n", argv[0], argv[0]);
    return -1;
  }

  if(argc < 3){
    l_size = c_size = atoi(argv[1]);
  } else if (argc > 2){
    l_size = atoi(argv[1]);
    c_size = atoi(argv[2]);
  }
  if(argc > 3){
    wait_time = atoi(argv[3]);
  }
  if(argc > 4){
    alea_percent = atoi(argv[4]) > 100 ? 100 : atoi(argv[4]) ;
    printf("ALEA=%d\n", alea_percent);
  }
  if(l_size < 5 || c_size < 5){
    printf("Error size min = 5\n");
    return -1;
  }

  printf("Use ^C to stop the GOL simulation.\n");
  
  grid = init_grid(l_size, c_size, alea_percent);
  grid_tmp = init_grid(l_size, c_size, alea_percent);


  /*
  for(counter = 0; counter < 4000; counter++){
    
    update_grid(&grid, &grid_tmp, l_size, c_size);
  }
  */
  
  
  while(1){
    
    printf("\nCycle %d\n", ++counter);
    display_grid(grid, l_size, c_size);
    usleep(wait_time * 1000);     
    update_grid(&grid, &grid_tmp, l_size, c_size);
    
  }
  
  // free_grid(grid, l_size, c_size);
  // free_grid(grid_tmp, l_size, c_size);
  
  return 0;
}
