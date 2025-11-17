#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 

// --- CONTADORES GLOBAIS ---
// Pedido no desafio para comparar a eficiência
int g_comparacoes_seq = 0;
int g_comparacoes_bin = 0;
int g_comparacoes_lista = 0;

// --- ESTRUTURA 1: O DADO ---
// O Item que será usado em ambas as mochilas
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

// --- ESTRUTURA 2: VETOR ---
// Um bloco de memória fixo (estático)
#define MAX_ITENS 10
typedef struct {
    Item itens[MAX_ITENS]; // O "estojo"
    int totalItens;        // O "marcador" de quantos itens temos
    int estaOrdenado;      // Flag (0 ou 1) para a busca binária
} MochilaVetor;

// --- ESTRUTURA 3: LISTA ENCADEADA ---
// Blocos de memória dinâmicos ligados por ponteiros
typedef struct No {
    Item dados;         // A "carga"
    struct No* proximo; // O "elo" para o próximo
} No;

// A Lista é apenas um ponteiro para o primeiro 'No'
typedef No* MochilaLista;

// ----------------------------------------------------
// FUNÇÕES UTILITÁRIAS
// ----------------------------------------------------

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Pede ao usuário para preencher os dados de um item
Item lerDadosItem() {
    Item novoItem;
    printf("Nome do item: ");
    fgets(novoItem.nome, 30, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(novoItem.tipo, 20, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada();
    return novoItem;
}

// ----------------------------------------------------
// FUNÇÕES DO VETOR (ARRAY)
// ----------------------------------------------------

// Insere no final do vetor
void inserirItemVetor(MochilaVetor* mochila) {
    if (mochila->totalItens >= MAX_ITENS) {
        printf("[ERRO] Mochila (Vetor) esta cheia!\n");
        return;
    }
    
    printf("\n--- Inserindo no Vetor ---\n");
    Item novoItem = lerDadosItem();
    
    mochila->itens[mochila->totalItens] = novoItem;
    mochila->totalItens++;
    mochila->estaOrdenado = 0; // Inserir desordena o vetor
    
    printf("[SUCESSO] Item '%s' inserido no vetor.\n", novoItem.nome);
}

// Lista todos os itens do vetor
void listarItensVetor(MochilaVetor* mochila) {
    printf("\n--- Itens no Vetor (%d/%d) ---\n", mochila->totalItens, MAX_ITENS);
    if (mochila->totalItens == 0) {
        printf("Mochila (Vetor) vazia.\n");
        return;
    }
    
    for (int i = 0; i < mochila->totalItens; i++) {
        Item item = mochila->itens[i];
        printf("%d. [%s] %s (Qtd: %d)\n", i + 1, item.tipo, item.nome, item.quantidade);
    }
    printf("Vetor esta ordenado? %s\n", mochila->estaOrdenado ? "Sim" : "Nao");
}

// Remove do vetor usando "shift-left"
void removerItemVetor(MochilaVetor* mochila) {
    printf("\n--- Removendo do Vetor ---\n");
    char nomeBusca[30];
    printf("Nome do item a remover: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indiceEncontrado = -1;
    for (int i = 0; i < mochila->totalItens; i++) {
        if (strcmp(mochila->itens[i].nome, nomeBusca) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("[ERRO] Item '%s' nao encontrado no vetor.\n", nomeBusca);
        return;
    }

    // "Shift-left": Cobre o "buraco" puxando os itens da direita
    for (int i = indiceEncontrado; i < mochila->totalItens - 1; i++) {
        mochila->itens[i] = mochila->itens[i + 1];
    }
    
    mochila->totalItens--;
    // Remover não estraga a ordenação, mas vamos simplificar
    // mochila->estaOrdenado = 0; 
    
    printf("[SUCESSO] Item '%s' removido do vetor.\n", nomeBusca);
}

// Ordena o vetor por nome usando Bubble Sort
void ordenarVetor(MochilaVetor* mochila) {
    printf("\nOrdenando vetor com Bubble Sort...\n");
    if (mochila->estaOrdenado) {
        printf("Vetor ja estava ordenado.\n");
        return;
    }

    for (int i = 0; i < mochila->totalItens - 1; i++) {
        for (int j = 0; j < mochila->totalItens - i - 1; j++) {
            // Se j > j+1 (ordem errada), troca
            if (strcmp(mochila->itens[j].nome, mochila->itens[j + 1].nome) > 0) {
                Item temp = mochila->itens[j];
                mochila->itens[j] = mochila->itens[j + 1];
                mochila->itens[j + 1] = temp;
            }
        }
    }
    
    mochila->estaOrdenado = 1;
    printf("[SUCESSO] Vetor ordenado por nome.\n");
}

// Busca Sequencial (lenta)
void buscarSequencialVetor(MochilaVetor* mochila) {
    printf("\n--- Busca Sequencial no Vetor ---\n");
    g_comparacoes_seq = 0; 

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < mochila->totalItens; i++) {
        g_comparacoes_seq++; // Conta +1 comparação
        if (strcmp(mochila->itens[i].nome, nomeBusca) == 0) {
            Item item = mochila->itens[i];
            printf("[ACHOU!] %s (%s) - Qtd: %d\n", item.nome, item.tipo, item.quantidade);
            printf("Busca Sequencial fez %d comparacoes.\n", g_comparacoes_seq);
            return;
        }
    }

    printf("[NAO ACHOU] Item '%s' nao encontrado.\n", nomeBusca);
    printf("Busca Sequencial fez %d comparacoes.\n", g_comparacoes_seq);
}

// Busca Binária (rápida, mas requer ordenação)
void buscarBinariaVetor(MochilaVetor* mochila) {
    printf("\n--- Busca Binaria no Vetor ---\n");
    
    if (!mochila->estaOrdenado) {
        printf("[ERRO] O vetor precisa estar ordenado! Use a Opcao 5.\n");
        return;
    }

    g_comparacoes_bin = 0; 

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    // Lógica de "procurar no dicionário"
    int inicio = 0;
    int fim = mochila->totalItens - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        Item itemMeio = mochila->itens[meio];
        
        g_comparacoes_bin++; // Conta +1 comparação
        int comp = strcmp(nomeBusca, itemMeio.nome);

        if (comp == 0) { // Achou
            printf("[ACHOU!] %s (%s) - Qtd: %d\n", itemMeio.nome, itemMeio.tipo, itemMeio.quantidade);
            printf("Busca Binaria fez %d comparacoes.\n", g_comparacoes_bin);
            return;
        }
        
        if (comp > 0) { // Nome vem "depois"
            inicio = meio + 1;
        } else { // Nome vem "antes"
            fim = meio - 1;
        }
    }

    printf("[NAO ACHOU] Item '%s' nao encontrado.\n", nomeBusca);
    printf("Busca Binaria fez %d comparacoes.\n", g_comparacoes_bin);
}

// ----------------------------------------------------
// FUNÇÕES DA LISTA ENCADEADA
// ----------------------------------------------------

// Insere no começo da lista
void inserirItemLista(MochilaLista* lista) {
    printf("\n--- Inserindo na Lista Encadeada ---\n");
    Item novoItem = lerDadosItem();
    
    // 1. Aloca memória para o novo "elo"
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("[ERRO] Falha ao alocar memoria para a lista!\n");
        return;
    }
    
    // 2. Preenche os dados
    novoNo->dados = novoItem;
    
    // 3. Liga o elo no começo da lista
    novoNo->proximo = *lista; // O novo aponta para o antigo começo
    *lista = novoNo;          // O começo agora é o novo
    
    printf("[SUCESSO] Item '%s' inserido na lista.\n", novoItem.nome);
}

// Lista os itens percorrendo a "corrente"
void listarItensLista(MochilaLista lista) {
    printf("\n--- Itens na Lista Encadeada ---\n");
    if (lista == NULL) { 
        printf("Mochila (Lista) vazia.\n");
        return;
    }
    
    No* atual = lista; // Ponteiro temporário para "caminhar"
    int i = 1;
    while (atual != NULL) { // Enquanto não chegar no fim (NULL)
        Item item = atual->dados;
        printf("%d. [%s] %s (Qtd: %d)\n", i, item.tipo, item.nome, item.quantidade);
        
        atual = atual->proximo; // Pula para o próximo elo
        i++;
    }
}

// Remove um item da lista
void removerItemLista(MochilaLista* lista) {
    printf("\n--- Removendo da Lista Encadeada ---\n");
    if (*lista == NULL) {
        printf("Mochila (Lista) vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a remover: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    No* atual = *lista;
    No* anterior = NULL;

    // 1. Caminha até achar o item
    while (atual != NULL && strcmp(atual->dados.nome, nomeBusca) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // 2. Se 'atual' é NULL, não achou
    if (atual == NULL) {
        printf("[ERRO] Item '%s' nao encontrado na lista.\n", nomeBusca);
        return;
    }

    // 3. "Cirurgia" de ponteiros para remover o 'atual'
    if (anterior == NULL) {
        // Removendo o primeiro item
        *lista = atual->proximo;
    } else {
        // Removendo do meio ou fim
        anterior->proximo = atual->proximo; // O anterior "pula" o atual
    }
    
    free(atual); // Libera a memória do elo removido!
    
    printf("[SUCESSO] Item '%s' removido da lista.\n", nomeBusca);
}

// Busca na lista (só pode ser sequencial)
void buscarSequencialLista(MochilaLista lista) {
    printf("\n--- Busca Sequencial na Lista ---\n");
    g_comparacoes_lista = 0; 

    if (lista == NULL) {
        printf("Mochila (Lista) vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    No* atual = lista;
    while (atual != NULL) {
        g_comparacoes_lista++; // Conta +1 comparação
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            Item item = atual->dados;
            printf("[ACHOU!] %s (%s) - Qtd: %d\n", item.nome, item.tipo, item.quantidade);
            printf("Busca na Lista fez %d comparacoes.\n", g_comparacoes_lista);
            return;
        }
        atual = atual->proximo;
    }

    printf("[NAO ACHOU] Item '%s' nao encontrado.\n", nomeBusca);
    printf("Busca na Lista fez %d comparacoes.\n", g_comparacoes_lista);
}

// Libera toda a memória da lista (ESSENCIAL!)
void liberarMemoriaLista(MochilaLista* lista) {
    No* atual = *lista;
    No* proximo;
    while (atual != NULL) {
        proximo = atual->proximo; // Guarda o endereço do próximo
        free(atual);              // Libera o atual
        atual = proximo;          // Pula para o próximo
    }
    *lista = NULL; // Marca a lista como vazia
}


// ----------------------------------------------------
// FUNÇÃO PRINCIPAL (O ÚNICO MENU)
// ----------------------------------------------------

int main() {
    // Semeia o 'rand' (só usamos na versão anterior, mas é bom manter)
    srand(time(NULL)); 

    // Inicializa as duas mochilas
    MochilaVetor mochilaVetor;
    mochilaVetor.totalItens = 0;
    mochilaVetor.estaOrdenado = 0;
    
    MochilaLista mochilaLista = NULL; // Lista começa vazia
    
    int opcao;
    
    do {
        // --- MENU PRINCIPAL ---
        printf("\n========= COMPARADOR DE ESTRUTURAS =========\n");
        printf("--- Mochila VETOR (Itens: %d/%d) ---\n", mochilaVetor.totalItens, MAX_ITENS);
        printf("--------------------------------------------\n");
        
        printf("--- VETOR ---\n");
        printf(" 1. Inserir Item (Vetor)\n");
        printf(" 2. Remover Item (Vetor)\n");
        printf(" 3. Listar Itens (Vetor)\n");
        printf(" 4. Buscar (Sequencial Vetor)\n");
        printf(" 5. ORDENAR Vetor\n");
        printf(" 6. Buscar (Binaria Vetor)\n");
        
        printf("\n--- LISTA ENCADEADA ---\n");
        printf(" 11. Inserir Item (Lista)\n");
        printf(" 12. Remover Item (Lista)\n");
        printf(" 13. Listar Itens (Lista)\n");
        printf(" 14. Buscar (Sequencial Lista)\n");
        
        printf("\n 0. Sair do Programa\n");
        printf("\nEscolha: ");
        
        scanf("%d", &opcao);
        limparBufferEntrada();
        
        switch (opcao) {
            // --- Opções do VETOR ---
            case 1: inserirItemVetor(&mochilaVetor); break;
            case 2: removerItemVetor(&mochilaVetor); break;
            case 3: listarItensVetor(&mochilaVetor); break;
            case 4: buscarSequencialVetor(&mochilaVetor); break;
            case 5: ordenarVetor(&mochilaVetor); break;
            case 6: buscarBinariaVetor(&mochilaVetor); break;
            
            // --- Opções da LISTA ---
            case 11: inserirItemLista(&mochilaLista); break;
            case 12: removerItemLista(&mochilaLista); break;
            case 13: listarItensLista(mochilaLista); break;
            case 14: buscarSequencialLista(mochilaLista); break;

            case 0:
                printf("Saindo e liberando memoria...\n");
                break;
            default:
                printf("[ERRO] Opcao invalida!\n");
        }
        
    } while (opcao != 0);
    
    // Libera a memória da lista antes de fechar
    liberarMemoriaLista(&mochilaLista);
    
    printf("Programa encerrado.\n");
    return 0;
}