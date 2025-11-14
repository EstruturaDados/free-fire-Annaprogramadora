
// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.

    
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10
#define MAX_COMPONENTES 20
#define NOME_LEN 30
#define TIPO_LEN 20

typedef struct {
    char nome[NOME_LEN];
    char tipo[TIPO_LEN];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

/*estruturas: COMPONENTES */
typedef struct {
    char nome[NOME_LEN];
    char tipo[TIPO_LEN];
    int prioridade; /* 1 a 10 */
} Componente;

/* Variáveis globais (mochila) */
Item vetor[MAX_ITENS];
int totalVetor = 0;

No* cabeca = NULL;
int totalLista = 0;

/* Variáveis globais (componentes) */
Componente componentes[MAX_COMPONENTES];
int totalComponentes = 0;

/* Contadores e métricas */

/* Buscas/ordenacoes relativas a mochila já implementada */
int seqComparacoesVetor = 0;
int binComparacoesVetor = 0;
int seqComparacoesLista = 0;

/* Métricas para o módulo de componentes */
long compsBubble = 0;
long compsInsertion = 0;
long compsSelection = 0;
long compsBuscaBinariaComponentes = 0;
double tempoBubble = 0.0;
double tempoInsertion = 0.0;
double tempoSelection = 0.0;

/* Utilitários gerais */

/* Remove '\n' se existir */
void trimNewline(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n-1] == '\n') s[n-1] = '\0';
}

