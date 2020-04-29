/* XXX: TODO: 
1. Add file save/load for NVM <DONE>
2. Add Cmake
3. Add unit tests <DONE>
4. Maybe use usb as bank cards <DONE>
5. Change PIN ability <DONE>
6. Put LOCK into accuont that has been locked <DONE>
7. Update nvm values of pin and balance before returning card <DONE>
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

#include "inc/atm.h"
#include "inc/atm_usb.h"

const char * welcome_msg = "Please choose one of the following operations:\n\
							1.\tWithdraw\n\
				 			2.\tDeposit\n\
							3.\tBalance Inquiry\n\
							4.\tChange PIN\n\
							5.\tReturn Card\n";

static double balance = 0.00;
static char pin[] = "0000";
static char account_num[ACCOUNT_NUM];

bool update_flag = false;
/********************************************************************************/
/*                                 PIN FUNCTIONS                                */
/********************************************************************************/
bool is_pin_correct( const char * entered_pin )
{
	return PIN_SIZE == strlen(entered_pin);
}

atm_status_t pin_check( void )
{
	char pin_tries[ PIN_SIZE ] = "0000";
	uint8_t tries = 3;

	while( tries-- ) {

		printf( "Please input your PIN: " );
		scanf( "%s", pin_tries );

		if ( !strncmp( pin_tries, pin, sizeof( pin_tries ) / sizeof( pin_tries[ 0 ] )))
			return ATM_SUCCESS;

		printf( "INVALID PIN\n" );
	}

	return ATM_WRONG_PIN;
}
/********************************************************************************/
/*                               ACCOUNT FUNCTIONS                              */
/********************************************************************************/
bool is_new_account( const char * file_name )
{
	if ( -1 == access( file_name, F_OK ) )
		return true;
	else
		return false;
}

atm_status_t set_up_new_account( const char * file_name )
{
	printf( "Please input your new PIN: \n" );
	scanf( "%s", pin );

	if ( !is_pin_correct( pin )) {

		printf( "Wrong PIN size. It has to be %d digits\n", PIN_SIZE );
		return ATM_FAILURE;
	}

	FILE * f = fopen( file_name, "w+" );
	fprintf( f, "%s: %s\n%s: %lf\n", "PIN", pin, "Balance", atm_get_balance() );
	fclose( f );

	printf( "Your new account has been set up.\n" );
	return ATM_SUCCESS;
}

void lock_account( const char * file_name )
{
	FILE * f = fopen( file_name, "w+" );
	fprintf( f, "LOCKED\n%s: %s\n%s: %lf\n", "PIN", pin, "Balance", atm_get_balance() );
	fclose( f );
}
/********************************************************************************/
/*                               SET/GET FUNCTIONS                              */
/********************************************************************************/
void set_nvm( const char * file_name )
{
	FILE * f = fopen( file_name, "w+" );
	fprintf( f, "%s: %s\n%s: %lf\n", "PIN", pin, "Balance", atm_get_balance() );
	fclose( f );
}

atm_status_t get_nvm( const char * file_name )
{
	atm_status_t ret = ATM_FAILURE;
	FILE *f = fopen( file_name, "r" );

	char * buf = NULL;
	size_t size = 0;
	char * curr_bal = NULL;

	while ( -1 != getline( &buf, &size, f ) ) {

		/* If Account Locked?? */
		if ( NULL != strstr(&buf[0], "LOCKED" )) {

			ret = ATM_ACC_LOCKED;
			goto out;
		}

		if ( NULL != strstr(&buf[0], "PIN" ))
			memcpy( pin, &buf[ 5 ], sizeof( char ) * PIN_SIZE );
		else if ( NULL != strstr( buf, "Balance" ))
			curr_bal = &buf[ 9 ];//strcpy(pin, &buf[9]);
	}

	/* Update the balance */
	if ( curr_bal )
		balance = strtod( curr_bal, NULL );

	ret = ATM_SUCCESS;

out:
	free( buf );
	fclose( f );
	return ret;
}

double atm_get_balance( void )
{
	return balance;
}
/********************************************************************************/
/*                             OPERATION FUNCTIONS                              */
/********************************************************************************/
atm_status_t atm_withdraw( uint16_t amount )
{					
	if ( amount < MIN_WITHDRAW || amount > MAX_WITHDRAW ) {

		printf( "Inavlid amount selected. Please select amount between $%d.00 - $%d.00\n", MIN_WITHDRAW, MAX_WITHDRAW );
		return ATM_INVALID_AMOUNT;
	} else if ( balance >= ( double )amount ) {

		balance -= ( double )amount;
		printf( "Your new balance is: %lf\n", balance );
		return ATM_SUCCESS;
	} else {

		printf( "You do not have enough funds\n" );
		return ATM_INSUFF_FUNDS;
	}
}

