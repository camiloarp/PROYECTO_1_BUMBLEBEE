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
int pointer_inderectodoble;
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
	
	int posinblock = cual-(blocknum*1024);
	
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

void placeinodo(int FE,long size,int donde,FILE* disco,FILE* mp3,char* tag){
inodo ind;
fseek(mp3,0,0);
unsigned char *block;
unsigned char *tagg;
tag = (unsigned char  *)malloc(sizeof(unsigned char )*124);
block = (unsigned char  *)malloc(sizeof(unsigned char )*1024);
ind.size_byte=size;
int sizedec=size;
ind.pointer_FE=FE;
strcpy (ind.tags,tag);
int i;
	for(i=0;i<12;i++){
		if(sizedec>0){
		ind.pointer_directo[i]=libre(disco);
		cambiar(disco,ind.pointer_directo[i],'1');
		fread(block,1024,1,mp3);
		movetoblock(ind.pointer_directo[i],disco);
		fwrite(block,1024,1,disco);
		sizedec=sizedec-1024;
		ind.num_bloques++;
		}

	}
	apunt apu;
	if(sizedec>0){
	ind.pointer_indirecto=libre(disco);
	cambiar(disco,ind.pointer_indirecto,'1');
	int o;
		for(o=0;o<256;o++){
			if(sizedec>0){
			int lib = libre(disco);
			cambiar(disco,lib,'1');
			apu.apuntadores[o]=lib;
			movetoblock(lib,disco);
			fread(block,1024,1,mp3);
			fwrite(block,1024,1,disco);
			sizedec=sizedec-1024;
			ind.num_bloques++;
			}
		}
	movetoblock(ind.pointer_indirecto,disco);
	fwrite(&apu,1024,1,disco);

	}

	apunt apu1;
	if(sizedec>0){
	ind.pointer_inderectodoble=libre(disco);
	cambiar(disco,ind.pointer_inderectodoble,'1');
	int a;
		for(a=0;a<256;a++){
			if(sizedec>0){
			int lib = libre(disco);
			cambiar(disco,lib,'1');
			apu1.apuntadores[a]=lib;
			int c;
			apunt apu2;
				for(c=0;c<256;c++){
					if(sizedec>0){
					int lib1=libre(disco);
					apu2.apuntadores[c]=lib1;
					cambiar(disco,lib1,'1');
					movetoblock(lib1,disco);
					fread(block,1024,1,mp3);
					fwrite(block,1024,1,disco);
					sizedec=sizedec-1024;
					ind.num_bloques++;
					}
				}
			movetoblock(lib,disco);
			fwrite(&apu2,1024,1,disco);
			
			}
		}
	movetoblock(ind.pointer_inderectodoble,disco);
	fwrite(&apu1,1024,1,disco);

	}
fseek(mp3,0,0);
fseek(mp3,size-125,0);
fread(tagg,125,1,mp3);
strcpy(ind.tag_cancion,tagg);

movetoblock(donde,disco);
printf("ftell: %ld \n",ftell(disco));
fwrite(&ind,1024,1,disco);


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

void addFE(FE tempF,header tempH,int tamfor,FILE* disco){

		movetoblock(tamfor+1+tempH.FSuse,disco);
		fwrite(&tempF,sizeof(tempF),1,disco);
}

void addTE(TE tempT,header tempH,int tamfor,FILE* disco){
	if(tempH.TSuse==0){
		movetoblock(tamfor+1,disco);
		fwrite(&tempT,sizeof(tempT),1,disco);
		}else{
		int n;
		for(n=0;n<tempH.TSuse+1;n++){
		fseek(disco,0,0);
		fseek(disco,(1024*(tamfor+1))+(sizeof(tempT)*n),0);
	
		TE temp1;
		fread(&temp1,sizeof(tempT),1,disco);
		if(strcmp(tempT.tag,temp1.tag)<=0){
		fseek(disco,0,0);
		fseek(disco,(1024*(tamfor+1))+(sizeof(tempT)*n),0);
		fwrite(&tempT,sizeof(tempT),1,disco);
		tempT=temp1;
		
		}
		}
	}
}

int istag(int cuantos,int donde,TE tempT,FILE* disco){
	movetoblock(donde,disco);
	
	int i;
	for(i=0;i<cuantos;i++){
	TE tagtemp;
	fread(&tagtemp,sizeof(tagtemp),1,disco);
	if(strcmp(tempT.tag,tagtemp.tag)==0){
	return i;
	}
	}
	return -1;
}

int buscarapuntador(apunt apuntadores){
int i;

for(i=0;i<256;i++){
	if(apuntadores.apuntadores[i]==-1){
	return i;
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
inodo ind;
apunt apuntadores;
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
		memset(tempT.tag,'z',28);
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
		
		strcpy(tempF.filename,argv[2]);
		
		tempF.pointer_inodo=libre(disco);
		cambiar(disco,tempF.pointer_inodo,'1');
		strcpy(tempT.tag,argv[3]);
		int tamfor = tempH.tam_mapabits;
		int tamforT=tamfor+tempH.cant_FS;
		placeinodo(tamfor+tempH.FSuse+1,size,tempF.pointer_inodo,disco,mp3,tempT.tag);
		tempH=getheader(disco);
		tempT.pointer=libre(disco);
		int istagg=istag(tempH.TSuse,tamforT+1,tempT,disco);
			if(istagg==-1){
				cambiar(disco,tempT.pointer,'1');
				addTE(tempT,tempH,tamforT,disco);
				movetoblock(tempT.pointer,disco);
				memset(apuntadores.apuntadores,-1,256);
				apuntadores.apuntadores[0]=tempF.pointer_inodo;
				fwrite(&apuntadores,1024,1,disco);
				tempH.TSuse=tempH.TSuse+1;
			}else{
			movetoblock(1+tamforT+(istagg/tempH.cant_TS),disco);
			fread(&tempT,sizeof(tempT),1,disco);
			movetoblock(tempT.pointer,disco);
			fread(&apuntadores,1024,1,disco);
			int findapuntador = buscarapuntador(apuntadores);
			apuntadores.apuntadores[findapuntador]=tempF.pointer_inodo;			
			movetoblock(tempT.pointer,disco);
			fwrite(&apuntadores,1024,1,disco);
			tempH.TSuse=tempH.TSuse+1;	
			}
		
		
		addFE(tempF,tempH,tamfor,disco);
		
		fseek(disco,0,0);
		tempH.FSuse=tempH.FSuse+1;
		fwrite(&tempH,sizeof(tempH),1,disco);

	
		fclose(mp3);
		fclose(disco);


	}



}


