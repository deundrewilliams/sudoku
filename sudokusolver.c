#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define EMPTY '0'

// Fail cases: 
// Current number is the same as another number in the same row
// Current number is the same as another number in the same column
// Current number is the same as another number in the same 3x3 box

char **read_grid(char *filename, char **grid) 
{
    int i, j;
    FILE *ifp = fopen(filename, "r");

    if (ifp == NULL) 
    {
        printf("Invalid file (%s)\n", filename);
        exit(1);
    }

    grid = malloc(sizeof(char *) * 9);
    
    for (i = 0; i < 9; i++)
        grid[i] = malloc(sizeof(char) * 9);
    
    for (i = 0; i < 9; i++)
        fscanf(ifp, "%s", grid[i]);

    return grid;

}
void print_grid(char **grid) 
{
    int i, j;
    printf("\n+================+\n");
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            printf("%c%s", grid[i][j], (j == 8) ? "\n" : " ");
}
// Returns 1 if finished, 0 if not
int finished(char **grid) 
{
    int i, j;

    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            if (grid[i][j] == '0')
                return 0;

    return 1;
}
// Returns 1 if valid, 0 if not
int valid_move(char new_number, char **grid, int row, int col) 
{
    int i, j, row_lower_bound, col_lower_bound, row_upper_bound, col_upper_bound;

    // Check row
    for (i = 0; i < 9; i++) 
    {
        if (grid[i][col] == new_number)
            return 0;
    }

    // Check column
    for (i = 0; i < 9; i++) 
    {
        if (grid[row][i] == new_number)
            return 0;
    }

    // Calculate correct 3x3 grid bounds
    row_lower_bound = (row / 3) * 3;
    col_lower_bound = (col / 3) * 3;
    row_upper_bound = row_lower_bound + 3;
    col_upper_bound = col_lower_bound + 3;

    for (i = row_lower_bound; i < row_upper_bound; i++)
        for (j = col_lower_bound; j < col_upper_bound; j++) 
        {
            if (grid[i][j] == new_number)
                return 0;
        }
            


    return 1;



}

// Returns the first empty cell in the column, if no empty cells, returns 9
int first_empty(char **grid, int col) 
{
    int i;

    for (i = 0; i < 9; i++) 
    {
        //printf("grid[%d][%d] is %c\n", i, col, grid[i][col]);
        if (grid[i][col] == EMPTY)
            break;
        
    }

    return i;

}

int solver_helper(char **grid, int col) 
{
    int i, j, row;
    char new_num, options[9] = {"123456789"};

    // Print the current grid
    print_grid(grid);

    // Check if the grid is solved
    if (finished(grid))
        return 1;
    
    // Bring the column back down to 0 if greater than 8
    if (col > 8)
        col = 0;

    // Determine the current row based on the first empty cell in the column
    row = first_empty(grid, col);

    // If the current column is full, skip to next column
    if (row == 9)
        if (solver_helper(grid, col + 1))
            return 1;

    // For loop for trying each of the numbers
    for (i = 0; i < 9; i++) 
    {
        // Generate new number to try
        new_num = options[i];

        // Check the validity of the potential move
        if (!valid_move(new_num, grid, row, col))
            continue;
            
        // Change state
        grid[row][col] = new_num;

        // Perform recursive descent
        if (solver_helper(grid, col + 1))
            return 1;

        // Revert state upon failure of recursive descent
        grid[row][col] = EMPTY;

        print_grid(grid);

    }

    return 0;

}

// Calls the actual solver function with the correct starting column parameter (0)
int solver(char **grid) 
{
    return solver_helper(grid, 0);
}

int main(int argc, char **argv) 
{

    int m;
    char **grid;

    if (argc < 2) 
    {
        printf("Proper syntax: ./a.out <grid file>\n");
        return 1;
    }

    grid = read_grid(argv[1], grid);

    m = solver(grid);

    return 0;
}