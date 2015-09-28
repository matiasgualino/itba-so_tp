#ifndef _CLIENT_
#define _CLIENT_
#define MAX_LENGTH 	300

// STRUCTURES
/* Structure containing all the info
for each booked flight 				*/
typedef struct flight flight; 
struct flight
{
	char origin[MAX_LENGTH];
	char destination[MAX_LENGTH] ;
	char date[MAX_LENGTH];
	char hour[MAX_LENGTH];
	char* code;
	int numberPass ;
}; 

/* Structure containing a reservation number,
the name and surname of the booker and the
info for each flight (if round flight) 	*/
typedef struct reservation reservation ; 
struct reservation
{
	int number ;
	char name[MAX_LENGTH] ;
	char surname[MAX_LENGTH];
	flight* goFlight ;
	flight* returnFlight ;
};

// FUNCTIONS PROTOTYPE

flight* createFlight() ;
reservation* createReservation(flight* goFlight, flight* returnFlight);
void welcomeMessage(reservation* tripData);
void getInfo(reservation* tripData);
int stringParsing(char *string, reservation* tripData, int mode);
char* selectFlight(flight* flightData);
int codeToInt(char* r_code);
void modifyNumberPass(int lineToMod, int numberPass);
int numberReservation();
void writeResData(reservation* tripData);
void listInfo();
void modifyInfo();


// GLOBAL VARIABLES 
char* destinations[2][8]=
{
{"Buenos_Aires","Cordoba","Mendoza","Salta","Bariloche","El_Calafate","Ushuaia","Iguazu"},
{"AEP","COR","MDZ","SLA","BRC","FTE","USH","IGR"}
} ;

#endif