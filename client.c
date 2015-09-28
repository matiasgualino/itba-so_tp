#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "client.h"

#define TRUE 	1
#define FALSE 	0

#define DATE_MODE_GO		0
#define CITY_MODE 			1
#define NAME_MODE			2
#define DATE_MODE_RETURN 	3

#define LINE_LENGTH 68 


///////////////////////// TO DO LIST //////////////////////////
/// 1) MODIFICATION DU NOMBRE DE PASSAGERS..				///
///	2) VERIFICATION NOMBRE DE PASSAGER = INT 				///
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

//	modifyNumberPass(3,4); // 18 avant éxécution
	resNumber=numberReservation();

	reservation tripData0 ;
	reservation* tripData = &tripData0 ;

	tripData->goFlight= goFlight ;
	tripData->returnFlight= returnFlight ;

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
			printf("Please keep your reservation number : %d\n\n",tripData->number);

			if(!noReturn)
			{
				codeTmp = selectFlight(tripData->returnFlight) ;
				tripData->returnFlight->code = codeTmp;
			}

			writeResData(tripData);
			break ;
			case 2 :
			listInfo();
			break ;
			case 3 :
			modifyInfo();
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
	while(!stringParsing(tripData->goFlight->date,tripData,DATE_MODE_GO))
	{
		printf("Invalide date format\nDate go as DD/MM/YYYY :\n");
	}

	printf("Date return as DD/MM/YYYY : \n");
	while(!stringParsing(tripData->returnFlight->date,tripData,DATE_MODE_RETURN))
	{
		printf("Invalide date format\nDate return as DD/MM/YYYY :\n");
	}

	printf("Number of passengers : \n");
	scanf("%d",&(tripData->goFlight->numberPass));
	strcpy(tripData->returnFlight->origin,tripData->goFlight->destination);
	strcpy(tripData->returnFlight->destination,tripData->goFlight->origin);
	tripData->returnFlight->numberPass=tripData->goFlight->numberPass ;
	tripData->number=resNumber++;
}

int stringParsing(char *string, reservation* tripData, int mode)
{
	int firstchar, character, cleanbuff;
	int counter = 0 ;
	int dateCounter = 1 ;
	int equal = FALSE ;
	int dateOK = TRUE ;

	if(((firstchar=getchar()) =='\n') && (mode==DATE_MODE_RETURN))
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

			if (mode == DATE_MODE_GO || mode == DATE_MODE_RETURN)
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

	if( ((strlen(string)!=10) && (mode==DATE_MODE_RETURN || mode==DATE_MODE_GO) ) && !noReturn) 
	{
		dateOK = FALSE ;
	}

	if (mode==DATE_MODE_GO || mode==DATE_MODE_RETURN)
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
	ssize_t readd ;
	flightList = fopen("flight_list.txt","r+");
	
	if(flightList == NULL)
		exit(EXIT_FAILURE);
	
	flight r_flightData ;

	char r_codeChoosen[7] ;
	char* r_code = malloc(7 * sizeof(char)) ;
	int foundFlight = FALSE ;
	int codeOK = FALSE ;
	
	printf("\nWhen do you want to travel ?\n");

	while ((readd = getline(&line, &len, flightList)) != -1)
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
	else
	{

		foundFlight = FALSE ;
		free(line);
		fseek(flightList,0,SEEK_SET);

		while (!codeOK)
		{
			printf("Enter the number of the flight you would like to choose.\n");
			read(0,r_codeChoosen,7) ;
			while ( ((readd = getline(&line, &len, flightList)) != -1) && (!codeOK) )
			{
				fscanf(flightList,"%*s %*s %*s %*s %*s %*s %*d %s",r_code);
				if (!strcmp(r_code,r_codeChoosen))
				{
					codeOK = TRUE ;
					strcpy(flightData->hour,r_flightData.hour) ;
					strcpy(r_code,r_codeChoosen);
				}
			}
			memset(r_codeChoosen,0,7);
			fseek(flightList,0,SEEK_SET);
		}

		printf("\nYou choose : \nFlight number : %s : From %s to %s, on the %s at %s\n\n",
			r_code,r_flightData.origin,r_flightData.destination,r_flightData.date,r_flightData.hour);
}
	fclose(flightList);
	return r_code ; 
}

