#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

typedef unsigned char cell_t; 

cell_t **allocate_board(int size) 
{
	cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
	int	i;
	for (i=0; i<size; i++)
		board[i] = (cell_t *) malloc(sizeof(cell_t)*size);
	return board;
}

void free_board(cell_t **board, int size) 
{
	int i;
	for (i=0; i<size; i++)
		free(board[i]);
	free(board);
}

int adjacent_to(cell_t **board, int size, int i, int j) 
{
	int	k, l, count=0;
	int sk = (i>0) ? i-1 : i;
	int ek = (i+1 < size) ? i+1 : i;
	int sl = (j>0) ? j-1 : j;
	int el = (j+1 < size) ? j+1 : j;

	for (k=sk; k<=ek; k++)
		for (l=sl; l<=el; l++)
			count += board[k][l];
	count -= board[i][j];
	return count;
}

void play(cell_t **board, cell_t **newboard, int size) 
{
	int	i, j, a;
	
	#pragma omp parallel for private(i, j, a) shared(board, newboard, size) collapse(2)
	for (i=0; i<size; i++)
		for (j=0; j<size; j++) {
			a = adjacent_to(board, size, i, j);
			if (a == 2) newboard[i][j] = board[i][j];
			if (a == 3) newboard[i][j] = 1;
			if (a < 2) newboard[i][j] = 0;
			if (a > 3) newboard[i][j] = 0;
		}
}

void print(cell_t **board, int size) 
{
	int	i, j;
	for (j=0; j<size; j++) {
		for (i=0; i<size; i++) 
			fprintf(stderr, "%c", board[i][j] ? 'x' : ' ');
		fprintf(stderr, "\n");
	}
}

void read_file(char *file, cell_t ***board, int *size) 
{
	int	i, j;
	FILE *f = fopen(file, "r");
	if (f == NULL) {
		perror(file);
		exit(-1);
	}
	
	fprintf(stdout, "Lendo arquivo... ");
	fscanf(f,"%d", size);
	while (fgetc(f) != '\n');
	
	*board = allocate_board(*size);
	char *s = (char *) malloc(*size + 10);
	for (j=0; j<*size; j++) {
		fgets(s, *size+10, f);
		for (i=0; i<*size; i++) {
			(*board)[i][j] = s[i] == 'x';
		}
	}
	free(s);
	fprintf(stdout, "ok\n");
	fclose(f);
}

int main(int argc, char *argv[])
{
	struct timeval start, stop;

	if (argc <=2) {
		fprintf(stdout, "Entrada inválida: use %s <arquivo> <passos>\n", argv[0]);
		exit(-1);
	}
	
	int size, steps = strtol(argv[2], NULL, 10);
	cell_t **prev;

	read_file(argv[1], &prev, &size);

	cell_t **next = allocate_board(size);
	cell_t **tmp;
	int i;

	gettimeofday(&start, NULL);

	#pragma omp parallel private(i)
	{
		for (i=0; i<steps; i++) {
			play(prev, next, size);
			tmp = next;
			next = prev;
			prev = tmp;
		}
	}

	gettimeofday(&stop, NULL); 

	print(prev, size);
	
	double t = (((double)(stop.tv_sec)*1000.0  + (double)(stop.tv_usec / 1000.0)) - \
					((double)(start.tv_sec)*1000.0 + (double)(start.tv_usec / 1000.0)));

	fprintf(stdout, "Tempo decorrido = %g ms\n", t);

	free_board(prev, size);
	free_board(next, size);
}
