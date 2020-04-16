#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "inc/atm.h"

const char * welcome_msg = "Welcome to ATM System.\nPlease choose one of the following operations:\n\
							1.\tWithdraw\n\
				 			2.\tDeposit\n\
							3.\tBalance Inquiry\n\
							4.\t Return Card\n";

//XXX: TODO: Write these values to a file and read it from them eerytime
static double balance = 1000.00;
static char * pin = "0456";

bool card_inserted = true;

/********************************************************************************/
/*                               PRIVATE FUNCTIONS                              */
/********************************************************************************/
atm_status_t pin_check(void)
{
	char pin_tries[4] = "0000";
	uint8_t tries = 3;

	while(tries--) {

		printf("Please input your PIN: \n");
		scanf("%s", pin_tries);

		if (!strncmp(pin_tries, pin, sizeof(pin_tries) / sizeof(pin_tries[0])))
			return ATM_SUCCESS;

		printf("INVALID PIN\n");
	}

	return ATM_WRONG_PIN;
}

atm_status_t atm_withdraw(int amount)
{					
	if (amount < MIN_WITHDRAW || amount > MAX_WITHDRAW) {

		printf("Inavlid amount selected.\
			Please select amount between $%d.00 - $%d.00\n", MIN_WITHDRAW, MAX_WITHDRAW);
		return ATM_INVALID_AMOUNT;
	} else if (balance >= (double)amount) {

		balance -= (double)amount;
		printf("Your new balance is: %lf\n", balance);
		return ATM_SUCCESS;
	} else {

		printf("You do not have enough funds\n");
		return ATM_INSUFF_FUNDS;
	}
}

atm_status_t atm_deposit(uint16_t amount)
{
	if (amount > 0 && amount <= MAX_DEPOSIT) {

		printf("Please Put your cash in an envelope and deposit via slot\n");
		sleep(5);
		balance += (double)amount;
		printf("Your new balance is %lf\n", balance);
		return ATM_SUCCESS; 
	} else {

		printf("Invalid amount selected.\
			Please select amount between $%d.00 - $%d.00\n", MIN_DEPOSIT, MAX_DEPOSIT);
		return ATM_INVALID_AMOUNT;
	}
}

double atm_get_balance(void)
{
	return balance;
}

bool is_card_inserted(void)
{
	//XXX: TODO: How to check if card has been inserted?
	return card_inserted;
}

static void atm_state_machine(void)
{
	//int amount = 0;
	//uint8_t choice;
	atm_state_t next_state = ATM_STATE_IDLE;

	while (1) {

		switch (next_state)
		{
			case ATM_STATE_IDLE:

				if (is_card_inserted())
					next_state = ATM_STATE_CARD_INSERTED;

				break;

			case ATM_STATE_CARD_INSERTED:
				
				if (ATM_SUCCESS == pin_check())
					next_state = ATM_STATE_PIN_CHECK;
				else
					next_state = ATM_STATE_ACCOUNT_LOCK;

				break;

			case ATM_STATE_OPERATIONS:
				
				printf("%s\nPlease enter your selection: ", welcome_msg);
				uint8_t choice = 0;
				scanf("%2" SCNu8, &choice);
				int amount = 0;
				
				switch (--choice) {

					case ATM_WITHDRAW:

						printf("Please input amount to withdraw from $%d.00 - $%d.00: ", MIN_WITHDRAW, MAX_WITHDRAW);
						scanf("%d", &amount);

						(void)atm_withdraw(amount);
						
						break;

					case ATM_DEPOSIT:

						printf("Please enter amount to be deposited. Max of $%d.00: ", MAX_DEPOSIT);
						scanf("%d", &amount);

						(void)atm_deposit((uint16_t)amount);
						
						break;

					case ATM_INQUIRY:

						printf("Your current balance is %lf\n", atm_get_balance());

						break;

					case ATM_RETURN_CARD:

						next_state = ATM_STATE_RETURN_CARD;

						break;

					default:

						printf("Invalid selection\n");

						break;
				}

				break;

			case ATM_STATE_RETURN_CARD:
				
				printf("Thank you and have a good day\n");
				next_state = ATM_STATE_IDLE;

				break;
			
			case ATM_STATE_ACCOUNT_LOCK:
				
				printf("You have entered wrong PIN 3 times\nYou will be locked out of this account for the day\nPlease visit nearest branch for help\n");
				
				//XXX: TODO: WHat to do here?
				card_inserted = false;
				next_state = ATM_STATE_IDLE;

				break;

			default:

				break;
		}
	}

out:
	return;
}

/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
void atm_start(void)
{
	atm_state_machine();

}