int codeToInt(char* r_code)
{
	printf("%s\n",r_code );
	int code = 0 ;
	int mut = 1 ;
	for (int i = 0; i < 5; i++) 
	{
		code += (r_code[6-i]-'0') * 1*mut ;
		mut = mut*10 ;
		printf("%d\n", code);

	}
	return code ;
}

void modifyNumberPass(int lineToMod, int numberPass)
{
	FILE* flightList = NULL ;
/*	char* line = NULL ;
	size_t len = 0 ;
	ssize_t readd ;
*/
	flightList = fopen("test.txt","r+");

	flight flightData ;
	char codOrigin[MAX_LENGTH];
	char codDestination[MAX_LENGTH];

	fseek(flightList,LINE_LENGTH*(lineToMod-1),SEEK_SET);

//	fputs("Salut j'ai niqué la ligne",flightList);
/*	TEST AFFICHAGE LIGNE POUR VOIR OU JE SUIS
	readd = getline(&line, &len, flightList) ;
	printf("%s\n",line  );
*/
	fscanf(flightList,"%s %s %s %s %s %s %d %s",
		flightData.origin,codOrigin,flightData.destination,codDestination,
		flightData.date,flightData.hour,&flightData.numberPass,flightData.code);
/*	printf("%s %s %s %s %s %s %d %s\n",flightData.origin,codOrigin,flightData.destination,codDestination,
		flightData.date,flightData.hour,flightData.numberPass,flightData.code);
*/
	fseek(flightList,LINE_LENGTH*(lineToMod+1),SEEK_SET);
	fputs("2222222222222222222222222222222222222222222222222222222222222222222",flightList);
/*
	fprintf(flightList,"%s %s %s %s %s %s %3d %s",
		flightData.origin,codOrigin,flightData.destination,codDestination,
		flightData.date,flightData.hour,flightData.numberPass-numberPass,&(flightData.code));
*/
	fseek(flightList,LINE_LENGTH*(lineToMod+2),SEEK_SET);
	fputs("3333333333333333333333333333333333333333333333333333333333333333333",flightList);
}

int numberReservation()
{
	FILE* flightList = NULL ;
	char* line = NULL ;
	size_t len = 0 ;
	ssize_t read ;
	flightList = fopen("reservation_list.txt","r");
	int counter = 1 ;

	while ( (read = getline(&line, &len, flightList)) != -1)
	{
		counter++ ;
	}
	return counter ;
}

void writeResData(reservation* tripData)
{
	FILE* resList = NULL ;
	resList = fopen("reservation_list.txt","a");
	
	if(resList == NULL)
		exit(EXIT_FAILURE);

	fprintf(resList,"%d %s %s %s %s %s %s %s %d ",
			tripData->number,tripData->name,tripData->surname,
			tripData->goFlight->code,tripData->goFlight->origin,
			tripData->goFlight->destination,tripData->goFlight->date,
			tripData->goFlight->hour,tripData->goFlight->numberPass);

	if (!noReturn)
	{
		fprintf(resList,"%s %s %s %s %s %d",
		tripData->returnFlight->code,tripData->returnFlight->origin,
		tripData->returnFlight->destination,tripData->returnFlight->date,
		tripData->returnFlight->hour,tripData->returnFlight->numberPass);	
	}	
	
	fprintf(resList,"\n");
	
	fclose(resList) ;
}

void listInfo()
{
	int resNumber = 0 ;
	int cleanbuff ;
	FILE* resList = NULL ;
	char* line    = NULL ;
	size_t len    = 0 ;

	resList = fopen("reservation_list.txt","r");
	
	printf("Reservation number \n");
	scanf("%d",&resNumber);
	while((cleanbuff=getchar())!='\n');

	for (int i = 0; i < resNumber ; i++)
	{
		getline(&line, &len, resList) ;
	}

	if (line == NULL)
	{
		printf("No reservation for this number\n");
	}
	else
	{
		printf("%s\n",line );
	}

}

void modifyInfo()
{
}