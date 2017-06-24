#ifndef __INC_PIEZZO_H
#define __INC_PIEZZO_H

typedef struct tNote {
    unsigned int freq;
    int duration;
} piezzo_note_t, *piezzo_note_p;

typedef struct tMelody {
    uint32_t nb_notes;
    uint32_t current;
    piezzo_note_p notes;
} piezzo_melody_t, *piezzo_melody_p;

typedef void *piezzo_cb(void);

void play(piezzo_melody_p melody, piezzo_cb *callback);
void play_next(piezzo_melody_p melody);

#endif /* __INC_PIEZZO_H */
