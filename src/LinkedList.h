typedef struct Date
{
    int jour;
    int mois;
    int annee;
}Date;
typedef enum Etat
{
    normal,
    critique,
    besoin_D_Aide,
    decede,
    gueris,
}Etat;
typedef struct Patient
{
    char cin[10];
    char ville[16];
    char secteur[20];
    char nom[21];
    char prenom[21];
    Date dateDeces;
    Date dateGuerison;
    Etat etat;
    int prisEnCharge;
}Patient;
typedef struct Contact
{
    char cinPatientAssocie[10];
    char ville[16];
    char secteur[20];
    char nom[21];
    char prenom[21];
    char cinContact[10];
}Contact;
typedef struct node
{
    Patient* patient;
    struct node* next;
}node;
typedef struct contactNode
{
    Contact* contact;
    struct contactNode* next;
}contactNode;
typedef struct LinkedList
{
    node* first;
}LinkedList;
typedef struct ContactList
{
    contactNode* first;
}ContactList;
typedef struct Ville
{
    char ville[16];
    int nbrLits;
    int nbrLitsRea;
    LinkedList* list;
    ContactList* contactList;
}Ville;

void AjoutPatient(Ville villes[],Patient* patient,int sizeT,char* fileName);
void ModifierPatient(Ville villes[],char* cin,Patient* newPatientData,int sizeT,char* fileName);
void SupprimerPatient(Ville villes[],char* cin,int sizeT,char* fileName);
int RechercherPatient(Ville villes[],char* cin,int sizeT);
void AjoutGueris(Ville villes[],char* cin,Date dateGuerison,int sizeT,char* fileName);
void AjoutDeces(Ville villes[],char* cin,Date dateDeces,int sizeT,char* fileName);

int CompteurPatients(LinkedList* list);
int CompteurPatientsEtat(LinkedList* list,Etat etat);
int CompteurPatientsGuerisDate(LinkedList* list,Date date);
int CompteurPatientsDecesDate(LinkedList* list,Date date);
int CompteurPatientsPrisEnCharges(LinkedList* list);
int len(LinkedList* list);
int PatientCount(Ville villes[],int sizeT);
float TauxOccupationLits(Ville ville);
float TauxOccupationLitsDeReanimation(Ville ville);

void AjoutContact(Ville villes[],Contact* contact,int sizeT,char* fileName);
void ModifierContact(Ville villes[],char* codeContact,Contact* newContactData,int sizeT,char* fileName);
void SupprimerContact(Ville villes[],char* cinContact,int sizeT,char* fileName);
int RechercherContact(Ville villes[],char* cin,int sizeT);
float TauxDeContact(Ville ville);

int clen(ContactList* list);
int ContactCount(Ville villes[],int sizeT);
