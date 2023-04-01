/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

/* 
 * Ativar o modo debug faz: 
 * > O tempo limite da caverna não passa.
*/

#define KEY_RELEASED 0
#define KEY_PRESSED 1

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "boulderdash.h"
#include "tiles.h"
#include "audio.h"
#include "render.h"

#define DIR_PREFIX "./resources"

int bd_cores[16][3]={
                        {0, 0, 0},            /* 0 : PRETO */
                        {255, 250, 250},      /* 1 : BRANCO */
                        {255, 0, 0},          /* 2 : VERMELHO */
                        {0, 255, 255},        /* 3 : CIANO */
                        {153, 51, 153},       /* 4 : ROXO */
                        {0, 255, 0},          /* 5 : VERDE */
                        {0, 0, 255},          /* 6 : AZUL */
                        {255, 255, 0},        /* 7 : AMARELO */
                        {255, 165, 0},        /* 8 : LARANJA */
                        {106, 84, 0},         /* 9 : MARROM */
                        {226, 114, 91},       /* 10: VERMELHO CLARO */
                        {138, 138, 138},      /* 11: CINZA 1 */
                        {99, 99, 99},         /* 12: CINZA 2 */
                        {173, 173, 173},      /* 13: CINZA CLARO */
                        {173, 216, 230},      /* 14: AZUL CLARO */
                        {112, 128, 144}       /* 15: CINZA 3 */
                    };

void inicializar_allegro(void){
    al_init();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
}

void inicializar_jogo(struct boulderdash *jogo){
    jogo->status = CARREGANDO;
    jogo->frame = 0;
    jogo->level = 0;
    jogo->pontuacao = 0;
    jogo->player.dir = PARADO;
    jogo->player.last_dir = DIREITA;
    jogo->player.x = 1;
    jogo->player.y = 1;
    jogo->campo = NULL;
    jogo->easterEggFlag = -2;
}

void destruir_jogo(struct boulderdash *jogo){
    int i;

    for(i=0; i<jogo->quantidade_de_cavernas; i++){
        free(jogo->cave[i].data[0]);
        free(jogo->cave[i].data);
    }

    free(jogo->campo[0]);
    free(jogo->campo);
    free(jogo->haveToUpdate[0]);
    free(jogo->haveToUpdate);
    free(jogo->HUD);
}

bool carregar_cavernas(struct boulderdash *jogo ,char *filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL)
        return 0;
    int quantidade_de_cavernas;
    int altura, largura;
    int i, l, c;

    fscanf(file, "%d", &quantidade_de_cavernas);
    jogo->quantidade_de_cavernas = quantidade_de_cavernas;
    struct caverna *caves = malloc(sizeof(struct caverna) * quantidade_de_cavernas);
    if(caves == NULL)
        return 0;

    for(i=0; i<quantidade_de_cavernas; i++){
        fscanf(file, "%d", &caves[i].level);
        fgetc(file);  /* Lê o \n. */

        fgets(caves[i].nome, 100, file);

        fgets(caves[i].descricao, 200, file);

        fscanf(file, "%d", &caves[i].delay_magic_wall_e_amoeba);
        fgetc(file);

        fscanf(file, "%d", &caves[i].pontos_por_diamantes);
        fgetc(file);

        fscanf(file, "%d", &caves[i].pontos_por_diamantes_extras);
        fgetc(file);

        fscanf(file, "%d %d %d %d %d", &caves[i].diamantes_necessarios[0], &caves[i].diamantes_necessarios[1], &caves[i].diamantes_necessarios[2], &caves[i].diamantes_necessarios[3], &caves[i].diamantes_necessarios[4]);
        fgetc(file);

        fscanf(file, "%d %d %d %d %d", &caves[i].tempo[0], &caves[i].tempo[1], &caves[i].tempo[2], &caves[i].tempo[3], &caves[i].tempo[4]);
        fgetc(file);

        fscanf(file, "%d", &caves[i].cor_background_1);
        fgetc(file);

        fscanf(file, "%d", &caves[i].cor_background_2);
        fgetc(file);

        fscanf(file, "%d", &caves[i].cor_foreground);
        fgetc(file);

        fscanf(file, "%d %d", &caves[i].largura, &caves[i].altura);
        fgetc(file);

        altura = caves[i].altura;
        largura = caves[i].largura;

        caves[i].data = malloc(altura * sizeof(unsigned char*));
        caves[i].data[0] = malloc(altura * largura * sizeof(unsigned char));
        if(caves[i].data == NULL || caves[i].data[0] == NULL)
            return 0;

        /* Ajusta o ponteiro das linhas. */
        for(l=1; l<altura; l++)
            caves[i].data[l] = caves[i].data[0] + (l * largura);

        /* Lê os chars da caverna. */
        for(l=0; l<altura; l++){
            for(c=0; c<largura; c++){
                fscanf(file, "%c", &caves[i].data[l][c]);
            }
            fgetc(file); /* Lê o \n. */
        }

        /* Transforma os chars nos IDs dos tiles. */
        for(l=0; l<altura; l++){
            for(c=0; c<largura; c++){
                switch (caves[i].data[l][c]){
                case 'X': caves[i].data[l][c] = PLAYER;     break;                
                case 'W': caves[i].data[l][c] = STEEL_WALL; break;
                case '.': caves[i].data[l][c] = DIRT;       break;                
                case 'd': caves[i].data[l][c] = DIAMOND;    break;
                case 'r': caves[i].data[l][c] = BOULDER;    break;                
                case 'P': caves[i].data[l][c] = EXIT_CLOSE;       break;
                case 'q': caves[i].data[l][c] = FIREFLY_UP;    break;                
                case 'w': caves[i].data[l][c] = WALL;       break;
                case 'B': caves[i].data[l][c] = BUTTERFLY_UP;  break;
                case 'b': caves[i].data[l][c] = BUTTERFLY_DOWN;  break;
                case 'a': caves[i].data[l][c] = AMOEBA;     break;
                case 'm': caves[i].data[l][c] = MAGIC_WALL; break;
                case ' ': caves[i].data[l][c] = EMPTY;      break;
                default:
                    fprintf(stderr, "CHAR %d DESCONHECIDO.\n", (int)caves[i].data[l][c]);
                    return 0;
                }
            }
        }
    }

    jogo->cave = caves;

    fclose(file);
    return 1;
}

