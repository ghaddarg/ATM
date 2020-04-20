/* XXX: TODO: 
1. Add file save/load for NVM
2. Add Cmake
3. Add unit tests
4. Myabe use usb as bank cards??
5. Change PIN ability
6. Put LOCK into accuont that has been locked
7. Update nvm values of pin and balance before returning card

*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

#include "inc/atm.h"

const char * welcome_msg = "Welcome to ATM System.\nPlease choose one of the following operations:\n\
							1.\tWithdraw\n\
				 			2.\tDeposit\n\
							3.\tBalance Inquiry\n\
							4.\t Return Card\n";
							//5.\t Change PIN\n";

//const char file_name[] = "nvm_file.txt";

static double balance = 0.00;
static char pin[] = "0000";
static char account_num[ACCOUNT_NUM] = {'0', 'x', '8', 'A', '2', 'B', '\0'};

//XXX: TODO: Use LIBUSBP
bool card_inserted = true;
bool update_flag = false;

/********************************************************************************/
/*                               PRIVATE FUNCTIONS                              */
/********************************************************************************/
bool is_new_account(const char * file_name)
{
	//printf("Account %s\n", file_name);
	if (-1 == access(file_name, F_OK))
		return true;
	else
		return false;
}

bool is_pin_correct(const char * entered_pin)
{
	return PIN_SIZE == strlen(entered_pin);
}

/*void set_nvm(const char * file_name)
{
	FILE *f = fopen(file_name, "r");

	char * buf = NULL;
	size_t size = 0;
	char * curr_bal = NULL;

	while (-1 != getline(&buf, &size, f)) {

		printf("NOPE\n");
		if (NULL != strstr(&buf[0], "PIN"))
			memcpy(pin, &buf[5], sizeof(char) * 4);
		else if (NULL != strstr(buf, "Balance"))
			curr_bal = &buf[9];//strcpy(pin, &buf[9]);
	}

	if (curr_bal)
		balance = strtod(curr_bal, NULL);

	free(buf);
	fclose(f);
}*/

atm_status_t set_up_new_account(const char * file_name)
{
	printf("Please input your new PIN: \n");
	scanf("%s", pin);

	if (!is_pin_correct(pin)) {

		printf("Wrong PIN size. It has to be %d digits\n", PIN_SIZE);
		return ATM_FAILURE;
	}

	FILE * f = fopen(file_name, "w+");
	fprintf(f, "%s: %s\n%s: %lf\n", "PIN", pin, "Balance", atm_get_balance());
	fclose(f);

	return ATM_SUCCESS;
}

void get_nvm(const char * file_name)
{
	FILE *f = fopen(file_name, "r");

	char * buf = NULL;
	size_t size = 0;
	char * curr_bal = NULL;

	while (-1 != getline(&buf, &size, f)) {

		if (NULL != strstr(&buf[0], "PIN"))
			memcpy(pin, &buf[5], sizeof(char) * 4);
		else if (NULL != strstr(buf, "Balance"))
			curr_bal = &buf[9];//strcpy(pin, &buf[9]);
	}

	/* Update the balance */
	if (curr_bal)
		balance = strtod(curr_bal, NULL);

	free(buf);
	fclose(f);
}

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

bool is_card_inserted(void)
{
	//XXX: TODO: How to check if card has been inserted?
	//XXX: TODO: Use LIBUSBP
	return card_inserted;
}
/********************************************************************************/
/*                             OPERATION FUNCTIONS                              */
/********************************************************************************/
atm_status_t atm_withdraw(uint16_t amount)
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
	if (amount <= MAX_DEPOSITS) {

		printf("Please Put your cash in an envelope and deposit via slot\n");
		sleep(5);
		balance += (double)amount;
		printf("Your new balance is %lf\n", balance);
		return ATM_SUCCESS; 
	} else {

		printf("Invalid amount selected.\
			Please select amount between $%d.00 - $%d.00\n", MIN_DEPOSITS, MAX_DEPOSITS);
		return ATM_INVALID_AMOUNT;
	}
}

double atm_get_balance(void)
{
	return balance;
}
/********************************************************************************/
/*                          ATM FINITE STATE MACHINE                            */
/********************************************************************************/
static void atm_state_machine(void)
{
	atm_state_t next_state = ATM_STATE_IDLE;
	atm_status_t ret;

	while (1) {

		switch (next_state)
		{
			case ATM_STATE_IDLE:

				if (is_card_inserted())
					next_state = ATM_STATE_CARD_INSERTED;

				break;

			case ATM_STATE_CARD_INSERTED:
				
				// Get acount number && Load the nvm for said account
				//XXX: TODO: Use LIBUSBP

				ret = ATM_SUCCESS;
				
				/* Does file exist i.e. is this a new account?? */
				if (is_new_account(account_num))
					ret = set_up_new_account(account_num);
				else
					get_nvm(account_num);

				if (ATM_SUCCESS == ret)
					next_state = ATM_STATE_PIN_CHECK;

				break;

			case ATM_STATE_PIN_CHECK:
				
				if (ATM_SUCCESS == pin_check())
					next_state = ATM_STATE_OPERATIONS;
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

						if (amount > 0) {

							(void)atm_withdraw((uint16_t)amount);
							
							//update_flag |= FLAG_UPDATE_BALA;
							update_flag = true;
						} else {
							printf("INVALID Amount\n");
						}
						
						break;

					case ATM_DEPOSIT:

						printf("Please enter amount to be deposited. Max of $%d.00: ", MAX_DEPOSITS);
						scanf("%d", &amount);

						if (amount > 0) {

							(void)atm_deposit((uint16_t)amount);
							
							//update_flag |= FLAG_UPDATE_BALA;
							update_flag = true;
						} else {
							printf("INVALID Amount\n");
						}
						
						break;

					case ATM_INQUIRY:

						printf("Your current balance is %lf\n", atm_get_balance());

						break;

					case ATM_RETURN_CARD:

						next_state = ATM_STATE_RETURN_CARD;

						break;

					/*case ATM_CHANGE_PIN:

						printf("Please enter Your new PIN: ");

						char pin_tries[4] = "0000";
						scanf("%s", &pin_tries);

						if (!is_pin_correct(pin_tries)) {

							printf("Wrong PIN size. It has to be %d digits\n", PIN_SIZE);
							return ATM_FAILURE;
						} else {
							
							//update_flag |= FLAG_UPDATE_PINS;
							printf("PIN Updated")
						}

						break;*/

					default:

						printf("Invalid selection\n");

						break;
				}

				break;

			case ATM_STATE_RETURN_CARD:

				//XXX: TODO: 7. Update nvm values of pin and balance before returning card
				//if (update_flag)
				//	ret = update_account_nvm(account_num);
				
				printf("Thank you and have a good day\n");
				next_state = ATM_STATE_IDLE;

				break;
			
			case ATM_STATE_ACCOUNT_LOCK:
				
				printf("You have entered wrong PIN 3 times\nYou will be locked out of this account\nPlease visit nearest branch for help\n");
				
				//XXX: TODO: WHat to do here?
				card_inserted = false;
				next_state = ATM_STATE_IDLE;

				break;

			default:

				break;
		}
	}
}

/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
void atm_start(void)
{
	atm_state_machine();
}
