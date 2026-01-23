/*
 * gd32_quadrature_decoder.cpp
 */

#undef NDEBUG

#include <cstdio>

#include "gd32.h"

#include "firmware/debug/debug_debug.h"

extern "C" {
void PWM_TIMERx_IRQHandler() {
	const auto nIntFlag = TIMER_INTF(PWM_TIMERx);

	if ((nIntFlag & TIMER_INT_FLAG_UP) == TIMER_INT_FLAG_UP) {
		if (TIMER_CTL0(PWM_TIMERx) & TIMER_CTL0_DIR) {
			puts("Up");
		} else {
			puts("Down");
		}
	}

	TIMER_INTF(PWM_TIMERx) = ~nIntFlag;
}
}

static void GpioConfig() {
	rcu_periph_clock_enable(PWM_CH0_RCU_GPIOx);
	rcu_periph_clock_enable(PWM_CH1_RCU_GPIOx);

#if defined (GPIO_INIT)
	rcu_periph_clock_enable(RCU_AF);
# if defined(PWM_TIMER_REMAP)
	gpio_pin_remap_config(PWM_TIMER_REMAP, ENABLE);
# endif
	gpio_init(PWM_CH0_GPIOx, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, PWM_CH0_GPIO_PINx | PWM_CH1_GPIO_PINx);
#else
	gpio_mode_set(PWM_CH0_GPIOx, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, PWM_CH0_GPIO_PINx | PWM_CH1_GPIO_PINx);
#endif
}

static void TimerConfig() {
	rcu_periph_clock_enable(PWM_RCU_TIMERx);

	timer_deinit(PWM_TIMERx);

	timer_parameter_struct timer_initpara;
	timer_initpara.prescaler         = 0;
	timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection  = TIMER_COUNTER_UP;
	timer_initpara.period            = 4; // Debounce ?
	timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(PWM_TIMERx, &timer_initpara);

	timer_quadrature_decoder_mode_config(PWM_TIMERx, TIMER_QUAD_DECODER_MODE2, TIMER_IC_POLARITY_BOTH_EDGE, TIMER_IC_POLARITY_BOTH_EDGE);

	timer_interrupt_disable(PWM_TIMERx, uint32_t(~0));
	timer_interrupt_enable(PWM_TIMERx, TIMER_INT_UP);
	timer_interrupt_flag_clear(PWM_TIMERx, uint32_t(~0));

	NVIC_SetPriority(PWM_TIMERx_IRQn, 2);
	NVIC_EnableIRQ(PWM_TIMERx_IRQn);

	timer_enable(PWM_TIMERx);
}

void gd32_quadrature_decoder_begin() {
	GpioConfig();
	TimerConfig();
}

static uint32_t s_nCount;

bool gd32_quadrature_decoder_read(uint32_t& nCount, uint32_t& nDirection) {
	const auto nTimerCounter = TIMER_CNT(PWM_TIMERx);

	if (nTimerCounter == s_nCount) {
		return false;
	}

	s_nCount = nTimerCounter;
	nCount = nTimerCounter;

	nDirection = TIMER_CTL0(PWM_TIMERx) & TIMER_CTL0_DIR;

	return true;
}