/* Le uma linha de forma segura (fgets) e remove newline */
void lerLinha(char *buffer, int tamanho) {
    if (fgets(buffer, tamanho, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    trimNewline(buffer);
}

/* Limpa entrada restante (quando usar scanf misturado) */
void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Funções já existentes: Vetor/Lista (resumidas) */

/* (Mantive as funções essenciais usadas antes, em versão resumida para foco) */

void listarItensVetor() {
    printf("\n--- Mochila (VETOR) — Total: %d item(s) ---\n", totalVetor);
    if (totalVetor == 0) {
        printf("Mochila (vetor) está vazia.\n");
        return;
    }
    for (int i = 0; i < totalVetor; i++) {
        printf(" %d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

/* Busca sequencial no vetor (usada pela mochila) */
int buscarSequencialVetor(const char *nome) {
    seqComparacoesVetor = 0;
    for (int i = 0; i < totalVetor; i++) {
        seqComparacoesVetor++;
        if (strcmp(vetor[i].nome, nome) == 0) return i;
    }
    return -1;
}

/* Inserir item no vetor (versão básica) */
void inserirItemVetor() {
    if (totalVetor >= MAX_ITENS) {
        printf("\n Mochila (vetor) cheia! Capacidade máxima %d atingida.\n", MAX_ITENS);
        return;
    }
    Item novo;
    char buffer[200];

    printf("\n Inserir item (VETOR)\n");
    printf("Nome: ");
    lerLinha(buffer, sizeof(buffer)); strncpy(novo.nome, buffer, NOME_LEN); novo.nome[NOME_LEN-1]='\0';
    printf("Tipo: ");
    lerLinha(buffer, sizeof(buffer)); strncpy(novo.tipo, buffer, TIPO_LEN); novo.tipo[TIPO_LEN-1]='\0';
    printf("Quantidade: ");
    lerLinha(buffer, sizeof(buffer)); novo.quantidade = atoi(buffer);

    int idx = buscarSequencialVetor(novo.nome);
    if (idx != -1) {
        vetor[idx].quantidade += novo.quantidade;
        printf(" Item existente. Quantidade atualizada (agora %d).\n", vetor[idx].quantidade);
    } else {
        vetor[totalVetor++] = novo;
        printf(" Item inserido com sucesso.\n");
    }
    listarItensVetor();
}

/* Remover item no vetor */
void removerItemVetor() {
    if (totalVetor == 0) { printf("\n Mochila vazia.\n"); return; }
    char buffer[200], nome[NOME_LEN];
    printf("Nome do item a remover: "); lerLinha(buffer,sizeof(buffer)); strncpy(nome,buffer,NOME_LEN); nome[NOME_LEN-1]='\0';
    int idx = buscarSequencialVetor(nome);
    if (idx == -1) { printf("Item não encontrado.\n"); }
    else {
        for (int i=idx;i<totalVetor-1;i++) vetor[i]=vetor[i+1];
        totalVetor--;
        printf("Item removido.\n");
    }
    listarItensVetor();
}

/* Lista encadeada (mantida mínima) */
void listarItensLista() {
    printf("\n--- Mochila (LISTA) — Total: %d item(s) ---\n", totalLista);
    if (cabeca == NULL) { printf("Mochila (lista) está vazia.\n"); return; }
    No* atual = cabeca; int i=1;
    while (atual!=NULL){
        printf(" %d) Nome: %s | Tipo: %s | Quantidade: %d\n",
            i, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo; i++;
    }
}

/* Buscar na lista (sequencial) */
No* buscarSequencialLista(const char *nome) {
    seqComparacoesLista = 0;
    No* atual = cabeca;
    while (atual != NULL) {
        seqComparacoesLista++;
        if (strcmp(atual->dados.nome, nome) == 0) return atual;
        atual = atual->proximo;
    }
    return NULL;
}

/* Inserir na lista (insere no início) */
void inserirItemLista() {
    char buffer[200];
    Item novo;
    printf("\n Inserir item (LISTA)\n");
    printf("Nome: "); lerLinha(buffer,sizeof(buffer)); strncpy(novo.nome,buffer,NOME_LEN); novo.nome[NOME_LEN-1]='\0';
    printf("Tipo: "); lerLinha(buffer,sizeof(buffer)); strncpy(novo.tipo,buffer,TIPO_LEN); novo.tipo[TIPO_LEN-1]='\0';
    printf("Quantidade: "); lerLinha(buffer,sizeof(buffer)); novo.quantidade = atoi(buffer);
    No* encontrado = buscarSequencialLista(novo.nome);
    if (encontrado != NULL) {
        encontrado->dados.quantidade += novo.quantidade;
        printf("Item existente: quantidade atualizada.\n");
    } else {
        No* n = (No*) malloc(sizeof(No));
        n->dados = novo; n->proximo = cabeca; cabeca = n; totalLista++;
        printf("Item inserido na lista.\n");
    }
    listarItensLista();
}

/* Remover item da lista */
void removerItemLista() {
    if (cabeca == NULL) { printf("\nMochila (lista) vazia.\n"); return; }
    char buffer[200], nome[NOME_LEN];
    printf("Nome a remover: "); lerLinha(buffer,sizeof(buffer)); strncpy(nome,buffer,NOME_LEN); nome[NOME_LEN-1]='\0';
    No* atual = cabeca; No* anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) cabeca = atual->proximo;
            else anterior->proximo = atual->proximo;
            free(atual); totalLista--; printf("Item removido da lista.\n");
            listarItensLista(); return;
        }
        anterior = atual; atual = atual->proximo;
    }
    printf("Item não encontrado na lista.\n");
    listarItensLista();
}

/*Módulo: Componentes (nova parte) */

/* Mostra componentes em vetor */
void mostrarComponentes(Componente arr[], int n) {
    printf("\n--- Componentes (%d) ---\n", n);
    if (n == 0) { printf("Nenhum componente cadastrado.\n"); return; }
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-25s | Tipo: %-12s | Prioridade: %d\n",
               i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

/* Bubble Sort por nome (string). conta comparacoes em *comparacoes */
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-1-i; j++) {
            (*comparacoes)++;
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

/* Insertion Sort por tipo (string). conta comparacoes */
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente chave = arr[i];
        int j = i - 1;
        /* comparar tipos enquanto for maior */
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else break;
        }
        arr[j+1] = chave;
    }
}

/* Selection Sort por prioridade (int). conta comparacoes */
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n-1; i++) {
        int menor = i;
        for (int j = i+1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[menor].prioridade) menor = j;
        }
        if (menor != i) {
            Componente tmp = arr[i];
            arr[i] = arr[menor];
            arr[menor] = tmp;
        }
    }
}

