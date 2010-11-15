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
int num_bloques;
char tags[250];
char tag_cancion[128];
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
inodo ind2;
fseek(mp3,0,0);
unsigned char *block;
unsigned char *tagg;
tagg = (unsigned char  *)malloc(sizeof(unsigned char )*125);
block = (unsigned char  *)malloc(sizeof(unsigned char )*1024);
ind2.size_byte=size;
ind2.num_bloques=0;
int sizedec=size;
ind2.pointer_FE=FE;
strcpy (ind2.tags,tag);
int bloquesc=0;
int i;
	for(i=0;i<12;i++){
		if(sizedec>0){
		ind2.pointer_directo[i]=libre(disco);
		cambiar(disco,ind2.pointer_directo[i],'1');
		fread(block,1024,1,mp3);
		movetoblock(ind2.pointer_directo[i],disco);
		fwrite(block,1024,1,disco);
		sizedec=sizedec-1024;
		bloquesc++;
		
		}

	}
	
	apunt apu;
	if(sizedec>0){
	ind2.pointer_indirecto=libre(disco);
	cambiar(disco,ind2.pointer_indirecto,'1');
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
			bloquesc++;
			}
		}
	movetoblock(ind2.pointer_indirecto,disco);
	fwrite(&apu,1024,1,disco);

	}

	apunt apu1;
	if(sizedec>0){
	ind2.pointer_inderectodoble=libre(disco);
	cambiar(disco,ind2.pointer_inderectodoble,'1');
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
					bloquesc++; 
					
					}
				}
			movetoblock(lib,disco);
			fwrite(&apu2,1024,1,disco);
			
			}
		}
	movetoblock(ind2.pointer_inderectodoble,disco);
	fwrite(&apu1,1024,1,disco);

	}

ind2.num_bloques=bloquesc;
fseek(mp3,0,0);
fseek(mp3,size-125,0);
fread(tagg,30,1,mp3);
strcpy(ind2.tag_cancion,tagg);
fread(tagg,60,1,mp3);
strcat(ind2.tag_cancion,";");
strcat(ind2.tag_cancion,tagg);
fread(tagg,34,1,mp3);
strcat(ind2.tag_cancion,";");
strcat(ind2.tag_cancion,tagg);
movetoblock(donde,disco);
fwrite(&ind2,1024,1,disco);

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

void addFE(FE tempF,header tempH,FILE* disco){
		int tamfor = tempH.tam_mapabits;
		movetoblock(tamfor+1+tempH.FSuse,disco);
		fwrite(&tempF,sizeof(tempF),1,disco);
}

