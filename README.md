# ATM
This is a simple finite state machine ATM system that was designed. 
The design of the system is as close as possible to an actual ATM bank machine.
Instead of cards we are using usb to signify when a card has been entered.
I'm using libusb1.0 inorder to get access to connected USBs

#OS
This SW was designed on Mac OSX 10.13

#Dependecies
- libusb1.0: http://libusb.sourceforge.net/api-1.0/
- gtest: https://github.com/google/googletest

#Version:
This is version 1

#TODO:
1. gtests
2. Bug: with scanf when expecting an integer and we enter float it gets stuck in loop
Please enter your selection: 1587.09
choice 15
Invalid selection
Please choose one of the following operations:
							1.	Withdraw
				 			2.	Deposit
							3.	Balance Inquiry
							4.	Change PIN
							5.	Return Card

Please enter your selection: choice 87
Invalid selection
Please choose one of the following operations:
							1.	Withdraw
				 			2.	Deposit
							3.	Balance Inquiry
							4.	Change PIN
							5.	Return Card

Please enter your selection: choice 0
Invalid selection
Please choose one of the following operations:
							1.	Withdraw
				 			2.	Deposit
							3.	Balance Inquiry
							4.	Change PIN
							5.	Return Card

Please enter your selection: choice 0
Invalid selection
Please choose one of the following operations:
							1.	Withdraw
				 			2.	Deposit
							3.	Balance Inquiry
							4.	Change PIN
							5.	Return Card

Please enter your selection: choice 0
Invalid selection
Please choose one of the following operations:
							1.	Withdraw
				 			2.	Deposit
							3.	Balance Inquiry
							4.	Change PIN
							5.	Return Card

Please enter your selection: choice 0