/* Busca binária por nome (apenas após ordenar por nome) */
/* Retorna índice ou -1. Conta comparações em *comparacoes */
int buscaBinariaPorNome(Componente arr[], int n, char chave[], long *comparacoes) {
    *comparacoes = 0;
    int low = 0, high = n-1;
    while (low <= high) {
        int mid = (low + high) / 2;
        (*comparacoes)++;
        int cmp = strcmp(chave, arr[mid].nome);
        if (cmp == 0) return mid;
        else if (cmp < 0) high = mid - 1;
        else low = mid + 1;
    }
    return -1;
}

/* Wrapper que mede tempo (clock) e invoca algoritmo de ordenação.
   Recebe ponteiro para função de ordenação com assinatura (arr,n,long*).
   Retorna tempo consumido em segundos e coloca comparacoes em *comparacoes.
*/
typedef void (*SortAlg)(Componente[], int, long*);

double medirTempo(SortAlg alg, Componente arr[], int n, long *comparacoes) {
    clock_t inicio = clock();
    alg(arr, n, comparacoes);
    clock_t fim = clock();
    return ((double)(fim - inicio)) / CLOCKS_PER_SEC;
}

/* Cadastro dos componentes (até MAX_COMPONENTES) */
void cadastrarComponentes() {
    char buffer[200];
    printf("\n Cadastro de componentes (max %d)\n", MAX_COMPONENTES);
    printf("Quantos componentes deseja cadastrar? ");
    lerLinha(buffer, sizeof(buffer));
    int q = atoi(buffer);
    if (q <= 0) { printf("Operação cancelada.\n"); return; }
    if (q + totalComponentes > MAX_COMPONENTES) {
        printf("Limite ultrapassado. Só é possível cadastrar %d componente(s) adicionais.\n", MAX_COMPONENTES - totalComponentes);
        q = MAX_COMPONENTES - totalComponentes;
    }
    for (int i = 0; i < q; i++) {
        Componente c;
        printf("\nComponente %d:\n", totalComponentes + 1);
        printf("Nome: "); lerLinha(buffer, sizeof(buffer)); strncpy(c.nome, buffer, NOME_LEN); c.nome[NOME_LEN-1]='\0';
        printf("Tipo (ex: controle, suporte, propulsao): "); lerLinha(buffer, sizeof(buffer)); strncpy(c.tipo, buffer, TIPO_LEN); c.tipo[TIPO_LEN-1]='\0';
        printf("Prioridade (1-10): "); lerLinha(buffer, sizeof(buffer));
        c.prioridade = atoi(buffer);
        if (c.prioridade < 1) c.prioridade = 1;
        if (c.prioridade > 10) c.prioridade = 10;
        componentes[totalComponentes++] = c;
    }
    printf(" Cadastro concluído. Total agora: %d componente(s).\n", totalComponentes);
    mostrarComponentes(componentes, totalComponentes);
}

