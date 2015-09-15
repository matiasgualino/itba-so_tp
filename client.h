#ifndef _CLIENT_
#define _CLIENT_
#define MAX_LENGTH 	300

// STRUCTURES //

// Structure containing all the info
// for each booked flight 
typedef struct info_flight info_flight; 
struct info_flight
{
	char origin[MAX_LENGTH] ;
	char destination[MAX_LENGTH] ;
	char date[MAX_LENGTH];
	char hour[MAX_LENGTH];
	char *flightCode;
	int numberPass ;
}; 

// Structure containing a reservation number
// and the info for each flight (if round flight)
typedef struct reservation reservation ; 
struct reservation
{
	int resNumber ;
	info_flight goFlight ;
	info_flight returnFlight ;
};

// FUNCTIONS PROTOTYPE //
void welcomeMessage(void);
void getInfo();
int stringParsing(char *string);
char* selectFlight(info_flight flightData);
void listInfo(void);
void modifyInfo(void);




// GLOBAL VARIABLES //
char* destinations[2][8]=
{
{"Buenos_Aires","Cordoba","Mendoza","Salta","Bariloche","El_Calafate","Ushuaia","Iguazu"},
{"AEP","COR","MDZ","SLA","BRC","FTE","USH","IGR"}
} ;

#endif