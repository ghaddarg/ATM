#ifndef ATM_DEF_H
#define ATM_DEF_H

#define MAX_DEPOSITS		10000
#define MIN_DEPOSITS		10

#define MIN_WITHDRAW		MIN_DEPOSITS
#define MAX_WITHDRAW		1000

//#define FLAG_UPDATE_PINS	0b00001111
//#define FLAG_UPDATE_BALA	0b11110000

/* This is usually 16 numbers i.e. 8 bytes 
 * but since we are using USB product ID 
 * then it is only 7 bytes */
#define ACCOUNT_NUM		7

#define PIN_SIZE		4

typedef enum {
	ATM_SUCCESS = 0,
	ATM_FAILURE,
	ATM_WRONG_PIN,
	ATM_INVALID_AMOUNT,
	ATM_INSUFF_FUNDS,
}atm_status_t;

enum {
	ATM_WITHDRAW = 0,
	ATM_DEPOSIT,
	ATM_INQUIRY,
	ATM_RETURN_CARD,
	ATM_CHANGE_PIN
};

typedef enum {
	ATM_STATE_IDLE = 0,
	ATM_STATE_CARD_INSERTED,
	ATM_STATE_PIN_CHECK,
	ATM_STATE_OPERATIONS,
	ATM_STATE_ACCOUNT_LOCK,
	ATM_STATE_RETURN_CARD
}atm_state_t;

#endif /* ATM_DEF_H */