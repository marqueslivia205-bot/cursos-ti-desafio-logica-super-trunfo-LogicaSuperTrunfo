/******************************************************************************
 * Super Trunfo - Nível Mestre
 * 
 * Este é um jogo de cartas baseado em comparação de atributos de cidades.
 * O jogo inclui um sistema completo com menu interativo, cadastro de cidades,
 * sistema de pontuação, salvamento de progresso e estatísticas.
 * 
 * Desenvolvido como parte do desafio de lógica de programação.
 ******************************************************************************/

// Bibliotecas necessárias
#include <stdio.h>    // Para entrada/saída padrão
#include <stdlib.h>   // Para funções de alocação de memória e system()
#include <string.h>   // Para manipulação de strings
#include <time.h>     // Para geração de números aleatórios
#include <unistd.h>   // Para função sleep() e usleep()

// Constantes do jogo
#define MAX_CIDADES 10      // Número máximo de cidades no jogo
#define MAX_NOME 50         // Tamanho máximo para nomes
#define CLEAR_SCREEN "clear"  // Comando para limpar a tela (use "cls" no Windows)

/**
 * Estrutura que representa uma cidade no jogo
 * Contém todos os atributos que podem ser comparados durante as partidas
 */
typedef struct {
    char codigo[10];               // Código único da cidade
    char nome[30];                // Nome da cidade
    char estado[3];               // Sigla do estado (ex: SP, RJ, MG)
    unsigned long int populacao;  // População total
    float area;                   // Área em km²
    float pib;                    // Produto Interno Bruto
    int pontos_turisticos;        // Quantidade de pontos turísticos
    float densidade;              // Densidade populacional (hab/km²)
    float pib_per_capita;         // PIB dividido pela população
    int ranking_qualidade_vida;   // Ranking de qualidade de vida (menor = melhor)
    int nivel_tecnologia;         // Nível de desenvolvimento tecnológico (1-100)
    int sustentabilidade;         // Índice de sustentabilidade (1-100)
} Cidade;

/**
 * Estrutura que representa um jogador
 * Mantém o controle das cartas, pontuação e estatísticas do jogador
 */
typedef struct {
    char nome[50];        // Nome do jogador
    int pontos;          // Pontuação atual
    int vitorias;        // Número de rodadas vencidas
    Cidade *cartas;      // Array dinâmico de cartas do jogador
    int num_cartas;      // Quantidade de cartas que o jogador possui
} Jogador;

// === Protótipos das funções do jogo ===

// Funções de interface do usuário
void limparTela();                // Limpa a tela do console
void mostrarMenuPrincipal();      // Exibe o menu principal do jogo
void animacaoCarregamento();      // Mostra uma animação de carregamento
void exibirCarta(Cidade cidade);  // Exibe uma carta formatada na tela

// Funções de gerenciamento do jogo
void inicializarJogo(Jogador *j1, Jogador *j2);  // Prepara um novo jogo
void cadastrarCidades(Cidade *cidades, int *num_cidades);  // Adiciona novas cidades
void mostrarRegras();            // Exibe as regras do jogo
void jogarPartida(Jogador *j1, Jogador *j2);  // Executa uma partida completa

// Funções de comparação e lógica
int compararAtributos(Cidade c1, Cidade c2, int atributo);  // Compara atributos das cartas

// Funções de persistência e estatísticas
void salvarJogo(Jogador j1, Jogador j2);        // Salva o estado atual do jogo
void carregarJogo(Jogador *j1, Jogador *j2);    // Carrega um jogo salvo
void mostrarRanking();                          // Exibe o ranking dos jogadores
void mostrarEstatisticas(Jogador j1, Jogador j2);  // Mostra estatísticas da partida

// Função para limpar a tela
void limparTela() {
    system(CLEAR_SCREEN);
}

// Função para criar animação de carregamento
void animacaoCarregamento() {
    printf("Carregando");
    for(int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(500000);
    }
    printf("\n");
}

