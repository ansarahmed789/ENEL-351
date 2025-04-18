#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

#define IR_PIN         GPIO_IDR_IDR0    // PA0
#define BUTTON_PIN     GPIO_IDR_IDR5    // PB5

#define LED_IR_ON()     GPIOA->BSRR = GPIO_BSRR_BS4   // IR LED on PA4
#define LED_IR_OFF()    GPIOA->BRR  = GPIO_BRR_BR4

#define LED_US_ON()     GPIOA->BSRR = GPIO_BSRR_BS1   // Ultrasonic LED on PA1
#define LED_US_OFF()    GPIOA->BRR  = GPIO_BRR_BR1

#define BUZZER_ON()     TIM1->CCER |= TIM_CCER_CC1E
#define BUZZER_OFF()    TIM1->CCER &= ~TIM_CCER_CC1E

void GPIO_init(void);
void USART1_init(void);
void TIM1_PWM_Init(uint32_t freq_hz);
void TIM2_init(void);
uint32_t measure_distance(void);
void send_string(const char *str);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

int main(void) {
    SystemInit();
    GPIO_init();
    USART1_init();
    TIM1_PWM_Init(1000);
    TIM2_init();
    BUZZER_OFF();

    char msg[50];
    uint32_t distance = 0;
    uint8_t buzzer_active = 0;

    while (1) {
        // --- IR Sensor Check ---
        if ((GPIOA->IDR & IR_PIN) == 0) {
            LED_IR_ON();
            buzzer_active = 1;
        } else {
            LED_IR_OFF();
        }

        if (((GPIOB->IDR & BUTTON_PIN) == 0) && buzzer_active) {
            buzzer_active = 0;
            BUZZER_OFF();
        }

        if (buzzer_active) {
            BUZZER_ON();
        } else {
            BUZZER_OFF();
        }

        // --- Ultrasonic Sensor ---
        distance = measure_distance();

        if (distance > 10 && distance < 50) {
            LED_US_ON();
            sprintf(msg, "Echo received! Distance: %lu cm\r\n", distance);
        } else if (distance == 0) {
            LED_US_OFF();
            sprintf(msg, "Echo not received (timeout)\r\n");
        } else {
            LED_US_OFF();
            sprintf(msg, "Invalid distance: %lu cm\r\n", distance);
        }

        send_string(msg);
        delay_ms(1000);
    }
}

void GPIO_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOA->CRL |= GPIO_CRL_CNF0_0;

    GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOA->CRL |= GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1;

    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_CNF4);
    GPIOA->CRL |= GPIO_CRL_MODE4_0 | GPIO_CRL_MODE4_1;

    GPIOA->CRH &= ~(GPIO_CRH_MODE8 | GPIO_CRH_CNF8);
    GPIOA->CRH |= GPIO_CRH_MODE8_1 | GPIO_CRH_CNF8_1;

    GPIOB->CRL &= ~(GPIO_CRL_MODE5 | GPIO_CRL_CNF5);
    GPIOB->CRL |= GPIO_CRL_CNF5_1;
    GPIOB->ODR |= GPIO_ODR_ODR5;

    GPIOC->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
    GPIOC->CRL |= (GPIO_CRL_MODE1_0 | GPIO_CRL_MODE1_1);

    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOC->CRL |= GPIO_CRL_CNF0_0;

    GPIOA->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
    GPIOA->CRH |= (GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1);
    GPIOA->CRH |= GPIO_CRH_CNF9_1;

    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
}

void USART1_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->BRR = (468 << 4) | 12;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void TIM1_PWM_Init(uint32_t freq_hz) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    uint32_t prescaler = 71;
    uint32_t period = 1000000 / freq_hz;

    TIM1->PSC = prescaler;
    TIM1->ARR = period - 1;
    TIM1->CCR1 = period / 2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_ARPE;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM2_init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 72 - 1;
    TIM2->ARR = 0xFFFF;
    TIM2->CR1 |= TIM_CR1_CEN;
}

uint32_t measure_distance(void) {
    uint32_t start = 0, end = 0, timeout = 0;
    GPIOC->BSRR = GPIO_BSRR_BS1;
    for (volatile int i = 0; i < 80; i++);
    GPIOC->BRR = GPIO_BRR_BR1;

    timeout = 30000;
    while (!(GPIOC->IDR & GPIO_IDR_IDR0)) {
        if (--timeout == 0) return 0;
    }
    start = TIM2->CNT;

    timeout = 30000;
    while (GPIOC->IDR & GPIO_IDR_IDR0) {
        if (--timeout == 0) return 0;
    }
    end = TIM2->CNT;

    uint32_t duration = (end >= start) ? (end - start) : (0xFFFF - start + end);
    return duration / 58;
}

void send_string(const char *str) {
    while (*str) {
        while (!(USART1->SR & USART_SR_TXE));
        USART1->DR = *str++;
    }
}

void delay_us(uint32_t us) {
    for (uint32_t i = 0; i < us * 8; i++) __NOP();
}

void delay_ms(uint32_t ms) {
    while (ms--) delay_us(100);
}