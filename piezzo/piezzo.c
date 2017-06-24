/* Quick'n'dirty piezzo driver */
#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "piezzo.h"

#define BUZZ_PIN 17
#define BUZZ_TIMER  NRF_TIMER2

static piezzo_melody_p mp_melody;
static piezzo_cb *m_callback;
uint32_t ticks;
uint32_t period;

void play_note(piezzo_note_p note) {
    ticks = (62500*note->duration)/1000;
    period = 0;

    if (note->freq > 0)
        period = (62500/(2*note->freq));
    else
        period = ticks+1;

    /* Setup duration timer. */
    /*
    NRF_TIMER1->TASKS_STOP = 1;
    NRF_TIMER1->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
    NRF_TIMER1->TASKS_CLEAR = 1;               // clear the task first to be usable for later
    NRF_TIMER1->PRESCALER = 8;                             //Set prescaler. Higher number gives slower timer. Prescaler = 0 gives 16MHz timer
    NRF_TIMER1->BITMODE = TIMER_BITMODE_BITMODE_24Bit;		 //Set counter to 16 bit resolution
    NRF_TIMER1->CC[0] = ticks;                             //Set value for TIMER2 compare register 0
    */
    nrf_gpio_pin_clear(BUZZ_PIN);

    /* Setup note frequency timer (pwm) */
    NRF_TIMER2->TASKS_STOP = 1;
    NRF_TIMER2->MODE = TIMER_MODE_MODE_Timer;  // Set the timer in Counter Mode
    NRF_TIMER2->TASKS_CLEAR = 1;               // clear the task first to be usable for later
    NRF_TIMER2->PRESCALER = 8;                             //Set prescaler. Higher number gives slower timer. Prescaler = 0 gives 16MHz timer
    NRF_TIMER2->BITMODE = TIMER_BITMODE_BITMODE_24Bit;		 //Set counter to 16 bit resolution
    NRF_TIMER2->CC[0] = period;                             //Set value for TIMER2 compare register 0
    NRF_TIMER2->CC[1] = ticks;

    /* Start both timers. */
    //NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    NRF_TIMER2->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos) | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);
    //NVIC_EnableIRQ(TIMER2_IRQn);

    //NRF_TIMER2->TASKS_START = 1;               // Start TIMER2
    NRF_TIMER2->TASKS_START = 1;               // Start TIMER2
}

void play_next(piezzo_melody_p melody) {
    if (++(melody->current) < melody->nb_notes) {
        play_note(&melody->notes[melody->current]);
    } else {
        m_callback();
    }
}


/** TIMER2 peripheral interrupt handler. This interrupt handler is called whenever there it a TIMER2 interrupt
 */
 /*
void TIMER2_IRQHandler(void)
{
    if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
    {
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;	       //Clear compare register 0 event
        //NRF_TIMER2->TASKS_CLEAR = 1;
        NRF_TIMER2->CC[0] += period;
        nrf_gpio_pin_toggle(BUZZ_PIN);         //Set LED
    }
}*/

void TIMER2_IRQHandler(void) {
    if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE0_Msk) != 0))
    {
        NRF_TIMER2->CC[0] += period;
        NRF_TIMER2->EVENTS_COMPARE[0] = 0;	       //Clear compare register 0 event
        nrf_gpio_pin_toggle(BUZZ_PIN);         //Set LED
    }
    if ((NRF_TIMER2->EVENTS_COMPARE[1] != 0) && ((NRF_TIMER2->INTENSET & TIMER_INTENSET_COMPARE1_Msk) != 0))
    {
        NRF_TIMER2->EVENTS_COMPARE[1] = 0;
        NRF_TIMER2->TASKS_STOP = 1;
        NRF_TIMER2->TASKS_CLEAR = 1;
        nrf_gpio_pin_clear(BUZZ_PIN);

        /* Play next note. */
        play_next(mp_melody);
    }
}


void play(piezzo_melody_p melody, piezzo_cb *callback) {
    NVIC_EnableIRQ(TIMER2_IRQn);
    NVIC_SetPriority(TIMER2_IRQn, 0);

    /* Save current melody. */
    mp_melody = melody;

    m_callback = callback;

    /* Play our first note. */
    if ((melody->nb_notes > 0) && (melody->notes != NULL)) {
        play_note(&melody->notes[0]);
    }
}
