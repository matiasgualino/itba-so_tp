#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "client.h"

#define TRUE 		1
#define FALSE 		0

#define DATE_MODE 	0
#define CITY_MODE 	1
#define NAME_MODE	2


///////////////////////////////////////////////////////////////
/// 1) NON RETOUR !!! A FAIRE 								///
/// 2) VERIFICATION DU NOMBRE DE PASSAGER = INT 			///
/// 3) INITIALISATION DES STRUCTURES 			 			///
///////////////////////////////////////////////////////////////


int resNumber ;
int noReturn = FALSE ;

flight goFlight0 ;
flight* goFlight = &goFlight0 ;

flight returnFlight0 ;
flight* returnFlight = &returnFlight0 ;

int main(void)
{	

//	flight* flightData = createFlight();
	reservation tripData0 ;
	reservation* tripData = &tripData0 ;

	tripData->goFlight= goFlight ;
	tripData->returnFlight=returnFlight ;

	welcomeMessage(tripData);

	return 0;
}

/*
flight* createFlight()
{
	flight* flightData = calloc(sizeof (flight),1) ; 

	return flightData ;
}

reservation* createReservation(flight* goFlight, flight* returnFlight)
{
	reservation* tripData = malloc(sizeof (reservation)) ; 
	
	tripData->goFlight = *goFlight ;
	tripData->returnFlight = *returnFlight ;
	
	return pToTripData ;
}
*/

void welcomeMessage(reservation* tripData)
{
	while(1)
	{
		int choice, cleanbuff ;
		char* codeTmp ;
		printf("\nWelcome to Aerolineas Argentinas flights booker\n");

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
			getInfo(tripData);
			codeTmp = selectFlight(tripData->goFlight) ;
			tripData->goFlight->code = codeTmp;

			if(!noReturn)
			{
				codeTmp = selectFlight(tripData->returnFlight) ;
				tripData->returnFlight->code = codeTmp;
			}

			listInfo(tripData);
			writeData(tripData);
			break ;
			case 2 :
			listInfo(tripData);
			break ;
			case 3 :
			modifyInfo(tripData);
			break ;
			default :
			printf("Wrong choice please try again\n");
			welcomeMessage(tripData);
			break ;
		}
	}
	
}

void getInfo(reservation* tripData)
{
	printf("\n\nYou choose to book a flight\n");
	printf("Please enter all the necessary information\n");
	printf("\n");


	printf("Name :\n");
	while(stringParsing(tripData->name,tripData,NAME_MODE))
	{
		printf("Please enter only characters\nName :\n");
	}

	printf("Surname :\n");
	while(stringParsing(tripData->surname,tripData,NAME_MODE))
	{
		printf("Please enter only characters\nSurname :\n");
	}

	printf("Origin : \n");
	while(stringParsing(tripData->goFlight->origin,tripData,CITY_MODE))
	{
		printf("Incorrect origin\nOrigin :\n");
	}

	printf("Destination : \n");
	while(stringParsing(tripData->goFlight->destination,tripData,CITY_MODE))
	{
		printf("Incorrect destination\nDestination :\n");
	}
	
	printf("Date go DD/MM/YYYY : \n");
	while(!stringParsing(tripData->goFlight->date,tripData,DATE_MODE))
	{
		printf("Invalide date format\nDate go as DD/MM/YYYY :\n");
	}

	printf("Date return as DD/MM/YYYY : \n");
	while(!stringParsing(tripData->returnFlight->date,tripData,DATE_MODE))
	{
		printf("Invalide date format\nDate return as DD/MM/YYYY :\n");
	}

	printf("Number of passengers : \n");
	scanf("%d",&(tripData->goFlight->numberPass));
	strcpy(tripData->returnFlight->origin,tripData->goFlight->destination);
	strcpy(tripData->returnFlight->destination,tripData->goFlight->origin);
	tripData->returnFlight->numberPass=tripData->goFlight->numberPass ;
	tripData->number=++resNumber ;
}

