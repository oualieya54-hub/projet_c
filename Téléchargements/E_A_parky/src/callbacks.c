#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "parking.h"


    void on_butajouter_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *idparking, *nomparking, *nbplaces, *capacite, *adresse , *labelparky  , *sortieparking ;
    char idpark[20], nb[20], cap[20], nnpark[50], adr[50];

    idparking = lookup_widget(GTK_WIDGET(button), "idparking");
    nomparking = lookup_widget(GTK_WIDGET(button), "nomparking");
    nbplaces = lookup_widget(GTK_WIDGET(button), "nbplaces");
    capacite = lookup_widget(GTK_WIDGET(button), "capacite");
    adresse = lookup_widget(GTK_WIDGET(button), "adresse");
    sortieparking= lookup_widget(GTK_WIDGET(button), "sortieparking");
  
     strcpy(idpark, gtk_entry_get_text(GTK_ENTRY(idparking)));
    strcpy(nnpark, gtk_entry_get_text(GTK_ENTRY(nomparking)));
    strcpy(nb, gtk_entry_get_text(GTK_ENTRY(nbplaces)));
    strcpy(cap, gtk_entry_get_text(GTK_ENTRY(capacite)));
    strcpy(adr, gtk_entry_get_text(GTK_ENTRY(adresse)));

    if (strlen(idpark) == 0 || strlen(nnpark) == 0 || strlen(nb) == 0 || 
        strlen(cap) == 0 || strlen(adr) == 0) {
        g_warning("Veuillez remplir tous les champs correctement");
       gtk_label_set_text(GTK_LABEL(sortieparking)," Veuillez remplir tous les champs correctement");
         return;
   }
    parking p;
    strcpy(p.id,idpark);
    strcpy(p.nomparking,nnpark);
    strcpy(p.capacite,cap);
    strcpy(p.nbplacesdispo,nb);
    strcpy(p.adresse,adr);


  int x=ajouterparking( p  );
  if (x=0){ 
   g_warning("parking non ajouté ");
    return ; }
  if (x=1) {
    g_warning("parking ajouté avec succées ");
   gtk_label_set_text(GTK_LABEL(sortieparking),"Parking ajouté  avec succées ");
     return ;}
  

}

void on_but1modifier_clicked(GtkButton *button, gpointer user_data) {

 GtkWidget *idmodif, *nommodif, *nbplacesmodif, *capacitemodif, *adressemodif, *modiflabel;
 char idparkmodif[20], nbmodif[20], capmodif[20], nnmodif[50], adrmodif[50] ,nb_str[10],cap_str[10];

    idmodif = lookup_widget(GTK_WIDGET(button), "idmodif");
    nommodif = lookup_widget(GTK_WIDGET(button), "nommodif");
    nbplacesmodif = lookup_widget(GTK_WIDGET(button), "nbplacesmodif");
    capacitemodif = lookup_widget(GTK_WIDGET(button), "capacitemodif");
    adressemodif = lookup_widget(GTK_WIDGET(button), "adressemodif");
     modiflabel= lookup_widget(GTK_WIDGET(button), "modiflabel");
    const gchar *idmodiftext= gtk_combo_box_get_active_text(GTK_COMBO_BOX(idmodif));
     strcpy(idparkmodif,idmodiftext);
    strcpy(nnmodif, gtk_entry_get_text(GTK_ENTRY(nommodif)));
   int nb = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(nbplacesmodif));
   sprintf( nb_str, "%d", nb);
    strcpy(nbmodif,nb_str);
     int cap = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(capacitemodif));
   sprintf( cap_str, "%d", cap);
    strcpy(capmodif,cap_str);
    strcpy(adrmodif, gtk_entry_get_text(GTK_ENTRY(adressemodif))) ; 

   
    parking nouv ; 
    strcpy(nouv.id,idparkmodif);
    strcpy(nouv.nomparking,nnmodif);
    strcpy(nouv.capacite,capmodif);
    strcpy(nouv.nbplacesdispo,nbmodif);
    strcpy(nouv.adresse,adrmodif);
    int tr  = modifierparking( idparkmodif , nouv  );

     if (tr==0){ 
     g_warning("parking non modifie ");
    return ; }
      if (tr==1) {
    g_warning("parking modifie avec succées ");
      gtk_label_set_text(GTK_LABEL(modiflabel),"Parking modifié avec succées "); 
     return ;}

}