atm_status_t atm_deposit( uint16_t amount )
{
	if ( amount <= MAX_DEPOSITS ) {

		printf( "Please Put your cash in an envelope and deposit via slot\n" );
		sleep( 5 );
		balance += ( double )amount;
		printf( "Your new balance is %lf\n", balance );
		return ATM_SUCCESS; 
	} else {

		printf( "Invalid amount selected. Please select amount between $%d.00 - $%d.00\n", MIN_DEPOSITS, MAX_DEPOSITS );
		return ATM_INVALID_AMOUNT;
	}
}
/********************************************************************************/
/*                          ATM FINITE STATE MACHINE                            */
/********************************************************************************/
static void atm_state_machine( void )
{
	atm_state_t next_state = ATM_STATE_IDLE;
	atm_status_t ret;
	uint8_t welcome_flag = 0;

	while ( true ) {

		switch ( next_state )
		{
			case ATM_STATE_IDLE:

				if ( !welcome_flag ) {

					printf( "Welcome to ATM System.\nPlease insert your card\n" );
					welcome_flag = 1;
				}

				if ( is_card_inserted() )
					next_state = ATM_STATE_CARD_INSERTED;

				break;

			case ATM_STATE_CARD_INSERTED:

				welcome_flag = 0;
				
				/* Get account number && Load the nvm for said account */
				if ( get_account_info( account_num ) <= 0 ) {

					printf( "\nError retreiving account number\n" );
					next_state = ATM_STATE_RETURN_CARD;
					break;
				}

				ret = ATM_SUCCESS;
				
				/* Does file exist i.e. is this a new account?? */
				if ( is_new_account( account_num ) ) {

					ret = set_up_new_account( account_num );
					if ( ATM_SUCCESS == ret )
						next_state = ATM_STATE_OPERATIONS;

				} else {

					ret = get_nvm( account_num );
					if ( ATM_SUCCESS == ret ) {	

						next_state = ATM_STATE_PIN_CHECK;
					} else if ( ATM_ACC_LOCKED == ret ) {

						printf( "Your Account is locked. Please visit nearest branch\n" );
						next_state = ATM_STATE_RETURN_CARD;
					}
				}

				break;

			case ATM_STATE_PIN_CHECK:
				
				if (ATM_SUCCESS == pin_check())
					next_state = ATM_STATE_OPERATIONS;
				else
					next_state = ATM_STATE_ACCOUNT_LOCK;

				break;

			case ATM_STATE_OPERATIONS:
				
				printf( "%s\nPlease enter your selection: ", welcome_msg );
				uint8_t choice = 10;
				int amount = 0;

				/* In case of error clear what is left in buffer, the * means only match and discard */
				if ( !scanf("%2" SCNu8, &choice) )
					scanf("%*[^\n]");

				//int c; while((c = getchar()) != '\n' && c != EOF);
				printf( "choice %d\n", choice );
				
				
				switch ( --choice ) {

					case ATM_WITHDRAW:

						printf( "Please input amount to withdraw from $%d.00 - $%d.00: ", MIN_WITHDRAW, MAX_WITHDRAW );
						if ( !scanf( "%d", &amount ) ) {

							scanf("%*[^\n]");
							amount = 0;
						}

						if ( amount > 0 ) {

							( void )atm_withdraw( ( uint16_t )amount );
							
							//update_flag |= FLAG_UPDATE_BALA;
							update_flag = true;
						} else {
							printf( "INVALID Amount\n" );
						}
						
						break;

					case ATM_DEPOSIT:

						printf( "Please enter amount to be deposited. Max of $%d.00: ", MAX_DEPOSITS );
						//scanf("%d", &amount);
						char buf[ 7 ];
						memset( buf, sizeof( buf ), 0 );

						while ( fgets( buf, sizeof( buf ), stdin )) {

							if ( -1 != sscanf( buf, "%d", &amount ))
								break;
						}

						printf( "amount %d\n", amount );

						if ( amount > 0 ) {

							( void )atm_deposit( ( uint16_t )amount );
							
							//update_flag |= FLAG_UPDATE_BALA;
							update_flag = true;
						} else {
							printf( "INVALID Amount\n" );
						}
						
						break;

					case ATM_INQUIRY:

						printf( "Your current balance is %lf\n", atm_get_balance() );

						break;

					case ATM_RETURN_CARD:

						next_state = ATM_STATE_RETURN_CARD;

						break;

					case ATM_CHANGE_PIN:

						printf( "Please enter Your new PIN: " );

						char pin_tries[] = "0000";
						scanf( "%s", pin_tries );

						//XXX: TODO: Is PIN same as old one???

						if (!is_pin_correct(pin_tries)) {

							printf( "Wrong PIN size. It has to be %d digits\n", PIN_SIZE );

						} else {
							
							//update_flag |= FLAG_UPDATE_PINS;
							printf( "PIN Updated\n" );
							memcpy( pin, pin_tries, PIN_SIZE );
							update_flag = true;
						}

						break;

					default:

						printf( "Invalid selection\n" );

						break;
				}

				break;

			case ATM_STATE_RETURN_CARD:

				if ( update_flag )
					set_nvm( account_num );

				update_flag = false;
				
				printf( "Please Take your card\n" );

				/* Block until usb is removed */
				while ( !is_usb_removed() );

				printf( "Thank you and have a good day\n" );
				next_state = ATM_STATE_IDLE;

				break;
			
			case ATM_STATE_ACCOUNT_LOCK:
				
				printf( "You have entered wrong PIN 3 times\n");
				printf("You will be locked out of this account\nPlease visit nearest branch for help\n" );
				
				lock_account( account_num );
				next_state = ATM_STATE_RETURN_CARD;

				break;

			default:

				break;
		}
	}
}
/********************************************************************************/
/*                               PUBLIC FUNCTIONS                               */
/********************************************************************************/
void atm_start( void )
{
	atm_state_machine();
}
