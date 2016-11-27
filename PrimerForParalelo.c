#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <pthread.h>
  int** matriz_A;
  int** matriz_B;
  int** matriz_C;
  int af, ac, bf, bc, i,j,k;
  int thread_count;
double get_time() {
	struct timespec tiempo;
	double sec;

	if (clock_gettime(CLOCK_REALTIME, &tiempo)==0){
		sec=tiempo.tv_sec+tiempo.tv_nsec/(double) 1000000000.0;
    return sec;
	}	else {
		printf("Obtener tiempo: NO HA HABIDO RESPUESTA DEL GETTIME\n");
		return 0;
	}
}

void* multiplica (void* file){
   int my_file= (int) file;
   int size = af/thread_count + af%thread_count;
   int from = my_file*size; 
   int to = (my_file+1)*size;
if (to > af) to = af;
   int x,y,z,sum;
  printf("entre %d\n",my_file);
   for(x = from; x< to;x++){
	for(y=0;y<bc;y++){
          sum=0;
          for(z=0;z<ac;z++){
              sum+=matriz_A[x][z]*matriz_B[z][y];
          }
	matriz_C[x][y] = sum;
      }
   }
return 0;
}


int main(int argc, char* argv[]){

  thread_count = strtol(argv[1],NULL, 10);
  
  printf("Filas de Matriz A-->");
  scanf("%d", &af);
  printf("Columnas de Matriz  A-->");
  scanf("%d", &ac);
  printf("Filas de matriz B-->");
  scanf("%d", &bf);
  printf("Columnas de matriz  B-->");
  scanf("%d", &bc);
  if(ac!=bf){
             printf("no es posible hacer la multiplicación, las columnas de la matriz A deben ser las mismas que las filas de la matriz B \n");
             system("pause");
             return 0;
  }


  struct tms t1,t2;
  clock_t start, end;
  clock_t astart, aend;
  double start_get, end_get;
  

  matriz_A = (int**) malloc(af*sizeof(int*));
  matriz_B = (int**) malloc(bf*sizeof(int*));
  matriz_C = (int**) malloc(af*sizeof(int*));

  for(i=0;i<af;i++){
	matriz_A[i] = (int*)malloc(ac*sizeof(int));
  }

  for(i=0;i<bf;i++){
	matriz_B[i] = (int*)malloc(bc*sizeof(int));
  }

  for(i=0;i<af;i++){ 
	matriz_C[i] = (int*) malloc(bc*sizeof(int));
  }


  pthread_t* hilos;
  hilos = malloc (thread_count*sizeof(pthread_t));
  astart=times(&t1);
  start=clock();
  start_get=get_time();
  for(i=0;i<thread_count;i++){		
      pthread_create(&hilos[i], NULL, multiplica ,(void*)i);
  }

  for(i=0;i<thread_count;i++){
	pthread_join(hilos[i],NULL);
  } 
  aend=times(&t2);
  end=clock();
  end_get=get_time();
  printf ("salida(times())=%ld\n", (long)(aend-astart));
  printf("La funcion clock() da:%lf\n",((double)(end-start)/CLOCKS_PER_SEC));
  printf ("La función get_time:%lf segundos\n",(end_get-start_get));
  
 
return 0;

	
}