void
on_butsupprimer_clicked (GtkButton *button, gpointer user_data)
{
   GtkWidget *idsupprimer;
   char idparksupp[10];

  
   idsupprimer = lookup_widget(GTK_WIDGET(button), "idsupprimer");

   if (GTK_IS_COMBO_BOX(idsupprimer)) {
       const gchar *idsupprimertext = gtk_combo_box_get_active_text(GTK_COMBO_BOX(idsupprimer));
       if (idsupprimertext != NULL) {
           strcpy(idparksupp, idsupprimertext);
       } else {
           g_warning("Aucun texte sélectionné dans le combo box");
           return;
       }
   }
   
   
   int tr = supprimerparking(idparksupp);
   if (tr == 1) {
       g_warning("Parking supprimé avec succès.");
       return;
   } else {
       g_warning("Échec de la suppression du parking.");
   }
}

void on_butajouteragent_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget *idagent, *nomagent, *prenomagent, *calendaragent , *sortieagent ;
    char id[50], nnagent[50], pragent[50];
    agent a;
    guint year, month, day;

    
    idagent = lookup_widget(GTK_WIDGET(button), "idagent");
    nomagent = lookup_widget(GTK_WIDGET(button), "nomagent");
    prenomagent = lookup_widget(GTK_WIDGET(button), "prenomagent");
    calendaragent = lookup_widget(GTK_WIDGET(button), "calendaragent");
    sortieagent= lookup_widget(GTK_WIDGET(button), "sortieagent");
   
     const gchar *idagenttext = gtk_combo_box_get_active_text(GTK_COMBO_BOX(idagent));
     strcpy(id, idagenttext);
      

   
    strcpy(nnagent, gtk_entry_get_text(GTK_ENTRY(nomagent)));
    strcpy(pragent, gtk_entry_get_text(GTK_ENTRY(prenomagent)));

 
    gtk_calendar_get_date(GTK_CALENDAR(calendaragent), &year, &month, &day);
    snprintf(a.date.jour, sizeof(a.date.jour), "%02d", day);
    snprintf(a.date.mois, sizeof(a.date.mois), "%02d", month + 1);  
    snprintf(a.date.annee, sizeof(a.date.annee), "%04d", year);


    if (strlen(id) == 0 || strlen(nnagent) == 0 || strlen(pragent) == 0 ||
        strlen(a.date.jour) == 0 || strlen(a.date.mois) == 0 || strlen(a.date.annee) == 0) {
        g_warning("Veuillez remplir tous les champs correctement.");
	 gtk_label_set_text(GTK_LABEL(sortieagent),"Veuillez remplir tous les champs correctement. ");
        return;
    }

    strcpy(a.idagent, id);
    strcpy(a.nomagent, nnagent);
    strcpy(a.prenomagent, pragent);
    strcpy(a.idparking, ""); 

    
    int x = ajouteragent(a);

    if (x == 0) {
        g_warning("Agent non ajouté.");
        return;
    }
    if (x == 1) {
        g_warning("Agent ajouté avec succès.");
	 gtk_label_set_text(GTK_LABEL(sortieagent),"agent ajouté avec succées ");
        return;
    }
}

void on_butaffecter_clicked(GtkWidget *objet, gpointer user_data) {
    
    GtkWidget *idagentaffecter = lookup_widget(objet,"idagentaffecter"); 
    GtkWidget *idparkingaffecter = lookup_widget(objet,"idparkingaffecter");  
	 GtkWidget *sortieaffecter ; 
    char idaffecter[50], idpaffecter[50];
	sortieaffecter= lookup_widget(GTK_WIDGET(objet), "sortieaffecter");
	 strcpy(idaffecter, gtk_combo_box_get_active_text(GTK_COMBO_BOX(idagentaffecter)));
	g_warning("id ag %s\n", idaffecter);
    if (idaffecter == NULL) {
        g_warning("Aucun agent sélectionné.");
        return;
    }
    


    const gchar *idparkingaffectertext = gtk_combo_box_get_active_text(GTK_COMBO_BOX(idparkingaffecter));
    if (idparkingaffectertext == NULL) {
        g_warning("Aucun parking sélectionné.");
        return;
    }
    strcpy(idpaffecter, idparkingaffectertext);
	g_warning("id park %s\n", idpaffecter);
  
    int x = affecteragent(idaffecter, idpaffecter);

    if (x == 0) {
        g_warning("Agent non affecté.");
	 gtk_label_set_text(GTK_LABEL(sortieaffecter),"agent non affecté  ");
    } else if (x == 1) {
        g_print("Agent affecté avec succès.\n");
	 gtk_label_set_text(GTK_LABEL(sortieaffecter),"agent affecté avec succées ");
    } else {
        g_warning("Erreur inconnue lors de l'affectation de l'agent.");
    }
}



void
on_buttonajoutprecedent_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *gererparking , *ajoutdeparking;
ajoutdeparking=lookup_widget(GTK_WIDGET(button),"ajoutdeparking");
 gererparking = create_gererparking ();