/* Menu do módulo de componentes */
void menuComponentes() {
    int opc;
    char buffer[200];
    Componente copia[MAX_COMPONENTES];
    long comps = 0;
    double tempo = 0.0;
    int ordenadoPorNomeFlag = 0; /* marca se a cópia atual está ordenada por nome */

    do {
        printf("\n=== MÓDULO: Componentes da Torre de Resgate ===\n");
        printf("1 - Cadastrar componentes\n");
        printf("2 - Mostrar componentes atuais\n");
        printf("3 - Ordenar por NOME (Bubble Sort)\n");
        printf("4 - Ordenar por TIPO (Insertion Sort)\n");
        printf("5 - Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("6 - Buscar componente-chave por NOME (Busca Binária — exige ordenação por nome)\n");
        printf("7 - Mostrar métricas (comparações e tempos das últimas ordenações)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        lerLinha(buffer, sizeof(buffer));
        opc = atoi(buffer);

        switch (opc) {
            case 1:
                cadastrarComponentes();
                ordenadoPorNomeFlag = 0;
                break;
            case 2:
                mostrarComponentes(componentes, totalComponentes);
                break;
            case 3:
                /* Fazer cópia para ordenar e medir (preserva original) */
                memcpy(copia, componentes, sizeof(Componente) * totalComponentes);
                tempo = medirTempo(bubbleSortNome, copia, totalComponentes, &comps);
                compsBubble = comps; tempoBubble = tempo;
                printf("\n Bubble Sort (por nome) executado: Comparações=%ld | Tempo=%.6f s\n", compsBubble, tempoBubble);
                mostrarComponentes(copia, totalComponentes);
                /* Atualiza a "versão atual" para a ordenação por nome — opcional: sobrescreve original */
                memcpy(componentes, copia, sizeof(Componente) * totalComponentes);
                ordenadoPorNomeFlag = 1;
                break;
            case 4:
                memcpy(copia, componentes, sizeof(Componente) * totalComponentes);
                tempo = medirTempo(insertionSortTipo, copia, totalComponentes, &comps);
                compsInsertion = comps; tempoInsertion = tempo;
                printf("\n Insertion Sort (por tipo) executado: Comparações=%ld | Tempo=%.6f s\n", compsInsertion, tempoInsertion);
                mostrarComponentes(copia, totalComponentes);
                /* opção: aplicar rearranjo na estrutura principal */
                memcpy(componentes, copia, sizeof(Componente) * totalComponentes);
                ordenadoPorNomeFlag = 0; /* não necessariamente ordenado por nome */
                break;
            case 5:
                memcpy(copia, componentes, sizeof(Componente) * totalComponentes);
                tempo = medirTempo(selectionSortPrioridade, copia, totalComponentes, &comps);
                compsSelection = comps; tempoSelection = tempo;
                printf("\n Selection Sort (por prioridade) executado: Comparações=%ld | Tempo=%.6f s\n", compsSelection, tempoSelection);
                mostrarComponentes(copia, totalComponentes);
                memcpy(componentes, copia, sizeof(Componente) * totalComponentes);
                ordenadoPorNomeFlag = 0;
                break;
            case 6:
                if (totalComponentes == 0) { printf("Nenhum componente cadastrado.\n"); break; }
                if (!ordenadoPorNomeFlag) {
                    printf("Atenção: para usar busca binária é necessário ordenar por nome primeiro.\n");
                    printf("Deseja ordenar por nome agora (Bubble Sort)? (s/n): ");
                    lerLinha(buffer, sizeof(buffer));
                    if (buffer[0]=='s' || buffer[0]=='S') {
                        memcpy(copia, componentes, sizeof(Componente) * totalComponentes);
                        tempo = medirTempo(bubbleSortNome, copia, totalComponentes, &comps);
                        compsBubble = comps; tempoBubble = tempo;
                        memcpy(componentes, copia, sizeof(Componente) * totalComponentes);
                        ordenadoPorNomeFlag = 1;
                        printf("Vetor ordenado por nome com Bubble Sort. Comparações=%ld Tempo=%.6f s\n", compsBubble, tempoBubble);
                    } else {
                        printf("Busca binária cancelada.\n");
                        break;
                    }
                }
                /* buscar componente-chave */
                printf("Informe o nome do componente-chave a buscar: ");
                lerLinha(buffer, sizeof(buffer));
                long compBusca = 0;
                int idx = buscaBinariaPorNome(componentes, totalComponentes, buffer, &compBusca);
                compsBuscaBinariaComponentes = compBusca;
                if (idx == -1) {
                    printf(" Componente não encontrado. Comparações na busca binária: %ld\n", compBusca);
                } else {
                    printf(" Componente encontrado no índice %d: Nome=%s | Tipo=%s | Prioridade=%d\n",
                           idx, componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                    printf("Comparações na busca binária: %ld\n", compBusca);
                    printf("\n Componente-chave presente — você pode iniciar a montagem da torre!\n");
                }
                break;
            case 7:
                printf("\n--- Métricas das últimas ordenações ---\n");
                printf("Bubble (por nome): Comparações=%ld | Tempo=%.6f s\n", compsBubble, tempoBubble);
                printf("Insertion (por tipo): Comparações=%ld | Tempo=%.6f s\n", compsInsertion, tempoInsertion);
                printf("Selection (prioridade): Comparações=%ld | Tempo=%.6f s\n", compsSelection, tempoSelection);
                printf("Última busca binária (por nome): Comparações=%ld\n", compsBuscaBinariaComponentes);
                break;
            case 0:
                printf("Saindo do módulo de componentes.\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opc != 0);
}

/*Menus principais (integração) */

void menuVetor(); /* protótipos para manter integração (podem chamar as funções já definidas) */
void menuLista();

void menuPrincipal() {
    int opc;
    char buffer[200];

    do {
        printf("\n=============================================\n");
        printf("  Modo Sobrevivência — Comparação: Vetor x Lista\n");
        printf("  + Módulo Avançado: Montagem da Torre de Resgate\n");
        printf("=============================================\n");
        printf("1 - Usar mochila (VETOR)\n");
        printf("2 - Usar mochila (LISTA ENCADEADA)\n");
        printf("3 - Módulo: Componentes da Torre de Resgate\n");
        printf("4 - Mostrar contadores de comparacoes (últimas buscas)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        lerLinha(buffer, sizeof(buffer));
        opc = atoi(buffer);

        switch (opc) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuLista();
                break;
            case 3:
                menuComponentes();
                break;
            case 4:
                printf("\n--- Contadores (últimas buscas realizadas na mochila) ---\n");
                printf("Busca sequencial (VETOR): %d comparações (última execução)\n", seqComparacoesVetor);
                printf("Busca binária (VETOR): %d comparações (última execução)\n", binComparacoesVetor);
                printf("Busca sequencial (LISTA): %d comparações (última execução)\n", seqComparacoesLista);
                break;
            case 0:
                printf("Encerrando aplicação...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opc != 0);
}

/* Implementações simplificadas dos menus específicos (reaproveitando funções já definidas no fluxo anterior) */

void menuVetor() {
    int opc;
    char buffer[200];
    do {
        printf("\n=== MENU: Mochila (VETOR) ===\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item por nome\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        lerLinha(buffer, sizeof(buffer));
        opc = atoi(buffer);
        switch (opc) {
            case 1: inserirItemVetor(); break;
            case 2: removerItemVetor(); break;
            case 3: listarItensVetor(); break;
            case 4: {
                if (totalVetor==0) { printf("Mochila vazia.\n"); break; }
                char nome[NOME_LEN];
                printf("Nome a buscar: "); lerLinha(buffer,sizeof(buffer)); strncpy(nome,buffer,NOME_LEN); nome[NOME_LEN-1]='\0';
                int idx = buscarSequencialVetor(nome);
                if (idx==-1) printf("Não encontrado. Comparações=%d\n", seqComparacoesVetor);
                else printf("Encontrado índice %d. Comparações=%d. Nome:%s Tipo:%s Qt:%d\n",
                            idx, seqComparacoesVetor, vetor[idx].nome, vetor[idx].tipo, vetor[idx].quantidade);
            } break;
            case 0: printf("Voltando...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);
}

void menuLista() {
    int opc;
    char buffer[200];
    do {
        printf("\n=== MENU: Mochila (LISTA ENCADEADA) ===\n");
        printf("1 - Inserir item\n");
        printf("2 - Remover item por nome\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar (sequencial)\n");
        printf("0 - Voltar\n");
        printf("Escolha: ");
        lerLinha(buffer, sizeof(buffer));
        opc = atoi(buffer);
        switch (opc) {
            case 1: inserirItemLista(); break;
            case 2: removerItemLista(); break;
            case 3: listarItensLista(); break;
            case 4: {
                if (cabeca==NULL) { printf("Lista vazia.\n"); break; }
                char nome[NOME_LEN];
                printf("Nome a buscar: "); lerLinha(buffer,sizeof(buffer)); strncpy(nome,buffer,NOME_LEN); nome[NOME_LEN-1]='\0';
                No* res = buscarSequencialLista(nome);
                if (res==NULL) printf("Não encontrado. Comparações=%d\n", seqComparacoesLista);
                else printf("Encontrado. Comparações=%d. Nome:%s Tipo:%s Qt:%d\n",
                            seqComparacoesLista, res->dados.nome, res->dados.tipo, res->dados.quantidade);
            } break;
            case 0: printf("Voltando...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);
}

/* Libera memória da lista encadeada */
void liberarLista() {
    No* atual = cabeca;
    while (atual != NULL) {
        No* prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    cabeca = NULL; totalLista = 0;
}


int main() {
    printf("=============================================\n");
    printf("  Modo Sobrevivência — Vetor x Lista + Torre\n");
    printf("  (Módulo de comparação de algoritmos e montagem)\n");
    printf("=============================================\n");
    menuPrincipal();
    liberarLista();
    return 0;
}



// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
