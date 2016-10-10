#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_CHARACTERS_LINE  80 //Max characters in a line
#define HISTORY_SIZE  5         //Max number of history items

char g_history[HISTORY_SIZE][MAX_CHARACTERS_LINE];    //stores the commands here(history)
int g_command_count = 0;                            //helper used inside displayHistory()

/**
 * Adds a command to the history.
 * @post The command has been added to the g_history[] data structure.
 */
int addCommandToHistory(char addCommand[], int current) //void to int
{
  //TODO: add the command into the history here.
  int temp;
  if (current == 0)
    exit(0);  
    if (current < 0)
    {
        perror("error reading the command");
        exit(-1);           /* terminate with error code of -1 */
    }
  else
    {
    addCommand[current]='\0';
    if(addCommand[0]=='r')
        {
        if(addCommand[1]=='r')
            {
            if(g_command_count==0)
                {
                printf("No recent command can be found in the history. \n");
                return 0;
                }
            strcpy(addCommand,g_history[(g_command_count)% HISTORY_SIZE]);
            }
        else
        {
            temp = atoi(&addCommand[1]);
            if(temp < 1 || temp > g_command_count || temp <= g_command_count -HISTORY_SIZE)
                {
                printf("Command number cannot be found. \n");
                return 0;
                }
            strcpy(addCommand,g_history[(temp-1)%HISTORY_SIZE]);
        }
        current = strlen(addCommand);
            }
    g_command_count++;
    strcpy(g_history[(g_command_count-1)%HISTORY_SIZE], addCommand);
}
}

/**
 * This is a helper function that displays the history items.
 * Use this to get an idea of how the history is handled.
 * This should give you some hints on how to implement addCommandToHistory()
**/

void displayHistory()

{ //DO NOT MODIFY
  int maxItems;
  if (g_command_count < HISTORY_SIZE)
    maxItems = g_command_count;
  else
    maxItems = HISTORY_SIZE;

  for (int i = 0; i < maxItems; i++)
  {
    printf("%d \t %s\n", i, g_history[i]);
  }
}

/**
 * The setup function is used to parse the input and prepare the arguments array args[].
 * @returns boolean: valid command parsed.
**/
int setup(char inputBuffer[], char *args[])
{
  int m_inputLength,                //Number of characters that read() reads from the command line
      i,                            //Arbitrary counter
      start,                        //temporary index ( helper) used to indicate the start of the next parameter
      argumentCharacterCounter;     //counter index for where to place the next parameter into args[]
  /**
   * Printing shell> and reading in from the standard input
   */
    do {
    printf("shell>");
    fflush(0);  //Flush 'shell>' out into the command line
    m_inputLength = read(STDIN_FILENO, inputBuffer, MAX_CHARACTERS_LINE);  //Read from command line into our inputBuffer
    }while(inputBuffer[0] != '\n');
  /**
   * Add the command to the history
   */
  addCommandToHistory(inputBuffer, m_inputLength);

    int temp;
  argumentCharacterCounter = 0;  //helper variable
  start = -1;                   //helper variable.

  /**
   * Parse the contents of inputBuffer
   */
  for (i = 0; i < m_inputLength; i++)
  {  //Parse every character in the inputBuffer
    //TODO parse every character
    //TODO use ' ' as the seperator between arguments
    //TODO store the arguments into args[] which will eventually be passed back into the shell function.


    switch (inputBuffer[i])
    {
    case ' ':
        case '\t' :               /* argument separators */
            if(start != -1)
            {
                args[argumentCharacterCounter] = &inputBuffer[start];    /* set up pointer */
                argumentCharacterCounter++;
            }
            inputBuffer[i] = '\0'; /* add a null char; make a C string */
            start = -1;
            break;

        case '\n':                 /* should be the final char examined */
            if (start != -1){
                args[argumentCharacterCounter] = &inputBuffer[start];
                argumentCharacterCounter++;
            }
            inputBuffer[i] = '\0';
            args[argumentCharacterCounter] = NULL; /* no more arguments to this command */
            break;

        case '&':
            // *background = 1;
            inputBuffer[i] = '\0';
            break;

        default :             /* some other character */
            if (start == -1)
                start = i;
    }
  }

  args[argumentCharacterCounter] = NULL; //just in case the input line was > 80
    for (i = 0; i < m_inputLength; i++)
    {
        if (inputBuffer[i] == '&')
        {
            inputBuffer[i] = '\0';
            // *background = 1;
            --inputBuffer;
            break;
        }

    }
  return 1;

}  //End of setup()

int shell(int argc, char *argv[])
{

  char m_inputBuffer[MAX_CHARACTERS_LINE];    //buffer to hold the command entered
  char *args[MAX_CHARACTERS_LINE / 2 + 1];    //command line (of 80) has max of 40 arguments
  pid_t m_child;                               //process id of the m_child process
  int m_status;                                 //result from execvp system call*/
  int m_isValidCommand = 1;

  while (42) {  //Program terminates when you enter the 'exit' command


    m_isValidCommand = setup(m_inputBuffer, args);       //get next command

    if (strncmp(m_inputBuffer, "exit", 4) == 0)
      return 0;
    else if (strncmp(m_inputBuffer, "g_history", 7) == 0) {
      displayHistory();
      continue;
    }

    // Start the external command execution here
    if (m_isValidCommand) {
      //TODO: use fork to create a child
          m_child = fork(); 
          if(m_child < 0)
    { /* if the child process didn't return 0, the fork is failed */
        printf("Fork failed! \n");
    }
else if(m_child==0)
        { /* child process */
        if(m_inputBuffer[0]=='history' || m_inputBuffer[0] =='h')
	printf("I am the child %d\n",pid);
    	execlp("/bin/ls","ls",NULL);
	{
            displayHistory();
//else

	{ /* parent process */
    /* parent will wait for the child to complete */
    printf("I am the parent %d\n",pid);
    wait(NULL);

    printf("Child Complete\n");
  }
            return 0;
        }
        m_status = execvp(args[0],args);
        if(m_status !=0){
            printf("%s: command not found. \n", args[0]);
        }

    }
          /* create a child process*/
      //TODO: use perror() to indicate errors
      //TODO: use execvp inside the child to start the program passed in as argument
      //TODO: make the parent wait for the child

    } else {
      printf("Invalid command!\n");
    }
  }


  return 0;
}