gtk_widget_hide(ajoutdeparking);
 gtk_widget_show (gererparking);
}


void
on_buttonsupprimerprecedent_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *gererparking , *supprimerparking;
supprimerparking=lookup_widget(GTK_WIDGET(button),"supprimerparking");
 gererparking = create_gererparking ();
gtk_widget_hide(supprimerparking);
 gtk_widget_show (gererparking);

}


void
on_buttonmodifprecedent_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *gererparking , *modifierparking;
modifierparking=lookup_widget(GTK_WIDGET(button),"modifierparking");
 gererparking = create_gererparking ();
gtk_widget_hide(modifierparking);
 gtk_widget_show (gererparking);
}


void
on_buttonaffichageprecent_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *gererparking , *afficherparking;
afficherparking=lookup_widget(GTK_WIDGET(button),"afficherparking");
 gererparking = create_gererparking ();
gtk_widget_hide(afficherparking);
 gtk_widget_show (gererparking);
}


void
on_buttongereraffectationprecedent__clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *gererparking , *gereraffectation_;
gereraffectation_=lookup_widget(GTK_WIDGET(button),"gereraffectation_");
 gererparking = create_gererparking ();
gtk_widget_hide(gereraffectation_);
 gtk_widget_show (gererparking);
}


void
on_buttonajouterparking_clicked (GtkButton       *button, gpointer         user_data)
{

GtkWidget *ajoutdeparking , *gererparking;

gererparking=lookup_widget(GTK_WIDGET(button),"gererparking");
ajoutdeparking = create_ajoutdeparking ();


gtk_widget_hide(gererparking);
gtk_widget_show (ajoutdeparking);
}


void
on_buttonmodifierparking_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *modifierparking , *gererparking ;
gererparking=lookup_widget(GTK_WIDGET(button),"gererparking");
 modifierparking = create_modifierparking ();
gtk_widget_hide(gererparking);
 gtk_widget_show (modifierparking);
}


void
on_buttonsupprimerparking_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *supprimerparking , *gererparking;
gererparking=lookup_widget(GTK_WIDGET(button),"gererparking");
 supprimerparking = create_supprimerparking ();
 gtk_widget_hide(gererparking);
 gtk_widget_show (supprimerparking);
}


void
on_buttonafficherparking_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *treeviewparking;
	GtkWidget *afficherparking; 
	GtkWidget *gererparking ;
	gererparking=lookup_widget(GTK_WIDGET(button),"gererparking");
	gtk_widget_hide(gererparking);
	afficherparking=lookup_widget(GTK_WIDGET(button),"afficherparking"); 
	afficherparking=create_afficherparking();
	gtk_widget_show(afficherparking);
	treeviewparking=lookup_widget(afficherparking,"treeviewparking");
	afficherparking_(treeviewparking);
}


void
on_button43_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *gereraffectation_ , *gererparking;
gererparking=lookup_widget(GTK_WIDGET(button),"gererparking");
 gereraffectation_ = create_gereraffectation_ ();
 gtk_widget_hide(gererparking);
 gtk_widget_show (gereraffectation_);
}


void
on_buttongereraffectation__clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
 GtkWidget *ajoutagent_ , *gereraffectation_;
gereraffectation_=lookup_widget(GTK_WIDGET(button),"gereraffectation_");

ajoutagent_ = create_ajoutagent_ ();
gtk_widget_hide(gereraffectation_);

 gtk_widget_show (ajoutagent_);
}

void
on_buttongereraffectationafficher_clicked(GtkButton *button,gpointer user_data)
{
        GtkWidget *treeview1;
	GtkWidget *afficheraffectation_ ; 
	GtkWidget *gereraffectation_ ;
	gereraffectation_=lookup_widget(GTK_WIDGET(button),"gereraffectation_");
	gtk_widget_hide(gereraffectation_);
	afficheraffectation_ =lookup_widget(GTK_WIDGET(button),"afficheraffectation_"); 
	afficheraffectation_ =  create_afficheraffectation_();
	gtk_widget_show(afficheraffectation_);
	treeview1=lookup_widget(afficheraffectation_ ,"treeview1");
	afficheragent(treeview1); 
}

void
on_buttongereraffecterunagent__clicked (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *affecteragent_ , *gereraffectation_;
gereraffectation_=lookup_widget(GTK_WIDGET(button),"gereraffectation_");
 affecteragent_ = create_affecteragent_ ();
gtk_widget_hide(gereraffectation_);
 gtk_widget_show (affecteragent_);
}

