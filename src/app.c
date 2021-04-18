#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"

#define MAXVILLES 68
#define BOXNUMBER 101
#define BUTTONNUMBER 126
#define ENTRYNUMBER 77
#define FIXEDNUMBER 6
#define COMBONUMBER 3
#define TREESTORENUMBER 22
#define TREEVIEWNUMBER 22
#define TREEVIEWCOLUMNNUMBER 90
#define TREESELECTIONNUMBER 22
#define CELLRENDERERNUMBER 90
#define VIEWNUMBER 44
#define ITERNUMBER 22
//GLOBAL VARIABLES
Ville globalVilles[MAXVILLES];
int patientCount;
int contactCount;
char cinDecesSupprimer[10]={0};
char cinDecesRecherche[10]={0};
char cinGuerisSupprimer[10]={0};
char cinGuerisRecherche[10]={0};
char cinSupprimer[10]={0};
char cinRecherche[10]={0};
char cinPatient[10]={0};
char villePatient[16]={0};
char secteurPatient[20]={0};
char nomPatient[21]={0};
char prenomPatient[21]={0};
int boolChargePatient;
Etat etatPatient=0;
Date globalDateGuerison;
Date globalDateDeces;
Date dateGuerisonModif;
Date dateDecesModif;
Date dateGuerionAffichage;
Date datedDecesAffichage;

char cinPatAssocie[10]={0};
char villeContact[16]={0};
char secteurContact[20]={0};
char nomContact[21]={0};
char prenomContact[21]={0};
char globalCinContact[10]={0};
char cinContactRecherche[10]={0};
char cinContactSupprimer[10]={0};

char villeAffichage[16]={0};
char secteurAffichage[20]={0};
Etat etatAffichage=0;

int cellColumnAssociationsBools[TREEVIEWNUMBER]={0};
//GTK GLOBAL VARIABLES
GtkWidget* window;
GtkWidget* stack;
GtkWidget* checkbutton1;
GtkWidget* checkbutton2;
GtkWidget* boxes[BOXNUMBER];
GtkWidget* buttons[BUTTONNUMBER];
GtkWidget* entries[ENTRYNUMBER];
GtkWidget* fixeds[FIXEDNUMBER];
GtkWidget* combos[COMBONUMBER];
GtkWidget* views[VIEWNUMBER];
GtkTreeStore* treeStores[TREESTORENUMBER];
GtkTreeView* tvs[TREEVIEWNUMBER];
GtkTreeViewColumn* cxs[TREEVIEWCOLUMNNUMBER];
GtkTreeSelection* selections[TREESELECTIONNUMBER];
GtkCellRenderer* crs[CELLRENDERERNUMBER];
GtkTreeIter iters[ITERNUMBER];

GtkBuilder* builder;