void removeFE(FE tempF,header tempH,FILE* disco){
FE TFE;
int i;
int tamfor = tempH.tam_mapabits;
int here;
	for(i=0;i<tempH.FSuse;i++){
	movetoblock(tamfor+1+i,disco);
	fread(&TFE,sizeof(FE),1,disco);
	if(strcmp(tempF.filename,TFE.filename)==0){
		here = i;
		}
	}
int c;

	for(c=here;c<tempH.FSuse-1;c++){
	movetoblock(tamfor+1+c+1,disco);
	fread(&TFE,sizeof(FE),1,disco);
	movetoblock(tamfor+1+c,disco);
	fwrite(&TFE,sizeof(FE),1,disco);
	}
fseek(disco,0,0);
tempH.FSuse--;
fwrite(&tempH,1024,1,disco);

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

FE buscar(char* filename,FILE* disco){
header tempH;
tempH = getheader(disco);
movetoblock(tempH.tam_mapabits+1,disco);
int i;
FE tempF;
	for(i=0;i<tempH.FSuse;i++){
	fread(&tempF,sizeof(FE),1,disco);
		if(strcmp(tempF.filename,filename)==0){
		
		return tempF;		
		}

	}
strcpy(tempF.filename,"NULL");
return tempF;
}

TE buscartag(char* tagname,FILE* disco){
header tempH;
tempH = getheader(disco);
movetoblock(tempH.tam_mapabits+1+tempH.cant_FS,disco);
int i;
TE tempT;
	for(i=0;i<tempH.TSuse;i++){
	fread(&tempT,sizeof(TE),1,disco);
		if(strcmp(tempT.tag,tagname)==0){
		
		return tempT;		
		}

	}
strcpy(tempT.tag,"NULL");
return tempT;

}

apunt memsetint(apunt apuntadores){
int i;
		for(i=0;i<256;i++){
		apuntadores.apuntadores[i]=-1;
		}
return apuntadores;
}

void taggear(header tempH,int tamforT,TE tempT,FILE* disco,FE tempF,apunt apuntadores){
			int istagg=istag(tempH.TSuse,tamforT+1,tempT,disco);
			if(istagg==-1){
				cambiar(disco,tempT.pointer,'1');
				addTE(tempT,tempH,tamforT,disco);
				movetoblock(tempT.pointer,disco);
				apuntadores=memsetint(apuntadores);
				apuntadores.apuntadores[0]=tempF.pointer_inodo;
				fwrite(&apuntadores,1024,1,disco);
				fseek(disco,0,0);
				tempH.TSuse=tempH.TSuse+1;
				fwrite(&tempH,1024,1,disco);

			}else{
			movetoblock(1+tamforT+(istagg/tempH.cant_TS),disco);
			fread(&tempT,sizeof(tempT),1,disco);
			movetoblock(tempT.pointer,disco);
			fread(&apuntadores,1024,1,disco);
			int findapuntador = buscarapuntador(apuntadores);
			apuntadores.apuntadores[findapuntador]=tempF.pointer_inodo;			
			movetoblock(tempT.pointer,disco);
			fwrite(&apuntadores,1024,1,disco);
			}	
}	

void main(int argc, char*argv[]){
FILE *disco;
FILE *mp3;
header tempH;
FE tempF;
TE tempT;
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
		movetoblock(tamfor+1+tempH.cant_FS,disco);
		memset(tempT.tag,'z',28);
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
		tempH=getheader(disco);
		int tamfor = tempH.tam_mapabits;
		int tamforT=tamfor+tempH.cant_FS;
		placeinodo(tamfor+tempH.FSuse+1,size,tempF.pointer_inodo,disco,mp3,tempT.tag);		
		tempT.pointer=libre(disco);
		taggear(tempH,tamforT,tempT,disco,tempF,apuntadores);
		tempH=getheader(disco);	
		addFE(tempF,tempH,disco);		
		fseek(disco,0,0);
		tempH.FSuse=tempH.FSuse+1;
		fwrite(&tempH,1024,1,disco);

	
		fclose(mp3);
		fclose(disco);


	}



	if(strcmp(argv[1],"-e")==0)
	{
	FILE* exp;	
			if(argc!=5){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[4],"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}




		FE tempF;
		tempF=buscar(argv[2],disco);
			if(strcmp(tempF.filename,"NULL")==0){
			printf("No se encuentra el archivo.\n");
			exit(4);		
			}
			if((exp = fopen(argv[3],"wb"))==NULL)
			{
			printf("No se pudo abrir el archivo destino.\n");
			exit(3);
			}
			
			inodo ind1;
			movetoblock(tempF.pointer_inodo,disco);
			fread(&ind1,sizeof(inodo),1,disco);
			int size= ind1.size_byte;
			int numb= ind1.num_bloques;
			unsigned char *block;
			block = (unsigned char  *)malloc(sizeof(unsigned char )*1024);
			int i;
			for(i=0;i<12;i++){
				if(numb>1){
				movetoblock(ind1.pointer_directo[i],disco);
				fread(block,1024,1,disco);
				fwrite(block,1024,1,exp);
				numb--;
				size=size-1024;
				}

			}

				if(numb>1){
				movetoblock(ind1.pointer_indirecto,disco);
				apunt apu;
				fread(&apu,sizeof(apunt),1,disco);
				int o;
					for(o=0;o<256;o++){
						if(numb>1){
							movetoblock(apu.apuntadores[o],disco);
							fread(block,1024,1,disco);
							fwrite(block,1024,1,exp);
							size=size-1024;
							numb--;
						}
					}

				}

			if(numb>1){
			movetoblock(ind1.pointer_inderectodoble,disco);
			apunt apu1;
			fread(&apu1,sizeof(apunt),1,disco);
			int a;
				for(a=0;a<256;a++){
					if(numb>1){
					movetoblock(apu1.apuntadores[a],disco);
					int c;
					apunt apu2;
					fread(&apu2,sizeof(apunt),1,disco);
						for(c=0;c<256;c++){
							if(numb>1){
							movetoblock(apu2.apuntadores[c],disco);
							fread(block,1024,1,disco);
							fwrite(block,1024,1,exp);
							size=size-1024;
							numb--;
							
							}
						}

			
					}
				}

		}
		
		unsigned char *finalb;
		finalb = (unsigned char  *)malloc(sizeof(unsigned char )*size);
		fread(finalb,size,1,disco);
		fwrite(finalb,size,1,exp);
		
		fclose(disco);
		fclose(exp);	

	}

	if(strcmp(argv[1],"-t")==0)
	{
		
			if(argc!=5){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[4],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		tempH=getheader(disco);
		int tamfor = tempH.tam_mapabits;
		int tamforT=tamfor+tempH.cant_FS;
		tempF=buscar(argv[3],disco);
		strcpy(tempT.tag,argv[2]);
		tempT.pointer=libre(disco);
		taggear(tempH,tamforT,tempT,disco,tempF,apuntadores);
		tempH=getheader(disco);
		fseek(disco,0,0);
		fwrite(&tempH,sizeof(1024),1,disco);
		inodo ind2;
		
		movetoblock(tempF.pointer_inodo,disco);
		fread(&ind2,sizeof(inodo),1,disco);
		strcat (ind2.tags,";");
		strcat (ind2.tags,argv[2]);
		movetoblock(tempF.pointer_inodo,disco);
		fwrite(&ind2,sizeof(inodo),1,disco);
		fclose(disco);


	}

if(strcmp(argv[1],"-it")==0)
	{
		
			if(argc!=4){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[3],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		tempF=buscar(argv[2],disco);
		inodo ind3;
		movetoblock(tempF.pointer_inodo,disco);
		fread(&ind3,sizeof(inodo),1,disco);
		printf("%s \n",ind3.tags);
		fclose(disco);


	}

if(strcmp(argv[1],"-s")==0)
	{
		
			if(argc!=4){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[3],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		tempT=buscartag(argv[2],disco);
		movetoblock(tempT.pointer,disco);
		apunt apuntador;
		fread(&apuntador,sizeof(apunt),1,disco);
		int i;
		for(i=0;i<256;i++){
		int apu=apuntador.apuntadores[i];
		if(apu>-1){
		printf("%d;",apu);
		}
		}
		printf("\n");
		fclose(disco);


	}

if(strcmp(argv[1],"-l")==0)
	{
		
			if(argc!=3){
			printf("FALTAS DE ARGUMENTOS");
			exit(1);
			}

		if((disco = fopen(argv[2],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		tempH=getheader(disco);
		movetoblock(tempH.tam_mapabits+1+tempH.cant_FS,disco);
		int i;
		for(i=0;i<tempH.TSuse;i++){
		fread(&tempT,sizeof(TE),1,disco);
		printf("%s;",tempT.tag);
		}
		printf("\n");
		fclose(disco);


	}

if(strcmp(argv[1],"-q")==0)
	{
	FILE* inodofile;	
		if(argc!=4){
		printf("FALTAS DE ARGUMENTOS");
		 exit(1);
			}

		if((disco = fopen(argv[3],"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}


		tempF=buscar(argv[2],disco);
		if(strcmp(tempF.filename,"NULL")==0){
		printf("No se encuentra el archivo.\n");
		exit(3);
		}
		if((inodofile = fopen("inodo.ind","wb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		inodo ind4;
		movetoblock(tempF.pointer_inodo,disco);
		fread(&ind4,sizeof(inodo),1,disco);
		fwrite(&ind4,sizeof(inodo),1,inodofile);
		fclose(inodofile);
		fclose(disco);


	}

if(strcmp(argv[1],"-qi")==0)
	{
	FILE* inodofile1;	
		if(argc!=4){
		printf("FALTAS DE ARGUMENTOS");
		 exit(1);
			}

		if((disco = fopen(argv[3],"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}

		if((inodofile1 = fopen(argv[2],"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		
		inodo ind5;
		fread(&ind5,sizeof(inodo),1,inodofile1);
		movetoblock(ind5.pointer_FE,disco);
		fread(&tempF,sizeof(FE),1,disco);
		printf("%s//",tempF.filename);
		printf("%d//",ind5.size_byte);
		printf("%s//",ind5.tags);
		printf("%s\n",ind5.tag_cancion);
	

				
		fclose(inodofile1);
		fclose(disco);


	}

if(strcmp(argv[1],"-qa")==0)
	{
		
		if(argc!=4){
		printf("FALTAS DE ARGUMENTOS");
		 exit(1);
			}

		if((disco = fopen(argv[3],"rb"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}

		tempF=buscar(argv[2],disco);
		if(strcmp(tempF.filename,"NULL")==0){
		printf("No se encuentra el archivo.\n");
		exit(3);
		}
		inodo ind6;
		movetoblock(tempF.pointer_inodo,disco);
		fread(&ind6,sizeof(inodo),1,disco);
		printf("%s//",tempF.filename);
		printf("%d//",ind6.size_byte);
		printf("%s//",ind6.tags);
		printf("%s\n",ind6.tag_cancion);
	

				
		
		fclose(disco);


	}

if(strcmp(argv[1],"-d")==0)
	{
		
		if(argc!=4){
		printf("FALTAS DE ARGUMENTOS");
		 exit(1);
			}

		if((disco = fopen(argv[3],"rb+"))==NULL)
		{
		printf("No se pudo abrir el archivo origen.\n");
		exit(2);
		}
		tempH=getheader(disco);
		tempF=buscar(argv[2],disco);
		int w;
		movetoblock(tempH.tam_mapabits+1+tempH.cant_FS,disco);
		for (w=0;w<tempH.TSuse;w++){
		fread(&tempT,sizeof(TE),1,disco);
		movetoblock(tempT.pointer,disco);	
		apunt apuntador;
		fread(&apuntador,sizeof(apunt),1,disco);
		int v;
		for(v=0;v<256;v++){
		int apu=apuntador.apuntadores[v];
		if(apu==tempF.pointer_inodo){
			apuntador.apuntadores[v]=-1;
				}
			}
		movetoblock(tempT.pointer,disco);
		
		fwrite(&apuntador,sizeof(apunt),1,disco);
		movetoblock(tempH.tam_mapabits+1+tempH.cant_FS,disco);
		fseek(disco,sizeof(TE),0);
		}

		inodo ind7;
		movetoblock(tempF.pointer_inodo,disco);
		fread(&ind7,sizeof(inodo),1,disco);
		int numb= ind7.num_bloques;
			int i;
			for(i=0;i<12;i++){
				if(numb>0){
				cambiar(disco,ind7.pointer_directo[i],'0');
				numb--;
				
				}

			}

				if(numb>0){
				movetoblock(ind7.pointer_indirecto,disco);
				apunt apu;
				fread(&apu,sizeof(apunt),1,disco);
				int o;
					for(o=0;o<256;o++){
						if(numb>0){
							cambiar(disco,apu.apuntadores[o],'0');
							numb--;
						}
					}

				}
			cambiar(disco,ind7.pointer_indirecto,'0');
			if(numb>0){
			movetoblock(ind7.pointer_inderectodoble,disco);
			apunt apu1;
			fread(&apu1,sizeof(apunt),1,disco);
			int a;
				for(a=0;a<256;a++){
					if(numb>0){
					movetoblock(apu1.apuntadores[a],disco);
					int c;
					apunt apu2;
					fread(&apu2,sizeof(apunt),1,disco);
						for(c=0;c<256;c++){
							if(numb>0){
							cambiar(disco,apu2.apuntadores[c],'0');
							numb--;
							
							}
						}

					cambiar(disco,apu1.apuntadores[a],'0');
					}
				}
			cambiar(disco,ind7.pointer_inderectodoble,'0');

		}
	
		cambiar(disco,tempF.pointer_inodo,'0');
		removeFE(tempF,tempH,disco);	
		tempH=getheader(disco);
		tempH.FSuse--;
		fseek(disco,0,0);
		fwrite(&tempH,1024,1,disco);	
		
		fclose(disco);


	}

}


