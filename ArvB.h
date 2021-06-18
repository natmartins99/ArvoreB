#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define TAM_STR 51

#define M 4
#define MM  (M * 2)

#define FALSE 0
#define TRUE  1

typedef struct PRODUTO 
{
	int referencia;
	char descricao[TAM_STR];
	float custo;
} TipoChave;

typedef struct TipoRegistro 
{
	TipoChave Chave;
} TipoRegistro;

typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina 
{
	short n;
	TipoRegistro r[MM];
	TipoApontador p[MM + 1];
} TipoPagina;

void Inicializa(TipoApontador *Dicionario)
{  
	*Dicionario = NULL;
}

int Pesquisa(TipoRegistro *x, TipoApontador Ap)
{ 
	long i = 1;
	if (Ap == NULL) 
	{ 
		
		return FALSE;
	}

	while (i < Ap->n && x->Chave.referencia > Ap->r[i-1].Chave.referencia) 
		i++;

	if (x->Chave.referencia == Ap->r[i-1].Chave.referencia) 
	{ 
		*x = Ap->r[i-1];
		return TRUE;
	}
	if (x->Chave.referencia < Ap->r[i-1].Chave.referencia) 
		return Pesquisa(x, Ap->p[i-1]);
	else 
		return Pesquisa(x, Ap->p[i]);
} 

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir)
{ 
	short naoAchouPosicao;
	int k;
	k = Ap->n;  naoAchouPosicao = (k > 0);

	while (naoAchouPosicao) 
	{ 
		if (Reg.Chave.referencia >= Ap->r[k-1].Chave.referencia) 
		{ 
			naoAchouPosicao = FALSE;
			break;
		}

		Ap->r[k] = Ap->r[k-1];
		Ap->p[k+1] = Ap->p[k];
		k--;
		if (k < 1) naoAchouPosicao = FALSE;
	}

	Ap->r[k] = Reg; 
	Ap->p[k+1] = ApDir;
	Ap->n++;
} 

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno,  TipoApontador *ApRetorno)
{ 
	long i = 1; long j;
  	TipoApontador apTemp;

  	if (Ap == NULL) 
	  { 
  		*Cresceu = TRUE; (*RegRetorno) = Reg; (*ApRetorno) = NULL;
		return;
  	}
	while (i < Ap->n && Reg.Chave.referencia > Ap->r[i-1].Chave.referencia)  
		i++;

  	if (Reg.Chave.referencia == Ap->r[i-1].Chave.referencia) 
	{ 
		printf(" Erro: Registro ja esta presente\n"); *Cresceu = FALSE;
    	return;
  	}

  	if (Reg.Chave.referencia < Ap->r[i-1].Chave.referencia) 
	  	i--;
  	Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);

  	if (!*Cresceu) return;
  	if (Ap->n < MM) 
	  { 
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    	*Cresceu = FALSE;
    	return;
    }
	
	apTemp = (TipoApontador)malloc(sizeof(TipoPagina));
	apTemp->n = 0;  
	apTemp->p[0] = NULL;

	if (i < M + 1) 
	{ 
		InsereNaPagina(apTemp, Ap->r[MM-1], Ap->p[MM]);
		Ap->n--;
		InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
	} 
	else 
		InsereNaPagina(apTemp, *RegRetorno, *ApRetorno);

	for (j = M + 2; j <= MM; j++) 
		InsereNaPagina(apTemp, Ap->r[j-1], Ap->p[j]);

	Ap->n = M;  
	apTemp->p[0] = Ap->p[M+1];
	*RegRetorno = Ap->r[M];  
	*ApRetorno = apTemp;
}

void Insere(TipoRegistro Reg, TipoApontador *Ap) 
{ 
	short Cresceu;
	TipoRegistro RegRetorno;
	TipoPagina *ApRetorno, *apTemp;
	Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);

	if (Cresceu)
	{   
		apTemp = (TipoPagina *)malloc(sizeof(TipoPagina));
		apTemp->n = 1; 
		apTemp->r[0] = RegRetorno;
		apTemp->p[1] = ApRetorno;
		apTemp->p[0] = *Ap;  *Ap = apTemp;
	}
}

