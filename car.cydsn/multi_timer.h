/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>  

#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include "stdint.h"

typedef struct Timer {
    uint32_t timeout;
    uint32_t repeat;
    void (*timeout_cb)(void);
    struct Timer* next;
}Timer;

#ifdef __cplusplus  
extern "C" {  
#endif  

void timer_init(struct Timer* handle, void(*timeout_cb)(), uint32_t timeout, uint32_t repeat);
int  timer_start(struct Timer* handle);
void timer_stop(struct Timer* handle);
void timer_ticks(void);
void timer_loop(void);

// void timer_again(struct Timer* handle);
// void timer_set_repeat(struct Timer* handle, uint32_t repeat);

#ifdef __cplusplus
} 
#endif

#endif
/* [] END OF FILE */