void
on_buttonaffichagajouter_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *ajoutdeparking , *afficherparking;
afficherparking=lookup_widget(GTK_WIDGET(button),"afficherparking");
ajoutdeparking = create_ajoutdeparking ();
gtk_widget_hide(afficherparking);
 gtk_widget_show (ajoutdeparking);
}


void
on_buttonaffichagemodif_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *modifierparking , *afficherparking;
afficherparking=lookup_widget(GTK_WIDGET(button),"afficherparking");
 modifierparking = create_modifierparking ();
gtk_widget_hide(afficherparking);
 gtk_widget_show (modifierparking);
}


void
on_buttonaffichagesupprimer_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *supprimerparking, *afficherparking;
afficherparking=lookup_widget(GTK_WIDGET(button),"afficherparking");
 supprimerparking = create_supprimerparking ();
gtk_widget_hide(afficherparking);
 gtk_widget_show (supprimerparking);
}


void
on_buttonaaficherajout__clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *ajoutagent_ , *afficheraffectation_;
afficheraffectation_=lookup_widget(GTK_WIDGET(button),"afficheraffectation_");

ajoutagent_ = create_ajoutagent_ ();
gtk_widget_hide(afficheraffectation_);

 gtk_widget_show (ajoutagent_);
}





void
on_affecterprecedent__clicked          (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *affecteragent_ , *gereraffectation_;
affecteragent_=lookup_widget(GTK_WIDGET(button),"affecteragent_");
gereraffectation_ = create_gereraffectation_ ();
gtk_widget_hide(affecteragent_);
 gtk_widget_show (gereraffectation_);;
}


void
on_button30_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *gereraffectation_;
 gereraffectation_ = create_gereraffectation_ ();
 gtk_widget_show (gereraffectation_);
}


void
on_button44_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *ajoutagent_ , *gereraffectation_;
ajoutagent_=lookup_widget(GTK_WIDGET(button),"ajoutagent_");
gereraffectation_ = create_gereraffectation_ ();
gtk_widget_hide(ajoutagent_);
 gtk_widget_show (gereraffectation_);

}


void
on_buttonadministrateur_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *admincitoyen, *login;
admincitoyen=lookup_widget(GTK_WIDGET(button),"admincitoyen");
login = create_login ();
gtk_widget_hide(admincitoyen);
 gtk_widget_show (login);
}


void
on_buttonnouveaucitoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen, *ajoutcitoyen_;

	admincitoyen=lookup_widget(GTK_WIDGET(button),"admincitoyen");
	ajoutcitoyen_= create_ajoutclient__ ();

	gtk_widget_hide(admincitoyen);
	gtk_widget_show (ajoutcitoyen_);
}


void
on_buttonparking_clicked               (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *gerer, *gererparking;

	gerer=lookup_widget(GTK_WIDGET(button),"gerer");
	gererparking = create_gererparking ();

	gtk_widget_hide(gerer);
 	gtk_widget_show (gererparking );
}

void on_button_add_citoyen_clicked(GtkButton *button, gpointer user_data) {
    	GtkWidget *cincitoyen, *prenomcitoyen, *nomcitoyen, *telcitoyen,*ajoutlogin,*ajoutmdp;
	GtkWidget *espace_citoyen,*msg_ajout;
	GtkWidget *ajoutclient_;

    
    char nn[50], pre[50], cin[50], tel[50],msg[50],login[50],mdp[50];
	
	ajoutlogin = lookup_widget(GTK_WIDGET(button), "ajoutlogin");
	ajoutmdp= lookup_widget(GTK_WIDGET(button), "ajoutmdp");
	nomcitoyen = lookup_widget(GTK_WIDGET(button), "nomcitoyen");
	prenomcitoyen = lookup_widget(GTK_WIDGET(button), "prenomcitoyen");
	cincitoyen = lookup_widget(GTK_WIDGET(button), "cincitoyen");
	telcitoyen = lookup_widget(GTK_WIDGET(button), "telcitoyen");
	msg_ajout= lookup_widget(GTK_WIDGET(button), "msg_ajout");

    	strcpy(nn, gtk_entry_get_text(GTK_ENTRY(nomcitoyen)));
    	strcpy(pre, gtk_entry_get_text(GTK_ENTRY(prenomcitoyen)));
    	strcpy(cin, gtk_entry_get_text(GTK_ENTRY(cincitoyen)));
    	strcpy(tel, gtk_entry_get_text(GTK_ENTRY(telcitoyen)));
	strcpy(login, gtk_entry_get_text(GTK_ENTRY(ajoutlogin)));
    	strcpy(mdp, gtk_entry_get_text(GTK_ENTRY(ajoutmdp)));

    	if (strlen(nn) == 0 || strlen(pre) == 0 || strlen(cin) == 0 || strlen(tel) == 0||strlen(login)==0 			||strlen(mdp)==0) {
        gtk_label_set_text(GTK_LABEL(msg_ajout),"veuillez remplir tout les champs");
    		}

    	else{
		Citoyen citoyen;
		strcpy(citoyen.login,login);
    		strcpy(citoyen.mdp,mdp);
    		strcpy(citoyen.id, cin);
    		strcpy(citoyen.nom, nn);
    		strcpy(citoyen.prenom, pre);
    		strcpy(citoyen.tel, tel);

    		int result = ajouterProfil("citoyen.txt",citoyen);
    		if (result == 1) {
   				gtk_label_set_text(GTK_LABEL(msg_ajout),"Félicitation! vous etes bien 	inscrit");

    				}
		}
}



