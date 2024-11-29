#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANHO_TABELA_INICIAL 20
#define FATOR_CARGA 0.7

typedef struct {
    int id;
    char nome[50];
    float preco;
    int estoque;
    int ocupado;
} Produto;

Produto* tabela_hash;
int tamanho_tabela = TAMANHO_TABELA_INICIAL;
int num_produtos = 0;

// Função hash
int hash(int chave) {
    return chave % tamanho_tabela;
}

// Redimensiona a tabela hash
void redimensionar_tabela() {
    clock_t inicio = clock(); 
    int novo_tamanho = tamanho_tabela * 2;
    Produto* nova_tabela = (Produto*)malloc(sizeof(Produto) * novo_tamanho);
    
    if (nova_tabela == NULL) {
        printf("Erro ao alocar memória para redimensionamento.\n");
        return;
    }

    // Inicializa a nova tabela
    for (int i = 0; i < novo_tamanho; i++) {
        nova_tabela[i].ocupado = 0;
    }

    // Reinsere os produtos na nova tabela
    for (int i = 0; i < tamanho_tabela; i++) {
        if (tabela_hash[i].ocupado) {
            int indice = hash(tabela_hash[i].id) % novo_tamanho;
            for (int j = 0; j < novo_tamanho; j++) {
                int tentativa = (indice + j) % novo_tamanho;
                if (!nova_tabela[tentativa].ocupado) {
                    nova_tabela[tentativa] = tabela_hash[i];
                    break;
                }
            }
        }
    }

    // Libera a memória da tabela antiga
    free(tabela_hash);
    tabela_hash = nova_tabela;
    tamanho_tabela = novo_tamanho;
    clock_t fim = clock();

    printf("\nTabela redimensionada para %d.\n", tamanho_tabela);
    printf("\nTempo de redimensionamento: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}

// Insere produto na tabela hash
void inserir_produto(int id, const char *nome, float preco, int estoque) {
    clock_t inicio = clock(); 

    // Verifica se a tabela precisa ser redimensionada
    if ((float)num_produtos / tamanho_tabela >= FATOR_CARGA) {
        redimensionar_tabela();
    }

    int indice = hash(id);
    for (int i = 0; i < tamanho_tabela; i++) {
        int tentativa = (indice + i) % tamanho_tabela;
        if (!tabela_hash[tentativa].ocupado) {
            tabela_hash[tentativa].id = id;
            strcpy(tabela_hash[tentativa].nome, nome);
            tabela_hash[tentativa].preco = preco;
            tabela_hash[tentativa].estoque = estoque;
            tabela_hash[tentativa].ocupado = 1;
            num_produtos++;
            clock_t fim = clock();
            printf("\nProduto inserido com sucesso.\n");
            printf("\nTempo de inserção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
            return;
        }
    }
    printf("Erro: tabela hash cheia.\n");
}

// Busca produto
Produto* buscar_produto(int id) {
    clock_t inicio = clock();
    int indice = hash(id);
    for (int i = 0; i < tamanho_tabela; i++) {
        int tentativa = (indice + i) % tamanho_tabela;
        if (tabela_hash[tentativa].ocupado && tabela_hash[tentativa].id == id) {
            clock_t fim = clock();
            printf("\nTempo de busca: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
            return &tabela_hash[tentativa];
        }
        if (!tabela_hash[tentativa].ocupado) break;
    }
    clock_t fim = clock();
    printf("\nTempo de busca: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
    return NULL;
}

// Remove produto
void remover_produto(int id) {
    clock_t inicio = clock(); 
    int indice = hash(id);
    for (int i = 0; i < tamanho_tabela; i++) {
        int tentativa = (indice + i) % tamanho_tabela;
        if (tabela_hash[tentativa].ocupado && tabela_hash[tentativa].id == id) {
            tabela_hash[tentativa].ocupado = 0;
            num_produtos--;
            clock_t fim = clock(); 
            printf("\nProduto '%d' removido com sucesso.\n", id);
            printf("\nTempo de remoção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
            return;
        }
        if (!tabela_hash[tentativa].ocupado) break;
    }
    clock_t fim = clock(); 
    printf("\nProduto não encontrado.\n");
    printf("\nTempo de remoção: %.8f segundos.\n", (double)(fim - inicio) / CLOCKS_PER_SEC);
}

// Exibe o catálogo de produtos
void exibir_catalogo() {
    printf("\n--- Catálogo de Produtos ---\n");
    printf("ID | Nome do Produto             | Preço   | Estoque\n");
    printf("--------------------------------------------------------\n");
    for (int i = 0; i < tamanho_tabela; i++) {
        if (tabela_hash[i].ocupado) {
            printf("%2d | %-25s | R$%-6.2f | %d\n", tabela_hash[i].id, tabela_hash[i].nome, tabela_hash[i].preco, tabela_hash[i].estoque);
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
        tabela_hash[i].ocupado = 0;
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
            fgets(nome, 50, stdin);
            nome[strcspn(nome, "\n")] = '\0';  // Remover o caractere de nova linha
            printf("Digite o preço do produto: ");
            float preco;
            scanf("%f", &preco);
            printf("Digite a quantidade em estoque: ");
            int estoque;
            scanf("%d", &estoque);
            inserir_produto(id, nome, preco, estoque);
        } else if (opcao == 3) {
            printf("\nDigite o ID do produto para remover: ");
            scanf("%d", &id);
            remover_produto(id);
        } else if (opcao == 4) {
            printf("\nDigite o ID do produto para buscar: ");
            scanf("%d", &id);
            Produto* p = buscar_produto(id);
            if (p != NULL) {
                printf("\nProduto encontrado: %d | %s | R$%.2f | Estoque: %d\n", p->id, p->nome, p->preco, p->estoque);
            } else {
                printf("Produto não encontrado.\n");
            }
        } else if (opcao == 5) {
            printf("Saindo...");
            break;
        } else {
            printf("Opção inválida!\n");
        }

        printf("Deseja continuar (s/n)? ");
        scanf(" %c", &continuar);
        if (continuar == 'n' || continuar == 'N') {
            printf("Saindo...");
            break;
        }
    }

    free(tabela_hash);
    return 0;
}