bool inicializar_level(struct boulderdash *jogo, int level, ALLEGRO_DISPLAY *display){
    if(level < 0 || level > jogo->quantidade_de_cavernas-1)
        return 1;

    int i, j;
    int level_anterior = jogo->level;
    char windowTitle[100] = "Boulderdash!  ";

    if(jogo->level == level)
        jogo->pontuacao = 0;
    jogo->level = level;
    jogo->status = JOGANDO;
    jogo->easterEggFlag = -2;
    jogo->tempo = jogo->cave[level].tempo[0];  /* Meu boulderdash não tem suporte para dificuldades, então vou sempre carregar o mais facil. */
    jogo->diamantes_pegos = 0;

    /* Atualiza o titulo da janela para corresponder ao level. */
    strncat(windowTitle, jogo->cave[level].nome, 86);
    al_set_window_title(display ,windowTitle);

    /* Carrega o mapa da caverna.  */
    int altura = jogo->cave[level].altura;
    int largura = jogo->cave[level].largura;
    if(jogo->campo == NULL){
        jogo->campo = malloc(altura * sizeof(unsigned char*));
        jogo->campo[0] = malloc(altura * largura * sizeof(unsigned char));
        jogo->haveToUpdate = malloc(altura * sizeof(bool*));
        jogo->haveToUpdate[0] = malloc(altura * largura *sizeof(bool));
        jogo->HUD = malloc(largura * sizeof(unsigned char));
    }
    else if(jogo->cave[level_anterior].altura != altura || jogo->cave[level_anterior].largura != largura){
        jogo->campo = realloc(jogo->campo, altura * sizeof(unsigned char*));
        jogo->campo[0] = realloc(jogo->campo[0], altura * largura * sizeof(unsigned char));
        jogo->haveToUpdate = realloc(jogo->haveToUpdate, altura * sizeof(bool*));
        jogo->haveToUpdate[0] = realloc(jogo->haveToUpdate[0], altura * largura *sizeof(bool));
        jogo->HUD = realloc(jogo->HUD ,largura * sizeof(unsigned char));
    }

    if(jogo->campo == NULL || jogo->campo[0] == NULL || jogo->HUD == NULL || jogo->haveToUpdate == NULL || jogo->haveToUpdate[0] == NULL)
        return 0;

    for(i=1; i<altura; i++){
        jogo->campo[i] = jogo->campo[0] + (i * largura);
        jogo->haveToUpdate[i] = jogo->haveToUpdate[0] + (i * largura);
    }
    
    /* Inicialização dos vetores/matrizes */
    memcpy(jogo->campo[0], jogo->cave[level].data[0], altura * largura * sizeof(unsigned char));
    memset(jogo->haveToUpdate[0], true, altura * largura *sizeof(bool));
    memset(jogo->HUD, EMPTY, largura * sizeof(unsigned char));

    /* Inicializa os valores constantes do HUD */
    jogo->HUD[4] = (int)(jogo->cave[level].diamantes_necessarios[0] / 10) + ZERO;
    jogo->HUD[5] = (jogo->cave[level].diamantes_necessarios[0] % 10) + ZERO;
    jogo->HUD[6] = ICONE;

    /* Atualiza a posição do player. */
    for(i=0; i<altura; i++)
        for(j=0; j<largura; j++){
            if(jogo->campo[i][j] == PLAYER){
                jogo->player.x = j;
                jogo->player.y = i;
                return 1; /* Sucesso ao inicializar. */
            }
        }

    return 0; /* Falha ao inicializar. */
}