// Função para exibir o menu principal
void mostrarMenuPrincipal() {
    limparTela();
    printf("\n=== SUPER TRUNFO CIDADES - NÍVEL MESTRE ===\n\n");
    printf("1. Novo Jogo\n");
    printf("2. Carregar Jogo\n");
    printf("3. Ver Regras\n");
    printf("4. Ver Ranking\n");
    printf("5. Cadastrar Novas Cidades\n");
    printf("6. Estatísticas\n");
    printf("0. Sair\n\n");
    printf("Escolha uma opção: ");
}

// Função para exibir uma carta
void exibirCarta(Cidade cidade) {
    printf("\n┌──────────────────────────────┐\n");
    printf("│ %-28s│\n", cidade.nome);
    printf("├──────────────────────────────┤\n");
    printf("│ Código: %-20s │\n", cidade.codigo);
    printf("│ Estado: %-20s │\n", cidade.estado);
    printf("│ População: %-17lu │\n", cidade.populacao);
    printf("│ Área: %-21.2f │\n", cidade.area);
    printf("│ PIB: R$ %-18.2f │\n", cidade.pib);
    printf("│ Pontos Turísticos: %-9d │\n", cidade.pontos_turisticos);
    printf("│ Densidade: %-17.2f │\n", cidade.densidade);
    printf("│ PIB per capita: R$ %-10.2f │\n", cidade.pib_per_capita);
    printf("│ Ranking Qualidade: %-9d │\n", cidade.ranking_qualidade_vida);
    printf("│ Nível Tecnologia: %-10d │\n", cidade.nivel_tecnologia);
    printf("│ Sustentabilidade: %-10d │\n", cidade.sustentabilidade);
    printf("└──────────────────────────────┘\n");
}

// Função para comparar atributos
int compararAtributos(Cidade c1, Cidade c2, int atributo) {
    switch(atributo) {
        case 1: return c1.populacao > c2.populacao ? 1 : -1;
        case 2: return c1.area > c2.area ? 1 : -1;
        case 3: return c1.pib > c2.pib ? 1 : -1;
        case 4: return c1.pontos_turisticos > c2.pontos_turisticos ? 1 : -1;
        case 5: return c1.densidade > c2.densidade ? 1 : -1;
        case 6: return c1.pib_per_capita > c2.pib_per_capita ? 1 : -1;
        case 7: return c1.ranking_qualidade_vida < c2.ranking_qualidade_vida ? 1 : -1;
        case 8: return c1.nivel_tecnologia > c2.nivel_tecnologia ? 1 : -1;
        case 9: return c1.sustentabilidade > c2.sustentabilidade ? 1 : -1;
        default: return 0;
    }
}

// Função para mostrar as regras
void mostrarRegras() {
    printf("\n=== REGRAS DO SUPER TRUNFO CIDADES ===\n\n");
    printf("1. Cada jogador recebe um conjunto de cartas no início do jogo\n");
    printf("2. Em cada rodada, um jogador escolhe um atributo de sua carta\n");
    printf("3. O jogador com o maior valor no atributo escolhido vence a rodada\n");
    printf("4. Atributos especiais:\n");
    printf("   - Ranking Qualidade de Vida: Menor valor vence\n");
    printf("   - Nível Tecnologia e Sustentabilidade: Maior valor vence\n");
    printf("5. Pontuação:\n");
    printf("   - Vitória: 3 pontos\n");
    printf("   - Empate: 1 ponto\n");
    printf("   - Derrota: 0 pontos\n\n");
    printf("Pressione Enter para voltar ao menu...");
    getchar();
    getchar();
}

// Função para inicializar o jogo
void inicializarJogo(Jogador *j1, Jogador *j2) {
    printf("\nDigite o nome do Jogador 1: ");
    scanf("%s", j1->nome);
    printf("Digite o nome do Jogador 2: ");
    scanf("%s", j2->nome);
    
    j1->pontos = 0;
    j1->vitorias = 0;
    j2->pontos = 0;
    j2->vitorias = 0;
    
    j1->num_cartas = MAX_CIDADES/2;
    j2->num_cartas = MAX_CIDADES/2;
    
    j1->cartas = (Cidade*)malloc(j1->num_cartas * sizeof(Cidade));
    j2->cartas = (Cidade*)malloc(j2->num_cartas * sizeof(Cidade));
}

