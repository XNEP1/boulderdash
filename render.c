/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#include "boulderdash.h"
#include "tiles.h"
#include "render.h"

#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define SCROLL_JUMP 5
#define TUTORIAL_LIMIT -600

/* ESPAÇO ENTRE AS LINHAS (SPACE BETWEEN LINES) */
#define SBL 30

int tutorialYdelta = 0;

const char descriptions[11][2][300] = {
    {{"Rockford: Seu herói, guie-o pelas cavernas, em busca de diamantes para ativar a saída."},{""}},
    {{"Terra: Rockford pode cavar na sujeira. A maioria das outras criaturas não pode se mover por ela."},{""}},
    {{"Pedregulho: O pedregulho cai quando há espaço vazio abaixo dele, então cuidado quando há pedregulhos acima de você."}, {" Você morre se esmagado! Também é possível para Rockford mover pedregulhos empurrando-os."}},
    {{"Diamante: Muito importante! Para completar uma caverna você tem que encontrar uma certa quantidade de diamantes."},{""}},
    {{"Parede: Paredes podem bloquear o caminho de Rockfords, felizmente é possível explodi-las explodindo vaga-lumes ou borboletas."},{""}},
    {{"Parede indestrutível: Também uma parede, mas indestrutível. Esperava o que? :/"},{""}},
    {{"Parede Mágica: Esta parede muito especial que converte pedregulhos em diamantes e vice-versa."},{""}},
    {{"Vagalume(?): Se isso te tocar, você morre. Apesar de quase impossivel, se um pedregulho cair nela ela explode."},{""}},
    {{"Borboleta (explosiva): Se acha que vagalumes que explodem foi pouco, ainda tem mais!"},{"É a mesma coisa que vagalumes, mas quando explodem geram diamentes."}},
    {{"Amoeba (a massinha de brincar): Ela cresce consumindo o que houver ao seu arredor, menos pedregulhos ou paredes."},{"Quando encurralada, ela morre e vira diamantes. Logo, prenda ela em volta de pedregulhos!"}},
    {{"Saída: Abre quando você pegou a quantidade necessária de diamantes. Mas não tenha pressa! Diamantes extras valem mais pontos."},{"Ou tenha pressa! Pois o tempo restante será transformado em pontos extras. "}}
};

void escalar_sprites(ALLEGRO_BITMAP *bitmap, int escala){
    ALLEGRO_TRANSFORM trans;

    al_identity_transform(&trans);
    al_scale_transform(&trans, escala, escala);
    al_use_transform(&trans);
}

void desenhar_sprite(struct sprite sprite, boulderdash *jogo, int x, int y, ALLEGRO_BITMAP *tileSet){
    int frame;
    if(sprite.frames != 0)
        frame = (int)(floor((sprite.fps/FRAMERATE) * jogo->frame)) % sprite.frames;
    else
        frame = 0;

    al_draw_bitmap_region(tileSet,(sprite.x + frame) * 32, sprite.y * 32, 32, 32, x*32, y*32, 0);
}

void desenhar_sprite_player(boulderdash *jogo, int x, int y, ALLEGRO_BITMAP *tileSet){
    struct sprite sprite;

    switch (jogo->player.dir){
    case PARADO:
        sprite = getTileInfo(PLAYER).idle;
        break;

    case CIMA:
        if(jogo->player.last_dir == ESQUERDA)
            sprite = getTileInfo(PLAYER).esquerda;
        else
            sprite = getTileInfo(PLAYER).direita;
        break;

    case BAIXO:
        if(jogo->player.last_dir == ESQUERDA)
            sprite = getTileInfo(PLAYER).esquerda;
        else
            sprite = getTileInfo(PLAYER).direita;
        break;

    case ESQUERDA:
        sprite = getTileInfo(PLAYER).esquerda;
        break;

    case DIREITA:
        sprite = getTileInfo(PLAYER).direita;
        break;
    }

    desenhar_sprite(sprite, jogo, x, y, tileSet);
}