void
on_saveupdate_clicked                  (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *updatenom,*updateprenom,*updatetel,*updateid,*updatesortie,*update_username,*edit_mdp;
	
	char nn[50],pre[50],tel[8],id[50],user[50],mdp[50];
	
	updatenom = lookup_widget(GTK_WIDGET(button), "updatenom");
    	updateprenom= lookup_widget(GTK_WIDGET(button), "updateprenom");
    	updatetel= lookup_widget(GTK_WIDGET(button), "updatetel");
    	updatesortie= lookup_widget(GTK_WIDGET(button), "updatesortie");
	updateid= lookup_widget(GTK_WIDGET(button), "updateid");
	update_username=lookup_widget(GTK_WIDGET(button), "update_username");
	edit_mdp= lookup_widget(GTK_WIDGET(button), "edit_mdp");
	
	
	
	strcpy(nn, gtk_entry_get_text(GTK_ENTRY(updatenom)));
    	strcpy(pre, gtk_entry_get_text(GTK_ENTRY(updateprenom)));
    	strcpy(tel, gtk_entry_get_text(GTK_ENTRY(updatetel)));
	strcpy(id, gtk_entry_get_text(GTK_ENTRY(updateid)));
	strcpy(user, gtk_entry_get_text(GTK_ENTRY(update_username)));
	strcpy(mdp, gtk_entry_get_text(GTK_ENTRY(edit_mdp)));
	
	if (strlen(nn) == 0 || strlen(pre) == 0 || strlen(tel) == 0  || strlen(user) == 0  || strlen(mdp) == 0 )
	{
		gtk_label_set_text(GTK_LABEL(updatesortie),"veuillez remplir tout les champs");
	}
	else
	{
		Citoyen modifier;
		strcpy(modifier.nom,nn);
		strcpy(modifier.prenom,pre);
		strcpy(modifier.tel,tel);
		strcpy(modifier.login,user);
		strcpy(modifier.mdp,mdp);
		strcpy(modifier.id,id);
		if (chercherprofil("citoyen.txt",id)==1)
		{
			modifierProfil("citoyen.txt",id,modifier);
			gtk_label_set_text(GTK_LABEL(updatesortie),"modification faite avec succées");
		}
		else
		{gtk_label_set_text(GTK_LABEL(updatesortie),"id incorrecte");}
	
    	}
}


void
on_del_citoyen_clicked                  (GtkButton       *button,
                                        gpointer         user_data){

	GtkWidget *del_id ,*checkbox_sur,*del_out;
	GtkWidget *admincitoyen, *suppressioncitoyen;

	char id[50];

	del_id= lookup_widget(GTK_WIDGET(button), "del_id");
	checkbox_sur=lookup_widget(GTK_WIDGET(button),"checkbox_sur");
	del_out= lookup_widget(GTK_WIDGET(button), "del_out");
	gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox_sur));

	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbox_sur))){
		gtk_label_set_text(GTK_LABEL(del_out), "veuillez confirmer");}
	else{
		strcpy(id, gtk_entry_get_text(GTK_ENTRY(del_id)));
   
		int x=supprimerProfil("citoyen.txt", id);
		if (x==1) {
			suppressioncitoyen=lookup_widget(GTK_WIDGET(button),"suppressioncitoyen");
			admincitoyen= create_admincitoyen();
			gtk_widget_hide(suppressioncitoyen);
 			gtk_widget_show (admincitoyen);
        
    			}
		 else {
        		gtk_label_set_text(GTK_LABEL(del_out), "Erreur lors de la suppression");
   			 }
			}
}

void
on_boutton_afficher_citoyen_clicked    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *afficher_citoyen;
	GtkWidget *espace_citoyen;

	espace_citoyen=lookup_widget(GTK_WIDGET(button),"espace_citoyen");
	afficher_citoyen=create_afficher_citoyen();

	gtk_widget_hide(espace_citoyen);
	gtk_widget_show(afficher_citoyen); 

}




