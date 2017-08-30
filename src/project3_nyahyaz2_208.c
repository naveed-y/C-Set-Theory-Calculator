#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef struct node node;
struct node 										// Each node holds one element of the set (a string)
{
	char str[1024];
	node *next;
	
};


int strcmpa(char *s1, char *s2); 							// Used to place strings in alphabetical order
void add(node **setList, char * str, int num);						// The following void function prototypes are all the set operations. The add function has extra parameters so that either the user or other functions can access the add() functionality
void rem(node **setList);
void print(node **setList);
void copy(node **setList);
void clear(node **setList, int set);							// clear() has an extra parameter so either the user or different functions can access its functionality
void setUnion(node **setList);
void intersect(node **setList);
void sym(node **setList);
int whichSet();										// whichSet() is used in most of the other functions and prompts the user to enter a set index to be operated on. whichSet() also contains the input validation


int strcmpa(char *s1, char *s2)								// Provided, used to compare strings after converting them to lowercase
{
   while (*s1 && tolower(*s1) == tolower(*s2))
   {
      s1++;
      s2++;
   }
   return tolower(*s1) - tolower(*s2);
}




int main()
{
	node **setList;									// Array of head nodes, contains all the data, used in every functio
	int i;										// Loop counter
	char menuInput[3];								// Used for fgets() buffer, since char only takes one byte we will use an array of size 2 including a null terminator	
	char caseVal;									// Switch case value


	setList = malloc(10*sizeof(node *));						// Allocate memory for the array of linked lists
	for (i=0; i<10; i++)
	{
		setList[i] = malloc(sizeof(node));
		if (setList == NULL || setList[i] == NULL)				// Check malloc()
		{
			printf("Memory allocation error exiting...\n");
			exit(1);
		}
		setList[i]->next = NULL;						// Assign NULL to all the heads' "next" values
	}

	
	while(1)									// Main menu, continuosly loops until user specifically asks to quit program
	{
		printf("Please choose from one of the following options:\n\tadd string (a)\n\tremove string (r)\n\tunion (u)\n\tintersection (i)\n\tsymmetric difference (s)\n\tcopy (c)\n\tclear (z)\n\tprint set (p)\n\tquit (q)\n-> ");
		fgets(menuInput, 3, stdin); 						// Take in an input from the user (stdin), add it and a null terminator into the array value
		sscanf(menuInput, "%c", &caseVal);					// Store the input to caseVal

		
											// Input validation check
		if (caseVal != 'a' && caseVal != 'r' && caseVal != 'u' && caseVal != 'i' && caseVal != 's' && caseVal != 'c' && caseVal != 'z' && caseVal != 'p' && caseVal != 'q' )
		{
			printf("Your input was incorrect, returning to menu...\n\n");
			continue;
		}
	
		switch(caseVal) 							// Determines what function to run based on what user inputs for caseVal
		{
			case 'a':
				add(setList, "-1", -1);					// throw in -1 for the 2nd and 3rd parameters. This tells the function that the user will be entering the string to be added and the set to be added to
				break;
			case 'r':
				rem(setList);
				break;
			case 'u':
				setUnion(setList);
				break;
			case 'i':
				intersect(setList);
				break;
			case 's':
				sym(setList);
				break;
			case 'c':
				copy(setList);
				break;
	
			case 'z':
				clear(setList, -1);					// Throw in -1 for 2nd parameter. This tells the function that the user will enter which set to be cleared
				break;
			case 'p':
				print(setList);
				break;
			case 'q':
				printf("\n\n~~Exiting Program~~\n\n");
				exit(1);						// Exit program if user enters 'q'

		}
	}	
	
	
	return 0;
}

int whichSet()
{
	char setStr;                                                                    // Used together with setNum to validate user's input
        int setNum;                                                                     // Which set will be altered
        char buffer[256];                                                               // Buffer for fgets() function

	
        while(1)                                                                        // Continuously loop until user enters a valid set index
        {
                printf("-> ");					                     	// Ask user which set to add to
                fgets(buffer,256,stdin);                                                // Take in user input using fgets and sscanf
                sscanf(buffer, "%c", &setStr);                                          // Scan in input as a char initially
                if(! isdigit(setStr))                                                   // First check if the first character in the user's input is a digit or not
                {
                        printf("That character is not a digit. Enter a number between 0-9\n");
                        continue;                                                       // Loop back if entry is invalid
                }
                sscanf(buffer, "%d", &setNum);                                          // After validating that the input is an int, scan it to 'setNum'
                if (setNum<0 || setNum>9)                                               // Check if input is in range
                {
                        printf("Input out of range. Please enter a number between 0-9\n");
                        continue;                                                       // Loop back if entry is out of range
                }
                break;                                                                  // If all validation checks pass, break out of loop

        }
	return setNum;
}

