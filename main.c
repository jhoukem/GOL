#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



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
 * Return 0 if there is no new cells or no dead cells
 */
int cpy_grid(int ** grid_old, int ** grid_new, int l_size, int c_size)
{
  int i, j, status;
  status = 0;
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
      if(grid_old[i][j] != grid_new[i][j]){
	status = 1;
	grid_old[i][j] = grid_new[i][j];
      }
    }
  }
  return status;
}


int update_grid(int ** grid_old, int ** grid_new, int l_size, int c_size)
{
  int i, j, n, status;
  status = 0;
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
      n = get_neighbours(grid_old, i, j, l_size, c_size);
      //printf("Cell %d %d has %d n\n", i, j, n);
      if(grid_old[i][j]){ // alive
	grid_new[i][j] = (n == 3 || n == 2 );
      }else { // dead cell
	grid_new[i][j] = (n == 3);
      }
    }
  }
 
  status = cpy_grid(grid_old, grid_new, l_size, c_size);
  return status;
}

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

int ** init_grid(int l_size, int c_size)
{
  int i, j;
  int ** grid = malloc(sizeof(*grid) * l_size);
  
  for (i = 0; i < l_size; i++){
    grid[i] = malloc(sizeof(**grid) * c_size);
  }

  srand(time(NULL));
  
  for (i = 0; i < l_size; i++){
    for (j = 0; j < c_size; j++){
       grid[i][j] = (rand()%100 < 80);
       // grid[i][j] = 0;
    }
  }
  /*
  grid[4][0] = 1;
  grid[4][1] = 1;
  grid[4][2] = 1;
  grid[3][2] = 1;
  grid[2][1] = 1;
  */
  
  return grid;
}


int main(int argc, char * argv[])
{
  int l_size, c_size, counter;
  int ** grid, ** grid_tmp;
  counter = 0;
  if(argc < 2){
    printf("Usage: %s [grid_size]\n or %s [line_size][columns_size] the minimal size is 5", argv[0], argv[0]);
    return -1;
  }

  if(argc < 3){
    l_size = c_size = atoi(argv[1]);
  } else if (argc > 2){
    l_size = atoi(argv[1]);
    c_size = atoi(argv[2]);
  }

  if(l_size < 5 || c_size < 5){
    printf("Error size min = 5\n");
    return -1;
  }

  printf("Use ^C to stop the GOL simulation.\n");
  
  grid = init_grid(l_size, c_size);
  grid_tmp = init_grid(l_size, c_size);

  while(1){

    printf("\nCycle %d\n", ++counter);
    display_grid(grid, l_size, c_size);
    sleep(1);    
    if(update_grid(grid, grid_tmp, l_size, c_size) == 0){ // No more update
      break;
    }
    counter++;
  }
  printf("Evolution stopped after %d generations\n", counter);
  
  // free_grid(grid, l_size, c_size);
  //free_grid(grid_tmp, l_size, c_size);
  
  return 0;
}
