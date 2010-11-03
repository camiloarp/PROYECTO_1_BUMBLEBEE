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
	int FSuse;
	int TSuse;
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

typedef struct
{
int apuntadores[256];
}apunt;

void cambiar(FILE *disco,int cual,char por){
	char *block;
	block = (char *)malloc(sizeof(char)*1024);
	header tempH;
	fseek(disco,0,0);
	fread(&tempH,sizeof(tempH),1,disco);
	fseek(disco,0,0);
	int tamfor = tempH.tam_mapabits;
	int blocknum= cual/1024;
	printf("blocknum: %d \n",blocknum);
	int posinblock = cual-(blocknum*1024);
	printf("posinblock: %d \n",posinblock);
	fseek(disco,1024+(blocknum)*1024,0);
	fread(block,1024,1,disco);
	block[posinblock]=por;
	fseek(disco,0,0),
	fseek(disco,1024+blocknum*1024,0);
	fwrite(block,1024,1,disco);

}
header getheader(FILE *disco){
header tempH;
fseek(disco,0,0);
fread(&tempH,sizeof(tempH),1,disco);
fseek(disco,0,0);
return tempH;
}

void movetoblock(int cual,FILE *disco){
fseek(disco,0,0);
fseek(disco,(cual*1024),0);

}

int libre(FILE *disco){


	header tempH;
	fseek(disco,0,0);
	fread(&tempH,sizeof(tempH),1,disco);
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
	

return -1;

}



void main(int argc, char*argv[]){
FILE *disco;
FILE *mp3;
header tempH;
FE tempF;
TE tempT;
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
		tempH.FSuse=0;
	        tempH.TSuse=0;		
		
		tempH.inicio_data=tempH.tam_mapabits+tempH.cant_FS+tempH.cant_TS+1;

		fseek(disco,0,SEEK_SET);
		fwrite(&tempH,1024,1,disco);
		
		int d;
		for(d=0;d<tempH.inicio_data;d++){		
		block[d]='1';
		}

		fseek(disco,1024,SEEK_SET);
		fwrite(block,1024,1,disco);
		int tamfor = tempH.tam_mapabits;		
		movetoblock(tamfor+1,disco);
		memset(tempF.filename,'z',60);
		int g;
		for(g=0;g<tempH.cant_FS*(1024/sizeof(tempF));g++){
		fwrite(&tempF,sizeof(tempF),1,disco);
		}
		int h;
		for(h=0;h<tempH.cant_TS*(1024/sizeof(tempT));h++){
		fwrite(&tempT,sizeof(tempT),1,disco);
		}
		
		fclose(disco);


	}


if(strcmp(argv[1],"-a")==0)
	{
		
			if(argc!=5){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((mp3 = fopen(argv[2],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		if((disco = fopen(argv[4],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		fseek (mp3, 0, SEEK_END);
    		long size=ftell (mp3);

		int cantblock=size/1024;
	
		unsigned char *block;
		block = (unsigned char  *)malloc(sizeof(unsigned char )*1024);

		strcpy(tempF.filename,argv[2]);
		tempF.pointer_inodo=libre(disco);
		cambiar(disco,tempF.pointer_inodo,'1');
		

		tempH=getheader(disco);
		int tamfor = tempH.tam_mapabits;
		
		

		
		
		if(tempH.FSuse==0){
		movetoblock(tamfor+1,disco);
		fwrite(&tempF,sizeof(tempF),1,disco);
		}else{
		int n;
		for(n=0;n<tempH.FSuse+1;n++){
		fseek(disco,0,0);
		fseek(disco,(1024*(tamfor+1))+(sizeof(tempF)*n),0);
	
		FE temp1;
		fread(&temp1,sizeof(tempF),1,disco);
		if(strcmp(tempF.filename,temp1.filename)<=0){
		fseek(disco,0,0);
		fseek(disco,(1024*(tamfor+1))+(sizeof(tempF)*n),0);
		fwrite(&tempF,sizeof(tempF),1,disco);
		tempF=temp1;
		
		}
		}
	}
		
		fseek(disco,0,0);
		tempH.FSuse=tempH.FSuse+1;
		fwrite(&tempH,sizeof(tempH),1,disco);

	
		fclose(mp3);
		fclose(disco);


	}



}


