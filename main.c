#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>



int get_neighbours(int ** grid, int i, int j, int size)
{
  int n = 0;
  
  n += grid[i][(j + 1) > (size - 1) ? 0 : (j + 1)];
  n += grid[i][(j - 1) >= 0 ? (j - 1) : (size - 1)];
  
  n += grid[(i + 1) > (size - 1) ? 0 : (i + 1)][j];
  n += grid[(i - 1) >= 0 ? (i - 1) : (size - 1)][j];
  
  n += grid[(i + 1) > (size - 1) ? 0 : (i + 1)][(j + 1) > (size - 1) ? 0 : (j + 1)];
  n += grid[(i + 1) > (size - 1) ? 0 : (i + 1)][(j - 1) >= 0 ? (j - 1) : (size - 1)];

  n += grid[(i - 1) >= 0 ? (i - 1) : (size - 1)][(j + 1) > (size - 1) ? 0 : (j + 1)];
  n += grid[(i - 1) >= 0 ? (i - 1) : (size - 1)][(j - 1) >= 0 ? (j - 1) : (size - 1)];
  
 
  return n;
}



void cpy_grid(int ** grid_old, int ** grid_new, int size)
{
  int i, j;
  for (i = 0; i < size; i++){
    for (j = 0; j < size; j++){
      grid_old[i][j] = grid_new[i][j];
    }
  } 
}

void update_grid(int ** grid_old, int ** grid_new, int size)
{
  int i, j, n;
  for (i = 0; i < size; i++){
    for (j = 0; j < size; j++){
      n = get_neighbours(grid_old, i, j, size);
      //printf("Cell %d %d has %d n\n", i, j, n);
      if(grid_old[i][j]){ // alive
	grid_new[i][j] = (n == 3 || n == 2 );
      }else { // dead cell
	grid_new[i][j] = (n == 3);
      }
    }
  }
  cpy_grid(grid_old, grid_new, size);
}

void display_grid(int ** grid, int size)
{
  int i, j;
  for (i = 0; i < size; i++){
    for (j = 0; j < size; j++){
      if(grid[i][j]){
	printf("O");
      } else {
	printf(".");
      }     
    }
    printf("\n");
  }  
}

int ** init_grid(int size)
{
  int i, j;
  int ** grid = malloc(sizeof(*grid) * size);
  
  for (i = 0; i < size; i++){
    grid[i] = malloc(sizeof(**grid) * size);
  }

  srand(time(NULL));
  
  for (i = 0; i < size; i++){
    for (j = 0; j < size; j++){
       grid[i][j] = (rand()%100<20);
       //grid[i][j] = 0;
    }
  }

  /* grid[size-1][0] = 1;
  grid[size-1][1] = 1;
  grid[size-1][2] = 1;*/
  return grid;
}


int main(int argc, char * argv[])
{
  int size, counter;
  int ** grid, ** grid_tmp;
  counter = 0;
  if(argc < 2){
    printf("Usage: %s [grid_size]\n", argv[0]);
    return -1;
  }

  if((size = atoi(argv[1])) < 10){
    printf("Error size min = 10");
    return -1;
  }

  printf("Use ^C to stop the GOL simulation.\n");
  
  grid = init_grid(size);
  grid_tmp = init_grid(size);
  
  while(1){
    printf("\nCycle %d\n", ++counter);
    display_grid(grid, size);
    sleep(1);
    update_grid(grid, grid_tmp, size);
  }
  
  return 0;
}
