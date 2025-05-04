

#ifndef TIM_H_
#define TIM_H_

#define SR_UIF (1U << 0)
#define SR_CC1IF (1U << 1)

void tim2_pa5_init(void);
void pa6_init_timer(void);
#endif