void renderGame(struct boulderdash *jogo, ALLEGRO_BITMAP *tileSet){
    int i, j;
    struct tile tile;
    int altura = jogo->cave[jogo->level].altura;
    int largura = jogo->cave[jogo->level].largura;

    jogo->frame++;
    al_clear_to_color(al_map_rgb(0, 0 ,0));

    /* Desenha o HUD */
    for(j=0; j<largura; j++){
        tile = getTileInfo(jogo->HUD[j]);
        desenhar_sprite(tile.sprite, jogo, j, 0, tileSet);
    }

    /* Desenha o campo */
    for(i=0; i<altura; i++){
        for(j=0; j<largura; j++){
            tile = getTileInfo(jogo->campo[i][j]);
            if(tile.tipo == PLAYER)
                desenhar_sprite_player(jogo, j, i + 1, tileSet);
            else
                desenhar_sprite(tile.sprite, jogo, j, i + 1, tileSet);
        }
    }

    al_flip_display();
}

static void updateTutorialY(int scroll){
    if(scroll == CIMA && (tutorialYdelta + SCROLL_JUMP) <= 0 )
        tutorialYdelta += SCROLL_JUMP;
    else if(scroll == BAIXO && (tutorialYdelta - SCROLL_JUMP) >= TUTORIAL_LIMIT)
        tutorialYdelta -= SCROLL_JUMP;
}

static int y(int y){
    /* Aplica o deslocamento delta na coordenada Y. */
    return y + tutorialYdelta;
}

static void desenhar_sprite_tutorial(struct sprite sprite, boulderdash *jogo, int x, int y, ALLEGRO_BITMAP *tileSet){
    int frame;
    if(sprite.frames != 0)
        frame = (int)(floor((sprite.fps/FRAMERATE) * jogo->frame)) % sprite.frames;
    else
        frame = 0;

    al_draw_bitmap_region(tileSet,(sprite.x + frame) * 32, sprite.y * 32, 32, 32, x, y, 0);
}

void renderTutorial(struct boulderdash *jogo, ALLEGRO_FONT *font, int scroll, ALLEGRO_BITMAP *tileSet){
    int i, type;

    jogo->frame++;
    al_clear_to_color(al_map_rgb(0, 0 ,0));
    updateTutorialY(scroll);

    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(1*SBL), 0, "2º PROJETO DE PROGRAMAÇÃO II: BOULDERDASH      POR BRUNO KRÜGEL");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(3*SBL), 0, "F1 e H abrem e fecham essa tela de tutorial.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(4*SBL), 0, "Use as setas ↑↓ do teclado rolar a página do tutorial.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(6*SBL), 0, "Use as setas ←↑↓→ do teclado para andar no jogo.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(7*SBL), 0, "ESC fecha o jogo.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(8*SBL), 0, "CTROL da direita pausa o jogo. Aperte qualquer botão (menos ESC, né) para retomar o jogo.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(9*SBL), 0, "PAGEUP pula para a próxima fase.");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(10*SBL), 0, "PAGEDOWN volta para a fase anterior.");

    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(12*SBL), 0, "Abaixo, uma descrição de cada coisa do jogo.");

    for(i=0; i<11; i++){
        switch(i){
            case 0:  type = PLAYER;       break;
            case 1:  type = DIRT;         break;
            case 2:  type = BOULDER;      break;
            case 3:  type = DIAMOND;      break;
            case 4:  type = WALL;         break;
            case 5:  type = STEEL_WALL;   break;
            case 6:  type = MAGIC_WALL;   break;
            case 7:  type = FIREFLY_UP;   break;
            case 8:  type = BUTTERFLY_UP; break;
            case 9:  type = AMOEBA;       break;
            case 10: type = EXIT_OPEN;    break;
        }

        desenhar_sprite_tutorial(getTileInfo(type).sprite, jogo, 10, y((14 + 2*i)*SBL), tileSet);
        al_draw_text(font, al_map_rgb(255, 255 ,255), 52, y((14 + 2*i)*SBL-8), 0, descriptions[i][0]);
        al_draw_text(font, al_map_rgb(255, 255 ,255), 52, y((14 + 2*i)*SBL+12), 0, descriptions[i][1]);
    }

    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(40*SBL), 0, "Boulderdash foi originalmente criado por Peter Liepa e publicado pela First Star Software.");

    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(42*SBL), 0, "Parabéns, você realmente leu o tutorial até o fim! Como recompensa te contarei um segredo:");
    al_draw_text(font, al_map_rgb(255, 255 ,255), 10, y(43*SBL), 0, "Se você apertar F2 nesse tutorial você ficará rico! Mas cuidado com as consequencias. Muahahaha.");

    al_flip_display();
}