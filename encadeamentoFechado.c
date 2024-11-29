#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_TABELA_INICIAL 20
#define FATOR_CARGA 0.7

typedef struct Produto {
    int id;
    char nome[50];
    float preco;
    int estoque;
    struct Produto* proximo;  // Ponteiro para o próximo produto na lista encadeada
} Produto;

Produto** tabela_hash;
int tamanho_tabela = TAMANHO_TABELA_INICIAL;
int num_produtos = 0;

// Função hash
int hash(int chave) {
    return chave % tamanho_tabela;
}

// Redimensiona a tabela hash
void redimensionar_tabela() {
    clock_t inicio = clock();  // Marca o tempo de início da operação

    int novo_tamanho = tamanho_tabela * 2;
    Produto** nova_tabela = (Produto*)malloc(sizeof(Produto) * novo_tamanho);
    
    if (nova_tabela == NULL) {
        printf("Erro ao alocar memória para redimensionamento.\n");
        return;
    }

    // Inicializa a nova tabela
    for (int i = 0; i < novo_tamanho; i++) {
        nova_tabela[i] = NULL;
    }

    // Reinsere os produtos na nova tabela
    for (int i = 0; i < tamanho_tabela; i++) {
        Produto* produto = tabela_hash[i];
        while (produto != NULL) {
            int indice = hash(produto->id) % novo_tamanho;
            Produto* novo_produto = (Produto*)malloc(sizeof(Produto));
            *novo_produto = *produto;
            novo_produto->proximo = nova_tabela[indice];
            nova_tabela[indice] = novo_produto;
            produto = produto->proximo;
        }
    }

    // Libera a memória da tabela antiga
    free(tabela_hash);
    tabela_hash = nova_tabela;
    tamanho_tabela = novo_tamanho;
    printf("\nTabela redimensionada para %d.\n", tamanho_tabela);

    clock_t fim = clock();  // Marca o tempo de término da operação
    printf("\nTempo de redimensionamento: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}

// Insere produto na tabela hash
void inserir_produto(int id, const char *nome, float preco, int estoque) {
    clock_t inicio = clock();  // Marca o tempo de início da operação

    // Verifica se a tabela precisa ser redimensionada
    if ((float)num_produtos / tamanho_tabela >= FATOR_CARGA) {
        redimensionar_tabela();
    }

    int indice = hash(id);
    Produto* novo_produto = (Produto*)malloc(sizeof(Produto));
    novo_produto->id = id;
    strcpy(novo_produto->nome, nome);
    novo_produto->preco = preco;
    novo_produto->estoque = estoque;
    novo_produto->proximo = tabela_hash[indice];  // Encadeia no início da lista
    tabela_hash[indice] = novo_produto;
    num_produtos++;

    clock_t fim = clock();  // Marca o tempo de término da operação
    printf("\nTempo de inserção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}

// Busca produto
Produto* buscar_produto(int id) {
    clock_t inicio = clock();  // Marca o tempo de início da operação

    int indice = hash(id);
    Produto* produto = tabela_hash[indice];
    while (produto != NULL) {
        if (produto->id == id) {
            clock_t fim = clock();  // Marca o tempo de término da operação
            printf("\nTempo de busca: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
            return produto;
        }
        produto = produto->proximo;
    }

    clock_t fim = clock();  // Marca o tempo de término da operação
    printf("\nTempo de busca: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    return NULL;
}

// Remove produto
void remover_produto(int id) {
    clock_t inicio = clock();  // Marca o tempo de início da operação

    int indice = hash(id);
    Produto* produto = tabela_hash[indice];
    Produto* anterior = NULL;

    while (produto != NULL) {
        if (produto->id == id) {
            if (anterior == NULL) {
                tabela_hash[indice] = produto->proximo;  // Remove da lista
            } else {
                anterior->proximo = produto->proximo;  // Remove da lista
            }
            free(produto);
            num_produtos--;
            printf("\nProduto '%d' removido com sucesso.\n", id);
            clock_t fim = clock();  // Marca o tempo de término da operação
            printf("\nTempo de remoção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
            return;
        }
        anterior = produto;
        produto = produto->proximo;
    }
    printf("\nProduto não encontrado.\n");

    clock_t fim = clock();  // Marca o tempo de término da operação
    printf("\nTempo de remoção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}

// Exibe o catálogo de produtos
void exibir_catalogo() {
    printf("\n--- Catálogo de Produtos ---\n");
    printf("ID | Nome do Produto             | Preço   | Estoque\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < tamanho_tabela; i++) {
        Produto* produto = tabela_hash[i];
        while (produto != NULL) {
            printf("%2d | %-25s | R$%-6.2f | %d\n", produto->id, produto->nome, produto->preco, produto->estoque);
            produto = produto->proximo;
        }
    }
    printf("\n");
}

int main() {
    // Aloca memória para a tabela hash dinamicamente
    tabela_hash = (Produto*)malloc(sizeof(Produto) * tamanho_tabela);
    
    // Verifica se a alocação foi bem-sucedida
    if (tabela_hash == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    for (int i = 0; i < tamanho_tabela; i++) {
        tabela_hash[i] = NULL;
    }
    
    // Inserção de produtos iniciais
    inserir_produto(1, "Regata Básica", 49.90, 1000);
    inserir_produto(2, "Camiseta Básica", 59.90, 1000);
    inserir_produto(3, "Camiseta Estampada", 69.90, 1000);
    inserir_produto(4, "Camisa Polo", 89.90, 1000);
    inserir_produto(5, "Camisa Social", 119.90, 1000);
    inserir_produto(6, "Bermuda Jeans", 159.90, 1000);
    inserir_produto(7, "Bermuda Sarja", 139.90, 1000);
    inserir_produto(8, "Calça Jeans", 199.90, 1000);
    inserir_produto(9, "Calça Sarja", 179.90, 1000);
    inserir_produto(10, "Calça Chino", 189.90, 1000);
    inserir_produto(11, "Cinto de Couro", 79.90, 1000);
    inserir_produto(12, "Gravata", 59.90, 1000);
    inserir_produto(13, "Tênis Esportivo", 299.90, 1000);
    inserir_produto(14, "Tênis Casual", 249.90, 1000);
    inserir_produto(15, "Sapato Social", 219.90, 1000);

    int opcao, id;
    char continuar;

    while (1) {
        printf("\n--- Gerenciamento de Catálogo ---\n");
        printf("1. Exibir catálogo de produtos\n");
        printf("2. Adicionar produto ao catálogo\n");
        printf("3. Remover produto do catálogo\n");
        printf("4. Buscar produto no catálogo\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibir_catalogo();
        } else if (opcao == 2) {
            printf("\nDigite o ID do produto: ");
            scanf("%d", &id);
            getchar();  // Limpar o buffer
            char nome[50];
            printf("Digite o nome do produto: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0'; // Remove o caractere de nova linha
            float preco;
            printf("Digite o preço do produto: ");
            scanf("%f", &preco);
            int estoque;
            printf("Digite o estoque do produto: ");
            scanf("%d", &estoque);
            inserir_produto(id, nome, preco, estoque);
        } else if (opcao == 3) {
            printf("\nDigite o ID do produto a ser removido: ");
            scanf("%d", &id);
            remover_produto(id);
        } else if (opcao == 4) {
            printf("\nDigite o ID do produto a ser buscado: ");
            scanf("%d", &id);
            Produto* produto = buscar_produto(id);
            if (produto != NULL) {
                printf("\nProduto encontrado:\n");
                printf("ID: %d\n", produto->id);
                printf("Nome: %s\n", produto->nome);
                printf("Preço: R$%.2f\n", produto->preco);
                printf("Estoque: %d\n", produto->estoque);
            } else {
                printf("\nProduto não encontrado.\n");
            }
        } else if (opcao == 5) {
            printf("Saindo...");
            break;
        } else {
            printf("Opção inválida.\n");
        }
    }

    // Libera a memória alocada
    for (int i = 0; i < tamanho_tabela; i++) {
        Produto* produto = tabela_hash[i];
        while (produto != NULL) {
            Produto* temp = produto;
            produto = produto->proximo;
            free(temp);
        }
    }
    free(tabela_hash);
    return 0;
}
