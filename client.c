#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;
char number[2],ID[25],pass[25],campionat_terminat[256];
int index_terminat,ok_campionat_terminat;
char username_administrator[50],password_administrator[50];
char scor[50];
int numar_linii;
int stop;
int ok_username;
int ok_register;
int ok_password;
int quit_login;
int login_number;
int ok_filename;
char campionate[100];
char campionat_ales[100];
char* camp_ales;
int numar,a,b;
char raspuns_participare[10];
int ok_participare;
char copie_campionat[100];
char mail[256];
int ok_max_participants=0;

char filename[35], sport[35], structura_campionat[35], draw[35];
int numar_part;

void print_file1(char filename[], int *ok)
{  
    (*ok)=1;
    char line[256];
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
   while(fgets(line,256,fptr)!=NULL)
   {
       printf("%d.%s",(*ok),line);
       (*ok)++;
   }
    printf("\n");
    fclose(fptr);
}

int fisier_seek(char nume[25], char *filename)
{
    char str[25];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return -1;
    }
    int ok=0,i=0,j=0;  
    char string[25];
    while(fgets(str,25,fp)!=NULL)
    {   
        memset(&string[0], 0, sizeof(string));        
        i=0;j=0;
        while(i<strlen(str)-1)
        {
            string[i++]=str[j++];
        }
        string[strlen(string)-1]=NULL;
        if(strcmp(string,nume)==0)
        {   
            ok=1;
            break;
        }
    }
    return ok;
}

int fisier_seek_prim(char nume[25], char filename[])
{
    char str[25];
    strcat(filename,".txt");
   
    const char *champ;
    champ=filename;
    FILE *fp = fopen(champ, "r");
    if (fp == NULL)
    {
        return -1;
    }
    int ok=0,i=0,j=0;  
    char string[25];
    while(fgets(str,25,fp)!=NULL)
    {   
        memset(&string[0], 0, sizeof(string));        
        i=0;j=0;
        while(i<strlen(str)-1)
        {
            string[i++]=str[j++];
        }
        string[strlen(string)-1]=NULL;
        if(strcmp(string,nume)==0)
        {   
            ok=1;
            break;
        }
    }
    return ok;
    fflush(fp);
    fclose(fp);
}

// void strreverse(char s[256])
// {
//     char r[256];
//     int begin, end, count = 0;
//     while (s[count] != '\0')
//      count++;

//    end = count - 1;
//    for (begin = 0; begin < count; begin++) {
//       r[begin] = s[end];
//       end--;
//    }
//    r[begin] = '\0';
//    strcpy(s,r);
// }

int charToINT(char number[2])
{
    char numbers[][2]={"1","2","3","4","5"};
    int number1;
    int i=0;
    while(strcmp(number,numbers[i])!=0)
    {
        i++;
    }
    number1=i+1;
    return number1;
}