void
on_button_supprimer_espace_citoyen_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *suppressioncitoyen;

	espace_citoyen=lookup_widget(GTK_WIDGET(button),"espace_citoyen");
	suppressioncitoyen = create_suppressioncitoyen();

	gtk_widget_hide(espace_citoyen);
	gtk_widget_show(suppressioncitoyen);
}


void
on_button_modifier_espace_citoyen_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *espace_citoyen;
GtkWidget *modifiercitoyen;
espace_citoyen=lookup_widget(GTK_WIDGET(button),"espace_citoyen");
modifiercitoyen= create_modifiercitoyen();

gtk_widget_hide(espace_citoyen);
gtk_widget_show(modifiercitoyen);
}





void
on_home_supprimer_citoyen_clicked      (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *suppressioncitoyen;

	suppressioncitoyen=lookup_widget(GTK_WIDGET(button),"suppressioncitoyen");
	espace_citoyen= create_espace_citoyen();

	gtk_widget_hide(suppressioncitoyen);
	gtk_widget_show(espace_citoyen);
}


void
on_home_afficher_citoyen_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *afficher_citoyen;

	afficher_citoyen=lookup_widget(GTK_WIDGET(button),"afficher_citoyen");
	espace_citoyen= create_espace_citoyen();

	gtk_widget_hide(afficher_citoyen);
	gtk_widget_show(espace_citoyen);
}


void
on_home_calculer_facture_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *calculer_facture;

	calculer_facture=lookup_widget(GTK_WIDGET(button),"calculer_facture");
	espace_citoyen= create_espace_citoyen();

	gtk_widget_hide(calculer_facture);
	gtk_widget_show(espace_citoyen);
}


void
on_home_modifier_citoyen_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *modifiercitoyen;

	modifiercitoyen=lookup_widget(GTK_WIDGET(button),"modifiercitoyen");
	espace_citoyen= create_espace_citoyen();

	gtk_widget_hide(modifiercitoyen);
	gtk_widget_show(espace_citoyen);
}


void
on_espace_calculer_facture_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *calculer_facture;

	espace_citoyen=lookup_widget(GTK_WIDGET(button),"espace_citoyen");
	calculer_facture= create_calculer_facture();

	gtk_widget_hide(espace_citoyen);
	gtk_widget_show(calculer_facture);
}


void
on_buttonanciencitoyen_clicked         (GtkButton       *button,
                                        gpointer         user_data)

{	
	GtkWidget *admincitoyen,*logincitoyen;

	admincitoyen=lookup_widget(GTK_WIDGET(button),"admincitoyen");
	logincitoyen= create_logincitoyen();

	gtk_widget_hide(admincitoyen);
	gtk_widget_show(logincitoyen);
}


void
on_button_login_citoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *login_citoyen,*mdp_citoyen,*msg_logincitoyen;
	GtkWidget *espace_citoyen,*logincitoyen;
	
	char login[50],mdp[50];
	
	login_citoyen= lookup_widget(GTK_WIDGET(button), "login_citoyen");
    	mdp_citoyen= lookup_widget(GTK_WIDGET(button), "mdp_citoyen");
	msg_logincitoyen= lookup_widget(GTK_WIDGET(button), "msg_logincitoyen");
	
	strcpy(login, gtk_entry_get_text(GTK_ENTRY(login_citoyen)));
    	strcpy(mdp, gtk_entry_get_text(GTK_ENTRY(mdp_citoyen)));
    	if (strlen(login) == 0 || strlen(mdp) == 0  )
	{
		gtk_label_set_text(GTK_LABEL(msg_logincitoyen),"veuillez remplir tout \n les champs correctement!");
	}
	
	int x=verifier_connexion( login, mdp );
	if (x==1){
		logincitoyen=lookup_widget(GTK_WIDGET(button),"logincitoyen");
		espace_citoyen= create_espace_citoyen();

		gtk_widget_hide(logincitoyen);
		gtk_widget_show(espace_citoyen);
		}
	if (x==-1){gtk_label_set_text(GTK_LABEL(msg_logincitoyen),"compte innexsistant");
	}
}


void
on_retour_init_clicked                 (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*logincitoyen;

	logincitoyen=lookup_widget(GTK_WIDGET(button),"logincitoyen");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(logincitoyen);
	gtk_widget_show(admincitoyen);
}


void
on_deconect_espace_citoyen_clicked     (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*espace_citoyen;

	espace_citoyen=lookup_widget(GTK_WIDGET(button),"espace_citoyen");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(espace_citoyen);
	gtk_widget_show(admincitoyen);
}





