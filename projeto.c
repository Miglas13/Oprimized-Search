//André Miguel da Silva Pinho nº32795 Engenharia Informática

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PESSOA{
	int ano,mes,dia,curso,nota,bi;
	struct PESSOA *nseg;
	struct PESSOA *nant;
}Pessoa;

typedef struct ARVOREAB{
	int nota;
	struct PESSOA *lista;
	struct ARVOREAB *fd;
	struct ARVOREAB *fe;
}ArvoreAB;

typedef struct LISTACURSO{
	int curso;
	struct ARVOREAB *AB;
	struct LISTACURSO *nseg;
}listaCurso;

typedef struct QUERY{
	int nota,curso;
	char symbol;
	struct QUERY *nseg;
}Query;

listaCurso *makeNodeC(){
	listaCurso *nv=(listaCurso *)malloc(sizeof(listaCurso));
	return nv;
}

ArvoreAB *makeNodeAB(){
	ArvoreAB *nvA=(ArvoreAB *)malloc(sizeof(ArvoreAB));
	return nvA;
}

Pessoa *makeNode(){
	Pessoa *nv=(Pessoa *)malloc(sizeof(Pessoa));
	return (nv);
}

Query *makeNodeQ(){
	Query *nv=(Query *)malloc(sizeof(Query));
	return nv;
}

ArvoreAB *insereAB(ArvoreAB *A, ArvoreAB *nvA){
	ArvoreAB *aux=A;
	if(aux==NULL)	return nvA;
	while(1){
		if(nvA->nota>A->nota){
			if(A->fd==NULL){
				A->fd=nvA;
				return aux;
			}
			A=A->fd;
			continue;
		}
		if(A->fe==NULL){
			A->fe=nvA;
			return aux;
		}
		A=A->fe;
	}
	return aux;
}

listaCurso *insertLista(listaCurso *LC, listaCurso *nv){
	if(LC==NULL)
		return (nv);
	nv->nseg=LC;
	return (nv);
}

Pessoa *insertFirst(Pessoa *v, Pessoa *nv){
	Pessoa *aux=v;
	if(aux==NULL)
		return (nv);
	nv->nseg=v;
	aux->nant=nv;
	return (nv);
}

Query *insereQuery(Query *Q, Query *nvQ){
	Query *aux=Q;
	if(aux==NULL)	return nvQ;
	nvQ->nseg=Q;
	return nvQ;
}

ArvoreAB *rodaDireita(ArvoreAB *A){
	ArvoreAB *aux=NULL;
	if(A==NULL)	return NULL;
	aux=A->fe;
	if(aux!=NULL){
		A->fe=aux->fd;
		aux->fd=A;
		A=aux;
	}
	return A;
}

ArvoreAB *rodaEsquerda(ArvoreAB *A){
	ArvoreAB *aux=NULL;
	if(A==NULL)	return NULL;
	aux=A->fd;
	if(aux!=NULL){
		A->fd=aux->fe;
		aux->fe=A;
		A=aux;
	}
	return A;
}

int contaNos(ArvoreAB *A){
	if(A==NULL)	return 0;
	return (contaNos(A->fd)+contaNos(A->fe));
}

int equilibrada(ArvoreAB *A){
	if(A==NULL)	return 1;
	return (contaNos(A->fe)-contaNos(A->fd));
}

ArvoreAB *equilibra(ArvoreAB *A, int eq){
	int i;
	if(eq==-2){
		i=equilibrada(A->fd);
		if(i==-1)	A=rodaEsquerda(A);
		if(i==1){
			A->fd=rodaDireita(A->fd);
			A=rodaEsquerda(A);
		}
	}
	if(eq==2){
		i=equilibrada(A->fe);
		if(i==1)	A=rodaDireita(A);
		if(i==-1){
			A->fe=rodaEsquerda(A->fe);
			A=rodaDireita(A);
		}
	}
	return A;
}

int exists(Pessoa *p, int bi){
	while(p!=NULL){
		if(p->bi==bi)	return 1;
		else	p=p->nseg;
	}
	return 0;
}

Pessoa *procuraNota(ArvoreAB *A, int nota, char symbol, Pessoa *p){
	Pessoa *nv=NULL;
	if(A==NULL)	return p;
	if(symbol=='+'){
		if(nota>A->nota){
			p=procuraNota(A->fd,nota,symbol,p);
			return p;
		}
		if(nota==A->nota){
			p=procuraNota(A->fd,nota,symbol,p);
			return p;
		}
		p=procuraNota(A->fe,nota,symbol,p);
		while(A->lista!=NULL){
			nv=makeNode();
			nv->bi=A->lista->bi;
			if(exists(p,nv->bi)!=1)
				p=insertFirst(p,nv);
			A->lista=A->lista->nseg;
		}
		p=procuraNota(A->fd,nota,symbol,p);
		return p;
	}
	if(symbol=='-'){
		if(nota<A->nota){
			p=procuraNota(A->fe,nota,symbol,p);
			return p;
		}
		if(nota==A->nota){
			p=procuraNota(A->fe,nota,symbol,p);
			return p;
		}
		p=procuraNota(A->fd,nota,symbol,p);
		while(A->lista!=NULL){
			nv=makeNode();
			nv->bi=A->lista->bi;
			if(exists(p,nv->bi)!=1)
				p=insertFirst(p,nv);
			A->lista=A->lista->nseg;
		}
		p=procuraNota(A->fe,nota,symbol,p);
		return p;
	}
}

