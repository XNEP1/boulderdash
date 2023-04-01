/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#ifndef BD_RENDER_H
#define BD_RENDER_H

/* DECLARAÇÕES IMCOMPLETAS DE DEPENDENCIAS */
typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct ALLEGRO_FONT ALLEGRO_FONT;


/* Aumenta a imagem dos sprites em uma certa escala. */
void escalar_sprites(ALLEGRO_BITMAP *bitmap, int escala);

/* Desenha o sprite de um tile no grid da tela do jogo. */
void desenhar_sprite(struct sprite sprite, boulderdash *jogo, int x, int y, ALLEGRO_BITMAP *tileSet);

/* Chama a função desenhar_sprite, mas definindo corretamente o sprite do player que depende da direção de sua movimentação. */
void desenhar_sprite_player(boulderdash *jogo, int x, int y, ALLEGRO_BITMAP *tileSet);

/* Renderiza o campo usando os sprites de tileSet. */
void renderGame(struct boulderdash *jogo, ALLEGRO_BITMAP *tileSet);

/* Mostra o tutorial na tela. */
void renderTutorial(struct boulderdash *jogo, ALLEGRO_FONT *font, int scroll, ALLEGRO_BITMAP *tileSet);

#endif