// Função para cadastrar cidades
void cadastrarCidades(Cidade *cidades, int *num_cidades) {
    if (*num_cidades >= MAX_CIDADES) {
        printf("Número máximo de cidades atingido!\n");
        sleep(2);
        return;
    }
    
    Cidade nova_cidade;
    printf("\nCadastro de Nova Cidade\n");
    printf("Código: ");
    scanf("%s", nova_cidade.codigo);
    printf("Nome: ");
    scanf("%s", nova_cidade.nome);
    printf("Estado (sigla): ");
    scanf("%s", nova_cidade.estado);
    printf("População: ");
    scanf("%lu", &nova_cidade.populacao);
    printf("Área (km²): ");
    scanf("%f", &nova_cidade.area);
    printf("PIB (R$): ");
    scanf("%f", &nova_cidade.pib);
    printf("Número de Pontos Turísticos: ");
    scanf("%d", &nova_cidade.pontos_turisticos);
    
    nova_cidade.densidade = nova_cidade.populacao / nova_cidade.area;
    nova_cidade.pib_per_capita = nova_cidade.pib / nova_cidade.populacao;
    
    printf("Ranking Qualidade de Vida (1-100): ");
    scanf("%d", &nova_cidade.ranking_qualidade_vida);
    printf("Nível de Tecnologia (1-100): ");
    scanf("%d", &nova_cidade.nivel_tecnologia);
    printf("Índice de Sustentabilidade (1-100): ");
    scanf("%d", &nova_cidade.sustentabilidade);
    
    cidades[*num_cidades] = nova_cidade;
    (*num_cidades)++;
    
    printf("\nCidade cadastrada com sucesso!\n");
    sleep(2);
}

/**
 * Função que executa uma partida completa entre dois jogadores
 * 
 * Fluxo da partida:
 * 1. Inicia um loop de 5 rodadas
 * 2. Em cada rodada:
 *    - Mostra o placar atual
 *    - Exibe a carta do jogador da vez
 *    - Permite escolher um atributo para comparar
 *    - Mostra a carta do adversário
 *    - Compara os atributos e define o vencedor
 *    - Atualiza os pontos e estatísticas
 * 3. Alterna entre os jogadores a cada rodada
 * 
 * @param j1 Ponteiro para o Jogador 1
 * @param j2 Ponteiro para o Jogador 2
 */
void jogarPartida(Jogador *j1, Jogador *j2) {
    int rodada = 1;              // Contador de rodadas
    int jogador_atual = 1;       // Controle do jogador da vez (1 ou 2)
    int escolha;                 // Atributo escolhido para comparação
    
    while (rodada <= 5) { // Limite de 5 rodadas por partida
        limparTela();
        printf("\n=== RODADA %d ===\n", rodada);
        printf("\nPlacar Atual:\n");
        printf("%s: %d pontos (%d vitórias)\n", j1->nome, j1->pontos, j1->vitorias);
        printf("%s: %d pontos (%d vitórias)\n", j2->nome, j2->pontos, j2->vitorias);
        
        printf("\nVez de %s\n", jogador_atual == 1 ? j1->nome : j2->nome);
        exibirCarta(jogador_atual == 1 ? j1->cartas[0] : j2->cartas[0]);
        
        printf("\nEscolha um atributo:\n");
        printf("1. População\n");
        printf("2. Área\n");
        printf("3. PIB\n");
        printf("4. Pontos Turísticos\n");
        printf("5. Densidade\n");
        printf("6. PIB per capita\n");
        printf("7. Ranking Qualidade de Vida\n");
        printf("8. Nível de Tecnologia\n");
        printf("9. Sustentabilidade\n");
        printf("Sua escolha: ");
        scanf("%d", &escolha);
        
        printf("\nCarta do adversário:\n");
        exibirCarta(jogador_atual == 1 ? j2->cartas[0] : j1->cartas[0]);
        
        int resultado = compararAtributos(j1->cartas[0], j2->cartas[0], escolha);
        
        if (resultado > 0) {
            printf("\n%s venceu a rodada!\n", j1->nome);
            j1->pontos += 3;
            j1->vitorias++;
        } else if (resultado < 0) {
            printf("\n%s venceu a rodada!\n", j2->nome);
            j2->pontos += 3;
            j2->vitorias++;
        } else {
            printf("\nEmpate! Cada jogador recebe 1 ponto.\n");
            j1->pontos++;
            j2->pontos++;
        }
        
        rodada++;
        jogador_atual = jogador_atual == 1 ? 2 : 1;
        
        printf("\nPressione Enter para continuar...");
        getchar();
        getchar();
    }
}

