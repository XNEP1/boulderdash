/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#ifndef BD_H
#define BD_H

/* Configurações */
#define WIDTH 1280
#define HEIGHT 736
#define FRAMERATE 60.0
#define TICKRATE 10.0
#define SCALE 1.25
#define DEBUG 0


/* DECLARAÇÕES IMCOMPLETAS DE DEPENDENCIAS */
#define bool _Bool
#define false 0
#define true 1
typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct ALLEGRO_DISPLAY ALLEGRO_DISPLAY;
typedef struct ALLEGRO_EVENT_QUEUE ALLEGRO_EVENT_QUEUE;
struct sprite;

/* =================================================================================================================================================== */
/* DEFINIÇÃO DE ESTRUTURAS */

enum {CARREGANDO, JOGANDO, VITORIA, DERROTA, TUTORIAL, EASTEREGG, PAUSADO, FIMJOGO} ESTADO ;
enum {CIMA, BAIXO, ESQUERDA, DIREITA, PARADO} POSSIVEIS_MOVIMENTOS;

struct caverna{
    int level;
    char nome[100];
    char descricao[200];
    int delay_magic_wall_e_amoeba;
    int pontos_por_diamantes;
    int pontos_por_diamantes_extras;
    int diamantes_necessarios[5];  /* Há 5 possiveis dificuldades */
    int tempo[5];  /* Há 5 possiveis dificuldades */
    int cor_background_1;
    int cor_background_2;
    int cor_foreground;
    int largura, altura;
    unsigned char **data;  /* Matriz com as informações de cada quadrado do campo.  */
};

struct player{
    int x, y;
    int dir;
    int last_dir;
};

typedef struct boulderdash {
    unsigned short status;
    unsigned short statusBeforePause;
    unsigned short level;
    int tempo;
    int diamantes_pegos;
    int pontuacao;
    int quantidade_de_cavernas;
    struct caverna *cave;     /* Vetor com todas as cavernas. Deve ser carregado com carregar_cavernas(). */
    unsigned char **campo;  /* Matriz com as informações de cada quadrado do campo.  */
    unsigned char *HUD;     /* Vetor que guarda quais tiles existem no HUD superior. */ 
    bool **haveToUpdate;     /* Diz se um quadrado deve ser atualizado. Impede que um boulder caia 2 ou mais quadrados em apenas um frame. */
    unsigned int frame;
    struct player player;
    int easterEggFlag;
} boulderdash;



/* =================================================================================================================================================== */
/* FUNCTIONS */


/* Executa todas as inicializações do Allegro. */
void inicializar_allegro(void);

/* Inicializa as variaveis da struct jogo. */
void inicializar_jogo(struct boulderdash *jogo);

/* Desaloca tudo que foi alocado anteriormente dentro do jogo. */
void destruir_jogo(struct boulderdash *jogo);

/* Carrega os dados e parametros das cavernas do arquivo e as guarda na struct jogo */
bool carregar_cavernas(struct boulderdash *jogo ,char *filename);

/* Inicia uma nova caverna para o jogador jogar. */
bool inicializar_level(struct boulderdash *jogo, int level, ALLEGRO_DISPLAY *display);

/* Verifica se "test" é igual a true, caso não, imprime a descrição do erro no terminal e fecha a execução. */
void must_init(bool test, const char *description);

/* Verefica que o jogador morreu e atualiza o status do jogo. */
void updateGameStatus(boulderdash *jogo);

/* Faz uma cena de vitória descontando os segundos restantes do timer e incrementando os pontos de acordo. 
* Essa função força a rendirização. E no final define o estado do jogo para CARREGANDO. */
void victoryScene(boulderdash *jogo, ALLEGRO_BITMAP *tileSet, ALLEGRO_EVENT_QUEUE *queue);

void easterEgg(boulderdash *jogo, ALLEGRO_DISPLAY *display);

#endif