#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA_INICIAL 10
#define FATOR_CARGA 0.7

typedef struct {
    int id;
    char nome[50];
    float preco;
    int estoque;
    int ocupado;
} Produto;

typedef struct {
    int id;
    int quantidade;
} ItemCarrinho;

Produto* tabela_hash;
ItemCarrinho* carrinho;
int tamanho_tabela = TAMANHO_TABELA_INICIAL;
int tamanho_carrinho = 0;
int num_produtos = 0;

// Função hash
int hash(int chave) {
    return chave % tamanho_tabela;
}

// Redimensiona a tabela hash
void redimensionar_tabela() {
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
    printf("Tabela redimensionada para %d.\n", tamanho_tabela);
}

// Insere produto na tabela hash
void inserir_produto(int id, const char *nome, float preco, int estoque) {
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
            return;
        }
    }
    printf("Erro: tabela hash cheia.\n");
}

// Busca produto
Produto* buscar_produto(int id) {
    int indice = hash(id);
    for (int i = 0; i < tamanho_tabela; i++) {
        int tentativa = (indice + i) % tamanho_tabela;
        if (tabela_hash[tentativa].ocupado && tabela_hash[tentativa].id == id) {
            return &tabela_hash[tentativa];
        }
        if (!tabela_hash[tentativa].ocupado) break;
    }
    return NULL;
}

// Adiciona produto ao carrinho
void adicionar_ao_carrinho(int id, int quantidade) {
    Produto *produto = buscar_produto(id);
    if (produto) {
        if (produto->estoque >= quantidade) {
            carrinho[tamanho_carrinho].id = id;
            carrinho[tamanho_carrinho].quantidade = quantidade;
            tamanho_carrinho++;
            produto->estoque -= quantidade;
            printf("Produto '%s' adicionado ao carrinho.\n", produto->nome);
        } else {
            printf("Estoque insuficiente para '%s'.\n", produto->nome);
        }
    } else {
        printf("Produto não encontrado.\n");
    }
}

// Remove itens do carrinho
void remover_do_carrinho(int id, int quantidade) {
    for (int i = 0; i < tamanho_carrinho; i++) {
        if (carrinho[i].id == id) {
            if (quantidade >= carrinho[i].quantidade) {
                Produto *produto = buscar_produto(id);
                if (produto) {
                    produto->estoque += carrinho[i].quantidade;
                }
                for (int j = i; j < tamanho_carrinho - 1; j++) {
                    carrinho[j] = carrinho[j + 1];
                }
                tamanho_carrinho--;
                printf("Item removido completamente do carrinho.\n");
            } else {
                Produto *produto = buscar_produto(id);
                if (produto) {
                    produto->estoque += quantidade;
                }
                carrinho[i].quantidade -= quantidade;
                printf("Quantidade ajustada. Restam %d unidades no carrinho.\n", carrinho[i].quantidade);
            }
            return;
        }
    }
    printf("Produto não encontrado no carrinho.\n");
}

// Exibe os itens do carrinho
void exibir_carrinho() {
    printf("\nCarrinho de Compras:\n");
    float total = 0;
    for (int i = 0; i < tamanho_carrinho; i++) {
        Produto *produto = buscar_produto(carrinho[i].id);
        if (produto) {
            printf("%s x%d - R$%.2f\n", produto->nome, carrinho[i].quantidade, produto->preco * carrinho[i].quantidade);
            total += produto->preco * carrinho[i].quantidade;
        }
    }
    printf("Total: R$%.2f\n\n", total);
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
    // Aloca memória para a tabela hash e carrinho dinamicamente
    tabela_hash = (Produto*)malloc(sizeof(Produto) * tamanho_tabela);
    carrinho = (ItemCarrinho*)malloc(sizeof(ItemCarrinho) * tamanho_tabela);
    
    // Verifica se a alocação foi bem-sucedida
    if (tabela_hash == NULL || carrinho == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    for (int i = 0; i < tamanho_tabela; i++) {
        tabela_hash[i].ocupado = 0;
    }

    // Inserção de produtos iniciais
    inserir_produto(1, "Camiseta Básica", 39.90, 100);
    inserir_produto(2, "Jeans Feminino", 89.90, 100);
    inserir_produto(3, "Tênis Casual", 179.90, 100);
    inserir_produto(4, "Blusa de Frio", 129.90, 100);
    inserir_produto(5, "Short Jeans", 59.90, 100);
    
    int opcao, id, quantidade;
    char continuar;

    while (1) {
        printf("\n--- Carrinho de Compras ---\n");
        printf("1. Exibir catálogo de produtos\n");
        printf("2. Adicionar item ao carrinho\n");
        printf("3. Remover item do carrinho\n");
        printf("4. Exibir carrinho\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibir_catalogo();
        } else if (opcao == 2) {
            do {
                printf("\nDigite o ID do produto que deseja adicionar ao carrinho: ");
                scanf("%d", &id);
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);
                adicionar_ao_carrinho(id, quantidade);
                printf("Deseja adicionar outro produto? (S/N): ");
                getchar();
                scanf("%c", &continuar);
            } while (continuar == 'S' || continuar == 's');
        } else if (opcao == 3) {
            printf("\nDigite o ID do produto que deseja remover do carrinho: ");
            scanf("%d", &id);
            printf("Digite a quantidade: ");
            scanf("%d", &quantidade);
            remover_do_carrinho(id, quantidade);
        } else if (opcao == 4) {
            exibir_carrinho();
        } else if (opcao == 5) {
            printf("Saindo...");
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }

    free(tabela_hash);
    free(carrinho);

    return 0;
}