void
on_cnx_loginadmin_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *loginadmin,*mdpadmin,*msg_loginadmin;
	GtkWidget *gerer,*login;
	
	char log[50],mdp[50];
	
	loginadmin= lookup_widget(GTK_WIDGET(button), "loginadmin");
    	mdpadmin= lookup_widget(GTK_WIDGET(button), "mdpadmin");
	msg_loginadmin= lookup_widget(GTK_WIDGET(button), "msg_loginadmin");
	
	strcpy(log, gtk_entry_get_text(GTK_ENTRY(loginadmin)));
    	strcpy(mdp, gtk_entry_get_text(GTK_ENTRY(mdpadmin)));
    	if (strlen(log) == 0 || strlen(mdp) == 0  )
	{
		gtk_label_set_text(GTK_LABEL(msg_loginadmin),"veuillez remplir tout \n les champs correctement!");
	}
	
	int x=verif_conx_admin( log, mdp );
	if (x==1){
		login=lookup_widget(GTK_WIDGET(button),"login");
		gerer= create_gerer();

		gtk_widget_hide(login);
		gtk_widget_show(gerer);
		}
	if (x==-1){gtk_label_set_text(GTK_LABEL(msg_loginadmin),"compte innexsistant");
	}
}



void
on_retour_admin_init_clicked           (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*login;
	login=lookup_widget(GTK_WIDGET(button),"login");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(login);
	gtk_widget_show(admincitoyen);
}


void
on_button_retour_ladmincitoyen_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*gerer;
	gerer=lookup_widget(GTK_WIDGET(button),"gerer");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(gerer);
	gtk_widget_show(admincitoyen);
}


void
on_retour_ajout_citoyen_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *espace_citoyen;
	GtkWidget *ajoutclient__;

	ajoutclient__=lookup_widget(GTK_WIDGET(button),"ajoutclient__");
	espace_citoyen= create_espace_citoyen();

	gtk_widget_hide(ajoutclient__);
	gtk_widget_show(espace_citoyen);
	gtk_widget_hide(ajoutclient__);
}


void
on_dcnx_du_calcul_clicked              (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*afficher_citoyen;

	afficher_citoyen=lookup_widget(GTK_WIDGET(button),"afficher_citoyen");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(afficher_citoyen);
	gtk_widget_show(admincitoyen);
}




void
on_button45_clicked                    (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *ajoutagent_ , *affecteragent_;

	ajoutagent_=lookup_widget(GTK_WIDGET(button),"ajoutagent_");
	affecteragent_ = create_affecteragent_ ();

	gtk_widget_hide(ajoutagent_);
	gtk_widget_show (affecteragent_);

}




void
on_boutton_calculer_fact_clicked       (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *sortie_cout, *mois_facture, *annee_facture, *read_id;
    	int mois, annee;
	char *id;
    	char result[50]; 

	mois_facture = lookup_widget(GTK_WIDGET(button), "mois_facture");
	annee_facture = lookup_widget(GTK_WIDGET(button), "annee_facture");
	read_id = lookup_widget(GTK_WIDGET(button), "read_id");
	sortie_cout = lookup_widget(GTK_WIDGET(button), "sortie_cout");

	mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(mois_facture));
	annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(annee_facture));
	id = (char *)gtk_entry_get_text(GTK_ENTRY(read_id));

    
    	float facture = calculerFactureMensuelle(id, mois, annee);

    	if (facture == -1.0) {
        	snprintf(result, sizeof(result), "Aucune réservation trouvée !");
    			} 
	else {
        	snprintf(result, sizeof(result), "%.2f TND", facture);
	
    		}

    gtk_label_set_text(GTK_LABEL(sortie_cout), result);
}


void
on_deconect_facture_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *admincitoyen,*calculer_facture;

	calculer_facture=lookup_widget(GTK_WIDGET(button),"calculer_facture");
	admincitoyen= create_admincitoyen();

	gtk_widget_hide(calculer_facture);
	gtk_widget_show(admincitoyen);
}


