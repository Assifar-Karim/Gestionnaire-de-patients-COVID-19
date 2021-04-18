#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"

//Fonctions Liste chainée

void FrontInsert(LinkedList* list,Patient* patient)
{
    node* new = malloc(sizeof(node));
    new->patient=patient;
    new->next=list->first;
    list->first=new;
}
void FrontInsertContact(ContactList* list,Contact* contact)
{
    contactNode* new = malloc(sizeof(contactNode));
    new->contact=contact;
    new->next=list->first;
    list->first=new;
}
void DeleteElem(LinkedList* list,char* cin)
{
    node* beforeP=NULL;
    node* p = list->first;
    while(p!=NULL && strcmp(p->patient->cin,cin)!=0)
    {
        beforeP=p;
        p=p->next;
    }
    if(p==NULL)
    {
        printf("Ce patient n'existe pas!\n");
        return;
    }
    else if(p==list->first)
    {
        list->first=p->next;
        free(p);
        return;
    }
    else if(p->next == NULL)
    {
        beforeP->next=NULL;
        free(p);
        return;
    }
    else
    {
        beforeP->next=p->next;
        free(p);
        return;
    }
}
void ModifyElem(LinkedList* list,char* cin,Patient* newPatientData)
{
    node* p = list->first;
    while(p!=NULL && strcmp(p->patient->cin,cin)!=0)
    {
        p=p->next;
    }
    if(p==NULL)
    {
        printf("Ce patient n'existe pas !");
    }
    else
    {
        strcpy(p->patient->cin,newPatientData->cin);
        strcpy(p->patient->prenom,newPatientData->prenom);
        strcpy(p->patient->nom,newPatientData->nom);
        strcpy(p->patient->ville,newPatientData->ville);
        strcpy(p->patient->secteur,newPatientData->secteur);
        p->patient->etat=newPatientData->etat;
    }
}
int len(LinkedList* list)
{
    int n=0;
    node* p =list->first;
    while(p!=NULL)
    {
        n++;
        p=p->next;
    }
    return n;
}
int clen(ContactList* list)
{
    int n=0;
    contactNode* p =list->first;
    while(p!=NULL)
    {
        n++;
        p=p->next;
    }
    return n;
}
int TrouverVille(Ville villes[],char* cin,int sizeT)
{
    int i=0;
    node* p;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->cin,cin)==0)
            {
                return i;
            }
            p=p->next;
        }
        i++;
    }
    return -1;
}
//Fonctions Patients, Guéris, Décédés

