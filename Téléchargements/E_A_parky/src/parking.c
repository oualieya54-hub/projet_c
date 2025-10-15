#include "parking.h"
#include <string.h>
#include <gtk/gtk.h>

int ajouterparking( parking p )
{
    FILE * f=fopen("parking.txt", "a");
    if(f!=NULL)
    {
        fprintf(f,"%s %s %s %s %s \n",p.id,p.nomparking,p.capacite,p.nbplacesdispo,p.adresse);
        fclose(f);
        return 1;
    }
    else return 0;
}
int modifierparking(  char id[10], parking nouv )
{
    int tr=0;
    parking p ; 
    FILE * f=fopen("parking.txt", "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %s %s \n",p.id,p.nomparking,p.capacite,p.nbplacesdispo,p.adresse)!=EOF)
        {
          if (strcmp(p.id, id) == 0)

            {
                fprintf(f2,"%s %s %s %s %s \n",nouv.id,nouv.nomparking,nouv.capacite,nouv.nbplacesdispo,nouv.adresse);
                tr=1;
            }
            else
                fprintf(f2,"%s %s %s %s %s \n",p.id,p.nomparking,p.capacite,p.nbplacesdispo,p.adresse);

        }
    }
    fclose(f);
    fclose(f2);
    remove("parking.txt");
    rename("nouv.txt", "parking.txt");
    return tr;

}
int supprimerparking( char id[10])
{
    int tr=0;
    parking p;
    FILE * f=fopen("parking.txt", "r");
    FILE * f2=fopen("nouv.txt", "w");
    if(f!=NULL && f2!=NULL)
    {
        while(fscanf(f,"%s %s %s %s %s \n",p.id,p.nomparking,p.capacite,p.nbplacesdispo,p.adresse)!=EOF)
        {
            if (strcmp(p.id, id) == 0)

                tr=1;
            else
                fprintf(f2,"%s %s %s %s %s \n",p.id,p.nomparking,p.capacite,p.nbplacesdispo,p.adresse);
        }
    }
    fclose(f);
    fclose(f2);
    remove("parking.txt");
    rename("nouv.txt", "parking.txt");
    return tr;
}
void afficherparking_(GtkWidget *liste){
	GtkCellRenderer *renderer; 
        GtkTreeViewColumn * column ; 
	GtkTreeIter iter ; 

	GtkListStore *store ; 

        char id[50];
	char nomparking[50]; 
	char  capacite[50];
	char nbplacesdispo[50];
	char adresse[100];

	store=NULL ; 

         FILE * f;
	  
         store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste))); 
 	 if (store==NULL)
	{ 
	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("id " , renderer , "text" ,ID,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);
 
        renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("nomparking " , renderer , "text" , Nom_parking ,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

 	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("capacite" , renderer , "text" ,Capacite,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("nbplacesdispo " , renderer , "text" ,Nb_places_disponibles,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("adresse" , renderer , "text" ,Adresse,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	store=gtk_list_store_new(Columns , G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	
	f=fopen("parking.txt","r"); 
  	if (f=NULL)
	{
		return; 
	}
	else 
	{ 
		f=fopen("parking.txt","a+") ; 
	
	while(fscanf(f,"%s %s %s %s %s \n",id,nomparking,capacite,nbplacesdispo,adresse)!=EOF){
	gtk_list_store_append(store,&iter); 
	gtk_list_store_set(store,&iter,ID,id, Nom_parking,nomparking ,Capacite ,capacite ,Nb_places_disponibles , nbplacesdispo, Adresse,adresse ,-1 ) ;
	} 
	  fclose(f); 
	gtk_tree_view_set_model(GTK_TREE_VIEW(liste),GTK_TREE_MODEL(store));
	g_object_unref(store);
        }
 }
}

int ajouteragent( agent a) {
    FILE *f = fopen("agent.txt", "a");
    if (f != NULL) {
        if (strlen(a.idparking) == 0) {
           fprintf(f, "%s %s %s %s %s %s NULL\n",
                a.idagent, a.nomagent, a.prenomagent,
                a.date.jour, a.date.mois, a.date.annee);
    } else {
        fprintf(f, "%s %s %s %s %s %s %s\n",
                a.idagent, a.nomagent, a.prenomagent,
                a.date.jour, a.date.mois, a.date.annee,
                a.idparking);
    }
}

    fclose(f);
    return 1;
}


int affecteragent( char idagent[50], char idparking[50]) {
    FILE *f = fopen("agent.txt", "r");
 
    FILE *temp = fopen("temp.txt", "w");
   

    agent a;
    int found = 0;
    if (f!= NULL){
    while (fscanf(f, "%s %s %s %s %s %s %s\n",
                  a.idagent, a.nomagent, a.prenomagent,
                  a.date.jour, a.date.mois, a.date.annee,
                  a.idparking) != EOF) {
		
        if (strcmp(a.idagent, idagent) == 0 ) {
            strcpy(a.idparking, idparking);
		printf("idpark %s\n",a.idparking);
            found = 1;
        }
        fprintf(temp, "%s %s %s %s %s %s %s\n", a.idagent, a.nomagent, a.prenomagent,
                a.date.jour, a.date.mois, a.date.annee, a.idparking);
    }}

    fclose(f);
    fclose(temp);

    if (found==1){
	 remove("agent.txt");
         rename("temp.txt", "agent.txt");
        return 1;
    } else {
        printf("Agent non trouvé ou déjà affecté.\n");
        remove("temp.txt");
        return 0;
    }
}




void afficheragent(GtkWidget *liste) {
        GtkCellRenderer *renderer; 
        GtkTreeViewColumn * column ; 
	GtkTreeIter iter ; 

	GtkListStore *store ; 

        char idagent[50];
	char nomagent[50]; 
	char  prenomagent[50];
	char date[50];
	char idparking[100];
	char jour [10]; 
	char mois[10]; 
	char annee[10]; 
	

	store=NULL ; 

         FILE * f;
	  
         store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(liste))); 
 	 if (store==NULL)
	{ 
	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("idagent " , renderer , "text" ,ID_agent,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);
 
        renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("nomagent " , renderer , "text" , Nom_agent ,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

 	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("prenomagent" , renderer , "text" ,Prenom_agent,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("jour  " , renderer , "text" ,Jour,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("mois  " , renderer , "text" ,Mois,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("annee  " , renderer , "text" ,Annee,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	renderer=gtk_cell_renderer_text_new(); 
	column=gtk_tree_view_column_new_with_attributes("idparking" , renderer , "text" ,ID_parking,NULL); 
	gtk_tree_view_append_column(GTK_TREE_VIEW(liste),column);

	store=gtk_list_store_new(COLUMNS , G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);
	
	f=fopen("agent.txt","r"); 
  	if (f=NULL)
	{
		return; 
	}
	else 
	{ 
		f=fopen("agent.txt","a+") ; 
	
	while(fscanf(f,"%s %s %s %s %s %s %s \n",idagent,nomagent,prenomagent,jour,mois,annee,idparking)!=EOF){
	gtk_list_store_append(store,&iter); 
	gtk_list_store_set(store,&iter,ID_agent ,idagent, Nom_agent,nomagent ,Prenom_agent ,prenomagent ,Jour , jour , Mois , mois ,Annee,annee ,ID_parking,idparking , -1 );}                       
	  fclose(f); 
	gtk_tree_view_set_model(GTK_TREE_VIEW(liste),GTK_TREE_MODEL(store));
	g_object_unref(store);
        }
}}
int verif_conx_admin( char login[50],char mdp[50] ){
	int verifier=0;
	connexion cnx;
	FILE* f;

	f=fopen("comptes.txt", "r");

	if(f!=NULL)
		{
	 while (fscanf(f, "%s %s\n",
		cnx.login,cnx.mdp) != EOF) 
				{        
				if(strcmp(cnx.login,login)==0 && strcmp(cnx.mdp,mdp)==0 )
        				{
					verifier= 1;
					
					}
				else{verifier=-1;}
				}
		}

	fclose(f);

	return verifier;
	}

///////////////////////////////////////////////////////////////
int ajouterProfil(char *filename,Citoyen citoyen) {
    	FILE *f = fopen("citoyen.txt", "a");
 	
    	if (f != NULL) {

        		fprintf(f, "%s %s %s %s %s %s\n",citoyen.login,citoyen.mdp, citoyen.nom, citoyen.prenom,citoyen.id, citoyen.tel);
			fclose(f);
    			return 1;} 	
			
	
        return 0 ; }
   
int modifierProfil(char *filename, char *id, Citoyen temp) {
    FILE *f = fopen(filename, "r");
    FILE *f2 = fopen("nouv.txt", "w");
    int trouv = 0;
    Citoyen citoyen;

    if (f == NULL || f2 == NULL) {
        g_warning("Erreur d'ouverture de fichier");
        return 0;
    }

    while (fscanf(f, "%s %s %s %s %s %s \n", citoyen.login, citoyen.mdp, citoyen.nom, citoyen.prenom, citoyen.id, citoyen.tel) != EOF) {
        if (strcmp(citoyen.id, id) == 0) {
            fprintf(f2, "%s %s %s %s %s %s \n", temp.login, temp.mdp, temp.nom, temp.prenom, temp.id, temp.tel);
            trouv = 1;
        } else {
            fprintf(f2, "%s %s %s %s %s %s \n", citoyen.login, citoyen.mdp, citoyen.nom, citoyen.prenom, citoyen.id, citoyen.tel);
        }
    }

    fclose(f);
    fclose(f2);

    if (trouv) {
        remove(filename);
        rename("nouv.txt", filename);
    } else {
        remove("nouv.txt");
    }

    return trouv;
}
int supprimerProfil(char *filename, char *id) {
    	FILE *f = fopen(filename, "r");
    	FILE *f2 = fopen("nouv.txt", "w");
    	int trouv = 0;
    	Citoyen citoyen;

    	if (f != NULL && f2 != NULL){
		while (fscanf(f, "%s %s %s %s %s %s\n",
citoyen.login,citoyen.mdp, citoyen.nom, citoyen.prenom,citoyen.id, citoyen.tel) != EOF) {

            		if (strcmp(citoyen.id, id)==0) {
               			trouv = 1;}
            		else {
            fprintf(f2,  "%s %s %s %s %s %s\n",
citoyen.login,citoyen.mdp, citoyen.nom, citoyen.prenom,citoyen.id, citoyen.tel);}
            }}

    fclose(f);
    fclose(f2);
    remove(filename);
    rename("nouv.txt", filename);
    return trouv;
}

int chercherprofil(char *filename, char *id){
	FILE *f = fopen("citoyen.txt", "r");
	Citoyen existing;
    	int trouv = -1;
    	if (f != NULL) {
        	while (fscanf(f, "%s %s %s %s %s %s\n",existing.login,existing.mdp,existing.nom, existing.prenom, existing.id,existing.tel) != EOF) {
            		if (strcmp(existing.id,id) == 0) {
				trouv=1;
			}
		}
	}
	fclose(f);
	return trouv;
		}
float calculerFactureMensuelle(char *id, int mois, int annee) {
    FILE *f = fopen("reservations.txt", "r");

    if (f == NULL) {
        perror("Erreur d'ouverture du fichier");
        return -1.0; 
    }

    float total = 0.0;
    Reservations reservation;

    while (fscanf(f, "%s %s %d %d %d %s %d %s %s %s %s %s \n", 
                  reservation.idReservation, 
                  reservation.idClient, 
                  &reservation.date.jour, 
                  &reservation.date.mois, 
                  &reservation.date.annee,
                  
                  reservation.parking.idParking,
                  &reservation.parking.placesDisponibles,
                  reservation.parking.nomParking,
                  reservation.service.id_park,
                  reservation.service.id,
                  reservation.service.libelle,
                  reservation.service.cout) != EOF) {
        
        // Debug: afficher toutes les valeurs lues
        printf("ID Client: %s, Mois: %d, Année: %d, Cout: %s\n", 
               reservation.idClient, reservation.date.mois, reservation.date.annee, reservation.service.cout);

        // Vérification de la comparaison de l'ID et de la date
        if (strcmp(reservation.idClient, id) == 0 && 
            reservation.date.annee == annee && 
            reservation.date.mois == mois) {
            
            // Debug: afficher le montant de chaque service avant de l'ajouter
            float montant = atof(reservation.service.cout);
            printf("Montant du service: %.2f\n", montant);

            total += montant;  // Ajout du montant au total
            printf("Total après ajout: %.2f\n", total);  // Affichage du total après chaque ajout
        }
    }

    fclose(f);

    // Debug: afficher le total final calculé
    printf("Total final calculé: %.2f\n", total);
    return total;
}
int verifier_connexion( char login[50], char mdp[50] )
{	
	int verifier=0;
	Citoyen citoyen;
	FILE* f;

	f=fopen("citoyen.txt", "r");

	if(f!=NULL)
		{
	 while (fscanf(f, "%s %s %s %s %s %s\n",
		citoyen.login,citoyen.mdp,citoyen.id,citoyen.nom, citoyen.prenom,  citoyen.tel) != EOF) 
				{        
				if(strcmp(citoyen.login,login)==0 && strcmp(citoyen.mdp,mdp)==0 )
        				{
					verifier= 1;
					
					}
				else{verifier=-1;}
				}
		}

	fclose(f);

	return verifier;

}

int chercherreservation(char *filename, char id[10]){
FILE *f = fopen("reservations.txt", "r");
	Reservations reserv;
    	int trouv = -1;
    	if (f != NULL) {
        	while (fscanf(f, "%s %s %d %d %d %s %d %s %s %s %s %s \n", reserv.idReservation, 	reserv.idClient,&reserv.date.jour,&reserv.date.mois,&reserv.date.annee, reserv.parking.idParking,&reserv.parking.placesDisponibles,reserv.parking.nomParking,
reserv.service.id_park,reserv.service.id,reserv.service.libelle,
reserv.service.cout ) != EOF) {
            		if (strcmp(reserv.idClient,id) == 0) {
				trouv=1;
			
		}
	}
	fclose(f);
	return trouv;}}
