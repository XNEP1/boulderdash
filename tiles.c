/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#include "boulderdash.h"
#include "tiles.h"
#include "audio.h"
#include <string.h>
#include <stdlib.h>
#include <allegro5/allegro_audio.h>

/* GAME tiles */
const struct tile player = {   .tipo = PLAYER, .rounded = false, .explodable = true, .triggerExplosion = true, .consumable = false, .sprite.x = 0, .sprite.y = 0, .sprite.frames = 0, .sprite.fps = 0,
                        .pisca.x = 0, .pisca.y = 1, .pisca.frames = 8, .pisca.fps = 20,
                        .idle.x = 0, .idle.y = 2, .idle.frames = 8, .idle.fps = 20,
                        .pisca_idle.x = 0, .pisca_idle.y = 3, .pisca_idle.frames = 8, .pisca_idle.fps = 20,
                        .esquerda.x = 0, .esquerda.y = 4, .esquerda.frames = 8, .esquerda.fps = 20,
                        .direita.x = 0, .direita.y = 5, .direita.frames = 8, .direita.fps = 20
                    };
const struct tile dirt = {            .tipo = DIRT,            .rounded = false, .explodable = true,   .triggerExplosion = false, .consumable = true,  .sprite.x = 1, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile boulder = {         .tipo = BOULDER,         .rounded = true,  .explodable = true,   .triggerExplosion = false, .consumable = false, .sprite.x = 0, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile falling_boulder = { .tipo = FALLING_BOULDER, .rounded = false, .explodable = true,   .triggerExplosion = false, .consumable = false, .sprite.x = 0, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile diamond = {         .tipo = DIAMOND,         .rounded = true,  .explodable = true,   .triggerExplosion = false, .consumable = true,  .sprite.x = 0, .sprite.y = 10, .sprite.frames = 8, .sprite.fps = 20};
const struct tile falling_diamond = { .tipo = FALLING_DIAMOND, .rounded = false, .explodable = true,   .triggerExplosion = false, .consumable = false, .sprite.x = 0, .sprite.y = 10, .sprite.frames = 8, .sprite.fps = 20};
const struct tile wall = {            .tipo = WALL,            .rounded = true,  .explodable = true,   .triggerExplosion = false, .consumable = false, .sprite.x = 3, .sprite.y = 6,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile steel_wall = {      .tipo = STEEL_WALL,      .rounded = false, .explodable = false,  .triggerExplosion = false, .consumable = false, .sprite.x = 1, .sprite.y = 6,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile magic_wall = {      .tipo = MAGIC_WALL,      .rounded = false, .explodable = true,   .triggerExplosion = false, .consumable = true,  .sprite.x = 4, .sprite.y = 6,  .sprite.frames = 4, .sprite.fps = 20};
const struct tile firefly_up = {      .tipo = FIREFLY_UP,      .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 9,  .sprite.frames = 8, .sprite.fps = 20};
const struct tile firefly_down = {    .tipo = FIREFLY_DOWN,    .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 9,  .sprite.frames = 8, .sprite.fps = 20};
const struct tile firefly_left = {    .tipo = FIREFLY_LEFT,    .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 9,  .sprite.frames = 8, .sprite.fps = 20};
const struct tile firefly_right = {   .tipo = FIREFLY_RIGHT,   .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 9,  .sprite.frames = 8, .sprite.fps = 20};
const struct tile butterfly_up = {    .tipo = BUTTERFLY_UP,    .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 11, .sprite.frames = 8, .sprite.fps = 20};
const struct tile butterfly_down = {  .tipo = BUTTERFLY_DOWN,  .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 11, .sprite.frames = 8, .sprite.fps = 20};
const struct tile butterfly_left = {  .tipo = BUTTERFLY_LEFT,  .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 11, .sprite.frames = 8, .sprite.fps = 20};
const struct tile butterfly_right = { .tipo = BUTTERFLY_RIGHT, .rounded = false, .explodable = true,   .triggerExplosion = false,  .consumable = false, .sprite.x = 0, .sprite.y = 11, .sprite.frames = 8, .sprite.fps = 20};
const struct tile amoeba = {          .tipo = AMOEBA,          .rounded = false, .explodable = true,   .triggerExplosion = true, .consumable = false, .sprite.x = 0, .sprite.y = 8,  .sprite.frames = 8, .sprite.fps = 20};
const struct tile empty = {           .tipo = EMPTY,           .rounded = false, .explodable = true,   .triggerExplosion = false, .consumable = true,  .sprite.x = 0, .sprite.y = 6,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode0 = {        .tipo = EXPLODE0,        .rounded = false, .explodable = true,   .triggerExplosion = true,  .consumable = true,  .sprite.x = 3, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode1 = {        .tipo = EXPLODE1,        .rounded = false, .explodable = true,   .triggerExplosion = true,  .consumable = true,  .sprite.x = 4, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode2 = {        .tipo = EXPLODE2,        .rounded = false, .explodable = true,   .triggerExplosion = true,  .consumable = true,  .sprite.x = 5, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode3 = {        .tipo = EXPLODE3,        .rounded = false, .explodable = true,   .triggerExplosion = true,  .consumable = true,  .sprite.x = 4, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode4 = {        .tipo = EXPLODE4,        .rounded = false, .explodable = true,   .triggerExplosion = true,  .consumable = true,  .sprite.x = 3, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile exitOpen = {        .tipo = EXIT_OPEN,       .rounded = false, .explodable = false,  .triggerExplosion = false, .consumable = false, .sprite.x = 1, .sprite.y = 6,  .sprite.frames = 2, .sprite.fps = 4};
const struct tile exitClose = {       .tipo = STEEL_WALL,      .rounded = false, .explodable = false,  .triggerExplosion = false, .consumable = false, .sprite.x = 1, .sprite.y = 6,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode_then_diamonds0 = { .tipo = EXPLODE_THEN_DIAMONDS0, .rounded = false, .explodable = false,  .triggerExplosion = true,  .consumable = true,  .sprite.x = 3, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode_then_diamonds1 = { .tipo = EXPLODE_THEN_DIAMONDS1, .rounded = false, .explodable = false,  .triggerExplosion = true,  .consumable = true,  .sprite.x = 4, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode_then_diamonds2 = { .tipo = EXPLODE_THEN_DIAMONDS2, .rounded = false, .explodable = false,  .triggerExplosion = true,  .consumable = true,  .sprite.x = 5, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode_then_diamonds3 = { .tipo = EXPLODE_THEN_DIAMONDS3, .rounded = false, .explodable = false,  .triggerExplosion = true,  .consumable = true,  .sprite.x = 4, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};
const struct tile explode_then_diamonds4 = { .tipo = EXPLODE_THEN_DIAMONDS4, .rounded = false, .explodable = false,  .triggerExplosion = true,  .consumable = true,  .sprite.x = 3, .sprite.y = 7,  .sprite.frames = 0, .sprite.fps = 0};

/* HUD tiles */
const struct tile icone = { .tipo = ICONE,  .sprite.x = 8 , .sprite.y = 0 };
const struct tile _0 = {    .tipo = ZERO,   .sprite.x = 8 , .sprite.y = 1 };
const struct tile _1 = {    .tipo = UM,     .sprite.x = 8 , .sprite.y = 2 };
const struct tile _2 = {    .tipo = DOIS,   .sprite.x = 8 , .sprite.y = 3 };
const struct tile _3 = {    .tipo = TRES,   .sprite.x = 8 , .sprite.y = 4 };
const struct tile _4 = {    .tipo = QUATRO, .sprite.x = 8 , .sprite.y = 5 };
const struct tile _5 = {    .tipo = CINCO,  .sprite.x = 8 , .sprite.y = 6 };
const struct tile _6 = {    .tipo = SEIS,   .sprite.x = 8 , .sprite.y = 7 };
const struct tile _7 = {    .tipo = SETE,   .sprite.x = 8 , .sprite.y = 8 };
const struct tile _8 = {    .tipo = OITO,   .sprite.x = 8 , .sprite.y = 9 };
const struct tile _9 = {    .tipo = NOVE,   .sprite.x = 8 , .sprite.y = 10 };

const struct tile titles_info[TILES_QUANTIDADE] = {player, dirt, boulder, falling_boulder, diamond, falling_diamond, wall, steel_wall, magic_wall, firefly_up, firefly_down, firefly_left, firefly_right, butterfly_up, butterfly_down, butterfly_left, butterfly_right, amoeba, empty, explode0, explode1, explode2, explode3, explode4, explode_then_diamonds0, explode_then_diamonds1, explode_then_diamonds2, explode_then_diamonds3, explode_then_diamonds4, exitOpen, exitClose, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, icone};

struct tile getTileInfo(int type){
    return titles_info[type];
}

bool isType(int type, boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    if(jogo->campo[y][x] == type)
        return true;
    else
        return false;
}

bool isNotType(int type, boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    if(jogo->campo[y][x] != type)
        return true;
    else
        return false;
}

bool isTypeInDir(int type, boulderdash *jogo, int x, int y, int dir, int times){
    switch (dir){
        case CIMA:     return isType(type, jogo, x, y-times); break;
        case BAIXO:    return isType(type, jogo, x, y+times); break;
        case ESQUERDA: return isType(type, jogo, x-times, y); break;
        case DIREITA:  return isType(type, jogo, x+times, y); break;
        default:       return isType(type, jogo, x, y);   break;
    }
}

bool isNotTypeInDir(int type, boulderdash *jogo, int x, int y, int dir, int times){
    switch (dir){
        case CIMA:     return isNotType(type, jogo, x, y-times); break;
        case BAIXO:    return isNotType(type, jogo, x, y+times); break;
        case ESQUERDA: return isNotType(type, jogo, x-times, y); break;
        case DIREITA:  return isNotType(type, jogo, x+times, y); break;
        default:       return isNotType(type, jogo, x, y);   break;
    }
}

bool isRounded(boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    return getTileInfo(jogo->campo[y][x]).rounded;
}

bool isExplodable(boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    return getTileInfo(jogo->campo[y][x]).explodable;
}

bool itsTriggersExplosion(boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    return getTileInfo(jogo->campo[y][x]).triggerExplosion;
}

bool itsTriggersExplosionDir(boulderdash *jogo, int x, int y, int dir){
    switch (dir){
        case CIMA:     return itsTriggersExplosion(jogo, x, y-1); break;
        case BAIXO:    return itsTriggersExplosion(jogo, x, y+1); break;
        case ESQUERDA: return itsTriggersExplosion(jogo, x-1, y); break;
        case DIREITA:  return itsTriggersExplosion(jogo, x+1, y); break;
        default:       return itsTriggersExplosion(jogo, x, y);   break;
    }
}

bool isConsumable(boulderdash *jogo, int x, int y){
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;
    if(x >= largura || x < 0 || y >= altura || y < 0 )
        return false;

    return getTileInfo(jogo->campo[y][x]).consumable;
}

bool isConsumableInDir(boulderdash *jogo, int x, int y, int dir, int times){
    switch (dir){
        case CIMA:     return isConsumable(jogo, x, y-times); break;
        case BAIXO:    return isConsumable(jogo, x, y+times); break;
        case ESQUERDA: return isConsumable(jogo, x-times, y); break;
        case DIREITA:  return isConsumable(jogo, x+times, y); break;
        default:       return isConsumable(jogo, x, y);   break;
    }
}

static int rotateDir(int dir){
    switch(dir){
        case CIMA:     return DIREITA;
        case BAIXO:    return ESQUERDA;
        case ESQUERDA: return CIMA;
        case DIREITA:  return BAIXO;
    }

    return dir;
}

static int antiRotateDir(int dir){
    switch(dir){
        case CIMA:     return ESQUERDA;
        case BAIXO:    return DIREITA;
        case ESQUERDA: return BAIXO;
        case DIREITA:  return CIMA;
    }

    return dir;
}


static int xForDir(int x, int dir){
    switch(dir){
        case CIMA:     return x;
        case BAIXO:    return x;
        case ESQUERDA: return x-1;
        case DIREITA:  return x+1;
    }

    return x;
}

static int yForDir(int y, int dir){
    switch(dir){
        case CIMA:     return y-1;
        case BAIXO:    return y+1;
        case ESQUERDA: return y;
        case DIREITA:  return y;
    }

    return y;
}

/* =================================================================================================================================================== */
/* UPDATE FUNCTIONS */

void updatePlayer(boulderdash *jogo, int x, int y){
    int dir = jogo->player.dir;

    if(isTypeInDir(EMPTY, jogo, x, y, dir, 1)){
        /* Movimentação padrao do player */
        mover_player(jogo, jogo->player.dir);
    }
    else if (isTypeInDir(DIRT, jogo, x, y, dir, 1)){
        /* O player cavou. */
        playSoundDig();
        mover_player(jogo, jogo->player.dir);
    }
    else if(isTypeInDir(DIAMOND, jogo, x, y, dir, 1)){
        /* Coletou um diamante */
        mover_player(jogo, jogo->player.dir);
        jogo->diamantes_pegos++;
        jogo->pontuacao += (jogo->diamantes_pegos < jogo->cave[jogo->level].diamantes_necessarios[0]) ?  jogo->cave[jogo->level].pontos_por_diamantes :  jogo->cave[jogo->level].pontos_por_diamantes_extras;
    }
    else if(isTypeInDir(BOULDER, jogo, x, y, dir, 1) && isTypeInDir(EMPTY, jogo, x, y, dir, 2) && dir != CIMA && dir != BAIXO){
        /* Esta movendo uma pedra */
        if(isTypeInDir(EMPTY, jogo, x, y+1, dir, 2))
            jogo->campo[yForDir(y, dir)][xForDir(x, dir)] = FALLING_BOULDER;
        else
            jogo->campo[yForDir(y, dir)][xForDir(x, dir)] = BOULDER;
        move(jogo, xForDir(x, dir), yForDir(y, dir), dir);
        mover_player(jogo, jogo->player.dir);
    }
    else if(isTypeInDir(EXIT_OPEN, jogo, x, y, dir, 1)){
        /* Chegou na saida da caverna */
        mover_player(jogo, jogo->player.dir);
        jogo->status = VITORIA;
    }
}

void updateBoulder(boulderdash *jogo, int x, int y){
    if(isType(EMPTY, jogo, x, y+1)){ /* A pedra caiu. */
        jogo->campo[y][x] = FALLING_BOULDER;
        move(jogo, x, y, BAIXO);
    }
    else if(isRounded(jogo, x, y+1) && isType(EMPTY, jogo, x-1, y) && isType(EMPTY, jogo, x-1, y+1)){ /* A pedra rolou para a esquerda. */
        jogo->campo[y][x] = FALLING_BOULDER;
        move(jogo, x, y, ESQUERDA);
    }
    else if(isRounded(jogo, x, y+1) && isType(EMPTY, jogo, x+1, y) && isType(EMPTY, jogo, x+1, y+1)){ /* A pedra rolou para a direita. */
        jogo->campo[y][x] = FALLING_BOULDER;
        move(jogo, x, y, DIREITA);
    }
    else{
        jogo->campo[y][x] = BOULDER;
        jogo->haveToUpdate[y][x] = true;
    }
}

void uptadeFallingBoulder(boulderdash *jogo, int x, int y){
    if(itsTriggersExplosion(jogo, x, y+1) && isNotType(AMOEBA, jogo, x, y+1))
        explode(jogo, x, y+1, 1);
    else if(isTypeInDir(MAGIC_WALL, jogo, x, y, BAIXO, 1) && isTypeInDir(EMPTY, jogo, x, y, BAIXO, 2)){
        jogo->campo[y][x] = EMPTY;
        jogo->campo[y+2][x] = DIAMOND; 
    }
    else
        updateBoulder(jogo, x, y);
}

void updateDiamond(boulderdash *jogo, int x, int y){
    if(isType(EMPTY, jogo, x, y+1)){ /* O diamante caiu. */
        jogo->campo[y][x] = FALLING_DIAMOND;
        move(jogo, x, y, BAIXO);
    }
    else if(isRounded(jogo, x, y+1) && isType(EMPTY, jogo, x-1, y) && isType(EMPTY, jogo, x-1, y+1)){ /* O diamante rolou para a esquerda. */
        jogo->campo[y][x] = FALLING_DIAMOND;
        move(jogo, x, y, ESQUERDA);
    }
    else if(isRounded(jogo, x, y+1) && isType(EMPTY, jogo, x+1, y) && isType(EMPTY, jogo, x+1, y+1)){ /* O diamante rolou para a direita. */
        jogo->campo[y][x] = FALLING_DIAMOND;
        move(jogo, x, y, DIREITA);
    }
    else{
        jogo->campo[y][x] = DIAMOND;
        jogo->haveToUpdate[y][x] = true;
    }
}

void updateFallingDiamond(boulderdash *jogo, int x, int y){
    if(itsTriggersExplosion(jogo, x, y+1) && isNotType(AMOEBA, jogo, x, y+1))
        explode(jogo, x, y+1, 1);
    else if(isTypeInDir(MAGIC_WALL, jogo, x, y, BAIXO, 1) && isTypeInDir(EMPTY, jogo, x, y, BAIXO, 2)){
        jogo->campo[y][x] = EMPTY;
        jogo->campo[y+2][x] = FALLING_BOULDER; 
    }
    else
        updateDiamond(jogo, x, y);
}

void updateFirefly(boulderdash *jogo, int x, int y){
    int dir = jogo->campo[y][x] - FIREFLY_UP;
    int newDir = dir;
    int i;

    /* O algo entrou em contado, portanto deve explodir. */
    if(itsTriggersExplosionDir(jogo, x, y, CIMA)){
        explode(jogo, x, y-1, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, BAIXO)){
        explode(jogo, x, y+1, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, ESQUERDA)){
        explode(jogo, x-1, y, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, DIREITA)){
        explode(jogo, x+1, y, 1);
        return;
    }

    /* Movimentação */
    if(isNotTypeInDir(EMPTY, jogo, x, y, dir, 1)){
        newDir = rotateDir(dir);

        if(isTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
            move(jogo, x, y, newDir);
            x = xForDir(x, dir);
            y = yForDir(y, dir);
            /* Move sem atualizar a direção da firefly */
        }
        else if(isNotTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
            jogo->campo[y][x] = FIREFLY_UP + newDir;
        }
    }
    else if(isTypeInDir(EMPTY, jogo, x, y, dir, 1)){
        move(jogo, x, y, dir);
        x = xForDir(x, dir);
        y = yForDir(y, dir);

        for(i=1; i<=4; i++){ /* Só existem 4 opções de direções. */
            newDir = antiRotateDir(newDir);

            if(isNotTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
                dir = newDir;
                break;
            }
        }
        jogo->campo[y][x] = FIREFLY_UP + dir;
    }
    
}

void updateButterfly(boulderdash *jogo, int x, int y){
    int dir = jogo->campo[y][x] - BUTTERFLY_UP;
    int newDir = dir;
    int i;

    /* O algo entrou em contado, portanto deve explodir. */
    if(itsTriggersExplosionDir(jogo, x, y, CIMA)){
        explodeThenDiamonds(jogo, x, y-1, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, BAIXO)){
        explodeThenDiamonds(jogo, x, y+1, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, ESQUERDA)){
        explodeThenDiamonds(jogo, x-1, y, 1);
        return;
    }
    else if(itsTriggersExplosionDir(jogo, x, y, DIREITA)){
        explodeThenDiamonds(jogo, x+1, y, 1);
        return;
    }

    /* Movimentação */
    if(isNotTypeInDir(EMPTY, jogo, x, y, dir, 1)){
        newDir = rotateDir(dir);

        if(isTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
            move(jogo, x, y, newDir);
            x = xForDir(x, dir);
            y = yForDir(y, dir);
            /* Move sem atualizar a direção da firefly */
        }
        else if(isNotTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
            jogo->campo[y][x] = BUTTERFLY_UP + newDir;
        }
    }
    else if(isTypeInDir(EMPTY, jogo, x, y, dir, 1)){
        move(jogo, x, y, dir);
        x = xForDir(x, dir);
        y = yForDir(y, dir);

        for(i=1; i<=4; i++){ /* Só existem 4 opções de direções. */
            newDir = antiRotateDir(newDir);

            if(isNotTypeInDir(EMPTY, jogo, x, y, newDir, 1)){
                dir = newDir;
                break;
            }
        }
        jogo->campo[y][x] = BUTTERFLY_UP + dir;
    }
}

static bool thisAmoebaCanGrow(boulderdash *jogo, int x, int y){
    /* Impede que essa emoeba cresca 2 quadrados no mesmo game tick. E retorna true se essa emoeba ainda consegue crescer mais. */
    bool canGrow;
    if(isConsumable(jogo, x, y))
        canGrow = true;
    else
        canGrow = false;

    if(isNotType(AMOEBA, jogo, x, y) || jogo->haveToUpdate[y][x] == false)
        return canGrow;
        
    jogo->haveToUpdate[y][x] = false;

    if( thisAmoebaCanGrow(jogo, x, yForDir(y, CIMA)) ||
        thisAmoebaCanGrow(jogo, x, yForDir(y, BAIXO)) ||
        thisAmoebaCanGrow(jogo, xForDir(x, ESQUERDA), y) ||
        thisAmoebaCanGrow(jogo, xForDir(x, DIREITA), y) )
        {
            canGrow = true;
        }

    return canGrow;
}

static void amoebaDied(boulderdash *jogo, int x, int y){
    if(isNotType(AMOEBA, jogo, x, y))
        return;

    jogo->campo[y][x] = DIAMOND;

    amoebaDied(jogo, x, yForDir(y, CIMA));
    amoebaDied(jogo, x, yForDir(y, BAIXO)); 
    amoebaDied(jogo, xForDir(x, ESQUERDA), y); 
    amoebaDied(jogo, xForDir(x, DIREITA), y);
}

void updateAmoeba(boulderdash *jogo, int x, int y){
    int r = rand() % (300 - jogo->tempo);

    switch(r){
        case 1:
        case 2:
        case 3:
            if(isConsumableInDir(jogo, x, y, CIMA, 1)){
                jogo->campo[yForDir(y, CIMA)][x] = AMOEBA;
                if( !(thisAmoebaCanGrow(jogo, x, y)))
                    amoebaDied(jogo, x, y);
            }
            break;
        
        case 4:
        case 5:
        case 6:
            if(isConsumableInDir(jogo, x, y, BAIXO, 1)){
                jogo->campo[yForDir(y, BAIXO)][x] = AMOEBA;
                if( !(thisAmoebaCanGrow(jogo, x, y)))
                    amoebaDied(jogo, x, y);
            }
            break;

        case 7:
        case 8:
        case 9:
            if(isConsumableInDir(jogo, x, y, ESQUERDA, 1)){
                jogo->campo[y][xForDir(x, ESQUERDA)] = AMOEBA;
                if( !(thisAmoebaCanGrow(jogo, x, y)))
                    amoebaDied(jogo, x, y);
            }
            break;

        case 10:
        case 11:
        case 12:
            if(isConsumableInDir(jogo, x, y, DIREITA, 1)){
                jogo->campo[y][xForDir(x, DIREITA)] = AMOEBA;
                if( !(thisAmoebaCanGrow(jogo, x, y)))
                    amoebaDied(jogo, x, y);
            }
            break;
    }
    
}

void updateExplosion(boulderdash *jogo, int x, int y){
    switch(jogo->campo[y][x]){
        case EXPLODE0: jogo->campo[y][x] = EXPLODE1; break;
        case EXPLODE1: jogo->campo[y][x] = EXPLODE2; break;
        case EXPLODE2: jogo->campo[y][x] = EXPLODE3; break;
        case EXPLODE3: jogo->campo[y][x] = EXPLODE4; break;
        case EXPLODE4:
            jogo->campo[y][x] = EMPTY;
            if(jogo->status == DERROTA)
                jogo->status = CARREGANDO; 
            break;
    }
}

void updateExplosionThenDiamonds(boulderdash *jogo, int x, int y){
    int i, j;
    int diamonds_qnt = 5;
    int radius;

    switch(jogo->campo[y][x]){
        case EXPLODE_THEN_DIAMONDS0: jogo->campo[y][x] = EXPLODE_THEN_DIAMONDS1; break;
        case EXPLODE_THEN_DIAMONDS1: jogo->campo[y][x] = EXPLODE_THEN_DIAMONDS2; break;
        case EXPLODE_THEN_DIAMONDS2: jogo->campo[y][x] = EXPLODE_THEN_DIAMONDS3; break;
        case EXPLODE_THEN_DIAMONDS3: jogo->campo[y][x] = EXPLODE_THEN_DIAMONDS4; break;
        case EXPLODE_THEN_DIAMONDS4:
            if(jogo->status == DERROTA)
                jogo->status = CARREGANDO; 

            for(radius=0; radius<5; radius++){
                for(i=-1*radius; i<=radius; i++)
                    for(j=-1*radius; j<=radius; j++){
                        if(diamonds_qnt <= 0)
                            return;

                        if(i != radius && i != -1*radius && j != radius && j != -1*radius)
                            continue;

                        if(isType(EMPTY, jogo, x+j, y+i) || isType(EXPLODE4, jogo, x+j, y+i) || isType(EXPLODE_THEN_DIAMONDS4, jogo, x+j, y+i)){
                            jogo->campo[y+i][x+j] = DIAMOND;
                            jogo->haveToUpdate[y+i][x+j] = false;
                            diamonds_qnt--;
                        }
                    }

            }

            break;

    }
}

void updateExit(boulderdash *jogo, int x, int y){
    if(jogo->diamantes_pegos >= jogo->cave[jogo->level].diamantes_necessarios[0]){
        playSoundExitOpen();
        jogo->campo[y][x] = EXIT_OPEN;
    }
}

void updateHUD(boulderdash *jogo){
    int level = jogo->level;

    /* Atualiza os valores do HUD */
    if(jogo->diamantes_pegos < jogo->cave[level].diamantes_necessarios[0]){
        jogo->HUD[7] = (int)(jogo->cave[level].pontos_por_diamantes / 10) + ZERO;
        jogo->HUD[8] = (jogo->cave[level].pontos_por_diamantes % 10) + ZERO;
    }
    else{
        jogo->HUD[7] = (int)(jogo->cave[level].pontos_por_diamantes_extras / 10) + ZERO;
        jogo->HUD[8] = (jogo->cave[level].pontos_por_diamantes_extras % 10) + ZERO;
    }

    /* DIAMANTES PEGOS */
    jogo->HUD[13] = (int)(jogo->diamantes_pegos / 10) + ZERO;
    jogo->HUD[14] = (jogo->diamantes_pegos % 10) + ZERO;

    /* TEMPO */
    jogo->HUD[25] = (int)(jogo->tempo / 100) + ZERO;
    jogo->HUD[26] = (int)((jogo->tempo % 100) / 10) + ZERO;
    jogo->HUD[27] = (jogo->tempo % 10) + ZERO;

    /* PONTUAÇÃO */
    jogo->HUD[31] = (int)(jogo->pontuacao / 100000) + ZERO;
    jogo->HUD[32] = (int)((jogo->pontuacao % 100000) / 10000) + ZERO;
    jogo->HUD[33] = (int)((jogo->pontuacao % 10000) / 1000) + ZERO;
    jogo->HUD[34] = (int)((jogo->pontuacao % 1000) / 100) + ZERO;
    jogo->HUD[35] = (int)((jogo->pontuacao % 100) / 10) + ZERO;
    jogo->HUD[36] = (jogo->pontuacao % 10) + ZERO;
}



/* =================================================================================================================================================== */
/* ACTIONS functions */
void move(boulderdash *jogo, int x, int y, int dir){
    int type = jogo->campo[y][x];
    jogo->campo[y][x] = EMPTY;
    
    switch (dir){
        case CIMA:
            jogo->campo[y-1][x] = type;
            jogo->haveToUpdate[y-1][x] = false;
            break;
        case BAIXO:
            jogo->campo[y+1][x] = type;
            jogo->haveToUpdate[y+1][x] = false;
            break;
        case ESQUERDA:
            jogo->campo[y][x-1] = type;
            jogo->haveToUpdate[y][x-1] = false;
            break;
        case DIREITA:
            jogo->campo[y][x+1] = type;
            jogo->haveToUpdate[y][x+1] = false;
            break;
        case PARADO:
            jogo->campo[y][x] = type;
            jogo->haveToUpdate[y][x] = false;
            break;
    }
}

void mover_player(struct boulderdash *jogo, int dir){
    if(jogo->status != JOGANDO || dir == PARADO)
        return;

    move(jogo, jogo->player.x, jogo->player.y, dir);
    switch (dir){
        case CIMA: jogo->player.y-- ; break;
        case BAIXO: jogo->player.y++ ; break;
        case ESQUERDA: jogo->player.x-- ; break;
        case DIREITA: jogo->player.x++ ; break;
    }
}

void explode(boulderdash *jogo, int x, int y, int radius){
    int i, j;

    playSoundExplode();

    for(i=-1*radius; i<=radius; i++)
        for(j=-1*radius; j<=radius; j++)
            if(isExplodable(jogo, x+j, y+i)){
                jogo->campo[y+i][x+j] = EXPLODE0;
                jogo->haveToUpdate[y+i][x+j] = false;
            }
}

void explodeThenDiamonds(boulderdash *jogo, int x, int y, int radius){
    explode(jogo, x, y, radius);
    jogo->campo[y][x] = EXPLODE_THEN_DIAMONDS0;
}

void atualizar_jogo(boulderdash *jogo){
    int x, y;
    int level = jogo->level;

    /* Time Out */
    if(jogo->tempo <= 0 && (jogo->status == JOGANDO))
        explode(jogo, jogo->player.x, jogo->player.y, 3);

    updateGameStatus(jogo);

    for(y=0; y<jogo->cave[level].altura; y++){
        for(x=0; x<jogo->cave[level].largura; x++){
            if(jogo->haveToUpdate[y][x] == true){
                switch(jogo->campo[y][x]){
                    case PLAYER:
                        updatePlayer(jogo, x, y);
                        break;

                    case BOULDER:
                        updateBoulder(jogo, x, y);
                        break;

                    case FALLING_BOULDER:
                        uptadeFallingBoulder(jogo, x, y);
                        break;

                    case DIAMOND:
                        updateDiamond(jogo, x, y);
                        break;

                    case FALLING_DIAMOND:
                        updateFallingDiamond(jogo, x, y);
                        break;

                    case FIREFLY_UP:
                    case FIREFLY_DOWN:
                    case FIREFLY_LEFT:
                    case FIREFLY_RIGHT:
                        updateFirefly(jogo, x, y);
                        break;

                    case BUTTERFLY_UP:
                    case BUTTERFLY_DOWN:
                    case BUTTERFLY_LEFT:
                    case BUTTERFLY_RIGHT:
                        updateButterfly(jogo, x, y);
                        break;

                    case AMOEBA:
                        updateAmoeba(jogo, x, y);
                        break;

                    case EXPLODE0:
                    case EXPLODE1:
                    case EXPLODE2:
                    case EXPLODE3:
                    case EXPLODE4:
                        updateExplosion(jogo, x, y);
                        break;

                    case EXPLODE_THEN_DIAMONDS0:
                    case EXPLODE_THEN_DIAMONDS1:
                    case EXPLODE_THEN_DIAMONDS2:
                    case EXPLODE_THEN_DIAMONDS3:
                    case EXPLODE_THEN_DIAMONDS4:
                        updateExplosionThenDiamonds(jogo, x, y);
                        break;

                    case EXIT_CLOSE:
                        updateExit(jogo, x, y);
                        break;
                }
            }
        }
    }

    updateHUD(jogo);

    /* Define todos os valores da matriz para true. */
    memset(jogo->haveToUpdate[0], true, jogo->cave[jogo->level].altura * jogo->cave[jogo->level].largura *sizeof(bool));
}