void AjoutPatient(Ville villes[],Patient* patient,int sizeT,char* fileName)
{
    FILE* file = fileName==NULL?NULL:fopen(fileName,"ab");
    FILE* lits=fopen("Villes.txt","r");
    int i;
    for(i=0;i<sizeT;i++)
    {
        if(strcmp(patient->ville,villes[i].ville)==0)
        {
            break;
        }
    }
    if(i<sizeT)
    {
        if(file!=NULL)
        {
            fwrite(patient,sizeof(Patient),1,file);   
        }
        FrontInsert(villes[i].list,patient);
    }
    else
    {
        i=0;
        while(i<sizeT && strcmp(villes[i].ville,"")!=0)
        {
            i++;
        }
        if(i==sizeT)
        {
            printf("Impossible d'ajouter une nouvelle ville !\n");
        }
        //Ajout de la ville dans le cas ou nous avons tjrs de la place
        else
        {
            char tempString[22]={0};
            int tempInt1=0;
            int tempInt2=0;
            int j=0;
            while(strcmp(tempString,patient->ville)!=0 && j<sizeT)
            {
                fscanf(lits,"%[^,],%d,%d",tempString,&tempInt1,&tempInt2);
                fseek(lits,2,SEEK_CUR);
                j++;
            }
            strcpy(villes[i].ville,patient->ville);
            villes[i].nbrLits=tempInt1;
            villes[i].nbrLitsRea=tempInt2;
            if(file!=NULL)
            {
                fwrite(patient,sizeof(Patient),1,file);   
            }
            FrontInsert(villes[i].list,patient);
        }
    }
    if(file!=NULL)
    {
        fclose(file);
    }
    fclose(lits);
}
void ModifierPatient(Ville villes[],char* cin,Patient* newPatientData,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    node* p;
    node* pWriter;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->cin,cin)==0)
            {
                strcpy(p->patient->cin,newPatientData->cin);
                strcpy(p->patient->prenom,newPatientData->prenom);
                strcpy(p->patient->nom,newPatientData->nom);
                strcpy(p->patient->ville,newPatientData->ville);
                strcpy(p->patient->secteur,newPatientData->secteur);
                p->patient->etat=newPatientData->etat;
                p->patient->prisEnCharge=newPatientData->prisEnCharge;
                p->patient->dateDeces=newPatientData->dateDeces;
                p->patient->dateGuerison=newPatientData->dateGuerison;
                break;
            }
            p=p->next;
        }
        i++;
    }
    if(i==sizeT)
    {
        printf("Patient introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        pWriter=villes[i].list->first;
        while(pWriter!=NULL)
        {
            fwrite(pWriter->patient,sizeof(Patient),1,file);
            pWriter=pWriter->next;
        }
    }
    fclose(file);
}
void SupprimerPatient(Ville villes[],char* cin,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    node* p;
    int i= TrouverVille(villes,cin,sizeT);
    if(i!=-1)
    {
        DeleteElem(villes[i].list,cin);
    }
    else
    {
        printf("Patient introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            fwrite(p->patient,sizeof(Patient),1,file);
            p=p->next;
        }
    }
    fclose(file);
}
int RechercherPatient(Ville villes[],char* cin,int sizeT)
{
    node* p;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->cin,cin)==0)
            {
                return 1;
            }
            p=p->next;
        }
        i++;
    }
    return 0;
}
void AjoutGueris(Ville villes[],char* cin,Date dateGuerison,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    node* p;
    node* pWriter;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->cin,cin)==0)
            {
                p->patient->etat=gueris;
                p->patient->dateGuerison=dateGuerison;
                p->patient->dateDeces.jour=0;
                p->patient->dateDeces.mois=0;
                p->patient->dateDeces.annee=0;
                p->patient->prisEnCharge=0;
                break;
            }
            p=p->next;
        }
        i++;
    }
    if(i==sizeT)
    {
        printf("Patient introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        pWriter=villes[i].list->first;
        while(pWriter!=NULL)
        {
            fwrite(pWriter->patient,sizeof(Patient),1,file);
            pWriter=pWriter->next;
        }
    }
    fclose(file);
}
void AjoutDeces(Ville villes[],char* cin,Date dateDeces,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    node* p;
    node* pWriter;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].list->first;
        while(p!=NULL)
        {
            if(strcmp(p->patient->cin,cin)==0)
            {
                p->patient->etat=decede;
                p->patient->dateDeces=dateDeces;
                p->patient->dateGuerison.jour=0;
                p->patient->dateGuerison.mois=0;
                p->patient->dateGuerison.annee=0;
                p->patient->prisEnCharge=0;
                break;
            }
            p=p->next;
        }
        i++;
    }
    if(i==sizeT)
    {
        printf("Patient introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        pWriter=villes[i].list->first;
        while(pWriter!=NULL)
        {
            fwrite(pWriter->patient,sizeof(Patient),1,file);
            pWriter=pWriter->next;
        }
    }
    fclose(file);
}
int PatientCount(Ville villes[],int sizeT)
{
    int n=0;
    for(int i=0;i<sizeT;i++)
    {
        n+=len(villes[i].list);
    }
    return n;
}
int CompteurPatients(LinkedList* list)
{
    int n=0;
    node* p =list->first;
    while(p!=NULL)
    {
        if(p->patient->etat!=3 && p->patient->etat!=4)
        {
            n++;
        }
        p=p->next;
    }
    return n;   
}
int CompteurPatientsPrisEnCharges(LinkedList* list)
{
    int n=0;
    node* p=list->first;
    while(p!=NULL)
    {
        if(p->patient->etat!=3 && p->patient->etat!=4 && p->patient->prisEnCharge)
        {
            n++;
        }
        p=p->next;
    }
    return n;
}
int CompteurPatientsEtat(LinkedList* list,Etat etat)
{
    int n=0;
    node* p =list->first;
    while(p!=NULL)
    {
        if(p->patient->etat==etat)
        {
            n++;
        }
        p=p->next;
    }
    return n;   
}
int CompteurPatientsGuerisDate(LinkedList* list,Date date)
{
    int n=0;
    node* p =list->first;
    while(p!=NULL)
    {
        if(p->patient->etat==gueris && p->patient->dateGuerison.jour==date.jour && p->patient->dateGuerison.mois==date.mois && p->patient->dateGuerison.annee==date.annee)
        {
            n++;
        }
        p=p->next;
    }
    return n;   
}
int CompteurPatientsDecesDate(LinkedList* list,Date date)
{
    int n=0;
    node* p =list->first;
    while(p!=NULL)
    {
        if(p->patient->etat==decede && p->patient->dateDeces.jour==date.jour && p->patient->dateDeces.mois==date.mois && p->patient->dateDeces.annee==date.annee)
        {
            n++;
        }
        p=p->next;
    }
    return n;   
}
float TauxOccupationLits(Ville ville)
{
    int n=0;
    node* p=ville.list->first;
    while(p!=NULL)
    {
        if(p->patient->prisEnCharge && p->patient->etat!=gueris && p->patient->etat!=decede)
        {
            n++;
        }
        p=p->next;
    }
    return (float)n/ville.nbrLits;
}
float TauxOccupationLitsDeReanimation(Ville ville)
{
    int n=0;
    node* p=ville.list->first;
    while(p!=NULL)
    {
        if(p->patient->prisEnCharge && (p->patient->etat==critique || p->patient->etat==besoin_D_Aide))
        {
            n++;
        }
        p=p->next;
    }
    return (float)n/ville.nbrLitsRea;
}

