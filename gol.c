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
 * Simulate the world and calculate the next generation of cells.
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
 * Initialise a random grid with the size given.
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

void free_grid(int ** grid, int l_size)
{
  int i;
  for(i = 0; i < l_size; i++){
    free(grid[i]);
  }
  free(grid);
}

int is_integer(char *string)
{
  char c;
  int i;

  for(i = 0; string[i] != 0; i++){
      c = string[i];
      if(c < '0' || c > '9'){
          return 0;
      }
  }
  return 1;
}

void handle_arg(int argc, char **argv, int *l_size, int *c_size, int *wait_time, int *alea_percent)
{
  char *options = "r:c:t:a:h";
  int current_option;
  char help[512];

  snprintf(help, 512, "Usage\n %s [OPTIONS]\n\nThe following options are availables:\n"
  "-h,          Show this help menu\n"
  "-r,          Set the number of row (5 min)\n"
  "-c,          Set the number of colomns (5 min)\n"
  "-t,          Set the time between each frame update (in microseconds)\n"
  "-a,          Set the percentage of cells apparition on initialistion(100 max)\n", argv[0]);


  while((current_option = getopt(argc, argv, options)) != -1){
    int *opt_addr;    
    
    switch(current_option){    
        case 'r': opt_addr = l_size; break;
        case 'c': opt_addr = c_size; break;
        case 't': opt_addr = wait_time; break;
        case 'a': opt_addr = alea_percent; break;
        case 'h': printf("%s", help); exit(0);
    }

    if(optarg != NULL){
        // The argument is a valid integer.
        if(is_integer(optarg)){
          if((current_option == 'r' || current_option == 'c') && (atoi(optarg) < 5)){
            fprintf(stderr, "Error: The minimum size for -%c is 5.\n", current_option);
            exit(-1);
          }
          if(current_option == 'a' && (atoi(optarg) > 100)){
            fprintf(stderr, "Error: The maximum value for -%c is 100.\n", current_option);
            exit(-1);
          }
          if(current_option == 't' && (atoi(optarg) < 0)){
            fprintf(stderr, "Error: The value for -%c should be positive.\n", current_option);
            exit(-1);
          }
        *opt_addr = atoi(optarg);
        } else {
            fprintf(stderr, "Error: The value for -%c should be an integer.\n", current_option);
            exit(-1);
        }
    } else {
        exit(-1);
    }
  }
}

int main(int argc, char * argv[])
{
  int l_size, c_size, counter, wait_time, alea_percent;
  int ** grid, ** grid_tmp;
  

  counter = 0;
  wait_time = 100;
  alea_percent = 15;
  l_size = c_size = 15;

  // Get the options.
  handle_arg(argc, argv, &l_size, &c_size, &wait_time, &alea_percent);
  
  // Initialization.
  grid = init_grid(l_size, c_size, alea_percent);
  grid_tmp = init_grid(l_size, c_size, alea_percent);
  
  // Game loop.
  while(1){
    
    printf("\nCycle %d\n", ++counter);
    display_grid(grid, l_size, c_size);
    usleep(wait_time * 1000);     
    update_grid(&grid, &grid_tmp, l_size, c_size);
    
  }
  
  free_grid(grid, l_size);
  free_grid(grid_tmp, l_size);
  
  return 0;
}
