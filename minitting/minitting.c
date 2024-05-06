#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX		32
#define G		3	//mini grid (3x3) edge side
#define S		5
#define N		(S*2+1)
#define N2		(N/2)
#define SEARCH		1000
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

  printf("Game Parameter: S = %d N = %d, N2 = %d\n\n", S, N, N2);

  srand(time(NULL));

  for (uiSearchIndex = 0;
       uiSearchIndex < SEARCH && iFindValidGrid(uiSuperSet, uiRowCount, uiColCount) == FALSE;
       ++uiSearchIndex)
  {
    for (uiIndexRow = 0; uiIndexRow < MAX; ++uiIndexRow)
    {
      uiRowValue[uiIndexRow] = (unsigned int)rand();
    }

    for (uiIndexRow = 0; uiIndexRow < MAX; ++uiIndexRow)
    {
      for (uiIndexCol = 0; uiIndexCol < MAX; ++uiIndexCol)
      {
        uiSuperSet[uiIndexRow][uiIndexCol] = uiRowValue[uiIndexRow] % 2;
        uiRowValue[uiIndexRow] = uiRowValue[uiIndexRow] >> 1;
      }
    }
  }

  if (uiSearchIndex == SEARCH)
  {
    printf("No valid game found!\n");
    return 1;
  }

  printf("Searched %d supersets\n\n", uiSearchIndex);

  printf(" ");
  for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
  {
    printf(" %d", uiColCount[uiIndexCol]);
  }
  printf("\n");

  for (uiIndexRow = 0; uiIndexRow < N; ++uiIndexRow)
  {
    printf("%d ", uiRowCount[uiIndexRow]);

    if (uiIndexRow % 2)
    {
      for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
      {
        if (uiIndexCol % 2)
        {
          printf("%d ", iGridOccupancy((unsigned int (*)[G])&(uiSuperSet[uiRowCount[N]+uiIndexRow-1][uiColCount[N]+uiIndexCol-1])));
        }
        else
        {
          printf("  ");
        }
      }
    }
    printf("\n");
  }

  return 0;
}

int iFindValidGrid(unsigned int uiSuperSet[MAX][MAX], unsigned int* uiRowCount, unsigned int* uiColCount)
{
  unsigned int uiIndexRow, uiIndexCol;

  for (uiIndexRow = 0; uiIndexRow < MAX - N; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < MAX - N; ++uiIndexCol)
    {
      if (iIsValidGrid((unsigned int (*)[N])&(uiSuperSet[uiIndexRow][uiIndexCol]), uiRowCount, uiColCount))
      {
        uiRowCount[N] = uiIndexRow;
        uiColCount[N] = uiIndexCol;

        return TRUE;
      }
    }
  }

  return FALSE;
}

int iIsValidGrid(unsigned int uiSubSet[N][N], unsigned int* uiRowCount, unsigned int* uiColCount)
{
  unsigned int uiIndexRow, uiIndexCol;

  memset(uiRowCount, 0, sizeof(unsigned int)*N);
  memset(uiColCount, 0, sizeof(unsigned int)*N);

  for (uiIndexRow = 0; uiIndexRow < N; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < N; ++uiIndexCol)
    {
      if (uiSubSet[uiIndexRow][uiIndexCol])
      {
        ++uiRowCount[uiIndexRow];
        ++uiColCount[uiIndexCol];

        if (uiRowCount[uiIndexRow] > N2 || uiColCount[uiIndexCol] > N2) return FALSE;

      }
    }
  }

  return TRUE;
}

int iGridOccupancy(unsigned int uiSubSet[G][G])
{
  unsigned int uiIndexRow, uiIndexCol;
  int iCount = 0;

  for (uiIndexRow = 0; uiIndexRow < G; ++uiIndexRow)
  {
    for (uiIndexCol = 0; uiIndexCol < G; ++uiIndexCol)
    {
      iCount += uiSubSet[uiIndexRow][uiIndexCol];
    }
  }

  return iCount;
}
