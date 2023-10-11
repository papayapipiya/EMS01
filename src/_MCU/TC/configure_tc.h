/*
 * configure_tc.h
 *
 * Created: 2021/4/19 下午 03:48:22
 *  Author: willchen
 */ 


#ifndef CONFIGURE_TC_H_
#define CONFIGURE_TC_H_

//! [module_inst]
struct	tc_module	tc_instance1;
struct	tc_module	tc_instance2;
struct	tc_module	tc_instance3;
struct	tc_module	tc_instance4;
#define MyTimer01	tc_instance1
#define MyTimer02	tc_instance2
#define MyTimer03	tc_instance3
#define MyTimer04	tc_instance4


void tc_callback_to_to_period_200us(struct tc_module *const module_inst);
void tc_callback_to_to_period_51200us(struct tc_module *const module_inst);
void configure_tc(void);
void configure_tc_callbacks(void);
void timer_configure(void);


#endif /* CONFIGURE_TC_H_ */