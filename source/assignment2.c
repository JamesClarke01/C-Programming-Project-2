/*
Program Description:

This program is a security authentication program based on an access code. The program allows the user to perform the following
functions which are accessed through a menu:

1. The user can enter a 4 digit numerical code. Any invalid input will be rejected.

2. The user's code is encrypted by a sophisticated encryption algorithm. The function will then compare the user's encrypted code to the access code and display an
appropriate message for if they match or if they don't match.
Note: this function will return the user to the main menu if no code has been entered yet or the code is already encrypted

3. This function decrypts the user's code
Note: This function will return to the main menu if no code has been entered yet or if the code is not encrypted

4. This function displays how many times the user's code has correctly and incorrectly matched with the access code

5. This function exits the program, asking the user if they are sure they would like to quit first

Program Author: James Clarke

Completion Date: 07/03/2021
*/

#include <stdio.h>
#include <stdlib.h>  //included for the exit() function
#include <string.h>

#define CODESIZE 4  //defines the length of the code that can be entered

enum status{notEntered, enteredUnencrypted, enteredEncrypted};  //defines different states the code can be in (more human readable than using 0, 1 and 2)

struct userCodeStruct
{
    //this structure is used for storing the users code and the status of this code, a structure was used to reduce the amount of parameters to be passed to each function

    int code[CODESIZE];  //this structure member is a 1-D array that the user's code will be stored in when it is entered
    enum status status;  //this variable stores the status of the users code (notEntered = no code entered, enteredUnencrypted = code entered but not encrypted, enteredEncrypted = code entered & encrypted)
};

//function signatures
enum status codeEntry(struct userCodeStruct *userCode);  //function for menu option 1

enum status codeEncrypt(struct userCodeStruct *userCode, const int *access_code, int *successCount, int *incorrectCount);  //function for menu option 2

enum status codeDecrypt(struct userCodeStruct *userCode);  //function for menu option 3

void displayCounters(int *successCount, int *incorrectCount);  //function for menu option 4

void terminateProgram(void);  //function for menu option 5

int main(void)
{
    int menuNumber = 0;  //variable for the menu selection

    struct userCodeStruct userCode;  //structure variable containing the code array and the status of the code
    
    userCode.status = notEntered; //status of code set to "no code entered"

    const int access_code[CODESIZE] = {4,5,2,3};  //access code array

    int successCount = 0;  //for counting the amount of times the code has been compared successfully
    int incorrectCount = 0;  //for counting the amount of times the code has been compared incorrectly

    int firstLoop = 0;  //used for the "character entered" error message for the menu select input

    while(1)  //this while loop loops until the program is ended
    {
        switch (menuNumber) //switch statement for each option on the menu
        {
            case 0: //this switch case is ran first when the program first starts
            {
                //instructions and menu below
                printf("\n=========Authenitcation Program=========\n\
                \rPlease enter the number corresponding to the menu option you wish to select:\n\
                \r1. Enter any code\n\
                \r2. Encrypt code entered and verify if correct\n\
                \r3. Decrypt code\n\
                \r4. Display the number of times the encrypted code has matched the authorised code\n\
                \r5. Exit Program\n");

                scanf("%d", &menuNumber); //user enters desired menu option
                
                printf("\n");  //skips line

                firstLoop = 1;  //flag for making sure the "entered character" error message is only displayed once

                //while loop clears invalid input from the input buffer
                while(getchar() != '\n')
                {
                    if(firstLoop == 1)
                    {
                        //if statement ensures error message is only displayed during the first loop
                        printf("Character detected, please enter a number from 1-5!\n");
                        
                        menuNumber = 0;  //ensures no menu option gets selected
                    }
                    firstLoop = 0;  //sets the flag to 0 so the message won't be displayed more than once
                }  

                break;
            }

            case 1:
            {
                //runs if the user selects option 1
                
                userCode.status = codeEntry(&userCode); //function will return enteredUnencrypted if user successfully enteres a code

                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
                break;
            }

            case 2:
            {
                //runs if the user selects option 2

                userCode.status = codeEncrypt(&userCode, access_code, &successCount, &incorrectCount); //function will return enteredEncrypted if the code is successfully encrypted
                
                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
                break;
            }

            case 3:
            {
                //runs if user selects option 3

                userCode.status = codeDecrypt(&userCode);  //function will return enteredUnencrypted if the code is successfully decrypted

                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
                break;
            }

            case 4:
            {
                //runs if user selects option 4

                displayCounters(&successCount, &incorrectCount);  //function displays the counters passed into it

                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
                break;
            }

            case 5:
            {
                //runs if user selects option 5

                terminateProgram();  //function asks users if they are sure, then ends the program

                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
                break;
            }

            default:
            {
                //this will be run if an invalid input is entered during the menu select

                printf("Invalid option entered!\n");

                menuNumber = 0; //sets the switch statement to case 0 so the main menu displays when the loop goes back around
            }
        }
    }
    return 0;
}

