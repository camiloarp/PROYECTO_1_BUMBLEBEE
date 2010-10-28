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