void add(node **setList, char * str, int num)
{
	int setNum;									// Which set will be altered
	char buffer[256];								// Buffer for fgets() function
	char nodeStr[256] = "";								// String which will be added to newNode
	node * newNode			;						// Node which will be added to the set
	node * temp;
	
	if (num == -1)									// if 3rd param is -1, user will be prompted to input a set index
	{
		printf("Which set would you like to add to?\n");
		setNum = whichSet();							// Use whichSet() function to acquire an valid input 0-9 from user
	}
	else										// Else, the value provided in the parameter will be used as the set index
	{
		setNum = num;
	}
	if (strcmp(str, "-1") == 0)							// If 2nd param is '-1', user will be prompted to enter a string
	{
		printf("Enter a string to be added to set (%d)\n-> ", setNum);		// Ask user for string input and save it to nodeStr variable
		fgets(buffer, 255, stdin);							
		sscanf(buffer, "%s", nodeStr);							
	}
	else										// Else, the value provided in the parameter will be used as the node string
	{
		strcpy(nodeStr, str);
	}

	if (setList[setNum]->next == NULL)						// If the specified linked list is empty, this code block will run
	{
		
		setList[setNum]->next = malloc(sizeof(node));				// Malloc the first node in the list and check malloc()
		if (setList[setNum]->next == NULL){printf("Memory allocation error, exiting...\n");exit(1);}
		
		strcpy(setList[setNum]->next->str, nodeStr);				// Copy the node string to the new  node
		setList[setNum]->next->next = NULL;					// Assign the new node's next to NULL
		
		return;	
	}
	
	temp = setList[setNum];								// Otherwise, we will create a temp node to scan through the linked list
	while(temp->next != NULL)							// Continue looping as long as there are more nodes in the list
	{

		if (strcmpa(nodeStr, temp->next->str) < 0)				// If the node we want to enter is less than the temp node, we place the newNode ahead of temp
		{
			newNode = malloc(sizeof(node));					// Malloc and check malloc
			if (newNode == NULL)
			{
				printf("Memory allocation error, exiting...\n");
				exit(1);
			}
			newNode->next = temp->next;					// Set the newNode's next value to the temp node
			strcpy(newNode->str, nodeStr);					// Copy in desired string
			temp->next = newNode;						// Reallign temp to point to the new node
			break;								// Exit loop
		}
		if (strcmpa(nodeStr, temp->next->str) == 0)				// If the value is the same as a value in the list, force break out of function
		{
			break;
		}
		if (strcmpa(nodeStr, temp->next->str) > 0)				// If the string is greater than temp, we either continue looping through the list or place the node after temp if temp->next->next is NULL
		{
			if (temp->next->next == NULL)
			{
				temp->next->next = malloc(sizeof(node));
				if (temp->next->next == NULL) {printf("Malloc error, exiting...\n");exit(1);}
				strcpy(temp->next->next->str, nodeStr);
				temp->next->next->next = NULL;
				break;
			}
			temp = temp->next;
			continue;
		}
	}

}

void rem(node **setList)								// This function removes a specific value from the list
{
	int setNum;									// Which set we will alter
	char buffer[256];								// Used for fgets
	char nodeStr[256];								// The desired string we want to remove
	node * temp;									// temp nodes used for looping through linked list
	node * temp2;

	printf("Which set would you like to remove a link from?\n");			// Prompt user to entire set to be altered
	setNum = whichSet();
	
	temp = setList[setNum];								// Set temp to the head of the linked list that the user wants to remove a value from
		
        printf("Enter a string to be removed from set (%d)\n-> ", setNum);              // Ask user for string input and save it to nodeStr variable
        fgets(buffer, 255, stdin);
        sscanf(buffer, "%s", nodeStr);
	
	while(temp->next != NULL)							// Continue looping until we reach the end of the list
	{	
		if (strcmpa(temp->next->str, nodeStr) == 0)				// If we find a string that is equal to what the user inputted, we remove it from the list
		{
			temp2 = temp->next;						// we set temp2 to the node we want to remove
			temp->next = temp->next->next;					// Reallign the list to two nodes down
			free(temp2);							// Free the node we want to remove
			break;		
		}
		temp = temp->next;							// Set temp to the next node in the list and continue looping
	}	


}

void setUnion(node **setList)
{
	int set1;									// First operand
	int set2;									// Second operand
	int destSet;									// Destination set
	node * temp;									// Temp node used to perform the union operation

        printf("Which set is your destination set?\n");					// Prompt user to specify both operands and dest set
        destSet = whichSet();
	printf("Which set is your first operand?\n");
	set1 = whichSet();
	printf("Which set is your second operand?\n");
	set2 = whichSet();

	temp = setList[set1];								// Set temp to list head
	clear(setList, destSet);							// Clear destination set if it isn't already empty
	while (temp->next != NULL)							// Loop through entire list
	{
		add(setList, temp->next->str, destSet);					// Add all values from the first operand list to the destination set
		temp = temp->next;
	}
	temp = setList[set2];								// Set temp to 2nd operand list head
	while (temp->next != NULL)
	{
		add(setList, temp->next->str, destSet);					// Add all values from 2nd list to dest set. If there is any overlap in the strings, add will skip that value
		temp = temp -> next;
	}
}