#include "app.h"
int main(int argc , char** argv)
{
    //Segment Chargement des données

    //Ouverture des fichiers contenants les données
    FILE* file = fopen("Patients.dta","rb");
    FILE* filecounter=fopen("PatientCounter.dta","rb");
    FILE* contacts = fopen("Contacts.dta","rb");
    FILE* contactscounter=fopen("ContactCounter.dta","rb");
    //Lecture des nombres de patients et contacts à charger
    fread(&patientCount,sizeof(int),1,filecounter);
    fread(&contactCount,sizeof(int),1,contactscounter);
    //Initialisation du tableau villes
    for(int i=0;i<MAXVILLES;i++)
    {
        strcpy(globalVilles[i].ville,"");
        globalVilles[i].list=malloc(sizeof(LinkedList));
        globalVilles[i].contactList=malloc(sizeof(ContactList));
        globalVilles[i].list->first=NULL;
        globalVilles[i].contactList->first=NULL;
    }
    //Chargement des patients dans la liste chainée des patients
    for(int i=0;i<patientCount;i++)
    {
        Patient* patient = malloc(sizeof(Patient));
        fread(patient,sizeof(Patient),1,file);
        AjoutPatient(globalVilles,patient,MAXVILLES,NULL);
    }
    //Chargement des contacts dans la liste chainée des contacts
    for(int i=0;i<contactCount;i++)
    {
        Contact* contact = malloc(sizeof(Contact));
        fread(contact,sizeof(Contact),1,contacts);
        AjoutContact(globalVilles,contact,MAXVILLES,NULL);
    }
    //Fermeture des fichiers ouverts
    fclose(file);
    fclose(filecounter);
    fclose(contacts);
    fclose(contactscounter);
    fclose(contacts);

    //Segment GTK

    gtk_init(&argc,&argv);

    builder = gtk_builder_new_from_file("app1k.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder,"window"));
    checkbutton1 = GTK_WIDGET(gtk_builder_get_object(builder,"checkbutton1"));
    checkbutton2 = GTK_WIDGET(gtk_builder_get_object(builder,"checkbutton2"));

    g_signal_connect(window,"destroy",G_CALLBACK(gtk_main_quit),NULL);

    gtk_builder_connect_signals(builder,NULL);

    stack=GTK_WIDGET(gtk_builder_get_object(builder,"stack"));
    for(int i=0;i<BOXNUMBER;i++)
    {
        char string[8];
        sprintf(string,"box%d",i+1);
        boxes[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<BUTTONNUMBER;i++)
    {
        char string[10];
        sprintf(string,"button%d",i+1);
        buttons[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<ENTRYNUMBER;i++)
    {
        char string[10];
        sprintf(string,"entry%d",i+1);
        entries[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<FIXEDNUMBER;i++)
    {
        char string[10];
        sprintf(string,"fixed%d",i+1);
        fixeds[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<COMBONUMBER;i++)
    {
        char string[10];
        sprintf(string,"combo%d",i+1);
        combos[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<VIEWNUMBER;i++)
    {
        char string[10];
        sprintf(string,"view%d",i+1);
        views[i]=GTK_WIDGET(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<TREESTORENUMBER;i++)
    {
        char string[10];
        sprintf(string,"treeStore%d",i+1);
        treeStores[i]=GTK_TREE_STORE(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<TREEVIEWNUMBER;i++)
    {
        char string[10];
        sprintf(string,"tv%d",i+1);
        tvs[i]=GTK_TREE_VIEW(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<TREEVIEWCOLUMNNUMBER;i++)
    {
        char string[10];
        sprintf(string,"cx%d",i+1);
        cxs[i]=GTK_TREE_VIEW_COLUMN(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<CELLRENDERERNUMBER;i++)
    {
        char string[10];
        sprintf(string,"cr%d",i+1);
        crs[i]=GTK_CELL_RENDERER(gtk_builder_get_object(builder,string));
    }
    for(int i=0;i<TREESELECTIONNUMBER;i++)
    {
        char string[10];
        sprintf(string,"selection%d",i+1);
        selections[i]=GTK_TREE_SELECTION(gtk_builder_get_object(builder,string));
    }

    gtk_widget_show(window);
    gtk_main();
    return 0;   
}

//Fonctions associées aux signaux émis par les boutons

void on_button1_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[1]); 
}
void on_button2_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[2]); 
}
void on_button3_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[3]); 
}
void on_button4_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[4]); 
}
void on_button5_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[0]); 
}
void on_button6_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),fixeds[0]);
}
void on_button7_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[5]); 
}
void on_button8_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[6]);
}
void on_button9_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button10_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[0]);    
}
void on_button11_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),fixeds[2]);
}
void on_button12_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[7]);
}
void on_button13_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[8]);
}
void on_button14_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
}
void on_button15_clicked(GtkButton* b)
{       
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[0]);    
}
void on_button16_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[9]);
}
void on_button17_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[10]);
}
void on_button18_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[11]);
}
void on_button19_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button20_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[0]);    
}
void on_button21_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[12]);
}
void on_button22_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[13]);
}
void on_button23_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[14]);
}
void on_button24_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button25_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[1]);
}
void on_button26_clicked(GtkButton* b)
{
    FILE* file = fopen("PatientCounter.dta","wb");
    Patient* patient = malloc(sizeof(Patient));
    strcpy(patient->cin,cinPatient);
    strcpy(patient->nom,nomPatient);
    strcpy(patient->prenom,prenomPatient);
    strcpy(patient->ville,villePatient);
    strcpy(patient->secteur,secteurPatient);
    patient->etat=etatPatient;
    patient->prisEnCharge=boolChargePatient;
    patient->dateGuerison.jour=0;
    patient->dateGuerison.mois=0;
    patient->dateGuerison.annee=0;
    patient->dateDeces.jour=0;
    patient->dateDeces.mois=0;
    patient->dateDeces.annee=0;
    AjoutPatient(globalVilles,patient,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[1]);
    patientCount++;
    fwrite(&patientCount,sizeof(int),1,file);
    fclose(file);
}
void on_button27_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child (GTK_STACK(stack),boxes[1]);
}
void on_button28_clicked(GtkButton* b)
{
    if(RechercherPatient(globalVilles,cinRecherche,MAXVILLES)==1)
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),fixeds[1]);
    }
    else
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),boxes[1]);
    }
}
void on_button29_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[5]);
}
void on_button30_clicked(GtkButton* b)
{
    Patient* patient = malloc(sizeof(Patient));
    strcpy(patient->cin,cinPatient);
    strcpy(patient->nom,nomPatient);
    strcpy(patient->prenom,prenomPatient);
    strcpy(patient->ville,villePatient);
    strcpy(patient->secteur,secteurPatient);
    patient->etat=etatPatient;
    patient->prisEnCharge=boolChargePatient;
    patient->dateGuerison.jour=0;
    patient->dateGuerison.mois=0;
    patient->dateGuerison.annee=0;
    patient->dateDeces.jour=0;
    patient->dateDeces.mois=0;
    patient->dateDeces.annee=0;
    ModifierPatient(globalVilles,cinRecherche,patient,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[1]);
}
void on_button31_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[1]);
}
void on_button32_clicked(GtkButton* b)
{
    FILE* file = fopen("PatientCounter.dta","wb");
    SupprimerPatient(globalVilles,cinSupprimer,MAXVILLES,"Patients.dta");
    patientCount=PatientCount(globalVilles,MAXVILLES);
    fwrite(&patientCount,sizeof(int),1,file);
    fclose(file);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[1]);
}
void on_button33_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button34_clicked(GtkButton* b)
{
    FILE* file = fopen("ContactCounter.dta","wb");
    Contact* contact = malloc(sizeof(Contact));
    strcpy(contact->cinPatientAssocie,cinPatAssocie);
    strcpy(contact->nom,nomContact);
    strcpy(contact->prenom,prenomContact);
    strcpy(contact->ville,villeContact);
    strcpy(contact->secteur,secteurContact);
    strcpy(contact->cinContact,globalCinContact);
    AjoutContact(globalVilles,contact,MAXVILLES,"Contacts.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
    contactCount++;
    fwrite(&contactCount,sizeof(int),1,file);
    fclose(file);  
}
void on_button35_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button36_clicked(GtkButton* b)
{
    if(RechercherContact(globalVilles,cinContactRecherche,MAXVILLES)==1)
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),fixeds[3]);
    }
    else
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),boxes[2]);
    }
}
void on_button37_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[7]);
}
void on_button38_clicked(GtkButton* b)
{
    Contact* contact = malloc(sizeof(Contact));
    strcpy(contact->cinPatientAssocie,cinPatAssocie);
    strcpy(contact->nom,nomContact);
    strcpy(contact->prenom,prenomContact);
    strcpy(contact->ville,villeContact);
    strcpy(contact->secteur,secteurContact);
    strcpy(contact->cinContact,globalCinContact);
    ModifierContact(globalVilles,cinContactRecherche,contact,MAXVILLES,"Contacts.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button39_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button40_clicked(GtkButton* b)
{
    FILE* file = fopen("ContactCounter.dta","wb");
    SupprimerContact(globalVilles,cinContactSupprimer,MAXVILLES,"Contacts.dta");
    contactCount=ContactCount(globalVilles,MAXVILLES);
    fwrite(&contactCount,sizeof(int),1,file);
    fclose(file);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button41_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button42_clicked(GtkButton* b)
{
    if(RechercherPatient(globalVilles,cinGuerisRecherche,MAXVILLES)==1)
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),fixeds[4]);
    }
    else
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),boxes[3]);
    }
}
void on_button43_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);   
}
void on_button44_clicked(GtkButton* b)
{
    FILE* file = fopen("PatientCounter.dta","wb");
    SupprimerPatient(globalVilles,cinGuerisSupprimer,MAXVILLES,"Patients.dta");
    patientCount=PatientCount(globalVilles,MAXVILLES);
    fwrite(&patientCount,sizeof(int),1,file);
    fclose(file);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button45_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button46_clicked(GtkButton* b)
{
    AjoutGueris(globalVilles,cinPatient,globalDateGuerison,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button47_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button48_clicked(GtkButton* b)
{
    AjoutDeces(globalVilles,cinPatient,globalDateDeces,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button49_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button50_clicked(GtkButton* b)
{
    FILE* file = fopen("PatientCounter.dta","wb");
    SupprimerPatient(globalVilles,cinDecesSupprimer,MAXVILLES,"Patients.dta");
    patientCount=PatientCount(globalVilles,MAXVILLES);
    fwrite(&patientCount,sizeof(int),1,file);
    fclose(file);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button51_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button52_clicked(GtkButton* b)
{
    if(RechercherPatient(globalVilles,cinDecesRecherche,MAXVILLES)==1)
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),fixeds[5]);
    }
    else
    {
        gtk_stack_set_visible_child (GTK_STACK(stack),boxes[4]);
    }
}
void on_button53_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[10]);
}
void on_button54_clicked(GtkButton* b)
{
    Patient* patient = malloc(sizeof(Patient));
    strcpy(patient->cin,cinPatient);
    strcpy(patient->nom,nomPatient);
    strcpy(patient->prenom,prenomPatient);
    strcpy(patient->ville,villePatient);
    strcpy(patient->secteur,secteurPatient);
    patient->etat=gueris;
    patient->dateGuerison=dateGuerisonModif;
    patient->dateDeces.jour=0;
    patient->dateDeces.mois=0;
    patient->dateDeces.annee=0;
    ModifierPatient(globalVilles,cinGuerisRecherche,patient,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button55_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[13]);   
}
void on_button56_clicked(GtkButton* b)
{
    Patient* patient = malloc(sizeof(Patient));
    strcpy(patient->cin,cinPatient);
    strcpy(patient->nom,nomPatient);
    strcpy(patient->prenom,prenomPatient);
    strcpy(patient->ville,villePatient);
    strcpy(patient->secteur,secteurPatient);
    patient->etat=decede;
    patient->dateDeces=dateDecesModif;
    patient->dateGuerison.jour=0;
    patient->dateGuerison.mois=0;
    patient->dateGuerison.annee=0;
    ModifierPatient(globalVilles,cinDecesRecherche,patient,MAXVILLES,"Patients.dta");
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button57_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[1]);
}
void on_button58_clicked(GtkButton* b)
{   
    if(cellColumnAssociationsBools[10]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[47+i],crs[47+i],"text",i);
        }
        cellColumnAssociationsBools[10]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%d",CompteurPatientsEtat(globalVilles[i].list,critique));
        gtk_tree_store_append(treeStores[10],&iters[10],NULL);
        gtk_tree_store_set(treeStores[10],&iters[10],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[10],&iters[10],1,temp,-1); 
        i++;
    }   
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[46]);
}
void on_button59_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[11]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[49+i],crs[49+i],"text",i);
        }
        cellColumnAssociationsBools[11]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%f",TauxOccupationLits(globalVilles[i]));
        gtk_tree_store_append(treeStores[11],&iters[11],NULL);
        gtk_tree_store_set(treeStores[11],&iters[11],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[11],&iters[11],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[47]);
}
void on_button60_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[12]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[51+i],crs[51+i],"text",i);
        }
        cellColumnAssociationsBools[12]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%f",TauxOccupationLitsDeReanimation(globalVilles[i]));
        gtk_tree_store_append(treeStores[12],&iters[12],NULL);
        gtk_tree_store_set(treeStores[12],&iters[12],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[12],&iters[12],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[48]);
}
void on_button61_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[0]==0)
    {
        for(int i=0;i<7;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[i],crs[i],"text",i);
        }
        cellColumnAssociationsBools[0]=1;
    }
    node* p;
    for(int i=0;i<MAXVILLES;i++)
    {
        if(strcmp(globalVilles[i].ville,"")!=0)
        {
            p=globalVilles[i].list->first;
            while(p!=NULL)
            {
                if(p->patient->etat!=gueris && p->patient->etat!=decede)
                {
                    gtk_tree_store_append(treeStores[0],&iters[0],NULL);
                    gtk_tree_store_set(treeStores[0],&iters[0],0,p->patient->cin,-1);
                    gtk_tree_store_set(treeStores[0],&iters[0],1,p->patient->nom,-1);
                    gtk_tree_store_set(treeStores[0],&iters[0],2,p->patient->prenom,-1);
                    gtk_tree_store_set(treeStores[0],&iters[0],3,p->patient->ville,-1);
                    gtk_tree_store_set(treeStores[0],&iters[0],4,p->patient->secteur,-1);
                    if(p->patient->etat==0)
                    {
                        gtk_tree_store_set(treeStores[0],&iters[0],5,"Normal",-1);
                    }
                    else if(p->patient->etat==1)
                    {
                        gtk_tree_store_set(treeStores[0],&iters[0],5,"Critique",-1);
                    }
                    else if(p->patient->etat==2)
                    {
                        gtk_tree_store_set(treeStores[0],&iters[0],5,"Besoin d'aide respiratoire",-1);
                    }
                    if(p->patient->prisEnCharge==1)
                    {
                        gtk_tree_store_set(treeStores[0],&iters[0],6,"Oui",-1);
                    }
                    else
                    {
                        gtk_tree_store_set(treeStores[0],&iters[0],6,"Non",-1);
                    }
                }
                p=p->next;
            }
        }
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[36]);
}
void on_button62_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[49]);
}
void on_button63_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[52]);
}
void on_button64_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[9]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[45+i],crs[45+i],"text",i);
        }
        cellColumnAssociationsBools[9]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%d",CompteurPatients(globalVilles[i].list));
        gtk_tree_store_append(treeStores[9],&iters[9],NULL);
        gtk_tree_store_set(treeStores[9],&iters[9],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[9],&iters[9],1,temp,-1); 
        i++;
    }   
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[45]);
}
void on_button65_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[2]);
}
void on_button66_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[56]);
}
void on_button67_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[4]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[27+i],crs[27+i],"text",i);
        }
        cellColumnAssociationsBools[4]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%d",clen(globalVilles[i].contactList));
        gtk_tree_store_append(treeStores[4],&iters[4],NULL);
        gtk_tree_store_set(treeStores[4],&iters[4],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[4],&iters[4],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[40]);
}
void on_button68_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[13]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[53+i],crs[53+i],"text",i);
        }
        cellColumnAssociationsBools[13]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%f",TauxDeContact(globalVilles[i]));
        gtk_tree_store_append(treeStores[13],&iters[13],NULL);
        gtk_tree_store_set(treeStores[13],&iters[13],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[13],&iters[13],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[54]);
}
void on_button69_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[3]);
}
void on_button70_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[62]);
}
void on_button71_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[6]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[35+i],crs[35+i],"text",i);
        }
        cellColumnAssociationsBools[6]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%d",CompteurPatientsEtat(globalVilles[i].list,gueris));
        gtk_tree_store_append(treeStores[6],&iters[6],NULL);
        gtk_tree_store_set(treeStores[6],&iters[6],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[6],&iters[6],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[42]);
}
void on_button72_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[14]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[55+i],crs[55+i],"text",i);
        }
        cellColumnAssociationsBools[14]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        int n = len(globalVilles[i].list);
        sprintf(temp,"%f",n!=0?CompteurPatientsEtat(globalVilles[i].list,gueris)/(float)n:0.0);
        gtk_tree_store_append(treeStores[14],&iters[14],NULL);
        gtk_tree_store_set(treeStores[14],&iters[14],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[14],&iters[14],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[55]);
}
void on_button73_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[4]);
}
void on_button74_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[68]);
}
void on_button75_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[8]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[43+i],crs[43+i],"text",i);
        }
        cellColumnAssociationsBools[8]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        sprintf(temp,"%d",CompteurPatientsEtat(globalVilles[i].list,decede));
        gtk_tree_store_append(treeStores[8],&iters[8],NULL);
        gtk_tree_store_set(treeStores[8],&iters[8],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[8],&iters[8],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[44]);
}
void on_button76_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[15]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[57+i],crs[57+i],"text",i);
        }
        cellColumnAssociationsBools[15]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        int n = len(globalVilles[i].list);
        sprintf(temp,"%f",n!=0?CompteurPatientsEtat(globalVilles[i].list,decede)/(float)n:0.0);
        gtk_tree_store_append(treeStores[15],&iters[15],NULL);
        gtk_tree_store_set(treeStores[15],&iters[15],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[15],&iters[15],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[60]);
}
void on_button77_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[0]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button78_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[1]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[49]);
}
void on_button79_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[2]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[52]);
}
void on_button80_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[3]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[56]);
}
void on_button81_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[4]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
}
void on_button82_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[5]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[62]);
}
void on_button83_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[6]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button84_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[7]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button85_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[8]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button86_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[9]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button87_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[10]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button88_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[11]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button89_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[12]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button90_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button91_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[1]==0)
    {
        for(int i=0;i<7;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[7+i],crs[7+i],"text",i);
        }
        cellColumnAssociationsBools[1]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->secteur,secteurAffichage)==0 && p->patient->etat!=gueris && p->patient->etat!=decede)
            {
                gtk_tree_store_append(treeStores[1],&iters[1],NULL);
                gtk_tree_store_set(treeStores[1],&iters[1],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[1],&iters[1],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[1],&iters[1],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[1],&iters[1],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[1],&iters[1],4,p->patient->secteur,-1);
                if(p->patient->etat==0)
                {
                    gtk_tree_store_set(treeStores[1],&iters[1],5,"Normal",-1);
                }
                else if(p->patient->etat==1)
                {
                    gtk_tree_store_set(treeStores[1],&iters[1],5,"Critique",-1);
                }
                else if(p->patient->etat==2)
                {
                    gtk_tree_store_set(treeStores[1],&iters[1],5,"Besoin d'aide respiratoire",-1);
                }
                if(p->patient->prisEnCharge==1)
                {
                    gtk_tree_store_set(treeStores[1],&iters[1],6,"Oui",-1);
                }
                else
                {
                    gtk_tree_store_set(treeStores[1],&iters[1],6,"Non",-1);
                }
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[37]);
    }
}
void on_button92_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
}
void on_button93_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[2]==0)
    {
        for(int i=0;i<7;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[14+i],crs[14+i],"text",i);
        }
        cellColumnAssociationsBools[2]=1;
    }
    node* p;
    for(int i=0;i<MAXVILLES;i++)
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(p->patient->etat==etatAffichage)
            {
                gtk_tree_store_append(treeStores[2],&iters[2],NULL);
                gtk_tree_store_set(treeStores[2],&iters[2],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[2],&iters[2],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[2],&iters[2],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[2],&iters[2],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[2],&iters[2],4,p->patient->secteur,-1);
                if(p->patient->etat==0)
                {
                    gtk_tree_store_set(treeStores[2],&iters[2],5,"Normal",-1);
                }
                else if(p->patient->etat==1)
                {
                    gtk_tree_store_set(treeStores[2],&iters[2],5,"Critique",-1);
                }
                else if(p->patient->etat==2)
                {
                    gtk_tree_store_set(treeStores[2],&iters[2],5,"Besoin d'aide respiratoire",-1);
                }
                if(p->patient->prisEnCharge==1)
                {
                    gtk_tree_store_set(treeStores[2],&iters[2],6,"Oui",-1);
                }
                else
                {
                    gtk_tree_store_set(treeStores[2],&iters[2],6,"Non",-1);
                }
            }
            p=p->next;
        }
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[38]);
}
void on_button94_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[13]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
}
void on_button95_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[14]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button96_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
}
void on_button97_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[3]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[21+i],crs[21+i],"text",i);
        }
        cellColumnAssociationsBools[3]=1;
    }
    contactNode* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
    }
    else
    {
        p=globalVilles[i].contactList->first;
        while(p!=NULL)
        {
            if(strcmp(p->contact->secteur,secteurAffichage)==0)
            {
                gtk_tree_store_append(treeStores[3],&iters[3],NULL);
                gtk_tree_store_set(treeStores[3],&iters[3],0,p->contact->cinPatientAssocie,-1);
                gtk_tree_store_set(treeStores[3],&iters[3],1,p->contact->cinContact,-1);
                gtk_tree_store_set(treeStores[3],&iters[3],2,p->contact->nom,-1);
                gtk_tree_store_set(treeStores[3],&iters[3],3,p->contact->prenom,-1);
                gtk_tree_store_set(treeStores[3],&iters[3],4,p->contact->ville,-1);
                gtk_tree_store_set(treeStores[3],&iters[3],5,p->contact->secteur,-1);
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[39]);
    }
}
void on_button98_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[15]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button99_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[77]);
}
void on_button100_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button101_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[5]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[29+i],crs[29+i],"text",i);
        }
        cellColumnAssociationsBools[5]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->secteur,secteurAffichage)==0 && p->patient->etat==gueris)
            {
                char temp[11];
                sprintf(temp,"%d/%d/%d",p->patient->dateGuerison.jour,p->patient->dateGuerison.mois,p->patient->dateGuerison.annee);
                gtk_tree_store_append(treeStores[5],&iters[5],NULL);
                gtk_tree_store_set(treeStores[5],&iters[5],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[5],&iters[5],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[5],&iters[5],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[5],&iters[5],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[5],&iters[5],4,p->patient->secteur,-1);
                gtk_tree_store_set(treeStores[5],&iters[5],5,temp,-1);
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[41]);
    }
}
void on_button102_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[80]);
}
void on_button103_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[83]);
}
void on_button104_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button105_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[7]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[37+i],crs[37+i],"text",i);
        }
        cellColumnAssociationsBools[7]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->secteur,secteurAffichage)==0 && p->patient->etat==decede)
            {
                char temp[11];
                sprintf(temp,"%d/%d/%d",p->patient->dateDeces.jour,p->patient->dateDeces.mois,p->patient->dateDeces.annee);
                gtk_tree_store_append(treeStores[7],&iters[7],NULL);
                gtk_tree_store_set(treeStores[7],&iters[7],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[7],&iters[7],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[7],&iters[7],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[7],&iters[7],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[7],&iters[7],4,p->patient->secteur,-1);
                gtk_tree_store_set(treeStores[7],&iters[7],5,temp,-1);
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[43]);
    }
}
void on_button106_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[86]);
}
void on_button107_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[89]);
}
void on_button108_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[95]);

}
void on_button109_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);   
}
void on_button110_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[16]==0)
    {
        for(int i=0;i<7;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[59+i],crs[59+i],"text",i);
        }
        cellColumnAssociationsBools[16]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(p->patient->etat!=gueris && p->patient->etat!=decede)
            {
                gtk_tree_store_append(treeStores[16],&iters[16],NULL);
                gtk_tree_store_set(treeStores[16],&iters[16],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[16],&iters[16],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[16],&iters[16],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[16],&iters[16],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[16],&iters[16],4,p->patient->secteur,-1);
                if(p->patient->etat==0)
                {
                    gtk_tree_store_set(treeStores[16],&iters[16],5,"Normal",-1);
                }
                else if(p->patient->etat==1)
                {
                    gtk_tree_store_set(treeStores[16],&iters[16],5,"Critique",-1);
                }
                else if(p->patient->etat==2)
                {
                    gtk_tree_store_set(treeStores[16],&iters[16],5,"Besoin d'aide respiratoire",-1);
                }
                if(p->patient->prisEnCharge==1)
                {
                    gtk_tree_store_set(treeStores[16],&iters[16],6,"Oui",-1);
                }
                else
                {
                    gtk_tree_store_set(treeStores[16],&iters[16],6,"Non",-1);
                }
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[79]);
    }
}
void on_button111_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[16]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[77]);
}
void on_button112_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
}
void on_button113_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[17]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[66+i],crs[66+i],"text",i);
        }
        cellColumnAssociationsBools[17]=1;
    }
    contactNode* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[16]);
    }
    else
    {
        p=globalVilles[i].contactList->first;
        while(p!=NULL)
        {
            gtk_tree_store_append(treeStores[17],&iters[17],NULL);
            gtk_tree_store_set(treeStores[17],&iters[17],0,p->contact->cinPatientAssocie,-1);
            gtk_tree_store_set(treeStores[17],&iters[17],1,p->contact->cinContact,-1);
            gtk_tree_store_set(treeStores[17],&iters[17],2,p->contact->nom,-1);
            gtk_tree_store_set(treeStores[17],&iters[17],3,p->contact->prenom,-1);
            gtk_tree_store_set(treeStores[17],&iters[17],4,p->contact->ville,-1);
            gtk_tree_store_set(treeStores[17],&iters[17],5,p->contact->secteur,-1);
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[82]);
    }
}
void on_button114_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[17]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[80]);
}
void on_button115_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button116_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[18]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[72+i],crs[72+i],"text",i);
        }
        cellColumnAssociationsBools[18]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(p->patient->etat==gueris)
            {
                char temp[11];
                sprintf(temp,"%d/%d/%d",p->patient->dateGuerison.jour,p->patient->dateGuerison.mois,p->patient->dateGuerison.annee);
                gtk_tree_store_append(treeStores[18],&iters[18],NULL);
                gtk_tree_store_set(treeStores[18],&iters[18],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[18],&iters[18],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[18],&iters[18],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[18],&iters[18],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[18],&iters[18],4,p->patient->secteur,-1);
                gtk_tree_store_set(treeStores[18],&iters[18],5,temp,-1);
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[85]);
    }
}
void on_button117_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[18]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[83]);
}
void on_button118_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button119_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[19]==0)
    {
        for(int i=0;i<6;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[79+i],crs[79+i],"text",i);
        }
        cellColumnAssociationsBools[19]=1;
    }
    node* p;
    int i=0;
    while(i<MAXVILLES && strcmp(villeAffichage,globalVilles[i].ville)!=0)
    {
        i++;
    }
    if(i==MAXVILLES)
    {
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[15]);
    }
    else
    {
        p=globalVilles[i].list->first;
        while(p!=NULL)
        {
            if(p->patient->etat==decede)
            {
                char temp[11];
                sprintf(temp,"%d/%d/%d",p->patient->dateDeces.jour,p->patient->dateDeces.mois,p->patient->dateDeces.annee);
                gtk_tree_store_append(treeStores[19],&iters[19],NULL);
                gtk_tree_store_set(treeStores[19],&iters[19],0,p->patient->cin,-1);
                gtk_tree_store_set(treeStores[19],&iters[19],1,p->patient->nom,-1);
                gtk_tree_store_set(treeStores[19],&iters[19],2,p->patient->prenom,-1);
                gtk_tree_store_set(treeStores[19],&iters[19],3,p->patient->ville,-1);
                gtk_tree_store_set(treeStores[19],&iters[19],4,p->patient->secteur,-1);
                gtk_tree_store_set(treeStores[19],&iters[19],5,temp,-1);
            }
            p=p->next;
        }
        gtk_stack_set_visible_child(GTK_STACK(stack),boxes[88]);
    }

}
void on_button120_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[19]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[86]);
}
void on_button121_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[18]);
}
void on_button122_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[20]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[86+i],crs[86+i],"text",i);
        }
        cellColumnAssociationsBools[20]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        int n = len(globalVilles[i].list);
        sprintf(temp,"%f",n!=0?CompteurPatientsGuerisDate(globalVilles[i].list,dateGuerionAffichage)/(float)n:0.0);
        gtk_tree_store_append(treeStores[20],&iters[20],NULL);
        gtk_tree_store_set(treeStores[20],&iters[20],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[20],&iters[20],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[94]);
}
void on_button123_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[20]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[89]);
}
void on_button124_clicked(GtkButton* b)
{
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[34]);
}
void on_button125_clicked(GtkButton* b)
{
    if(cellColumnAssociationsBools[21]==0)
    {
        for(int i=0;i<2;i++)
        {
            gtk_tree_view_column_add_attribute(cxs[88+i],crs[88+i],"text",i);
        }
        cellColumnAssociationsBools[21]=1;
    }
    int i=0;
    while(i<MAXVILLES && strcmp(globalVilles[i].ville,"")!=0)
    {
        char temp[10];
        int n = len(globalVilles[i].list);
        sprintf(temp,"%f",n!=0?CompteurPatientsDecesDate(globalVilles[i].list,datedDecesAffichage)/(float)n:0.0);
        gtk_tree_store_append(treeStores[21],&iters[21],NULL);
        gtk_tree_store_set(treeStores[21],&iters[21],0,globalVilles[i].ville,-1); 
        gtk_tree_store_set(treeStores[21],&iters[21],1,temp,-1); 
        i++;
    }
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[100]);
}
void on_button126_clicked(GtkButton* b)
{
    gtk_tree_store_clear(treeStores[21]);
    gtk_stack_set_visible_child(GTK_STACK(stack),boxes[95]);
}