//Fonctions Contacts

void AjoutContact(Ville villes[],Contact* contact,int sizeT,char* fileName)
{
    FILE* file = fileName==NULL?NULL:fopen(fileName,"ab");
    int i;
    for(i=0;i<sizeT;i++)
    {
        if(strcmp(contact->ville,villes[i].ville)==0)
        {
            break;
        }
    }
    if(i<sizeT)
    {
        if(file!=NULL)
        {
            fwrite(contact,sizeof(Contact),1,file);   
        }
        FrontInsertContact(villes[i].contactList,contact);
    }
    else
    {
        i=0;
        while(i<sizeT && strcmp(villes[i].ville,"")!=0)
        {
            i++;
        }
        if(i==sizeT)
        {
            printf("Impossible d'ajouter une nouvelle ville !\n");
        }
        //Ajout de la ville dans le cas ou nous avons tjrs de la place
        else
        {
            strcpy(villes[i].ville,contact->ville);
            if(file!=NULL)
            {
                fwrite(contact,sizeof(Contact),1,file);   
            }
            FrontInsertContact(villes[i].contactList,contact);
        }
    }
    if(file!=NULL)
    {
        fclose(file);
    }
}
void ModifierContact(Ville villes[],char* cinContact,Contact* newContactData,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    contactNode* p;
    contactNode* pWriter;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].contactList->first;
        while(p!=NULL)
        {
            if(strcmp(p->contact->cinContact,cinContact)==0)
            {
                strcpy(p->contact->cinContact,newContactData->cinContact);
                strcpy(p->contact->cinPatientAssocie,newContactData->cinPatientAssocie);
                strcpy(p->contact->prenom,newContactData->prenom);
                strcpy(p->contact->nom,newContactData->nom);
                strcpy(p->contact->ville,newContactData->ville);
                strcpy(p->contact->secteur,newContactData->secteur);
                break;
            }
            p=p->next;
        }
        i++;
    }
    if(i==sizeT)
    {
        printf("Contact introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        pWriter=villes[i].contactList->first;
        while(pWriter!=NULL)
        {
            fwrite(pWriter->contact,sizeof(Contact),1,file);
            pWriter=pWriter->next;
        }
    }
    fclose(file);
}
void SupprimerContact(Ville villes[],char* cinContact,int sizeT,char* fileName)
{
    FILE* file = fopen(fileName,"wb"); 
    contactNode* p;
    contactNode* beforeP = NULL;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
            p=villes[i].contactList->first;
            while(p!=NULL && strcmp(p->contact->cinContact,cinContact)!=0)
            {
                beforeP=p;
                p=p->next;
            }
            if(p!=NULL && strcmp(p->contact->cinContact,cinContact)==0)
            {
                if(p==villes[i].contactList->first)
                {
                    villes[i].contactList->first=p->next;
                    free(p);
                    break;
                }
                else if(p->next == NULL)
                {
                    beforeP->next=NULL;
                    free(p);
                    break;
                }
                else
                {
                    beforeP->next=p->next;
                    free(p);
                    break;
                }
                
            }
            i++;
    }
    if(i==sizeT)
    {
        printf("Contact introuvable ! \n");
    }
    for(int i=0;i<sizeT;i++)
    {
        p=villes[i].contactList->first;
        while(p!=NULL)
        {
            fwrite(p->contact,sizeof(Contact),1,file);
            p=p->next;
        }
    }
    fclose(file);
}
int RechercherContact(Ville villes[],char* cin,int sizeT)
{
    contactNode* p;
    int i=0;
    while(i<sizeT && strcmp(villes[i].ville,"")!=0)
    {
        p=villes[i].contactList->first;
        while(p!=NULL)
        {
            if(strcmp(p->contact->cinContact,cin)==0)
            {
                return 1;
            }
            p=p->next;
        }
        i++;
    }
    return 0;
}
int ContactCount(Ville villes[],int sizeT)
{
    int n=0;
    for(int i=0;i<sizeT;i++)
    {
        n+=clen(villes[i].contactList);
    }
    return n;
}
int Rechercher(Ville ville,char* cin)
{
    node* p;
    p=ville.list->first;
    while(p!=NULL)
    {
        if(strcmp(p->patient->cin,cin)==0)
        {
            return 1;
        }
        p=p->next;
    }
    return 0;
}
float TauxDeContact(Ville ville)
{
    int n=0;
    int length=CompteurPatients(ville.list);
    printf("%d\n",length);
    if(length==0)
    {
        return 0.0;
    }
    contactNode* cp=ville.contactList->first;
    while(cp!=NULL)
    {
        if(Rechercher(ville,cp->contact->cinContact)==1)
        {
            n++;
        }
        cp=cp->next;
    }
    printf("%d, %f\n",n,n/(float)length);
    return n/(float)length;
}
