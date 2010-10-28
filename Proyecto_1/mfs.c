#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int cant_block;
	int cant_FS;
	int cant_TS;
	int tam_mapabits;
	int inicio_data;

}header;

void marcarbit(int cual,int valor_bit,unsigned char *m){


      
 
     if (valor_bit==1)
       m[cual] = m[cual] | (1 << (7-cual%8));
     else
       m[cual] = m[cual] & ~(1 << (7-cual%8));
         
    
}


void mostrarbits(unsigned char byte)
{
   unsigned char MASK = 1 << 7; 


       printf("%c", byte & MASK ? '1' : '0' );
       byte <<= 1; 
      
   
}

void mostrarmapa(unsigned char *m, int cuantos)
{
   int i;
   for (i=0; i< cuantos; i++)
   {
     mostrarbits(m[i]);      
   }
   printf("\n ------- fin del mapa --------\n");
}

void main(int argc, char*argv[]){
FILE *disco;
header tempH;


	if(strcmp(argv[1],"-c")==0)
	{
		
			if(argc!=6){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[5],"wb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		char *block;
		block = (char *)malloc(sizeof(char)*1024);
		memset(block,'0',1024);
		int i;
		int  cant_block;
		cant_block = atoi(argv[2]);
		
		for (i=0;i<cant_block;i++){
		fwrite(block,1024,1,disco);

		}
		
		tempH.cant_block = cant_block;
		tempH.cant_FS= atoi(argv[3]);
		tempH.cant_TS= atoi(argv[4]);
		tempH.tam_mapabits = (cant_block/8)/1024;
		tempH.inicio_data=tempH.tam_mapabits+tempH.cant_FS+tempH.cant_TS+1;
		fseek(disco,0,SEEK_SET);
		fwrite(&tempH,1024,1,disco);		
		int d;
		for(d=0;d<tempH.inicio_data;d++){		
		block[d]='1';
		}
		fseek(disco,1024,SEEK_SET);
		fwrite(block,1024,1,disco);

		
				

		

		
		fclose(disco);



	}



}