//Fonctions associées aux signaux émis par les entrées

void on_entry1_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry2_changed(GtkEntry* e)
{
    strcpy(nomPatient,gtk_entry_get_text(e));
}
void on_entry3_changed(GtkEntry* e)
{
    strcpy(prenomPatient,gtk_entry_get_text(e));
}
void on_entry4_changed(GtkEntry* e)
{
    strcpy(villePatient,gtk_entry_get_text(e));
}
void on_entry5_changed(GtkEntry* e)
{
    strcpy(secteurPatient,gtk_entry_get_text(e));
}
void on_comboentry1_changed(GtkEntry* e)
{
    char temp[35];
    strcpy(temp,gtk_entry_get_text(e));
    if(strcmp(temp,"normal")==0)
    {
        etatPatient=normal;
    }
    else if(strcmp(temp,"critique")==0)
    {
        etatPatient=critique;
    }
    else if(strcmp(temp,"besoin d'aide respiratoire")==0)
    {
        etatPatient=2;
    }
}
void on_entry6_changed(GtkEntry* e)
{
    strcpy(cinRecherche,gtk_entry_get_text(e));
}
void on_entry7_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry8_changed(GtkEntry* e)
{
    strcpy(nomPatient,gtk_entry_get_text(e));
}
void on_entry9_changed(GtkEntry* e)
{
    strcpy(prenomPatient,gtk_entry_get_text(e));
}
void on_entry10_changed(GtkEntry* e)
{
    strcpy(villePatient,gtk_entry_get_text(e));
}
void on_entry11_changed(GtkEntry* e)
{
    strcpy(secteurPatient,gtk_entry_get_text(e));
}
void on_comboentry2_changed(GtkEntry* e)
{
    char temp[35];
    strcpy(temp,gtk_entry_get_text(e));
    if(strcmp(temp,"normal")==0)
    {
        etatPatient=normal;
    }
    else if(strcmp(temp,"critique")==0)
    {
        etatPatient=critique;
    }
    else if(strcmp(temp,"besoin d'aide respiratoire")==0)
    {
        etatPatient=2;
    }
}
void on_entry12_changed(GtkEntry* e)
{
    strcpy(cinSupprimer,gtk_entry_get_text(e));
}
void on_entry13_changed(GtkEntry* e)
{
    strcpy(cinPatAssocie,gtk_entry_get_text(e));
}
void on_entry14_changed(GtkEntry* e)
{
    strcpy(globalCinContact,gtk_entry_get_text(e));
}
void on_entry15_changed(GtkEntry* e)
{
    strcpy(nomContact,gtk_entry_get_text(e));
}
void on_entry16_changed(GtkEntry* e)
{
    strcpy(prenomContact,gtk_entry_get_text(e));
}
void on_entry17_changed(GtkEntry* e)
{
    strcpy(villeContact,gtk_entry_get_text(e));
}
void on_entry18_changed(GtkEntry* e)
{
    strcpy(secteurContact,gtk_entry_get_text(e));
}
void on_entry19_changed(GtkEntry* e)
{
    strcpy(cinContactRecherche,gtk_entry_get_text(e));
}
void on_entry20_changed(GtkEntry* e)
{
    strcpy(cinPatAssocie,gtk_entry_get_text(e));

}
void on_entry21_changed(GtkEntry* e)
{
    strcpy(globalCinContact,gtk_entry_get_text(e));
}
void on_entry22_changed(GtkEntry* e)
{
    strcpy(nomContact,gtk_entry_get_text(e));
}
void on_entry23_changed(GtkEntry* e)
{
    strcpy(prenomContact,gtk_entry_get_text(e));
}
void on_entry24_changed(GtkEntry* e)
{
    strcpy(villeContact,gtk_entry_get_text(e));
}
void on_entry25_changed(GtkEntry* e)
{
    strcpy(secteurContact,gtk_entry_get_text(e));
}
void on_entry26_changed(GtkEntry* e)
{
    strcpy(cinContactSupprimer,gtk_entry_get_text(e));
}
void on_entry27_changed(GtkEntry* e)
{
    strcpy(cinGuerisRecherche,gtk_entry_get_text(e));
}
void on_entry28_changed(GtkEntry* e)
{
    strcpy(cinGuerisSupprimer,gtk_entry_get_text(e));
}
void on_entry29_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry30_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry31_changed(GtkEntry* e)
{
    strcpy(cinDecesSupprimer,gtk_entry_get_text(e));
}
void on_entry32_changed(GtkEntry* e)
{
    strcpy(cinDecesRecherche,gtk_entry_get_text(e));
}
void on_entry33_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry34_changed(GtkEntry* e)
{
    strcpy(nomPatient,gtk_entry_get_text(e));
}
void on_entry35_changed(GtkEntry* e)
{
    strcpy(prenomPatient,gtk_entry_get_text(e));
}
void on_entry36_changed(GtkEntry* e)
{
    strcpy(villePatient,gtk_entry_get_text(e));
}
void on_entry37_changed(GtkEntry* e)
{
    strcpy(secteurPatient,gtk_entry_get_text(e));
}
void on_entry38_changed(GtkEntry* e)
{
    strcpy(cinPatient,gtk_entry_get_text(e));
}
void on_entry39_changed(GtkEntry* e)
{
    strcpy(nomPatient,gtk_entry_get_text(e));
}
void on_entry40_changed(GtkEntry* e)
{
    strcpy(prenomPatient,gtk_entry_get_text(e));
}
void on_entry41_changed(GtkEntry* e)
{
    strcpy(villePatient,gtk_entry_get_text(e));
}
void on_entry42_changed(GtkEntry* e)
{
    strcpy(secteurPatient,gtk_entry_get_text(e));
}
void on_entry43_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry44_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry45_changed(GtkEntry* e)
{
    strcpy(secteurAffichage,gtk_entry_get_text(e));
}
void on_entry46_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry47_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));    
}
void on_entry48_changed(GtkEntry* e)
{
    strcpy(secteurAffichage,gtk_entry_get_text(e));    
}
void on_entry49_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(datedDecesAffichage.jour));
}
void on_entry50_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry51_changed(GtkEntry* e)
{
    strcpy(secteurAffichage,gtk_entry_get_text(e));
}
void on_entry52_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry53_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry54_changed(GtkEntry* e)
{
    strcpy(secteurAffichage,gtk_entry_get_text(e));
}
void on_entry55_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerionAffichage.jour));
}
void on_entry56_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerionAffichage.mois));
}
void on_entry57_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerionAffichage.annee));
}
void on_entry58_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(datedDecesAffichage.mois));
}
void on_entry59_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(datedDecesAffichage.annee));
}
void on_entry60_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerisonModif.jour));
}
void on_entry61_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerisonModif.mois));
}
void on_entry62_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateGuerisonModif.annee));
}
void on_entry63_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateDecesModif.jour));
}
void on_entry64_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateDecesModif.mois));
}
void on_entry65_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(dateDecesModif.annee));
}
void on_entry66_changed(GtkEntry* e)
{
}
void on_entry67_changed(GtkEntry* e)
{
}
void on_entry68_changed(GtkEntry* e)
{
}
void on_entry69_changed(GtkEntry* e)
{
}
void on_entry70_changed(GtkEntry* e)
{
    strcpy(villeAffichage,gtk_entry_get_text(e));
}
void on_entry71_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateGuerison.jour));
}
void on_entry72_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateGuerison.mois));
}
void on_entry73_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateGuerison.annee));
}
void on_entry74_changed(GtkEntry* e)
{
}
void on_entry75_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateDeces.jour));
}
void on_entry76_changed(GtkEntry* e)
{
    char temp[3];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateDeces.mois));
}
void on_entry77_changed(GtkEntry* e)
{
    char temp[5];
    strcpy(temp,gtk_entry_get_text(e));
    sscanf(temp,"%d",&(globalDateDeces.annee));
}
void on_comboentry3_changed(GtkEntry* e)
{
    char temp[35];
    strcpy(temp,gtk_entry_get_text(e));
    if(strcmp(temp,"normal")==0)
    {
        etatAffichage=normal;
    }
    else if(strcmp(temp,"critique")==0)
    {
        etatAffichage=critique;
    }
    else if(strcmp(temp,"besoin d'aide respiratoire")==0)
    {
        etatAffichage=2;
    }
}

//Fonctions associées aux signaux émis par les checks boxes

void on_checkbutton1_toggled(GtkCheckButton* b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if(T)
    {
        boolChargePatient=1;
    }
    else
    {
        boolChargePatient=0;
    }
}
void on_checkbutton2_toggled(GtkCheckButton* b)
{
    gboolean T = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(b));
    if(T)
    {
        boolChargePatient=1;
    }
    else
    {
        boolChargePatient=0;
    }
}
