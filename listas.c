#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1. A "planta" de como será cada item
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// 2. Constante para o tamanho da mochila
#define MAX_ITENS 10

// ----------------------------------------------------
// FUNÇÕES
// ----------------------------------------------------

// Limpa o buffer de entrada (stdin)
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Adiciona um item na mochila
void inserirItem(Item mochila[], int* totalItens) {
    printf("\n--- 1. Inserir Item ---\n");

    // Checa se a mochila está cheia
    if (*totalItens >= MAX_ITENS) {
        printf("[ERRO] A mochila esta cheia! (Max: %d)\n", MAX_ITENS);
        return;
    }

    // O próximo item será salvo no índice "totalItens"
    int novoIndice = *totalItens;

    printf("Nome do item: ");
    fgets(mochila[novoIndice].nome, 30, stdin);
    mochila[novoIndice].nome[strcspn(mochila[novoIndice].nome, "\n")] = '\0';

    printf("Tipo do item (ex: Arma, Cura): ");
    fgets(mochila[novoIndice].tipo, 20, stdin);
    mochila[novoIndice].tipo[strcspn(mochila[novoIndice].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[novoIndice].quantidade);
    limparBufferEntrada(); // Limpa o \n do scanf

    // Atualiza o contador de itens
    (*totalItens)++;
    
    printf("\n[SUCESSO] Item '%s' adicionado a mochila.\n", mochila[novoIndice].nome);
}

// Lista todos os itens atualmente na mochila
void listarItens(Item mochila[], int totalItens) {
    printf("\n--- 3. Listar Itens ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("Itens na Mochila (%d/%d):\n", totalItens, MAX_ITENS);
    // O loop vai até "totalItens" (lógico) e não "MAX_ITENS" (físico)
    for (int i = 0; i < totalItens; i++) {
        printf("%d. [%s] %s (Qtd: %d)\n",
               i + 1,
               mochila[i].tipo,
               mochila[i].nome,
               mochila[i].quantidade);
    }
}

// Busca um item pelo nome e exibe seus dados (Busca Sequencial)
void buscarItem(Item mochila[], int totalItens) {
    printf("\n--- 4. Buscar Item ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia. Nao ha nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("Digite o nome exato do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Busca Sequencial: "Olhe um por um, do início ao fim"
    for (int i = 0; i < totalItens; i++) {
        // strcmp == 0 significa "strings são exatamente iguais"
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[ENCONTRADO!]\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Qtd: %d\n", mochila[i].quantidade);
            return; // Achou, sai da função
        }
    }

    // Se o loop terminou e não achou
    printf("\n[NAO ENCONTRADO] Item '%s' nao esta na mochila.\n", nomeBusca);
}


// Remove um item da mochila pelo nome
void removerItem(Item mochila[], int* totalItens) {
    printf("\n--- 2. Remover Item ---\n");

    if (*totalItens == 0) {
        printf("A mochila ja esta vazia.\n");
        return;
    }

    // 1. Encontrar o índice do item
    char nomeBusca[30];
    printf("Digite o nome exato do item a remover: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indiceEncontrado = -1; // -1 = "não achei ainda"
    for (int i = 0; i < *totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            indiceEncontrado = i;
            break; // Para o loop assim que achar
        }
    }

    // 2. Checar se o item foi encontrado
    if (indiceEncontrado == -1) {
        printf("\n[NAO ENCONTRADO] Item '%s' nao esta na mochila.\n", nomeBusca);
        return;
    }

    // 3. Lógica do "Shift-Left" para cobrir o buraco
    // Puxa todos os itens da direita para a esquerda,
    // começando da posição que queremos apagar.
    
    for (int i = indiceEncontrado; i < *totalItens - 1; i++) {
        mochila[i] = mochila[i + 1]; // Copia a struct inteira
    }

    // 4. Atualizar o contador
    (*totalItens)--; 
    
    printf("\n[SUCESSO] Item '%s' foi removido.\n", nomeBusca);
}


// ----------------------------------------------------
// FUNÇÃO PRINCIPAL (O "Menu")
// ----------------------------------------------------

int main() {
    Item mochila[MAX_ITENS];
    int totalItens = 0; // Contador de quantos itens realmente temos
    int opcao;

    do {
        printf("\n----------------------------------\n");
        printf("MOCHILA VIRTUAL (Itens: %d/%d)\n", totalItens, MAX_ITENS);
        printf("----------------------------------\n");
        printf("1. Inserir Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("\nEscolha uma opcao: ");

        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o '\n' que o scanf deixou!

        switch (opcao) {
            case 1:
                // Passa o ENDEREÇO (&) para a função poder MUDAR o valor
                inserirItem(mochila, &totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens);
                break;
            case 3:
                // Passa o VALOR, pois a função só vai LER
                listarItens(mochila, totalItens);
                break;
            case 4:
                buscarItem(mochila, totalItens);
                break;
            case 0:
                printf("Fechando a mochila...\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0); // Repete o menu enquanto a opção não for 0

    return 0;
}