// Função para mostrar estatísticas
void mostrarEstatisticas(Jogador j1, Jogador j2) {
    printf("\n=== ESTATÍSTICAS DA PARTIDA ===\n\n");
    printf("Jogador: %s\n", j1.nome);
    printf("Pontos totais: %d\n", j1.pontos);
    printf("Vitórias: %d\n", j1.vitorias);
    printf("Taxa de vitória: %.2f%%\n\n", j1.vitorias * 100.0 / (j1.vitorias + j2.vitorias));
    
    printf("Jogador: %s\n", j2.nome);
    printf("Pontos totais: %d\n", j2.pontos);
    printf("Vitórias: %d\n", j2.vitorias);
    printf("Taxa de vitória: %.2f%%\n\n", j2.vitorias * 100.0 / (j1.vitorias + j2.vitorias));
    
    printf("Pressione Enter para voltar ao menu...");
    getchar();
    getchar();
}

// Função para salvar o jogo
void salvarJogo(Jogador j1, Jogador j2) {
    FILE *arquivo = fopen("salvamento.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar o jogo!\n");
        return;
    }
    
    fwrite(&j1, sizeof(Jogador), 1, arquivo);
    fwrite(&j2, sizeof(Jogador), 1, arquivo);
    fclose(arquivo);
    
    printf("Jogo salvo com sucesso!\n");
    sleep(2);
}

// Função para carregar o jogo
void carregarJogo(Jogador *j1, Jogador *j2) {
    FILE *arquivo = fopen("salvamento.bin", "rb");
    if (arquivo == NULL) {
        printf("Nenhum jogo salvo encontrado!\n");
        sleep(2);
        return;
    }
    
    fread(j1, sizeof(Jogador), 1, arquivo);
    fread(j2, sizeof(Jogador), 1, arquivo);
    fclose(arquivo);
    
    printf("Jogo carregado com sucesso!\n");
    sleep(2);
}

// Função para mostrar ranking
void mostrarRanking() {
    printf("\n=== RANKING DE JOGADORES ===\n\n");
    printf("Em desenvolvimento...\n\n");
    printf("Pressione Enter para voltar ao menu...");
    getchar();
    getchar();
}

/**
 * Função principal do jogo
 * Implementa o loop principal do menu e gerencia o fluxo do jogo
 * 
 * O jogo segue o seguinte fluxo:
 * 1. Mostra menu principal
 * 2. Processa escolha do usuário
 * 3. Executa a função correspondente
 * 4. Repete até o usuário escolher sair
 */
int main() {
    // Inicialização das variáveis principais
    Jogador jogador1, jogador2;     // Estruturas dos dois jogadores
    Cidade cidades[MAX_CIDADES];    // Array para armazenar todas as cidades disponíveis
    int num_cidades = 0;            // Contador de cidades cadastradas
    int opcao;                      // Opção escolhida no menu
    
    // Loop principal do menu
    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Novo Jogo
                limparTela();
                animacaoCarregamento();
                inicializarJogo(&jogador1, &jogador2);
                jogarPartida(&jogador1, &jogador2);
                break;
                
            case 2: // Carregar Jogo
                limparTela();
                animacaoCarregamento();
                carregarJogo(&jogador1, &jogador2);
                break;
                
            case 3: // Ver Regras
                limparTela();
                mostrarRegras();
                break;
                
            case 4: // Ver Ranking
                limparTela();
                mostrarRanking();
                break;
                
            case 5: // Cadastrar Cidades
                limparTela();
                cadastrarCidades(cidades, &num_cidades);
                break;
                
            case 6: // Estatísticas
                limparTela();
                mostrarEstatisticas(jogador1, jogador2);
                break;
                
            case 0: // Sair
                limparTela();
                printf("\nObrigado por jogar! Até a próxima!\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
                sleep(2);
        }
    } while(opcao != 0);
    
    return 0;
}