void print_file(char filename[])
{   int ok=1;
    strcat(filename,".txt");
    char* fisier;
    fisier=filename;
    FILE *fptr;
    char c;
    fptr = fopen(fisier, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    c = fgetc(fptr);
    while (c != EOF)
    {  
        
        if(ok>2 || (strcmp(filename,"championships.txt")==0) || (isdigit(c)==0 && ok==2))
        printf("%c", c);
        c = fgetc(fptr);
        ok++;
    }
  
    fclose(fptr);
}

int max_participants(char filename[])
{   
    int ok=1;
    FILE *fp = fopen(filename, "r");
    char string[100];
    char string2[100];
    char c[3];
    int a;
    while(fgets(string, 100, fp)) 
    {
    // if(ok==5)    
    // printf("%s", string);
    // printf("%d\n",ok);
    ok++;
    }
   
    string[strlen(string)-2]=NULL;
    strcpy(string2,string);
    if(strlen(string)==31)
    {
        c[0]=string2[30];
        c[1]=NULL;
    }
    else
        if(strlen(string)==32)
        {
            c[0]=string2[30];
            c[1]=string2[31];
            c[2]=NULL;
        }
    a=atoi(c);
    return a;
    fclose(fp);
}

int first_line(char filename[])
{   
    
    char* fisier;
    fisier=filename;
    FILE *fptr;
    char* c;
    char line[10];
    int a;
    fptr = fopen(fisier, "r");
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    if((c=fgets(line,10,fptr))!=NULL)
        { a=atoi(c);return a;}
    
    fclose(fptr);
}
void replace_number(char filename[],int number)
{
        FILE *fptr1, *fptr2;
        int lno=1, linectr = 0;
        char str[256];        
        char temp[] = "temp.txt";
		int new_number;
        fptr1 = fopen(filename, "r");
        
        if (!fptr1) 
        {
                printf("Unable to open the input file!!\n");
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2) 
        {
                printf("Unable to open a temporary file to write!!\n");
                fclose(fptr1);
        }
         // copy all contents to the temporary file other except specific line
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, 256, fptr1);
            
            if (!feof(fptr1)) 
            {
                linectr++;
                if (linectr != lno) 
                    {
                        fprintf(fptr2, "%s", str);
                    } 
                    else 
                    {
                        fprintf(fptr2, "%d", number);
                        fprintf(fptr2,"\r\n");
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(filename);
        rename(temp, filename);
 }

void replace_line1(char filename[],char scor[50],int lno)
{
        FILE *fptr1, *fptr2;
        int linectr = 0;
        char str[256];        
        char temp[] = "temp.txt";
		int new_number;
        fptr1 = fopen(filename, "r");
        
        if (!fptr1) 
        {
                printf("Unable to open the input file!!\n");
        }
        fptr2 = fopen(temp, "w");
        if (!fptr2) 
        {
                printf("Unable to open a temporary file to write!!\n");
                fclose(fptr1);
        }
         // copy all contents to the temporary file other except specific line
        while (!feof(fptr1)) 
        {
            strcpy(str, "\0");
            fgets(str, 256, fptr1);
            
            if (!feof(fptr1)) 
            {
                linectr++;
                if (linectr != lno) 
                    {
                        fprintf(fptr2, "%s", str);
                    } 
                    else 
                    {
                        str[strlen(str)-2]=NULL;
                        fprintf(fptr2, "%s %s", str,scor);
                        fprintf(fptr2,"\r\n");
                    }
                }
        }
        fclose(fptr1);
        fclose(fptr2);
        remove(filename);
        rename(temp, filename);
 }

 
// sursa: https://www.geeksforgeeks.org/check-if-given-email-address-is-valid-or-not-in-cpp/
// Function to check email id is valid or not
bool isChar(char c)
{
    return ((c >= 'a' && c <= 'z')
            || (c >= 'A' && c <= 'Z'));
}
  
// Function to check the character
// is an digit or not
bool isDigit(const char c)
{
    return (c >= '0' && c <= '9');
}
  
// Function to check email id is
// valid or not
bool is_valid(char email[])
{
    // Check the first character
    // is an alphabet or not
    if (!isChar(email[0])) {
  
        // If it's not an alphabet
        // email id is not valid
        return 0;
    }
    // Variable to store position
    // of At and Dot
    int At = -1, Dot = -1;
  
    // Traverse over the email id
    // string to find position of
    // Dot and At
    for (int i = 0;
         i < strlen(email); i++) {
  
        // If the character is '@'
        if (email[i] == '@') {
  
            At = i;
        }
  
        // If character is '.'
        else if (email[i] == '.') {
  
            Dot = i;
        }
    }
  
    // If At or Dot is not present
    if (At == -1 || Dot == -1)
        return 0;
  
    // If Dot is present before At
    if (At > Dot)
        return 0;
  
    // If Dot is present at the end
    return !(Dot >= (strlen(email) - 1));
}       

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100];		// mesajul trimis

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
    
    while(stop!=1)
    {

        
        printf("Select what you would like to do!\n");
        printf("1.Register\n");
        printf("2.Login\n");
        printf("3.Exit\n");
        printf("4.Login as administrator.\n");
        scanf("%s",number);
        /* trimiterea numarului la server */
        if (write (sd, number, 100) <= 0)
            {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
            }
    
            switch(charToINT(number))    
                {
                   case 1:  printf("Please enter your desired username:");
                            scanf("%s",ID);
                            if (write (sd, ID,100) <= 0) //trimitem username-ul ales
                            {
                                perror ("[client]Eroare la write() spre server.\n");
                                return errno;
                            }
                        if (read (sd, &ok_register, sizeof(ok_register)) < 0)
                            {
                                perror ("[client]Eroare la read() de la server.\n");
                                return errno;
                            }
                            if(ok_register==0)
                            printf("Username taken, please try again.\n");
                            else {
                                        printf("Now enter your desired password:");
                                        scanf("%s",pass);
                                    if (write (sd, pass,100) <= 0) //trimitem parola
                                        {
                                            perror ("[client]Eroare la write() spre server.\n");
                                            return errno;
                                        } 
                                    
                                        printf("Registration completed.\n");  
                                  }
                            break;

                    case 2: 
                            printf("Username:");
                            scanf("%s",ID);
                            if (write (sd, ID,100) <= 0) //trimitem username-ul
                                {
                                    perror ("[client]Eroare la write() spre server.\n");
                                    return errno;
                                }
                            if (read (sd, &ok_username, sizeof(ok_username)) < 0) //citim ok_username
                                {
                                    perror ("[client]Eroare la read() de la server.\n");
                                    return errno;
                                }
                            if(ok_username==0)
                            {
                                printf("Username doesn't exist.\n");
                            }
                            else
                                {
                                    printf("Password:");
                                    scanf("%s",pass);
                                    if (write (sd, pass,100) <= 0) //trimitem parola
                                        {
                                            perror ("[client]Eroare la write() spre server.\n");
                                            return errno;
                                        }
                                    if (write (sd, ID,100) <= 0) //trimitem id
                                        {
                                            perror ("[client]Eroare la write() spre server.\n");
                                            return errno;
                                        }    
                                    if (read (sd, &ok_password, sizeof(ok_password)) < 0) //citim ok_password
                                        {
                                            perror ("[client]Eroare la read() de la server.\n");
                                            return errno;
                                        }
                                    if(ok_password==0)
                                    {
                                        printf("Password is incorrect. Try again.\n");
                                    }
                                    else
                                    {
                                        printf("You have successfully logged in!\nSelect what you would like to do next.\n");
                                        quit_login=0;
                                        while(quit_login!=1)
                                        {
                                            printf("1.Creeaza un campionat.\n");
                                            printf("2.Inscrie-te la un campionat.\n");
                                            printf("3.Back.\n");
                                            scanf("%d",&login_number);
                                            if (write (sd, &login_number,sizeof(login_number)) <= 0) //trimitem nr comenzii la server
                                            {
                                                perror ("[client]Eroare la write() spre server.\n");
                                                return errno;
                                            }  

                                            switch(login_number)
                                            {
                                                case 1:
                                                printf("Numele campionatului:");
                                                scanf("%s",filename);
                                                if (write (sd, filename,35) <= 0) //trimitem numele campionatului la server
                                                {
                                                    perror ("[client]Eroare la write() spre server.\n");
                                                    return errno;
                                                }
                                                if (read (sd, &ok_filename, sizeof(ok_filename)) < 0) //citim ok_password
                                                {
                                                    perror ("[client]Eroare la read() de la server.\n");
                                                    return errno;
                                                }
                                                if(ok_filename==0)
                                                {
                                                    printf("Championship name already taken. Please try again!\n");
                                                }
                                                else
                                                {
                                                    
                                                 while(1)
                                                    {
                                                        printf("Specifica sportul (tenis sau fotbal):");
                                                        scanf("%s",sport);
                                                        if(strcmp(sport,"tenis")==0 || strcmp(sport,"fotbal")==0)
                                                        break;
                                                    }
                                                    
                                                    if (write (sd, sport,35) <= 0) //trimitem sportul la server
                                                    {
                                                        perror ("[client]Eroare la write() spre server.\n");
                                                        return errno;
                                                    }
                                                 while(1)
                                                    {
                                                        printf("Selectati structura campionatului (single-elimination sau double-elimination):");
                                                        scanf("%s",structura_campionat);
                                                        if(strcmp(structura_campionat,"single-elimination")==0 || strcmp(structura_campionat,"double-elimination")==0)
                                                        break;
                                                    }
                                                    if (write (sd, structura_campionat,35) <= 0) //trimitem structura la server
                                                    {
                                                        perror ("[client]Eroare la write() spre server.\n");
                                                        return errno;
                                                    }
                                                    while(1)
                                                    {
                                                    printf("Selectati modul de extragere al partidelor (blind-draw sau seeded-draw):");
                                                    scanf("%s",draw);
                                                    if(strcmp(draw,"blind-draw")==0 || strcmp(draw,"seeded-draw")==0)
                                                    break;
                                                    }
                                                    if (write (sd, draw,35) <= 0) //trimitem tipul de draw la server
                                                    {
                                                        perror ("[client]Eroare la write() spre server.\n");
                                                        return errno;
                                                    }
                                                    while(1)
                                                    {
                                                    if(strcmp(sport,"fotbal")==0)
                                                        printf("Numarul maxim de echipe (trebuie sa fie o putere de a lui 2):");
                                                        else
                                                    printf("Numarul maxim de participanti (trebuie sa fie o putere de a lui 2):");
                                                    scanf("%d",&numar_part);
                                                    numar=numar_part;
                                                        while(numar_part%2==0)
                                                            numar_part/=2;
                                                         if(numar_part==1)
                                                            break;   
                                                    }
                                                    if (write (sd, &numar,sizeof(numar)) <= 0) //trimitem numarul de participanti la server
                                                    {
                                                        perror ("[client]Eroare la write() spre server.\n");
                                                        return errno;
                                                    }
                                                }
                                                break;

                                                case 2: strcpy(campionate,"championships");
                                                        print_file(campionate);
                                                       while(1) 
                                                        {
                                                        printf("Introdu numele campionatului despre care vrei sa afli mai multe:\n");
                                                        strcpy(campionate,"championships");    
                                                        scanf("%s",campionat_ales);
                                                        strcpy(copie_campionat,campionat_ales);
                                                       
                                                        if(fisier_seek_prim(campionat_ales,campionate)==0)
                                                            {
                                                                printf("Campionatul acesta nu exista!\n");
                                                                sleep(1);
                                                                printf("Incearca din nou.\n");
                                                                sleep(1);
                                                                strcpy(campionate,"championships"); 
                                                                print_file(campionate);
                                                            }
                                                          else
                                                            {   
                                                                print_file(campionat_ales);
                                                                printf("Doriti sa va inscrieti la acest campionat?\n");
                                                                ok_participare = 0;
                                                                while(1)
                                                                {   printf("Da/Nu:");
                                                                    
                                                                    scanf("%s",raspuns_participare);
                                                                   
                                                                    if(strcmp(raspuns_participare,"Da")==0 || strcmp(raspuns_participare,"Nu")==0 
                                                                      || strcmp(raspuns_participare,"da")==0 || strcmp(raspuns_participare,"nu")==0)
                                                                      {
                                                                         
                                                                          if(strcmp(raspuns_participare,"Da")==0 || strcmp(raspuns_participare,"da")==0)
                                                                          {
                                                                              strcat(copie_campionat,"_users.txt");
                                                                              if(fisier_seek(ID,copie_campionat)==1)
                                                                              {
                                                                                  printf("Acest user deja participa la campionat.\n");
                                                                                  if (write (sd, &ok_participare,sizeof(ok_participare)) <= 0) 
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    }
                                                                                  break;
                                                                              }
                                                                              a=first_line(campionat_ales);
                                                                              b=max_participants(campionat_ales);
                                                                              if(a==b)
                                                                              {
                                                                                  printf("Nu mai sunt locuri la acest campionat.\n");
                                                                                  if (write (sd, &ok_participare,sizeof(ok_participare)) <= 0) 
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    }
                                                                                  break;
                                                                              }
                                                                              a++;
                                                                              replace_number(campionat_ales,a);
                                                                            //  printf("%d\n",a);
                                                                              ok_participare=1;
                                                                              if (write (sd, &ok_participare,sizeof(ok_participare)) <= 0) 
                                                                                {
                                                                                    perror ("[client]Eroare la write() spre server.\n");
                                                                                    return errno;
                                                                                }
                                                                               if (write (sd, copie_campionat,100) <= 0) //trimitem camp ales la server
                                                                                {
                                                                                    perror ("[client]Eroare la write() spre server.\n");
                                                                                    return errno;
                                                                                } 
                                                                              printf("Pana acum s-au inscris %d/%d jucatori.\n",first_line(campionat_ales),
                                                                              max_participants(campionat_ales));
                                                                              
                                                                           printf("Introduceti mail-ul pe care doriti sa fiti contactat:");
                                                                           while(1) 
                                                                            {
                                                                                scanf("%s",mail);
                                                                                if(is_valid(mail)==0)
                                                                                {
                                                                                    printf("The mail is not valid, try again.\n");
                                                                                }
                                                                                else
                                                                               {
                                                                                   printf("The mail is valid.\n");
                                                                                   if (write (sd, mail,256) <= 0) //trimitem mail-ul la server
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    } 
                                                                                   break;
                                                                               }
                                                                            }                                                                                      
                                                                          }
                                                                        else if(strcmp(raspuns_participare,"Nu")==0 || strcmp(raspuns_participare,"nu")==0)  
                                                                            {
                                                                                if (write (sd, &ok_participare,sizeof(ok_participare)) <= 0) 
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    }
                                                                                    break;
                                                                            }
                                                                            if(a==b)
                                                                               {
                                                                                    printf("Ati ocupat ultimul loc disponibil, informatiile legate de desfasurarea campionatului v-au fost trimise pe mail.\n");
                                                                                    ok_max_participants=1;
                                                                                    if (write (sd, &ok_max_participants,sizeof(ok_max_participants)) <= 0) 
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    }
                                                                               }
                                                                               else
                                                                                   {
                                                                                     ok_max_participants=0;  
                                                                                     if (write (sd, &ok_max_participants,sizeof(ok_max_participants)) <= 0) 
                                                                                    {
                                                                                        perror ("[client]Eroare la write() spre server.\n");
                                                                                        return errno;
                                                                                    }
                                                                                   } 
                                                                      break;
                                                                      }
                                                                }
                                                                break;
                                                            }
                                                        }  
                                                        break; 

                                                case 3: quit_login=1;
                                                        break;
                                            }
                                        }

                                    }        
                                }
                            break;
                     case 3: stop=1;
                             break;
                     case 4: ok_campionat_terminat=0;
                             printf("Username:");
                             scanf("%s",username_administrator);
                             if(strcmp(username_administrator,"administrator")!=0)
                             {
                                 printf("Username gresit.\n");
                                 break;
                             }
                              printf("Password:");
                              scanf("%s",password_administrator);
                             if(strcmp(password_administrator,"administrator1")!=0)
                             {
                                 printf("Parola gresita.\n");
                                 break;
                             }
                             print_file1("CampionateTerminate.txt",&numar_linii);
                             printf("Scrieti campionatul caruia vreti sa puneti scorurile.\n");
                             scanf("%s",campionat_terminat);
                             strcat(campionat_terminat,"_meciuri.txt");
                             print_file1(campionat_terminat,&numar_linii);
                             numar_linii=numar_linii-1;                            
                             index_terminat=1;
                             while(numar_linii!=0)
                             {
                                 printf("Scor meciul %d:",index_terminat);
                                 scanf("%s",scor);
                                 replace_line1(campionat_terminat,scor,index_terminat);
                                 index_terminat++;
                                 numar_linii--;
                             }
                             break;
                }
           
    }                   
    /* inchidem conexiunea, am terminat */
    close (sd);
    return 0;
}