int stringParsing(char *string, reservation* tripData, int mode)
{
	int firstchar, character, cleanbuff;
	int counter = 0 ;
	int dateCounter = 1 ;
	int equal = FALSE ;
	int dateOK = TRUE ;

	if(((firstchar=getchar()) =='\n') && (mode==DATE_MODE))
	{
		putchar(firstchar);
		dateOK = TRUE ; 
		noReturn = TRUE ;
	}
	else
	{
		ungetc(firstchar,stdin);
		while((character=getchar())!='\n')
		{
			if (character == ' ' && (mode == CITY_MODE))
				string[counter]='_' ;
			else
				string[counter]=character ;

			if(counter++==MAX_LENGTH-1)
			{
				printf("Max string length exceeded\n");
				while((cleanbuff=getchar())!='\n');
				break ;
			}

			if (mode == DATE_MODE)
			{			

				if(character == ' ')
					break ; 

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

	if( ((strlen(string)!=10) && mode==DATE_MODE ) && !noReturn) 
	{
		dateOK = FALSE ;
	}

	if (mode==DATE_MODE)
		return dateOK ;
	else if (mode==CITY_MODE)
	{
		for (int i = 0; i < 8; i++)
		{
			if (!strcmp(destinations[0][i],string))
				equal = TRUE ;
		}
		return !equal ;
	}
	else
		return 0 ;
}


char* selectFlight(flight* flightData)
{
	FILE* flightList = NULL ;
	char* line = NULL ;
	size_t len = 0 ;
	ssize_t read ;
	flightList = fopen("flight_list.txt","r+");
	
	if(flightList == NULL)
		exit(EXIT_FAILURE);
	
	flight r_flightData ;

	char r_codeChoosen[MAX_LENGTH] ;
	char* r_code = malloc(7 * sizeof(char)) ;
	int foundFlight = FALSE ;
	int codeOK = FALSE ;
	
	printf("\nWhen do you want to travel ?\n");

	while ((read = getline(&line, &len, flightList)) != -1)
	{
		fscanf(flightList,"%s %*s %s %*s %s %s %d %s",
		r_flightData.origin,r_flightData.destination,
		r_flightData.date,r_flightData.hour,&r_flightData.numberPass,r_code);

		if (!strcmp(r_flightData.origin,flightData->origin)
		 	&& !strcmp(r_flightData.destination,flightData->destination)
			&& !strcmp(r_flightData.date,flightData->date)
		 	&& r_flightData.numberPass>=flightData->numberPass)
		{
			printf("Flight number : %s : From %s to %s, on the %s at %s\n\n",
				r_code,r_flightData.origin,r_flightData.destination,r_flightData.date,r_flightData.hour);
			foundFlight = TRUE ;
		}
	}

	if (!foundFlight)
		printf("No flights found\n");

	foundFlight = FALSE ;
	free(line);
	fseek(flightList,0,SEEK_SET);
	
	while (!codeOK)
	{
		printf("Enter the number of the flight you would like to choose\n");
		scanf("%s",r_codeChoosen);
		
		while ( ((read = getline(&line, &len, flightList)) != -1) && (!codeOK) )
		{
			fscanf(flightList,"%s %*s %s %*s %s %s %d %s",
				r_flightData.origin,r_flightData.destination,
				r_flightData.date,r_flightData.hour,&r_flightData.numberPass,r_code);

			if (!strcmp(r_code,r_codeChoosen))
			{
				codeOK = TRUE ;
				strcpy(flightData->hour,r_flightData.hour) ;
			}
		}
	}

	printf("\nYou choose : \nFlight number : %s : From %s to %s, on the %s at %s\n\n",
				r_codeChoosen,r_flightData.origin,r_flightData.destination,r_flightData.date,r_flightData.hour);

	fclose(flightList);
	return r_code ; 
}

void writeData(reservation* tripData)
{
	FILE* resList = NULL ;
	resList = fopen("reservation_list.txt","a");
	
	if(resList == NULL)
		exit(EXIT_FAILURE);

	fprintf(resList,"%d %s %s %s %s %s %s %s %d ",
			tripData->number,tripData->name,tripData->surname,
			tripData->goFlight->code,tripData->goFlight->origin,tripData->goFlight->destination,tripData->goFlight->date,tripData->goFlight->hour,tripData->goFlight->numberPass);

	if (!noReturn)
	{
		fprintf(resList,"%s %s %s %s %s %d",
		tripData->returnFlight->code,tripData->returnFlight->origin,tripData->returnFlight->destination,tripData->returnFlight->date,tripData->returnFlight->hour,tripData->returnFlight->numberPass);	
	}	
	
	fprintf(resList,"\n");
	
	fclose(resList) ;
}

void listInfo(reservation* tripData)
{
	printf("Reservation number : %d\n",tripData->number);
	printf("Name : %s Surname : %s\n",tripData->name,tripData->surname);
	printf("Go from %s to %s on the %s at %s on flight number %s\n",tripData->goFlight->origin,tripData->goFlight->destination,tripData->goFlight->date,tripData->goFlight->hour,tripData->goFlight->code);
	if (!noReturn)
	{
		printf("Return flight from %s to %s on the %s at %s on flight number %s\n",tripData->returnFlight->origin,tripData->returnFlight->destination,tripData->returnFlight->date,tripData->returnFlight->hour,tripData->returnFlight->code);
	}
}

void modifyInfo(reservation* tripData)
{
}