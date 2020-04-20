#ifndef ATM_H
#define ATM_H

#include <stdint.h>
#include <stdbool.h>

#include "inc/atm_def.h"

//XXX: TODO: Make Private functions static and remove from header file

/********************************************************************************/
/*                               PRIVATE FUNCTIONS                              */
/********************************************************************************/

bool is_card_inserted(void);
bool is_new_account(const char * file_name);
atm_status_t set_up_new_account(const char * file_name);
void get_nvm(const char * file_name)
atm_status_t pin_check(void);

/********************************************************************************/
/*                             OPERATION FUNCTIONS                              */
/********************************************************************************/

atm_status_t atm_withdraw(uint16_t amount);
atm_status_t atm_deposit(uint16_t amount);
double atm_get_balance(void);

/********************************************************************************/
/*                                PUBLIC FUNCTIONS                              */
/********************************************************************************/
void atm_start(void);

#endif /* ATM_H */