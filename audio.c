/*  
 *  Trabalho 2 de Programação 2.
 *  Jogo Boulderdash.
 *  De Bruno Krügel.
 */

#include "boulderdash.h"
#include "audio.h"
#include <string.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define AUDIO_DIR_PREFIX "./resources/sounds/"

ALLEGRO_SAMPLE *samples[SOUND_QUANTIDADE];
ALLEGRO_SAMPLE_INSTANCE *instances[SOUND_QUANTIDADE];
ALLEGRO_VOICE *voice;
ALLEGRO_MIXER *soundEffectsMixer;

ALLEGRO_SAMPLE *getSample(int type){
    return samples[type];
}

ALLEGRO_SAMPLE_INSTANCE *getSampleInstance(int type){
    return instances[type];
}


void initSoundEffects(void){
    int i;

    /* Cria o voice */
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    must_init(voice, "Falha ao criar o voice.\n");

    /* Cria os mixers */
    soundEffectsMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    must_init(soundEffectsMixer, "Falha ao carregar mixer de sound effects.\n");
    al_attach_mixer_to_voice(soundEffectsMixer, voice);


    /* Carrega os samples de sons */
    samples[SOUND_EXPLOSION] = al_load_sample(AUDIO_DIR_PREFIX "explosion.wav");
    must_init(samples[SOUND_EXPLOSION], "Falha ao carregar audio de explosão em \"./resources/sounds/explosion.wav\".\n");

    samples[DIGGING_1] = al_load_sample(AUDIO_DIR_PREFIX "digging_1.wav");
    samples[DIGGING_2] = al_load_sample(AUDIO_DIR_PREFIX "digging_2.wav");
    samples[DIGGING_3] = al_load_sample(AUDIO_DIR_PREFIX "digging_3.wav");
    samples[DIGGING_4] = al_load_sample(AUDIO_DIR_PREFIX "digging_4.wav");
    samples[DIGGING_5] = al_load_sample(AUDIO_DIR_PREFIX "digging_5.wav");
    samples[DIGGING_6] = al_load_sample(AUDIO_DIR_PREFIX "digging_6.wav");
    samples[DIGGING_7] = al_load_sample(AUDIO_DIR_PREFIX "digging_7.wav");
    samples[DIGGING_8] = al_load_sample(AUDIO_DIR_PREFIX "digging_8.wav");
    samples[DIGGING_9] = al_load_sample(AUDIO_DIR_PREFIX "digging_9.wav");
    samples[DIGGING_10] = al_load_sample(AUDIO_DIR_PREFIX "digging_11.wav");
    samples[DIGGING_11] = al_load_sample(AUDIO_DIR_PREFIX "digging_11.wav");
    for(i=DIGGING_1; i<=DIGGING_11; i++)
        must_init(samples[i], "Falha ao carregar audio de cavar em \"./resources/sounds/digging_VARIANT.wav\".\n");

    samples[SOUND_EXIT_OPEN] = al_load_sample(AUDIO_DIR_PREFIX "exitOpen.wav");
    must_init(samples[SOUND_EXIT_OPEN], "Falha ao carregar audio de abrir a saída em \"./resources/sounds/exitOpen.wav\".\n");


    /* Cria as instancias de sons */
    instances[SOUND_EXPLOSION] = al_create_sample_instance(samples[SOUND_EXPLOSION]);
    must_init(instances[SOUND_EXPLOSION], "Falha ao carregar audio instance.\n");

    instances[DIGGING_1] = al_create_sample_instance(samples[DIGGING_1]);
    instances[DIGGING_2] = al_create_sample_instance(samples[DIGGING_2]);
    instances[DIGGING_3] = al_create_sample_instance(samples[DIGGING_3]);
    instances[DIGGING_4] = al_create_sample_instance(samples[DIGGING_4]);
    instances[DIGGING_5] = al_create_sample_instance(samples[DIGGING_5]);
    instances[DIGGING_6] = al_create_sample_instance(samples[DIGGING_6]);
    instances[DIGGING_7] = al_create_sample_instance(samples[DIGGING_7]);
    instances[DIGGING_8] = al_create_sample_instance(samples[DIGGING_8]);
    instances[DIGGING_9] = al_create_sample_instance(samples[DIGGING_9]);
    instances[DIGGING_10] = al_create_sample_instance(samples[DIGGING_10]);
    instances[DIGGING_11] = al_create_sample_instance(samples[DIGGING_11]);
    for(i=DIGGING_1; i<=DIGGING_11; i++)
        must_init(instances[i], "Falha ao carregar audio instance.\n");

    instances[SOUND_EXIT_OPEN] = al_create_sample_instance(samples[SOUND_EXIT_OPEN]);
    must_init(instances[SOUND_EXIT_OPEN], "Falha ao carregar audio instance.\n");

    /* 'attach' as instancias no mixer de efeitos sonoros */
    for(i=0; i<SOUND_QUANTIDADE; i++)
        al_attach_sample_instance_to_mixer(instances[i], soundEffectsMixer);
}

void destroySoundEffects(void){
    int i;
    for(i=0; i<SOUND_QUANTIDADE; i++){
        al_destroy_sample(samples[i]);
    }

    for(i=0; i<SOUND_QUANTIDADE; i++){
        al_destroy_sample_instance(instances[i]);
    }

    al_destroy_mixer(soundEffectsMixer);
}

void playSoundDig(void){
    int i;
    for(i=DIGGING_1; i<=DIGGING_11; i++)
        if(al_get_sample_instance_playing(instances[i]))
            return; /* Esse som já está tocando no momento. */

    int variant = rand() % 11; /* Pega uma das variantes de som de cavar.  */
    al_play_sample_instance(getSampleInstance(DIGGING_1 + variant));
}

void playSoundExplode(void){
    al_play_sample_instance(getSampleInstance(SOUND_EXPLOSION));
}

void playSoundExitOpen(void){
    al_play_sample_instance(getSampleInstance(SOUND_EXIT_OPEN));
}