void must_init(bool test, const char *description){
    if(test) 
        return;

    fprintf(stderr ,"Erro!\nDescrição: %s\n", description);
    exit(1);
}

void updateGameStatus(boulderdash *jogo){
    int playerX = jogo->player.x;
    int playerY = jogo->player.y;

    if(isNotType(PLAYER, jogo, playerX, playerY))
        jogo->status = DERROTA;

    if(jogo->easterEggFlag == -1)
        explode(jogo, jogo->player.x, jogo->player.y, 40);
}

void victoryScene(boulderdash *jogo, ALLEGRO_BITMAP *tileSet, ALLEGRO_EVENT_QUEUE *queue){
    while(jogo->tempo > 0){
        jogo->pontuacao++;
        jogo->tempo--;
        updateHUD(jogo);
        renderGame(jogo, tileSet);
    }

    if(jogo->level < jogo->quantidade_de_cavernas)
        jogo->level++;

    jogo->status = CARREGANDO;
    al_flush_event_queue(queue);
}

void easterEgg(boulderdash *jogo, ALLEGRO_DISPLAY *display){
    int level = jogo->level;
    int x, y;

    for(y=0; y<jogo->cave[level].altura; y++){
        for(x=0; x<jogo->cave[level].largura; x++){
            if(isType(DIRT, jogo, x, y))
                jogo->campo[y][x] = DIAMOND;
        }
    }

    jogo->status = JOGANDO;
    jogo->easterEggFlag = 10;
    al_set_window_title(display, "Boulderdash! Será que valeu a pena apertar o botão?");
}