//this function allows the user to enter a 4 digit code.
//Will return "enteredUnencrypted" if the code is entered successfully or will return the current code status if invalid input is encountered
//NOTE: this function will update the contents of the pointers passed into this array
enum status codeEntry(struct userCodeStruct *userCode)
{
    char codeString[CODESIZE + 1];  //string is allocated one more space than the code length for the null character
    int codeInt, i;

    printf("Please enter a 4 digit numerical code and press enter:\n");

    gets(codeString);  //gets a string from the user (gets was used instead of fgets because fgets stores the return character in the string which was unnesesary for my program)
    
    printf("\n");  //skips line

    if(strlen(codeString) > CODESIZE)
    {
        //runs if code is too long
        printf("Code too long!\n");
        
        return userCode->status;  //will exit the function and not update the code status
    }
    else if(strlen(codeString) < CODESIZE)
    {
        //runs if code is too short
        printf("Code too short!\n");

        return userCode->status;  //will exit the function and not update the code status
    }

    //for loop converts the input string into an array
    for(i = 0; i< CODESIZE; i++)
    {
        codeInt = *(codeString + i) - '0';  //turns the current character in the string into an integer

        if ((codeInt >= 0) && (codeInt <= 9)) //makes sure that only the integers 0-9 are accepted (any other characters will not be included as their ascii value will be out of the range of 0-9)
        {
            *(userCode->code + i) = codeInt;  //adds the current integer to the code array
        }
        else
        {
            //runs if any characters entered are not 0-9
            printf("Code Invalid!\n");
            return userCode->status;  //will exit the function and not update the code status
        }
    }

    printf("Entered code is: ");

    //outputs the entered code to the user
    for(i = 0; i<CODESIZE; i++)
    {
        printf("%d", *(userCode->code + i));
    }

    printf("\n");  //skips line

    return enteredUnencrypted;  //returns enteredUnencrypted if the code entry was successful, updating the code status
}

//This function encrypts the user code, compares it to the access code and outputs if it is correct or incorrect, and returns enteredEncrypted when completed successfully
//will return the current code status if an error occurs
//NOTE: this function will update the contents of the pointers passed into this array
enum status codeEncrypt(struct userCodeStruct *userCode, const int *access_code, int *successCount, int *incorrectCount)
{
    int t;  //temporary variable used for swapping numbers for decryption
    int correctCount = 0;  //counter that's used in comparing the two codes
    int i;

    //checks if the code is allowed to be encrypted
    if(userCode->status == notEntered)
    {
        //runs if no code has been entered yet
        printf("No code entered!\n");
        
        return userCode->status;  //will exit the function and not update the code status
    }
    else if (userCode->status == enteredEncrypted)
    {
        //runs if the code is already encrypted
        printf("Code is already encrypted!\n");

        return userCode->status;  //will exit the function and not update the code status
    }

    //swaps 1st num with 3rd num
    t = *(userCode->code + 2); //value of 3rd num stored in t

    *(userCode->code + 2) = *(userCode->code);  //value of 1st num stored in positon 3

    *(userCode->code) = t;  //value of 3rd num(t) stored in position 1

    //swaps the 2nd num with the 4th num
    t = *(userCode->code + 3);  //value of 4th num stored in t

    *(userCode->code + 3) = *(userCode->code + 1);  //value of 2nd num stored in positon 4

