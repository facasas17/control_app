/*!
 * EasyNextionLibrary.cpp - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 * Adapted for ESP-IDF by Fabiola de las Casas < fabioladelascasas@gmail.com >
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "EasyNextionLibrary.h"
#include "callTriggers.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern uint8_t global_cmd;
extern uint8_t global_len;
extern easyNex_t myNextion;
/*******************************************************************************
 * Code - Private
 ******************************************************************************/


/*******************************************************************************
 * Code - Public
 ******************************************************************************/
void EasyNex_readCommand(void)
{
    char data[24];

    switch(global_cmd)
    {
        case 'P': /*or <case 0x50:>  If 'P' matches, we have the command group "Page". 
               *The next byte is the page <Id> according to our protocol.
               *
               * We have write in every page's "preinitialize page" the command
               *  printh 23 02 50 xx (where xx is the page id in HEX, 00 for 0, 01 for 1, etc).
               * <<<<Every event written on Nextion's pages preinitialize page event will run every time the page is Loaded>>>>
               *  it is importand to let the Arduino "Know" when and which Page change.
               */
            myNextion.lastCurrentPageId = myNextion.currentPageId;
            UART_ReadData(myNextion.uart_port, data, 1);
            myNextion.currentPageId = data[0];                 
        break;
      
      
        case 'T':   /* Or <case 0x54:>  If 'T' matches, we have the command group "Trigger". 
                 * The next byte is the trigger <Id> according to our protocol.
                 *
                 * We have to write in a Touch Event on Nextion the following: < printh 23 02 54 xx >
                 * (where xx is the trigger id in HEX, 01 for 1, 02 for 2, ... 0A for 10 etc).
                 * With the switch(){case}, we call a predefined void with prefixed names
                 * as we have declare them on trigger.cpp file. Starting from trigger0()......up to trigger50()
                 * The maximum number of predefined void is 255
                 * We declare the trigger() at this file: trigger.cpp
                 * 
                 * You can use the trigger() in your code as a function, 
                 * in order to write any kind of code and run it, by sending the < printh > command needed,
                 * from a Touch event on Nextion, such as pressing a button. Example:
                 */
                /**                   void trigger0(){
                                        digitalWrite(13, HIGH); // sets the digital pin 13 on
                                        delay(1000);            // waits for a second
                                        digitalWrite(13, LOW);  // sets the digital pin 13 off
                                      }
                 */
                /* In Touch Press Event of a button on Nextion write <printh 23 02 54 01>
                 * Every time we press the Button the command <printh 23 02 54 01> will be sent over Serial
                 * Then, the library will call the void trigger1()
                 * the code inside the trigger1() will run once ...
                 */
            EasyNex_callTriggerFunction(); // We made a separate file callTriggers.cpp for this function, 
                             // as it would be too long to fit in here, because there are
                             // 50 predefined cases for the triggers. 
        break;
    
        default:
            myNextion.cmdGroup = global_cmd;  // stored in the public variable cmdGroup for later use in the main code
            myNextion.cmdLength = global_len;  // stored in the public variable cmdLength for later use in the main code
            easyNexReadCustomCommand();
                    
        break;
               
            /*   More for custom protocol and commands https://seithan.com/Easy-Nextion-Library/Custom-Protocol/
               
      easyNexReadCustomCommand() has a weak attribute and will be created only when user
      declare this function on the main code
      More for custom protocol and commands https://seithan.com/Easy-Nextion-Library/Custom-Protocol/
      our commands will have this format: <#> <len> <cmd> <id> <id2>
      and we must send them from Nextion as HEX with the printh command
      like: printh 23 03 4C 01 01

      <#> start marker, declares that a command is followed
      <len> declares the number of bytes that will follow
      <cmd> declares the task of the command or command group
      <id> declares the properties of the command
      <id2> a second property of the command
      
      When we send a custom command with the above format, the function NextionListen() will capture the start marker # and the len (first 2 bytes)
      and it will wait until all the bytes of the command, as we have declared with the len byte, arrive to the Serial buffer and inside the timeout limits.
      
      After that, the function will read the next byte, which is the command group and the function readCommand() takes over and through a switch command
      tries to match the _cmd variable that holds the command group value with the statements of the cases.
      
      If we do NOT have a match with the predefined, cmd of P for page and T for triggers, it will continue to the default where we store the _cmd and _len to the public variables
      cmdGroup and cmdLenght as we are going to need access to them from the main code in the next step.
      
      Next we call the the easyNexReadCustomCommand() with the precondition and ONLY if we have declared the function in the main code.
      
      From this point we can handle the assign of cmdGroup and IDs from the easyNexReadCustomCommand() in the user code, where we can go on with a switch case
      for the cmdGroup, the one that we have stored the _cmd for public use and we can call it with myObject.cmdGroup. This is why we made cmdGroup a public variable.
                     */
    }
}