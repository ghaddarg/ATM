#include <stdio.h>
//#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "inc/atm_def.h"


#define MAX_DEPOSIT		10000
#define MIN_DEPOSIT		10
#define MAX_WITHDRAW	1000
#define MIN_WITHDRAW	MIN_DEPOSIT

const char * welcome_msg = "Welcome to ATM System.\nPlease choose one of the following operations:\n\
							1.\tWithdraw\n\
				 			2.\tDeposit\n\
							3.\tBalance Inquiry\n";

static double balance = 1000.0;

/********************************************************************************/
/*                               PRIVATE FUNCTIONS                              */
/********************************************************************************/

static void atm_state_machine(uint8_t operation)
{
	int amount = 0;

	switch (operation)
	{
		case ATM_WITHDRAW:
			
			//int amount = 0;
			printf("Please input amount to withdraw from $%d.00 - $%d.00: ", MIN_WITHDRAW, MAX_WITHDRAW);
			scanf("%d", &amount);
			
			if (amount < MIN_WITHDRAW || amount > MAX_WITHDRAW) {
				printf("Inavlid amount selected. Please select amount between $%d.00 - $%d.00\n", MIN_WITHDRAW, MAX_WITHDRAW);
			} else if (balance >= (double)amount) {
				balance -= (double)amount;
				printf("Your new balance is: %lf\n", balance);
			} else {
				printf("You do not have enough funds\n");
			}
			break;

		case ATM_DEPOSIT:

			
			printf("Please enter amount to be deposited. Max of $10,000.00: ");
			scanf("%d", &amount);

			if (amount > 0 && amount <= MAX_DEPOSIT) {
				printf("Please Put your cash in an envelope and deposit via slot\n");
				sleep(5);
				balance += (double)amount;
				printf("Your new balance is %lf\n", balance);
			} else {
				printf("Invalid amount selected. Please select amount between $%d.00 - $%d.00\n", MIN_DEPOSIT, MAX_DEPOSIT);
			}
			
			break;

		case ATM_INQUIRY:

			printf("Your current balance is %lf\n", balance);

			break;
		default:

			printf("Invalid selection\n");

			break;
	}
}

/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
void atm_start(void)
{
	int operation;

	//Get your pin first

	printf("%s\nPlease enter your selection: ", welcome_msg);
	scanf("%d", &operation);
	operation--;

	if (operation < ATM_WITHDRAW || operation > ATM_INQUIRY) {
		printf("Invalid selection. Goodbye\n");
		return;
	}

	atm_state_machine(operation);

}

