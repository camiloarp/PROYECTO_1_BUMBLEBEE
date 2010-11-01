//Camilo Rivera
//Jaime Escoto
//leonel Sanchez
//BUMBLEBEE


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int cant_block ;
	int cant_FS;
	int cant_TS;
	int tam_mapabits;
	int inicio_data;

}header;

typedef struct
{
char filename[60];
int pointer_inodo;
}FE;

typedef struct
{
char tag[28];
int pointer;
}TE;

typedef struct
{
int size_byte;
int pointer_directo[12];
int pointer_indirecto;
int pointer_inderectodoble[4];
int pointer_FE;
char tags[250];
char tag_cancion[128];
int num_bloques;
}inodo;

int libre(char* file){

FILE *disco;
if((disco = fopen(file,"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}

	header tempH;
	fseek(disco,0,0);
	fread(&tempH,20,1,disco);
	fseek(disco,0,0);
	char *block;
	block = (char *)malloc(sizeof(char)*1024);
	int i;
	int tamfor = tempH.tam_mapabits;
	
	for(i=0;i<tamfor;i++){
		int a;
		fseek(disco,1024+i*1024,0);
		fread(block,1024,1,disco);
		for (a=0;a<1024;a++){
			
			if(block[a]=='0'){
			return 1024*i+a;
			}		
		}

	}
	
fclose(disco);
return -1;

}

void main(int argc, char*argv[]){
FILE *disco;
FILE *mp3;
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
		int libree = libre(argv[5]);
		printf("return %d \n",libree);

	}


}
