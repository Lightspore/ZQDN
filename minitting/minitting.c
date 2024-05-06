#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX		32	// superset edge size (MAX x MAX)
#define G		3	// mini grid (3 x 3) edge side
#define S		5	// step value to generate valid grid edge size N
#define N		(S*2+1) // game grid edge size N x N
#define N2		(N/2)	// half of game grid edge value
#define SEARCH		1000	// how many superset to search before giving up
#define TRUE		1
#define FALSE		0

int iFindValidGrid(unsigned int uiSuperSet[MAX][MAX], unsigned int* uiRowCount, unsigned int* uiColCount);
int iIsValidGrid(unsigned int uiSubSet[N][N], unsigned int* uiRowCount, unsigned int* uiColCount);
int iGridOccupancy(unsigned int uiSubSet[G][G]);

int main (int argc, char** argv)
{
  unsigned int uiSuperSet[MAX][MAX];
  unsigned int uiRowValue[MAX];
  unsigned int uiRowCount[N+1];
  unsigned int uiColCount[N+1];
  unsigned int uiIndexRow, uiIndexCol;
  unsigned int uiSearchIndex;

  // info - print game searching parameters
  printf("Game Parameter: S = %d N = %d, N2 = %d\n\n", S, N, N2);

  // initiatise random number generator
  srand(time(NULL));

  // loop while under search count limit and search valid game grid
  for (uiSearchIndex = 0;
       uiSearchIndex < SEARCH && iFindValidGrid(uiSuperSet, uiRowCount, uiColCount) == FALSE;
       ++uiSearchIndex)
  {

    // generate new 32 bits value where each bit represent superset row occupancy for all rows 0 - 31
    //     C0 C1 C2 ... C30 C31
    // R0   0  1  0 ...   1   0  =  unsigned int value
    // R1   1  1  1 ...   0   0
    // R2   0  0  1 ...   1   1
    // .....
    // R30  0  1  0 ...   0   0
    // R31  0  0  0 ...   1   0
    for (uiIndexRow = 0; uiIndexRow < MAX; ++uiIndexRow)
    {
      // use random number generator for value generation
      uiRowValue[uiIndexRow] = (unsigned int)rand();
    }

    // loop and transcribe occupancy data into superset array
    for (uiIndexRow = 0; uiIndexRow < MAX; ++uiIndexRow)
    {
      for (uiIndexCol = 0; uiIndexCol < MAX; ++uiIndexCol)
      {
        // bitwise odd = occupied
        uiSuperSet[uiIndexRow][uiIndexCol] = uiRowValue[uiIndexRow] % 2;
        // shift right to get next bit
        uiRowValue[uiIndexRow] = uiRowValue[uiIndexRow] >> 1;
      }
    }
  }

  // check if predefine number of supersets have exhausted
  // if true, message user and exit
  // if false, it means we have found a valid game and continue
  if (uiSearchIndex == SEARCH)
  {
    printf("No valid game found!\n");
    return 1;
  }

  // valid game found, continuing

  // info - print out how many superset searches performed
  printf("Searched %d supersets\n\n", uiSearchIndex);

  //################# Print game grid #################
  // print line 1 - column occupancy values
  printf(" ");
  for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
  {
    printf(" %d", uiColCount[uiIndexCol]);
  }
  printf("\n");

  // printer line 2 to last
  for (uiIndexRow = 0; uiIndexRow < N; ++uiIndexRow)
  {
    // print first number row occupancy value
    printf("%d ", uiRowCount[uiIndexRow]);

    // local mini grid (3 x 3) occupancy value indicator live at odd row/column index
    if (uiIndexRow % 2) // odd row index
    {
      // we need to check every column position
      for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
      {
        if (uiIndexCol % 2) // odd column index
        {
          // calculate mini grid occupancy and print
          printf("%d ", iGridOccupancy((unsigned int (*)[G])&(uiSuperSet[uiRowCount[N]+uiIndexRow-1][uiColCount[N]+uiIndexCol-1])));
        }
        else // even column index
        {
          // print empty space
          printf("  ");
        }
      }
    }
    // move onto next line
    printf("\n");
  }

  return 0;
}

// Given a superset find a N x N valid game grid
int iFindValidGrid(unsigned int uiSuperSet[MAX][MAX], unsigned int* uiRowCount, unsigned int* uiColCount)
{
  unsigned int uiIndexRow, uiIndexCol;

  // map an eligible N x N grid start position inside superset array
  for (uiIndexRow = 0; uiIndexRow < MAX - N; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < MAX - N; ++uiIndexCol)
    {
      // check mapped N x N start position against valid conditions
      if (iIsValidGrid((unsigned int (*)[N])&(uiSuperSet[uiIndexRow][uiIndexCol]), uiRowCount, uiColCount))
      {
        // store position of valid game grid as last value in occupancy array
        uiRowCount[N] = uiIndexRow;
        uiColCount[N] = uiIndexCol;

        // valid game grid found
        return TRUE;
      }
    }
  }

  // no valid game grid found
  return FALSE;
}

// Given a subset from superset where [0][0] is starting position of a N x N grid
// check if game grid satisfy the following conditions:
// - all column occupancy must be less than N2
// - all row occupancy must be less than N2
int iIsValidGrid(unsigned int uiSubSet[N][N], unsigned int* uiRowCount, unsigned int* uiColCount)
{
  unsigned int uiIndexRow, uiIndexCol;

  // reset row and column occupancy values
  memset(uiRowCount, 0, sizeof(unsigned int)*N);
  memset(uiColCount, 0, sizeof(unsigned int)*N);

  // go through all rows and column occupancy
  for (uiIndexRow = 0; uiIndexRow < N; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
    {
      // check if current location is occupied
      if (uiSubSet[uiIndexRow][uiIndexCol])
      {
        // increase current occupancy count for corresponding row and column in respective array
        ++uiRowCount[uiIndexRow];
        ++uiColCount[uiIndexCol];

        // if row or column occupancy > N2 then return game grid not valid
        if (uiRowCount[uiIndexRow] > N2 || uiColCount[uiIndexCol] > N2) return FALSE;

      }
    }
  }

  // game grid passed condition check and is valid
  return TRUE;
}

// Given a subset from superset where [0][0] is starting position of a 3 x 3 mini grid
// calculate the total occupancy
int iGridOccupancy(unsigned int uiSubSet[G][G])
{
  unsigned int uiIndexRow, uiIndexCol;
  int iCount = 0;

  // go through all positions
  for (uiIndexRow = 0; uiIndexRow < G; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < G; ++uiIndexCol)
    {
      // add up all occupied positions
      iCount += uiSubSet[uiIndexRow][uiIndexCol];
    }
  }

  return iCount;
}
