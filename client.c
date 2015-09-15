#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

#define TRUE 		1
#define FALSE 		0


info_flight flightDataGo ;
info_flight flightDataReturn ;

int main(void)
{
	welcomeMessage();
	return 0;
}

void welcomeMessage(void)
{
	int choice, cleanbuff ;
	printf("Welcome to Aerolineas Argentinas flights booker\n");

	printf("What do you want to do?\n\n");
	printf("1. Book a flight\n");
	printf("2. See your reservation\n");
	printf("3. Modify your reservation\n");
	printf("\nWrite 1, 2 or 3 and press enter\nChoice : ");

	scanf("%d",&choice);
	while((cleanbuff=getchar())!='\n');

	switch(choice)
	{
		case 1 :
			getInfo();
			flightDataGo.flightCode = selectFlight(flightDataGo);		
			flightDataReturn.flightCode = selectFlight(flightDataReturn);
			printf("Go : %s, Return : %s\n",flightDataGo.flightCode,flightDataReturn.flightCode);
			break ;
		case 2 :
			listInfo();
			break ;
		case 3 :
			modifyInfo();
			break ;
		default :
			printf("Wrong choice please try again\n");
			welcomeMessage();
			break ;
	}
}

void getInfo()
{
	printf("You choose to book a flight\n");
	printf("Please enter all the necessary information\n");
	printf("\n");
	printf("Origin : \n");
	while(stringParsing(flightDataGo.origin))
	{
		printf("Incorrect origin\nOrigin :\n");
	}

	printf("Destination : \n");
	while(stringParsing(flightDataGo.destination))
	{
		printf("Incorrect destination\nDestination :\n");
	}
	
	printf("Date go DD/MM/YYYY : \n");
	while(!stringParsing(flightDataGo.date))
	{
		printf("%s\n",flightDataGo.date);
		printf("Invalide date format\nDate go as DD/MM/YYYY :\n");
	}

	printf("Date return as DD/MM/YYYY : \n");
	while(!stringParsing(flightDataReturn.date))
	{
		printf("Invalide date format\nDate return as DD/MM/YYYY :\n");
	}

	printf("Number of passengers : \n");
	scanf("%d",&flightDataGo.numberPass);
	strcpy(flightDataReturn.origin,flightDataGo.destination);
	strcpy(flightDataReturn.destination,flightDataGo.origin) ;
	flightDataReturn.numberPass=flightDataGo.numberPass ;
}

int stringParsing(char *string)
{
	int firstchar, character, cleanbuff;
	int counter = 0 ;
	int dateCounter = 1 ;
	int equal = FALSE ;
	int dateOK = TRUE ;

	if((firstchar=getchar())=='-')
	{
		printf("Error, you didn't enter a letter\n");
	}
	else
	{
		ungetc(firstchar,stdin);
		while((character=getchar())!='\n')
		{
			if (character == ' ' )
				string[counter]='_' ;
			else
				string[counter]=character ;

			if(counter++==MAX_LENGTH-1)
			{
				printf("Max string length exceeded\n");
				while((cleanbuff=getchar())!='\n');
				break ;
			}

			if (string==flightDataGo.date || string==flightDataReturn.date)
			{
				// Really ugly line but can't find another way to do it 
				if ( ( (dateCounter==3 || dateCounter==6) && (character!='/') ) 
					|| ( (dateCounter==1 || dateCounter==2 || dateCounter==4 
					   		|| dateCounter==5 || dateCounter==7 || dateCounter==8 
					   		|| dateCounter==9 ||dateCounter==10) 
					&& (character<'0' || character>'9')))
				{
					while((cleanbuff=getchar())!='\n');
					dateOK = FALSE ;
					break ;
				}			
				dateCounter++ ;																
			}
			else
			{
				if((character>='a' && character<='z') || (character>='A' && character<='Z') || (character==' ') )
					continue ;
				else 
				{
					printf("Invalid input, enter only character please\n");
					while((cleanbuff=getchar())!='\n');
					break;
				}
			}
		}
	}	
	string[counter]=0;

	if (string==flightDataGo.date || string==flightDataReturn.date)
		return dateOK ;
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (!strcmp(destinations[0][i],string))
				equal = TRUE ;
		}
		return !equal ;
	}
}


char* selectFlight(info_flight flightData)
{
	
	FILE* flightList = NULL ;
	char* line = NULL ;
	size_t len = 0 ;
	ssize_t read ;
	flightList = fopen("flight_list_space.txt","r");

	info_flight r_flightData ;	
	char r_cod_origin[MAX_LENGTH] ; 
	char r_cod_destination[MAX_LENGTH] ;
	char* r_flightCode = malloc(7 * sizeof(char)) ;
	
	
	if(flightList == NULL)
		exit(EXIT_FAILURE);

	printf("\nWhen do you want to travel ?\n");

	while ((read = getline(&line, &len, flightList)) != -1)
	{
		fscanf(flightList,"%s %s %s %s %s %s %d %s",
		r_flightData.origin,r_cod_origin,r_flightData.destination,r_cod_destination,
		r_flightData.date,r_flightData.hour,&r_flightData.numberPass,r_flightCode);
		
		if (!strcmp(r_flightData.origin,flightData.origin)
		 	&& !strcmp(r_flightData.destination,flightData.destination)
		 	&& !strcmp(r_flightData.date,flightData.date)
		 	&& r_flightData.numberPass>=flightData.numberPass)
		{
			
			printf("Flight number : %s : From %s to %s, on the %s at %s\n\n",
				r_flightCode,r_flightData.origin,r_flightData.destination,r_flightData.date,r_flightData.hour);
		}
	}
	free(line);

	printf("Enter the number of the flight you would like to choose\n");
	scanf("%s",r_flightCode);


	fclose(flightList);
	return r_flightCode ; 
}

void listInfo(void)
{
}

void modifyInfo(void)
{
}