void on_affiche_fact_clicked(GtkButton *button, gpointer user_data) {
	GtkWidget *read_id, *treeviewfact,*msg;
	char id[10];
	GtkTreeIter iter;
	msg= lookup_widget(GTK_WIDGET(button), "msg");
	read_id = lookup_widget(GTK_WIDGET(button), "read_id");
	if (!read_id || !GTK_IS_ENTRY(read_id)) {
	gtk_label_set_text(GTK_LABEL(msg),"invalid id");
		
	    }

	treeviewfact = lookup_widget(GTK_WIDGET(button), "treeviewfact");

	const char *id_text = gtk_entry_get_text(GTK_ENTRY(read_id));
	if (!id_text || strlen(id_text) == 0) {
	gtk_label_set_text(GTK_LABEL(msg),"invalid id");
		
	    }
	strcpy(id, id_text);
	GtkListStore *store = gtk_list_store_new(COLUMNNS, G_TYPE_STRING, G_TYPE_INT, 
G_TYPE_INT,G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING);

	    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewfact), GTK_TREE_MODEL(store));

	    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeviewfact)) == 0) {
		GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
		GtkTreeViewColumn *column;

		column = gtk_tree_view_column_new_with_attributes("ID Reservation", renderer, "text", IDRESERVATION, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);

		column = gtk_tree_view_column_new_with_attributes("Mois", renderer, "text", MOIS, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);

		column = gtk_tree_view_column_new_with_attributes("Jour", renderer, "text", JOUR, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);

		column = gtk_tree_view_column_new_with_attributes("Annee", renderer, "text", ANNEE, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);

		column = gtk_tree_view_column_new_with_attributes("Service", renderer, "text", LIBELLE, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);

		column = gtk_tree_view_column_new_with_attributes("Cout", renderer, "text", COUT, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(treeviewfact), column);
	    }

	    FILE *f = fopen("reservations.txt", "r");
	    if (!f) {
		g_warning("Erreur : Impossible d'ouvrir le fichier 'reservations.txt' !");
		return;
	    }

	    char id_reservation[50], idClient[50], idParking[50], nomParking[50], id_park[50], id1[50], service[50], cout[50];
	    int placesDisponibles, mois, jour, annee;

	 
	    gtk_list_store_clear(store);

	    while (fscanf(f, "%s %s %d %d %d %s %d %s %s %s %s %s \n", id_reservation, idClient, &jour, &mois, &annee, idParking, &placesDisponibles, nomParking, id_park, id1, service, cout) != EOF) {
		if (chercherreservation("reservations.txt",id)==1) { 
		    gtk_list_store_append(store, &iter);
		    gtk_list_store_set(store, &iter,IDRESERVATION, id_reservation,MOIS, mois,JOUR, jour,ANNEE, annee,LIBELLE, service,COUT, cout, -1);
		}
	    }

	    fclose(f);

	    gtk_tree_view_set_model(GTK_TREE_VIEW(treeviewfact), GTK_TREE_MODEL(store));
	    g_object_unref(store); 
	}

void on_Affiche_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *afficher_citoyen, *id_affiche, *treeview1, *output;
    char id[10];
    afficher_citoyen = lookup_widget(GTK_WIDGET(button), "afficher_citoyen");
    treeview1 = lookup_widget(afficher_citoyen, "treeview1");
    output = lookup_widget(GTK_WIDGET(button), "output");
    id_affiche = lookup_widget(GTK_WIDGET(button), "id_affiche");


    const char *id_text = gtk_entry_get_text(GTK_ENTRY(id_affiche));
    if (!id_text || strlen(id_text) == 0) {
        gtk_label_set_text(GTK_LABEL(output), "ID invalide.");
        return;
    }
    strcpy(id, id_text);

   
    GtkListStore *store = NULL;
    if (GTK_IS_TREE_VIEW(treeview1)) {
        store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview1)));
    }
    if (!store) {
        store = gtk_list_store_new(6, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,
                                   G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview1), GTK_TREE_MODEL(store));
    }


    if (store) {
        gtk_list_store_clear(store);
    }

    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview1)) == 0) {
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *column;

        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Login", renderer, "text", 0, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

        column = gtk_tree_view_column_new_with_attributes("Mot de passe", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

        column = gtk_tree_view_column_new_with_attributes("CIN", renderer, "text", 2, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

        column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 3, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

        column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 4, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);

        column = gtk_tree_view_column_new_with_attributes("Téléphone", renderer, "text", 5, NULL);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview1), column);
    }


    FILE *f = fopen("citoyen.txt", "r");
    if (!f) {
        gtk_label_set_text(GTK_LABEL(output), "Erreur : Impossible d'ouvrir le fichier.");
        return;
    }

    char nom[50], prenom[50], tel[30], cin[50], login[50], mdp[50];
    GtkTreeIter iter;
    int found = 0;
    while (fscanf(f, "%s %s %s %s %s %s", login, mdp, nom, prenom, cin, tel) == 6) {
        if (strcmp(id, cin) == 0) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, login,1, mdp,2, cin,3, nom, 4, prenom, 5, tel,-1);
            found = 1;
        }
    }
    fclose(f);

 
    if (!found) {
        gtk_label_set_text(GTK_LABEL(output), "Aucun citoyen trouvé avec cet ID.");
    } else {
        gtk_label_set_text(GTK_LABEL(output), "Citoyen trouvé !");
    }

  
    gtk_widget_show_all(afficher_citoyen);
}