void Reconstitui(TipoApontador ApPag, TipoApontador ApPai, int PosPai, short *Diminuiu)
{ 
	TipoPagina *Aux;  long DispAux, j;

  	if (PosPai < ApPai->n)  
	{ 
		Aux = ApPai->p[PosPai+1];  DispAux = (Aux->n - M + 1) / 2;
    	ApPag->r[ApPag->n] = ApPai->r[PosPai];
    	ApPag->p[ApPag->n + 1] = Aux->p[0];  
		ApPag->n++;

    	if (DispAux > 0)  
    	{ 
			for (j = 1; j < DispAux; j++)
        		InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);

      		ApPai->r[PosPai] = Aux->r[DispAux-1];  
			Aux->n -= DispAux;
      		for (j = 0; j < Aux->n; j++) Aux->r[j] = Aux->r[j + DispAux];
      		for (j = 0; j <= Aux->n; j++) Aux->p[j] = Aux->p[j + DispAux];
      		*Diminuiu = FALSE;
    	}
    	else 
      	{ 
			for (j = 1; j <= M; j++)
          		InsereNaPagina(ApPag, Aux->r[j-1], Aux->p[j]);
        	free(Aux);

        	for (j = PosPai + 1; j < ApPai->n; j++) 
          	{ 
				ApPai->r[j-1] = ApPai->r[j]; 
	    		ApPai->p[j] = ApPai->p[j+1]; 
	  		}

        	ApPai->n--;
        	if (ApPai->n >= M) *Diminuiu = FALSE;
      	}
  	}
  	else 
    { 
		Aux = ApPai->p[PosPai-1]; 
		DispAux = (Aux->n - M + 1) / 2;

		for (j = ApPag->n; j >= 1; j--) ApPag->r[j] = ApPag->r[j-1];
			ApPag->r[0] = ApPai->r[PosPai-1];
		for (j = ApPag->n; j >= 0; j--) ApPag->p[j+1] = ApPag->p[j];
			ApPag->n++;

		if (DispAux > 0) 
		{ 
			for (j = 1; j < DispAux; j++)
          		InsereNaPagina(ApPag, Aux->r[Aux->n - j], 
	        Aux->p[Aux->n - j + 1]);
        	ApPag->p[0] = Aux->p[Aux->n - DispAux + 1];
        	ApPai->r[PosPai-1] = Aux->r[Aux->n - DispAux];
        	Aux->n -= DispAux;  
			*Diminuiu = FALSE;
      	}
      	else 
        { 
			for (j = 1; j <= M; j++)
            	InsereNaPagina(Aux, ApPag->r[j-1], ApPag->p[j]);
          	free(ApPag);  ApPai->n--;
          	if (ApPai->n >= M)  *Diminuiu = FALSE;
        }
    }
}

void Antecessor(TipoApontador Ap, int Ind, TipoApontador ApPai, short *Diminuiu)
{
	if (ApPai->p[ApPai->n] != NULL) 
  	{ 
		Antecessor(Ap, Ind, ApPai->p[ApPai->n], Diminuiu);
    	if (*Diminuiu) 
    		Reconstitui(ApPai->p[ApPai->n], ApPai, (long)ApPai->n, Diminuiu);
    	return;
  	}

  	Ap->r[Ind-1] = ApPai->r[ApPai->n - 1]; 
  	ApPai->n--;  *Diminuiu = (ApPai->n < M);
} 