listaCurso *existe(listaCurso *LC, int curso){
	if(LC==NULL)	return NULL;
	while(LC!=NULL){
		if(LC->curso==curso)	return LC;
		LC=LC->nseg;
	}
	return NULL;
}

listaCurso *abreFicheiro(listaCurso *LC){
	Pessoa *nv=NULL,*P=NULL;
	ArvoreAB *nvA=NULL, *A=NULL;
    	listaCurso *aux=LC, *nvC=NULL;
    	int x,eq;
	FILE *fp=fopen("est_dados_14_15.txt","r");
	if(fp==NULL){
		printf("Erro na abertura do ficheiro.\n");
		exit(1);
	}
	while(!feof(fp)){
        	nv=makeNode();
		if(fscanf(fp,"%d/%d/%d;%d;%d;%d",&nv->dia,&nv->mes,&nv->ano,&nv->bi,&nv->curso,&nv->nota)!=0){
            		aux=existe(aux,nv->curso);	
			if(aux==NULL){
				nvC=makeNodeC();
				nvC->curso=nv->curso;
				LC=insertLista(LC,nvC);
				aux=LC;
			}
            		nvA=makeNodeAB();
			nvA->nota=nv->nota;
			nvA->lista=insertFirst(nvA->lista,nv);
			aux->AB=insereAB(aux->AB,nvA);
			eq=equilibrada(aux->AB);
			if(abs(eq)>1)	aux->AB=equilibra(aux->AB,eq);
			aux=LC;	
		}
	}
	fclose(fp);
	return (aux);
}

Query *splitQuery(char *query){
	Query *Q=NULL, *nvQ=NULL;
	int i,n,c;
	char symbol;
	char *pch;
	for(i=0;i<(strlen(query)-1);i++){
        	if(query[i]=='C'){
			nvQ=makeNodeQ();
            		pch=&query[i+1];
            		c=atoi(pch);
			nvQ->curso=c;
        	}
        	if(query[i]=='+'){
            		symbol=query[i];
			nvQ->symbol=symbol;
            		pch=&query[i+1];
            		n=atoi(pch);
			nvQ->nota=n;
			Q=insereQuery(Q,nvQ);
        	}
        	if(query[i]=='-'){
            		symbol=query[i];
			nvQ->symbol=symbol;
            		pch=&query[i+1];
            		n=atoi(pch);
			nvQ->nota=n;
			Q=insereQuery(Q,nvQ);
        	}
    	}
	return Q;
}

listaCurso *procuraCurso(listaCurso *LC,int c){
	while(LC!=NULL){
		if(LC->curso==c)	return LC;
		LC=LC->nseg;
	}
	return NULL;
}

Pessoa *procuraQuery(listaCurso *LC, Query *Q){
	Pessoa *p=NULL;
	listaCurso *aux=LC;
	while(Q!=NULL){
		LC=procuraCurso(LC,Q->curso);
		p=procuraNota(LC->AB,Q->nota,Q->symbol,p);
		LC=aux;
		Q=Q->nseg;
	}
	return p;
}

Pessoa *findLlargest(Pessoa *p){
	Pessoa *aux=p;
	while(p!=NULL){
		if(p->bi>aux->bi)	aux=p;
		p=p->nseg
	}
	return aux;
}

Pessoa *ordena(Pessoa *p){
	Pessoa *aux,*LO=NULL;
	while(p!=NULL){
		aux=findLargest(p);
		if(aux->nant==NULL){
			p=p->nseg;
			if(p!=NULL)	p=p->nseg;
			aux->nseg=NULL;
		}
		if(aux->nseg==NULL){
			aux->nant->nseg=NULL;
			aux->nant=NULL;
		}
		else{
			aux->nant->nseg=aux->nseg;
			aux->nseg->nant=aux->nant;
			aux->nseg=NULL;
			aux->nant=NULL;
		}
		LO=insertFirst(LO,aux);
	}
	return LO;
}

int totalElements(Pessoa *p){
	Pessoa *aux=p;
	int elements=0;
	while(aux!=NULLl){
		elements++;
		aux=aux->nseg;
	}
	return elements;
}

void show(Pessoa *p){
	while(p!=NULL){
		printf("%d\n",p->bi);
		p=p->nseg;
	}
}

int main(){
	char query[20];
	listaCurso *LC=NULL;
	Pessoa *p=NULL;
	Query *Q=NULL;
	int elements;
	LC=abreFicheiro(LC);
	printf("Introduza a query: ");
	scanf("%s",query);
	Q=splitQuery(query);
    p=procuraQuery(LC,Q);
	p=ordena(p);
	elements=totalElements(p);
	printf("%d\n",elements);
	show(p);
	return (0);
}