    *(userCode->code + 1) = t;  //value of 4th num(t) stored in position 2

    //code below adds 1 to each number
    for(i = 0; i<CODESIZE; i++)
    {
        *(userCode->code + i) += 1; //increments current number of the code by 1

        if (*(userCode->code + i) == 10)
        {
            //if the current number is equal to 10, set to 0
            *(userCode->code + i) = 0;
        }
    }

    printf("Your code is now encrypted!\n");

    //checking if code matches the access code
    for(i = 0; i<CODESIZE; i++)
    {   
        if(*(userCode->code + i) == *(access_code + i))
        {
            //count is incremented if the current item in userCode matches the current item in access_code
            correctCount += 1;
        }
    }

    if(correctCount == CODESIZE)
    {
        //runs if both codes match
        printf("Correct Code Entered!\n");
        *successCount += 1;
    }
    else
    {
        //runs if both codes do not match
        printf("Wrong Code Entered!\n");
        *incorrectCount += 1;
    }

    return enteredEncrypted;  //returns enteredEncrypted which updates the code status, meaning that the code is now encrypted
}

//This function will attempt to decrypt the current code and will return a status of "enteredUnencrypted" if successful or will return the current status if the code cannot be decrypted
//NOTE: this function will update the contents of the pointers passed into this array
enum status codeDecrypt(struct userCodeStruct *userCode)
{
    int t;  //temporary variable used for swapping numbers for decryption
    
    //checks if code is allowed be decrypted
    if(userCode->status == notEntered)
    {
        //runs if no code has been entered yet
        printf("No code entered!\n");

        return userCode->status;  //will exit the function and not update the code status
    }
    else if(userCode->status == enteredUnencrypted)
    {
        //runs if code is not encrypted
        printf("Code is not encrypted!\n");

        return userCode->status;  //will exit the function and not update the code status
    }

    //subracts 1 from each element in the code
    for(int i = 0; i<CODESIZE; i++)
    {
        *(userCode->code + i) -= 1; //decrements current number of the code by 1

        if (*(userCode->code + i) == -1)
        {
            //if the current number is equal to -1, set to 9
            *(userCode->code + i) = 9;
        }
    }

    //swaps 1st num with 3rd num
    t = *(userCode->code + 2); //value of 3rd num stored in t

    *(userCode->code + 2) = *(userCode->code);  //value of 1st num stored in positon 3

    *(userCode->code) = t;  //value of 3rd num(t) stored in positioni 1

    //swaps the 2nd num with the 4th num
    t = *(userCode->code + 3); //value of 4th num stored in t

    *(userCode->code + 3) = *(userCode->code + 1);//value of 2nd num stored in positon 4

    *(userCode->code + 1) = t;  //value of 4th num(t) - store it in position 2
    
    printf("You code is now decrypted!\n");

    return enteredUnencrypted;  //returns enteredUnencrypted which updates the codes status, meaning that the code is now unencrypted
}

//this function prints to the user the amount of times that the entered code has matched with the access code
void displayCounters(int *successCount, int *incorrectCount)
{
    printf("The code has been verified %d times correctly and %d times incorrectly.\n", *successCount, *incorrectCount);  //prints the amount of times the code and been entered successfully and incorrectly
    
    return;
}

//this function asks the user if they want to exit the program or not and exits the program if the user says yes or returns to the main menu if the user says no
void terminateProgram(void)
{
    char choice;  //used for the exit menu
    
    printf("Do you wish to exit the program? Enter 'y' to exit or 'n' to return to the main menu:\n");

    scanf("%c", &choice);  //user enters their choice (y for yes and n for no)

    printf("\n");  //skips line

    switch (choice)
    {
        case 'y':
        {
            //if the user wishes to leave the program
            printf("Thank you for using the Authentication Program, have a nice day!\n");

            exit(0);  //exits program
        }
        case 'n':
        {
            //if the user wishes to return to the menu
            printf("Returning to main menu...\n");

            return;  //returns to main menu
        }
        default:
        {
            while(getchar() != '\n');  //clears invalid characters from input buffer

            printf("Invalid option entered!\n");

            return;  //returns to main menu
        }
    }
}