void Ret(TipoChave Ch, TipoApontador *Ap, short *Diminuiu)
{ 
	long j, Ind = 1;
	TipoApontador Pag;
	if (*Ap == NULL) 
	{ 
		printf("Erro: registro nao esta na arvore\n"); 
		*Diminuiu = FALSE;
		return;
	}

  	Pag = *Ap;
  	while (Ind < Pag->n && Ch.referencia > Pag->r[Ind-1].Chave.referencia) 
		Ind++;

  	if (Ch.referencia == Pag->r[Ind-1].Chave.referencia) 
	{ 
	  	if (Pag->p[Ind-1] == NULL)   
	    { 
			Pag->n--;
	      	*Diminuiu = (Pag->n < M);
	      	for (j = Ind; j <= Pag->n; j++) 
	        { 
				Pag->r[j-1] = Pag->r[j];  
				Pag->p[j] = Pag->p[j+1]; 
			}
	      	return;
	    }

	    Antecessor(*Ap, Ind, Pag->p[Ind-1], Diminuiu);
	    if (*Diminuiu) 
	    	Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
	    return;
	}
	if (Ch.referencia > Pag->r[Ind-1].Chave.referencia) 
		Ind++;
	Ret(Ch, &Pag->p[Ind-1], Diminuiu);
	if (*Diminuiu) 
		Reconstitui(Pag->p[Ind-1], *Ap, Ind - 1, Diminuiu);
}  

void Retira(TipoChave Ch, TipoApontador *Ap)
{ 
	short Diminuiu;
	TipoApontador Aux;
	Ret(Ch, Ap, &Diminuiu);
	if (Diminuiu && (*Ap)->n == 0)  
	{ 
		Aux = *Ap;   
		*Ap = Aux->p[0]; 
		free(Aux);
	}
}

void ImprimeI(TipoApontador p, int nivel)
{ 
	long i;
	if (p == NULL) return;

	for (i = 0; i < p->n; i++) 
	{
		puts("Proximo produto:");
		printf("referencia: %d\n", p->r[i].Chave.referencia);
		printf("Descricao: %s\n", p->r[i].Chave.descricao);
		printf("Custo: %.2f\n\n", p->r[i].Chave.custo);
	}

	nivel++;
	for (i = 0; i <= p->n; i++)
		ImprimeI(p->p[i], nivel);
} 

void Imprime(TipoApontador p){ 
	int n = 0;
	ImprimeI(p, n);
}

void pegarCustoTotal(TipoApontador p, int nivel, float *somaCustos){ 
	long i;
	if (p == NULL) return;

	for (i = 0; i < p->n; i++) 
	{
		*somaCustos += p->r[i].Chave.custo;
	}

	nivel++;
	for (i = 0; i <= p->n; i++)
		pegarCustoTotal(p->p[i], nivel, somaCustos);
} 

int pegarreferencia(char *referenciareferencia) {
	while(*referenciareferencia != '>') 
	{
		referenciareferencia++;
	}
	referenciareferencia++;
	
	char numStr[TAM_STR];
	strcpy(numStr, "");
	int i;

	for(i = 0; *referenciareferencia != '<'; i++, referenciareferencia++) 
	{
		numStr[i] = *referenciareferencia;
	}
	return atoi(numStr);
}

char *pegarDescricao(char *referenciaDescricao) 
{
	while(*referenciaDescricao != '>') 
	{
		referenciaDescricao++;
	}
	referenciaDescricao++;
	
	char *descricao = (char*)malloc(TAM_STR * sizeof(char));
	strcpy(descricao, "");
	int i;

	for(i = 0; *referenciaDescricao != '<'; i++, referenciaDescricao++) 
	{
		descricao[i] = *referenciaDescricao;
	}

	descricao[i] = '\0';
	return descricao;
}

float pegarCusto(char *referenciaCusto) 
{
	while(*referenciaCusto != '>') 
	{
		referenciaCusto++;
	}
	referenciaCusto++;
	
	char numStr[TAM_STR];
	strcpy(numStr, "");
	int i;
	for(i = 0; *referenciaCusto != '<'; i++, referenciaCusto++) 
	{
		numStr[i] = (*referenciaCusto == ',') ? '.' : *referenciaCusto;	
	}
	return atof(numStr);
}

TipoRegistro criarRegistro(int referencia, char *descricao, float custo) 
{
	TipoRegistro tr;
	
	tr.Chave.referencia = referencia;
	strcpy(tr.Chave.descricao, descricao);
	tr.Chave.custo = custo;
	
	return tr;	
}