void intersect(node **setList)								// This fuction adds values present in two operand sets to a destination set
{

        int set1;
        int set2;
        int destSet;
        node * temp;									// Temp nodes used to perform intersection operation
	node * temp2;

        printf("Which set is your destination set?\n");					// Prompt user for all three sets
        destSet = whichSet();
        printf("Which set is your first operand?\n");
        set1 = whichSet();
        printf("Which set is your second operand?\n");
        set2 = whichSet();
        	
	temp = setList[set1];								// Set temp to first oeprand list's  head
	clear(setList, destSet);							// Clear dest set
	while (temp->next != NULL)							// Loop through entire list
	{	
		temp2 = setList[set2];							// Set temp2 to second operand list's head
		while(temp2->next != NULL)						// Loop through entire second list every time first loop runs
		{
			if (strcmpa(temp->next->str, temp2->next->str) == 0)		// If any value of temp2 is equal to any value of temp, we add that value to dest set and break out of inner loop
			{
				add(setList, temp->next->str, destSet);
				break;
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}

}

void sym(node **setList)								// This function finds values in either one operand list or the either and adds those values to the dest set
{

        int set1;									// Same values as union/intersect function
        int set2;
        int destSet;
        node * temp;
        node * temp2;

       	printf("Which set is your destination set?\n");					// Prompt user for  all three sets
        destSet = whichSet();
	printf("Which set is your first operand?\n");
        set1 = whichSet();
        printf("Which set is your second operand?\n");
        set2 = whichSet();

        temp = setList[set1];								//  Set temp to first operand list head
        clear(setList, destSet);							// Clear destination set
        while (temp->next != NULL)							// Loop through entire first list
        {
                temp2 = setList[set2];							// Set temp2 to head of 2nd operand list every time loop runs
                while(temp2->next != NULL)						// Loop through entire second list
                {
                        if (strcmpa(temp->next->str, temp2->next->str) == 0)		// If we find any values that are equal, we skip those values
                        {
				break;
                        }
			else if (temp2->next->next != NULL)				// Otherwise, if we still haven't reached end of second list, continue looping
			{
				temp2 = temp2->next;
				continue;
			}
			add(setList, temp->next->str, destSet);				// If we reach the end of second list, that means no value in the second list is equal to the temp value in the first list. We add that value to destSet
                    	temp2 = temp2->next; 		
                }
                temp = temp->next;							// Check every value in the first list for equality with every value in the second list
        }
	temp2 = setList[set2];								// We perform the double while loop operation twice, once to add all the values from temp to the destSet, once to add all the values from temp2 to the destSet
        while (temp2->next != NULL)
        {
                temp = setList[set1];
                while(temp->next != NULL)
                {
                        if (strcmpa(temp->next->str, temp2->next->str) == 0)
                        {
				break;
                        }
			else if (temp->next->next != NULL)
			{
				temp = temp->next;	
				continue;
			}
                        add(setList, temp2->next->str, destSet);
                        temp = temp->next;
                }
                temp2 = temp2->next;
        }


}






void print(node **setList)								// This function prints a specified set
{
	printf("Which set would you like to print?\n");
	int setNum = whichSet();							// determine which set user wants to alter
	
	node * temp = setList[setNum];
	printf("{");									// Open bracket at beginning of print string
	while(temp->next != NULL)
	{
		printf("%s", temp->next->str);						// Print string value of every node in the linked list
		if (temp->next->next != NULL)
		{
			printf(", ");							// add a comma and a space if the current node isn't the last node in the list
		}
		temp = temp->next;
	}
	printf("}\n");									// Add a closed bracket after loop ends to close the print string
}

void copy(node **setList)								// This function copies one linked list to another
{
	int srcSet;									// Source and destination sets
	int destSet;
	node * temp;
        
        printf("Which set would you like to copy to?\n");				// Prompt user for set indeces
        destSet = whichSet();
	printf("Which set would you like to copy?\n");
	srcSet = whichSet();
		
	temp = setList[srcSet];								// Set temp to head of source set
	clear(setList, destSet);							// Clear destination set (we don't want any old values in the destination set to remain before we copy the new values in
	while(temp->next != NULL)							// Loop through entire source list
	{
		add(setList, temp->next->str, destSet);					// Add every string of every node to the index of the destSet
		temp = temp->next;
	}
	
}

void clear(node **setList, int set)							// This functin clears an entire set, minus the head node
{
	int setNum;
 	node * temp;

	if(set == -1)									// If 2nd param is -1, we allow the user to specify which set we want to clear
	{
		printf("Which set would you like to clear?\n");
		setNum = whichSet();
	}
	else										// Else, we allow a different function to specify which set we want to clear
	{
		setNum = set;
	}

	temp = setList[setNum];								// Set temp to list head
	while(setList[setNum]->next != NULL)						// Loop through entire list
	{
		
		setList[setNum] = setList[setNum]->next;				// With this call, we move the head node one node down the list
		free(temp);								// Then we free the previous head node
		temp = setList[setNum];							// We reassign temp to the new head of the list
	}
	strcpy(setList[setNum]->str, "");						// After the entire list is cleared, we need to rebuild the head node, set its string to ""
	setList[setNum]->next = NULL;							// and set its next value to be NULL
}
											

											// END OF PROGRAM
