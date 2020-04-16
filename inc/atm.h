#ifndef ATM_H
#define ATM_H

#include <stdint.h>
#include <stdbool.h>

#include "inc/atm_def.h"

atm_status_t pin_check(void);

atm_status_t atm_withdraw(int amount);

atm_status_t atm_deposit(uint16_t amount);

double atm_get_balance(void);

bool is_card_inserted(void);

void atm_start(void);

#endif /* ATM_H */