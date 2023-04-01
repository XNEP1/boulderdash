/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#ifndef BD_TILES_H
#define BD_TILES_H

/* DECLARAÇÕES IMCOMPLETAS DE DEPENDENCIAS */
#define bool _Bool
#define false 0
#define true 1
typedef struct boulderdash boulderdash;

/* =================================================================================================================================================== */
/* DEFINIÇÃO DOS TILES */

enum {PLAYER, DIRT, BOULDER, FALLING_BOULDER, DIAMOND, FALLING_DIAMOND, WALL, STEEL_WALL, MAGIC_WALL, FIREFLY_UP, FIREFLY_DOWN, FIREFLY_LEFT, FIREFLY_RIGHT, BUTTERFLY_UP, BUTTERFLY_DOWN, BUTTERFLY_LEFT, BUTTERFLY_RIGHT, AMOEBA, EMPTY, EXPLODE0, EXPLODE1, EXPLODE2, EXPLODE3, EXPLODE4, EXPLODE_THEN_DIAMONDS0, EXPLODE_THEN_DIAMONDS1, EXPLODE_THEN_DIAMONDS2, EXPLODE_THEN_DIAMONDS3, EXPLODE_THEN_DIAMONDS4, EXIT_OPEN, EXIT_CLOSE, ZERO, UM, DOIS, TRES, QUATRO, CINCO, SEIS, SETE, OITO, NOVE, ICONE, TILES_QUANTIDADE} TIPOS_DE_TILES;

struct sprite{
    int x;
    int y;
    int frames;
    int fps;
};

struct tile{
    int tipo;
    bool rounded;
    bool explodable;
    bool triggerExplosion;
    bool consumable;
    struct sprite sprite;
    struct sprite esquerda, direita, pisca, idle, pisca_idle;
};

/* =================================================================================================================================================== */
/* FUNCTIONS */

struct tile getTileInfo(int type);
bool isType(int type, boulderdash *jogo, int x, int y);
bool isNotType(int type, boulderdash *jogo, int x, int y);
bool isTypeInDir(int type, boulderdash *jogo, int x, int y, int dir, int times);
bool isNotTypeInDir(int type, boulderdash *jogo, int x, int y, int dir, int times);
bool isRounded(boulderdash *jogo, int x, int y);
bool isExplodable(boulderdash *jogo, int x, int y);
bool itsTriggersExplosion(boulderdash *jogo, int x, int y);
bool itsTriggersExplosionDir(boulderdash *jogo, int x, int y, int dir);


/* UPDATE functions */
void updatePlayer(boulderdash *jogo, int x, int y);
void updateBoulder(boulderdash *jogo, int x, int y);
void uptadeFallingBoulder(boulderdash *jogo, int x, int y);
void updateDiamond(boulderdash *jogo, int x, int y);
void updateFallingDiamond(boulderdash *jogo, int x, int y);
void updateFirefly(boulderdash *jogo, int x, int y);
void updateButterfly(boulderdash *jogo, int x, int y);
void updateAmoeba(boulderdash *jogo, int x, int y);
void updateExplosion(boulderdash *jogo, int x, int y);
void updateExplosionThenDiamonds(boulderdash *jogo, int x, int y);
void updateExit(boulderdash *jogo, int x, int y);
void updateHUD(boulderdash *jogo);

/* ACTIONS functions */
/* Move o que estiver em (x,y) no campo em 1 unidade em direção de dir. */
void move(boulderdash *jogo, int x, int y, int dir);

/* Move o player em 1 quadrado para alguma direção. */
void mover_player(struct boulderdash *jogo, int movimento);

/* Define todos os tiles de (x-radius, y-radius) até (x+radius, y+radius) como EXPLOSION0. */
void explode(boulderdash *jogo, int x, int y, int radius);

/* Explode igual ao explode, mas coloca diamantes após a explosão. 
 * Usado para as butterflys. */
void explodeThenDiamonds(boulderdash *jogo, int x, int y, int radius);

/* Atualiza todos os quadrados em campo, ou seja, atualiza o jogo para o proximo estado.
* Essa função também pode mudar o estado do jogo caso algum gatilho ocorra.
* Caso o jogador morra nessa atualização, define o estado do jogo para DERROTA. 
* Caso o estado do jogo seja DERROTA, define o estado do jogo para CARREGANDO no final da explosão.*/
void atualizar_jogo(boulderdash *jogo);

#endif