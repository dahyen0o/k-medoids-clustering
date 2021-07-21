#include <stdio.h>
#include <stdlib.h>

#define SEED 1000

typedef struct NODE{
	double *arr;
	struct NODE *link;
} NODE;
NODE** head;

void Insertnode(int, NODE **);
NODE* Newnode(int);
double Length(int, double *, double *);
int UpdateMedoid(int, int, double **);

int main(){
	int n, f, k; NODE *newnode;
	FILE *finput = fopen("input.txt", "r");
	fscanf(finput, "%d %d %d", &n, &f, &k);
	double **tarr = (double **)malloc(sizeof(double *) * n);
	int *index = (int *)malloc(sizeof(int) * k);
	head = (NODE **)malloc(sizeof(NODE *) * n);
	srand(SEED);

	for(int i = 0;i < n;i++){ // read input
		*(tarr + i) = (double *)malloc(sizeof(double) * f);
		for(int j = 0;j < f;j++)
			fscanf(finput, "%lf", &(tarr[i][j]));
	}

	for(int i = 0;i < k;i++){ // random medoid k
		int idx = rand() % n;
		index[i] = idx;
		newnode = Newnode(f);
		for(int j = 0;j < f;j++)
			newnode->arr[j] = tarr[idx][j];
		head[i] = newnode;
	}

	for(int i = 0;i < n;i++){ // make cluster k
		// i: tarr idx
		int j = 0;
		for(int x = 0;x < k;x++){ // head
			if(i == index[x]) {
				j = 1;
				break;
			}
		}
		if(j == 1) continue;

		newnode = Newnode(f);
		for(int x = 0;x < f;x++)
			newnode->arr[x] = tarr[i][x];
		
		double nearest = Length(f, head[0]->arr, tarr[i]);
	   	int nearidx = 0; double len;
		for(int x = 1;x < k;x++){
			len = Length(f, head[x]->arr, tarr[i]);
			if(len <= nearest) {
				nearest = len; nearidx = x;
			}
		}
		
		Insertnode(nearidx, &newnode);
	}

	/*
	for(int i = 0;i < k;i++){
		printf("head[%d]\n", i);
		NODE *curr = head[i];
		for(;curr != NULL;curr = curr->link){
			for(int j = 0;j < f;j++){
				printf("%f ", curr->arr[j]);
			}
			printf("\n");
		}
	}
	*/

	for(int it = 0;;it++){
		NODE *curr; int count = 0;
		for(int i = 0;i < k;i++){ // medoid update
			// i: head[i]
			if(UpdateMedoid(i, f, tarr) == 0) count++;
		}
		if(count == k || it == 1000){
			FILE *foutput = fopen("output.txt", "w");
			for(int i = 0;i < k;i++){
				fprintf(foutput, "%d\n", i);
				curr = head[i];
				for(;curr != NULL;curr = curr->link){
					for(int j = 0;j < f;j++)
						fprintf(foutput, "%.8f ", curr->arr[j]);
					fprintf(foutput, "\n");
				}
			}
			fclose(foutput);
			break;
		}

		/*
		for(int i = 0;i < k;i++){
			printf("head[%d]\n", i);
			NODE *curr = head[i];
			for(;curr != NULL;curr = curr->link){
				for(int j = 0;j < f;j++){
					printf("%f ", curr->arr[j]);
				}
				printf("\n");
			}
		} printf("\n");
		*/

		int idx = 0, nidx = 0; 
		double nearest, len;
		curr = head[0]->link;
		NODE *prev = head[0], *tcurr, *tprev;
		while(1){ // curr, prev
			// move curr
			nearest = 9999; nidx = 0;
			for(int i = 0;i < k;i++){ // len with head[i]
				len = Length(f, head[i]->arr, curr->arr);
				//printf("len:%f nearest:%f\n", len, nearest);
				if(len <= nearest){
					nearest = len;
					nidx = i; // head idx
				}
			}

			if(nidx != idx){
				newnode = Newnode(f);
				for(int j = 0;j < f;j++) newnode->arr[j] = curr->arr[j];
				prev->link = curr->link;
				free(curr);

				Insertnode(nidx, &newnode);

				curr = head[0]->link;
				prev = head[0];
				idx = 0;
			}
			else{
				if(curr->link == NULL){ // last
					if(++idx == k) break;
					curr = head[idx]->link;
					prev = head[idx];
				}
				else{
					curr = curr->link;
					prev = prev->link;
				}
			}
		}
	}

	fclose(finput);
	NODE *delete;
	for(int i = 0;i < k;i++){
		newnode = head[i];
		while(newnode){
			delete = newnode;
			newnode = newnode->link;
			free(delete->arr); free(delete);
		}
	}
	for(int i = 0;i < n;i++) free(tarr[i]);
	free(tarr); free(index);
	return 0;
}

NODE* Newnode(int f){
	NODE *newnode = (NODE *)malloc(sizeof(NODE));
	newnode->arr = (double *)malloc(sizeof(double) * f);
	newnode->link = NULL;
	return newnode;
}

double Length(int f, double *a, double *b){
	double sum = 0;
	for(int i = 0;i < f;i++){
		sum += (a[i] - b[i]) * (a[i] - b[i]);
	}

	return sum;
}

void Insertnode(int i, NODE **newnode){
	if(head[i]->link == NULL)
		head[i]->link = *newnode;
	else{
		(*newnode)->link = head[i]->link;
		head[i]->link = *newnode;
	}
}

int UpdateMedoid(int i, int f, double **tarr){ // head[i]
	// find new head[i]
	NODE *temp = head[i]; 
	int nidx = 0, k; 
	double nearest = 9999, len = 0;
	for(k = 0;temp != NULL;k++){ // temp-other
		NODE *curr = head[i]; len = 0;
		for(;curr != NULL;curr = curr->link){
			len += Length(f, temp->arr, curr->arr);
		}
		if(len <= nearest){
			nearest = len;
			nidx = k; // temp index
		}
		//printf("nidx:%d\n", nidx);
		temp = temp->link;
	}

	if(nidx == 0) return 0; // not change
	temp = head[i]->link;
	NODE *prev = head[i];
	for(k = 1;k < nidx;k++){ // temp : medoid
		prev = prev->link; 
		temp = temp->link;
	}
	prev->link = temp->link;
	temp->link = head[i];
	head[i] = temp;
	return 1;
}