int main(void){
    boulderdash jogo;
    ALLEGRO_EVENT event;
    bool redraw = true;
    bool level_status, caves_status;
    int tutorialScroll = PARADO;
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, KEY_RELEASED, sizeof(key));
    srand(time(NULL));

    inicializar_allegro();
    inicializar_jogo(&jogo);
    
    caves_status = carregar_cavernas(&jogo ,DIR_PREFIX "/caves.data");
    must_init(caves_status, "Falha ao carregar o mapa das cavernas.\n");
    ALLEGRO_TIMER *fps_timer = al_create_timer(1.0 / FRAMERATE);
    must_init(fps_timer, "Falha ao inicializar fps_timer.\n");
    ALLEGRO_TIMER *tick_timer = al_create_timer(1.0 / TICKRATE);
    must_init(tick_timer, "Falha ao inicializar tick_timer.\n");
    ALLEGRO_TIMER *ingame_timer = al_create_timer(1.0);
    must_init(ingame_timer, "Falha ao inicializar ingame_timer.\n");
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(fps_timer, "Falha ao inicializar queue.\n");
    ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
    must_init(fps_timer, "Falha ao inicializar display.\n");
    ALLEGRO_FONT *font = al_load_font(DIR_PREFIX "/fonts/arial.ttf", 20, 0);
    must_init(font, "Falha ao carregar a fonte.\n");
    ALLEGRO_BITMAP *tileSet = al_load_bitmap(DIR_PREFIX "/sprites/sprites.png");
    must_init(tileSet, "Falha ao carregar sprites.\n");
    initSoundEffects();

    escalar_sprites(tileSet, SCALE);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(fps_timer));
    al_register_event_source(queue, al_get_timer_event_source(tick_timer));
    al_register_event_source(queue, al_get_timer_event_source(ingame_timer));

    level_status = inicializar_level(&jogo, 0, display);
    must_init(level_status, "Falha ao carregar campo.");
    al_start_timer(fps_timer);
    al_start_timer(tick_timer);
    al_start_timer(ingame_timer);
    jogo.status = JOGANDO;
    do{
        al_wait_for_event(queue, &event);

       switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                if(event.timer.source == fps_timer){
                    if(jogo.status != PAUSADO)
                        redraw = true;
                }
                else if(event.timer.source == ingame_timer){
                    #if DEBUG == 0
                    if(jogo.tempo > 0 && jogo.status != PAUSADO)
                        jogo.tempo--;
                    #endif
                    if(jogo.status != -2){
                        jogo.easterEggFlag--;
                    }
                }
                else if(event.timer.source == tick_timer){
                    switch(jogo.status){
                        case JOGANDO: atualizar_jogo(&jogo); break;
                        case DERROTA: atualizar_jogo(&jogo); break; /* WIP */
                        case VITORIA: victoryScene(&jogo, tileSet, queue);   break;
                        case PAUSADO: /* ESPERANDO ALGUM BOTÃO SER APERTADO */ break;
                        case TUTORIAL: /* ESPERANDO H OU F1 SEREM APERTADOS */ break;
                        case CARREGANDO: must_init(inicializar_level(&jogo, jogo.level, display), "Falha ao carregar campo."); break;
                    }
                }

                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if(jogo.status == PAUSADO)
                    jogo.status = jogo.statusBeforePause;

                key[event.keyboard.keycode] = KEY_PRESSED;

                switch(event.keyboard.keycode){
                    case ALLEGRO_KEY_H:      
                    case ALLEGRO_KEY_F1:     
                        if(jogo.status != TUTORIAL){
                            jogo.statusBeforePause = jogo.status; 
                            jogo.status = TUTORIAL; /* Abre o tutorial. */
                        }
                        else
                            jogo.status = jogo.statusBeforePause; /* Fecha o tutorial. */
                        break;
                    case ALLEGRO_KEY_F2:
                        if(jogo.status == TUTORIAL)
                            easterEgg(&jogo, display);
                        break;
                    case ALLEGRO_KEY_RCTRL:  jogo.statusBeforePause = jogo.status; jogo.status = PAUSADO; break;
                    case ALLEGRO_KEY_ESCAPE: jogo.status = FIMJOGO; break;
                    case ALLEGRO_KEY_PGUP:   must_init(inicializar_level(&jogo, jogo.level+1, display), "Falha ao carregar campo."); break;
                    case ALLEGRO_KEY_PGDN:   must_init(inicializar_level(&jogo, jogo.level-1, display), "Falha ao carregar campo."); break;
                    case ALLEGRO_KEY_LEFT:   jogo.player.dir = ESQUERDA; jogo.player.last_dir = ESQUERDA; break;
                    case ALLEGRO_KEY_RIGHT:  jogo.player.dir = DIREITA;  jogo.player.last_dir = DIREITA;  break;
                    case ALLEGRO_KEY_UP:     jogo.player.dir = CIMA;  tutorialScroll = CIMA;  break;
                    case ALLEGRO_KEY_DOWN:   jogo.player.dir = BAIXO; tutorialScroll = BAIXO; break;
                }
                break;

            case ALLEGRO_EVENT_KEY_UP:
                key[event.keyboard.keycode] = KEY_RELEASED;
                if(key[ALLEGRO_KEY_UP])         {jogo.player.dir = CIMA;  tutorialScroll = CIMA; }
                else if(key[ALLEGRO_KEY_DOWN])  {jogo.player.dir = BAIXO; tutorialScroll = BAIXO;}
                else if(key[ALLEGRO_KEY_LEFT])   jogo.player.dir = ESQUERDA;
                else if(key[ALLEGRO_KEY_RIGHT])  jogo.player.dir = DIREITA;
                else                            {jogo.player.dir = PARADO; tutorialScroll = PARADO;}
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                jogo.status = FIMJOGO;
                break;
        }

        if(redraw){
            if(jogo.status == TUTORIAL)
                renderTutorial(&jogo, font, tutorialScroll, tileSet);
            else
                renderGame(&jogo, tileSet);

            redraw = false;
        }

    } while (jogo.status != FIMJOGO);

    /* Desaloca e destroi tudo */
    al_destroy_timer(fps_timer);
    al_destroy_timer(tick_timer);
    al_destroy_timer(ingame_timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(tileSet);
    al_destroy_font(font);
    destroySoundEffects();
    destruir_jogo(&jogo);

    return 0;
}