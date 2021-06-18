#include "ArvB.h"

int main() 
{
	FILE *arq = fopen("Entregas.xml", "rt");
	if(arq == NULL) 
	{
		puts("Falha ao abrir arquivo");
		return -1;
	}
	
	TipoApontador dicionario;
	Inicializa(&dicionario);
	
	char linha[TAM_STR], novaDesc[TAM_STR];
	int novaReferencia, opcao;
	float novoCusto;

	while((fscanf(arq, "%s", linha)) != EOF) 
	{
		if(strcmp(linha, "<Entrega>") == 0) 
		{
			fscanf(arq, "%s", linha);
			novaReferencia = pegarreferencia(linha);
			fscanf(arq, "%s", linha);
			strcpy(novaDesc, pegarDescricao(linha));
			fscanf(arq, "%s", linha);
			novoCusto = pegarCusto(linha);
			Insere(criarRegistro(novaReferencia, novaDesc, novoCusto), &dicionario);
		}
	}	

	TipoRegistro registroAux;

	while(1)
    {
        system("cls");
        fflush(stdin);
        printf("Entregas :\n1 = Pesquisar (*referencia)\n2 = Exibe tudo.\n3 = Custo total dos produtos.\n4 = Sair.\n");
        scanf("%c",&opcao);
        system("cls");
        
        switch(opcao)
        {
            case '1':
                
            	printf("Insira o referencia do produto desejado: ");
				scanf("%d", &novaReferencia);
				registroAux.Chave.referencia = novaReferencia;
				if(Pesquisa(&registroAux, dicionario) == FALSE) 
				{
					puts("Produto nao foi encontrado");
				} 
				else 
				{
					printf("Produto foi encontrado:\n");
					printf("Descricao do produto: %s\n", registroAux.Chave.descricao);
					printf("Valor do produto: %.2f\n", registroAux.Chave.custo);
				}
                break;

            case '2':
                puts("Todos os produtos: ");
				Imprime(dicionario);
                break;

            case '3':
               	novoCusto = 0;
				pegarCustoTotal(dicionario, 0, &novoCusto);
				printf("Custo total dos produtos: %.2f\n", novoCusto);
                break;

            case '4':
                system("pause");
                exit(0);
                break;

            default:
                break;
        	
        	system("pause");
    	}
    	system("pause");
	}
	fclose(arq);
    return 0;
}
