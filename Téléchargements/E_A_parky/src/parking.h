#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
#include <stdio.h>
#include <gtk/gtk.h>
enum {
ID, 
Nom_parking,
Capacite,
Nb_places_disponibles, 
Adresse,
Columns
};
enum {
ID_agent, 
Nom_agent,
Prenom_agent,
Jour, 
Mois , 
Annee,
ID_parking,
COLUMNS
};

typedef struct {
	char jour[10]; 
	char mois[10]; 
	char annee[10]; 
}date ; 
 
typedef struct{
	char idagent [50]; 
	char nomagent[50]; 
	char prenomagent[50];
	date date;
	char idparking[50];
        
}agent;

typedef struct{
	char id[50];
	char nomparking[50]; 
	char  capacite[50];
	char nbplacesdispo[50];
	char adresse[100]; 
}parking;

int ajouterparking( parking p );
int modifierparking(char id[10], parking nouv);
int supprimerparking( char id[10] );
void afficherparking_(GtkWidget  *liste);
int ajouteragent( agent a );
int affecteragent( char idparking[50], char idagent[50]) ;
void afficheragent(GtkWidget *liste);


enum{
LOGIN,
MDP,
ID_C,
NOM,
PRENOM,
TEL,
COLUMNSS};
enum{
IDRESERVATION,
MOIS,
JOUR,
ANNEE,
LIBELLE,
COUT,
COLUMNNS};
typedef struct {
char login[50];
char mdp[50];
char nom[50];
char prenom[50];
char id[50];
char tel[50];

} Citoyen;
typedef struct {
	int jour; 
	int mois; 
	int annee; 
}datee ; 
 

typedef struct{
char idParking[50];
int placesDisponibles;
char nomParking[50];
}Parking;
typedef struct{
char id_park[50];
char id[10];
char libelle[50];
char cout[50];
}service;
typedef struct{
char idReservation[50];
char idClient[50];
datee date;
Parking parking;
service service ;
}Reservations;
typedef struct{
char login[50];
char mdp[50];
}connexion;
int verifier_connexion( char login[10], char mdp[10] );
int verif_conx_admin( char login[50],char mdp[50] );

int ajouterProfil(char *filename, Citoyen citoyen);
int modifierProfil(char *filename, char*id, Citoyen temp);
int supprimerProfil(char *filename, char *id);
float calculerFactureMensuelle(char *id, int mois, int annee);
int chercherprofil(char *filename, char id[10]);
int chercherreservation(char *filename, char id[10]);



#endif // POINT_H_INCLUDED
