#ifndef ATM_H
#define ATM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#include "inc/atm_def.h"

//XXX: TODO: Make Private functions static and remove from header file

/********************************************************************************/
/*                               ACCOUNT FUNCTIONS                              */
/********************************************************************************/
bool is_new_account(const char * file_name);
atm_status_t set_up_new_account(const char * file_name);
void lock_account( const char * file_name );
/********************************************************************************/
/*                                 PIN FUNCTIONS                                */
/********************************************************************************/
atm_status_t pin_check(void);
bool is_pin_correct(const char * entered_pin);
/********************************************************************************/
/*                               SET/GET FUNCTIONS                              */
/********************************************************************************/
void set_nvm(const char * file_name);
atm_status_t get_nvm(const char * file_name);
double atm_get_balance(void);
/********************************************************************************/
/*                             OPERATION FUNCTIONS                              */
/********************************************************************************/
atm_status_t atm_withdraw(uint16_t amount);
atm_status_t atm_deposit(uint16_t amount);
/********************************************************************************/
/*                                PUBLIC FUNCTIONS                              */
/********************************************************************************/
void atm_start(void);

#ifdef __cplusplus
}
#endif

#endif /* ATM_H */