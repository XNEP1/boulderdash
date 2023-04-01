/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#ifndef BD_AUDIO_H
#define BD_AUDIO_H

/* DECLARAÇÕES IMCOMPLETAS DE DEPENDENCIAS */
typedef struct ALLEGRO_SAMPLE ALLEGRO_SAMPLE;
typedef struct ALLEGRO_SAMPLE_INSTANCE ALLEGRO_SAMPLE_INSTANCE;



enum {SOUND_EXPLOSION, DIGGING_1, DIGGING_2, DIGGING_3, DIGGING_4, DIGGING_5, DIGGING_6, DIGGING_7, DIGGING_8, DIGGING_9, DIGGING_10, DIGGING_11, SOUND_EXIT_OPEN, SOUND_QUANTIDADE} soundEffects;

/* =================================================================================================================================================== */
/* FUNCTIONS */
ALLEGRO_SAMPLE *getSample(int type);

void initSoundEffects(void);
void destroySoundEffects(void);

void playSoundDig(void);
void playSoundExplode(void);
void playSoundExitOpen(void);

#endif