
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* portul folosit */
#define PORT 2025

/* codul de eroare returnat de anumite apeluri */
extern int errno;

      int ok_username=1,ok_max_participants,ok_campionat_terminat;
      char ID[25];
      char user[25];
      char pass[25];
      int ok;
      int ok_register=1;
      int ok_password=1;
      int login_number;
      int login_quit;
      char filename[35], sport[35], structura_campionat[35], draw[35];
      int numar_part;
      int ok_filename=1;
      int participanti_curent;
      int ok_participare;
      char camp_ales[100];
      char mail[256];
      char camp_ales_copie[256];
int stop=0;

void send_mail(char to[],char body[]);

void fisier_write1(char nume[25], char *filename)
{
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
    }
        fprintf(fp,"%s",nume);
    fflush(fp);    
    fclose(fp);    
}

void swap(int *a, int *b) 
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void randomize(int arr[], int n) 
{
    srand(time(NULL));
    int i;
    for(i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

void get_Data(int *day, int *month, int *year)
{   
    time_t now;
    time(&now);
    ctime(&now);
    struct tm *local = localtime(&now);
    *day = local->tm_mday;        
    *month = local->tm_mon + 1;      
    *year = local->tm_year + 1900;   
}

void draw_games(char filename[])
{  
    int day,month,year;
    char orar[20][6]={"10:00","12:00","14:00","16:00","18:00","20:00"};
    int indice_orar=0;
    char copie_filename[256];
    strncpy(copie_filename,filename,strlen(filename)-10);
    strcat(copie_filename,"_meciuri.txt");
    get_Data(&day,&month,&year);
    day=day+2;;
    FILE *fp;
    fp=fopen(filename,"r");
    char line[256]={};
    char cuvinte[256][256]={};
    char mail[256][256];
    char mail_sent[2000];
    int indice_mail_sent=0;
    int nr=1,i=0,j,y;
    int ora=10;
    int index_mail=0;
    while(fgets(line,256,fp)!=NULL)
    {
        if(nr%2!=0)
           {   
               line[strlen(line)-2]=NULL;
               strcpy(cuvinte[i++],line);
           }
           else
           {
               line[strlen(line)-2]=NULL;
               strcpy(mail[index_mail++],line);
           } 
        nr++;    
    }
    //strcat(mail[index_mail-1],"om");
    // for(j=0;j<i;j++)
    //     printf("%s\n",cuvinte[j]);

    int index[100],x,ind=0;
    for(x=0;x<i;x++)
      index[x]=ind++;
    randomize(index,ind);
    for(x=0;x<ind-1;x=x+2)
        {   y=x+1;
            
            if(indice_mail_sent>4)
            {
               indice_mail_sent=0;
               indice_orar++;
            }
            fisier_write1(cuvinte[index[x]],copie_filename);
            fisier_write1(" - ",copie_filename);
            fisier_write1(cuvinte[index[y]],copie_filename);
            fisier_write1("\n",copie_filename);
            strcpy(mail_sent,"Salut, sunt administratorul aplicatiei Championship. Meciul si ora desfasurarii meciului tau: ");
            strcat(mail_sent,cuvinte[index[x]]);
            strcat(mail_sent," - ");
            strcat(mail_sent,cuvinte[index[y]]);
            strcat(mail_sent,", ora:");
            strcat(mail_sent,orar[indice_orar]);
            mail_sent[strlen(mail_sent)]=NULL;
            send_mail(mail[x],mail_sent);
            send_mail(mail[y],mail_sent);
            indice_mail_sent+=2;
        }
    // for(x=0;x<index_mail;x++)
    // printf("%s\n",mail[x]);    
    fclose(fp);

}

void send_mail(char to[],char body[])
{
        char cmd[1000]; // to hold the command.
        FILE *fp = fopen("mail_send.txt","w"); // open it for writing.
        fprintf(fp,"To: %s\n",to);
        fprintf(fp,"From: Championship\n");
        fprintf(fp,"Subject: Orar meci\n");
        fprintf(fp,"\n");
        fprintf(fp,"%s\n",body);        // write body to it.
        fclose(fp);             // close it.

        system("sendmail -t < mail_send.txt"); 
}

void campionat_fisier(char filename[35],char sport[35], char structura[35], char draw[35], int numar_part,int participanti)
{   
    int ok=0;
    if(strcmp(filename,"fotbal")==0)
        ok=1;
    strcat(filename,".txt");
    const char *filename1;
    filename1=filename;
    FILE *fp = fopen(filename1,"a");
    fprintf(fp,"%d",participanti);
    fprintf(fp,"\r\n");
    fprintf(fp,"Sportul este:%s",sport);
    fprintf(fp,"\r\n");
    fprintf(fp,"Structura campionatului este:%s",structura);
    fprintf(fp,"\r\n");
    fprintf(fp,"Deciderea partidelor:%s",draw);
    fprintf(fp,"\r\n");
    if(ok==1)
    fprintf(fp,"Numarul maxim de echipe:%d",numar_part);
    else
        fprintf(fp,"Numarul maxim de participanti:%d",numar_part);
    fprintf(fp,"\r\n");
    fflush(fp);    
    fclose(fp);    
}

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
    fflush(fp);
    fclose(fp);
}

void fisier_write(char nume[25], char *filename)
{
    FILE *fp = fopen(filename, "a");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
    }
        fprintf(fp,"%s",nume);
        fprintf(fp,"\r\n");
    fflush(fp);    
    fclose(fp);    
}

int nlines(char nume[25], char *filename)
{
    
    char str[25];
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        return -1;
    }
    int ok=0,i=0,j=0;  
    char string[25];
    int nlines=0;
    while(fgets(str,25,fp)!=NULL)
    {  nlines++;
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
        memset(&str[0], 0, sizeof(str));         
    }
    if(ok==1)
    {
       return nlines;
    }
    else
    return 0;
    
}
int password_verifier(char user[25], char password[25])
{
    if(fisier_seek(password,"passwords.txt")==1)
    {
        int x=nlines(password,"passwords.txt");
        int y=nlines(user,"usernames.txt");
        if(x==y)
        {
            return 1;
        }
    }
    return 0;
}

int main ()
{
    struct sockaddr_in server;	// structura folosita de server
    struct sockaddr_in from;
    char msg[100];		//mesajul primit de la client
    char msgrasp[100]=" ";        //mesaj de raspuns pentru client
    int sd;			//descriptorul de socket

    /* crearea unui socket */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
    	perror ("[server]Eroare la socket().\n");
    	return errno;
    }

    /* pregatirea structurilor de date */
    bzero (&server, sizeof (server));
    bzero (&from, sizeof (from));

    /* umplem structura folosita de server */
    /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;
    /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    /* utilizam un port utilizator */
    server.sin_port = htons (PORT);

    /* atasam socketul */
    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
    	perror ("[server]Eroare la bind().\n");
    	return errno;
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen (sd, 1) == -1)
    {
    	perror ("[server]Eroare la listen().\n");
    	return errno;
    }

    /* servim in mod concurent clientii... */
    while (1)
    {
    	int client;
    	int length = sizeof (from);

    	printf ("[server]Asteptam la portul %d...\n",PORT);
    	fflush (stdout);

    	/* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    	client = accept (sd, (struct sockaddr *) &from, &length);

    	/* eroare la acceptarea conexiunii de la un client */
    	if (client < 0)
    	{
    		perror ("[server]Eroare la accept().\n");
    		continue;
    	}

    	int pid;
    	if ((pid = fork()) == -1) {
    		close(client);
    		continue;
    	} else if (pid > 0) {
    		// parinte
    		close(client);
    		while(waitpid(-1,NULL,WNOHANG));
    		continue;
    	} 
        
        else if (pid == 0) 
        {   
    		// copil
    		close(sd);
          while(stop!=1)
          {
                /* s-a realizat conexiunea, se astepta mesajul */
                bzero (msg, 100);
                printf ("[server]Asteptam mesajul...\n");
                fflush (stdout);
        
                /* citirea mesajului */
                if (read (client, msg, 100) <= 0)
                {
                    perror ("[server]Eroare la read()1 de la client.\n");
                    close (client);	/* inchidem conexiunea cu clientul */
                    continue;		/* continuam sa ascultam */
                }
                
                int number=charToINT(msg);
                printf ("[server]Mesajul a fost receptionat...%s\n", msg);
               
                ok_register=1;
                ok_username=1;
                ok_password=1;
                switch(number)
                {
                    case 1: printf("sunt in case 1\n");
                            if (read (client, ID, 100) <= 0)
                                {
                                    perror ("[server]Eroare la read() de la client.\n");
                                    close (client);	/* inchidem conexiunea cu clientul */
                                    continue;		/* continuam sa ascultam */
                                }

                            if(fisier_seek(ID,"usernames.txt")==0) //daca id-ul este disponibil
                                {   
                                    if (write (client, &ok_register, sizeof(ok_register)) <= 0)
                                    {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;		/* continuam sa ascultam */
                                    } 
                                    printf("scriu in fisier id-ul\n");
                                    fisier_write(ID,"usernames.txt"); //scriem in fisier username-ul
                                    printf("am scris in fisier\n");

                                    if (read (client, pass, 100)<= 0) //citim parola trimisa de client
                                        {
                                        perror ("[server]Eroare la read() de la client.\n");
                                        close (client);	/* inchidem conexiunea cu clientul */
                                        continue;		/* continuam sa ascultam */
                                        } 
                                        
                                        fisier_write(pass,"passwords.txt"); // scriem in fisier parola 
                                }
                            else
                                {   ok_register=0;
                                    if (write (client, &ok_register, sizeof(ok_register)) <= 0)
                                    {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;		/* continuam sa ascultam */
                                    } 
                                }
                                break;

                    case 2: printf("sunt in case 2\n");

                            if (read (client, ID, 100) <= 0)
                                {
                                    perror ("[server]Eroare la read() de la client.\n");
                                    close (client);	/* inchidem conexiunea cu clientul */
                                    continue;		/* continuam sa ascultam */
                                }

                            printf("am primit username %s\n",ID);
                            
                            if(fisier_seek(ID,"usernames.txt")==0)
                                ok_username=0;

                            if (write (client, &ok_username, sizeof(ok_username)) <= 0)
                                    {
                                        perror ("[server]Eroare la write() catre client.\n");
                                        continue;		/* continuam sa ascultam */
                                    }

                            if(ok_username==1)
                            {    strcpy(user,ID);
                                printf("user este %s\n",user);
                                if (read (client, pass, 100) <= 0)
                                        {
                                            perror ("[server]Eroare la read() de la client.\n");
                                            close (client);	/* inchidem conexiunea cu clientul */
                                            continue;		/* continuam sa ascultam */
                                        }
                                if (read (client, user, 100) <= 0)
                                        {
                                            perror ("[server]Eroare la read() de la client.\n");
                                            close (client);	/* inchidem conexiunea cu clientul */
                                            continue;		/* continuam sa ascultam */
                                        }              
                                printf("user %s\n",user);
                                printf("parola e %s\n",pass);
                                
                                if(password_verifier(user,pass)==0)
                                    {printf("usernme este %s\n",user);ok_password=0;}

                                if (write (client, &ok_password, sizeof(ok_password)) <= 0)
                                        {
                                            perror ("[server]Eroare la write() catre client.\n");
                                            continue;		/* continuam sa ascultam */
                                        }
                                
                                
                                login_quit=0;
                             if(ok_password==1)  
                             { 
                                while(login_quit!=1)
                                {   ok_filename=1;
                                    if (read (client, &login_number, sizeof(login_number)) <= 0) //citim nr comenzii de la client
                                        {
                                            perror ("[server]Eroare la read() de la client.\n");
                                            close (client);	/* inchidem conexiunea cu clientul */
                                            continue;		/* continuam sa ascultam */
                                        }
                                        printf("[server] nr comenzii este %d\n",login_number);
                                    switch(login_number)
                                    {
                                        case 1:  if (read (client, filename, 35) <= 0) //citim numele ales de client
                                                {
                                                    perror ("[server]Eroare la read() de la client.\n");
                                                    close (client);	/* inchidem conexiunea cu clientul */
                                                    continue;		/* continuam sa ascultam */
                                                }
                                                printf("[server] numele campionatului este %s\n",filename);
                                                if(fisier_seek(filename,"championships.txt")==0)
                                                {   
                                                    if (write (client, &ok_filename, sizeof(ok_filename)) <= 0)
                                                    {
                                                        perror ("[server]Eroare la write() catre client.\n");
                                                        continue;		/* continuam sa ascultam */
                                                    } 
                                                    fisier_write(filename,"championships.txt");                                                    
                                                    
                                                    
                                                    if (read (client, sport, 35) <= 0) //citim sportul aleasa de client
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] sportul ales este:%s\n",sport);
                                                    if (read (client, structura_campionat, 35) <= 0) //citim structura aleasa de client
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] structura aleasa este %s\n",structura_campionat);
                                                    if (read (client, draw, 35) <= 0) //citim tipul de draw ales de client
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] tipul de draw ales este:%s\n",draw);
                                                    if (read (client, &numar_part, sizeof(numar_part)) <= 0) //citim numarul de participanti ales de client
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] numarul de participanti este:%d\n",numar_part);
                                                    campionat_fisier(filename, sport, structura_campionat, draw, numar_part,participanti_curent); // creem fisierul campionatului
                                                }
                                                else
                                                {
                                                    ok_filename=0;
                                                    if (write (client, &ok_filename, sizeof(ok_filename)) <= 0)
                                                    {
                                                        perror ("[server]Eroare la write() catre client.\n");
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                }
                                                printf("[server] ok_filename este %d",ok_filename);            
                                                break;

                                        case 2: if (read (client,&ok_participare , sizeof(ok_participare)) <= 0)
                                                {
                                                    perror ("[server]Eroare la read() de la client.\n");
                                                    close (client);	/* inchidem conexiunea cu clientul */
                                                    continue;		/* continuam sa ascultam */
                                                }
                                               
                                                printf("[server] ok_participare este %d\n",ok_participare);
                                                if(ok_participare==1)
                                                  {  if (read (client, camp_ales, 100) <= 0)
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] campionatul ales este %s\n",camp_ales);
                                                     strncpy(camp_ales_copie,camp_ales,strlen(camp_ales)-10);
                                                     
                                                    fisier_write(user,camp_ales);
                                                    printf("[server] am scris participantul %s in fisier.\n",user);
                                                    if (read (client, mail, 256) <= 0)
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    fisier_write(mail,camp_ales);
                                                    printf("[server] am scris mail-ul %s in fisier.\n",mail);
                                                  
                                                  if (read (client,&ok_max_participants , sizeof(ok_max_participants)) <= 0)
                                                    {
                                                        perror ("[server]Eroare la read() de la client.\n");
                                                        close (client);	/* inchidem conexiunea cu clientul */
                                                        continue;		/* continuam sa ascultam */
                                                    }
                                                    printf("[server] ok_max_participants este %d",ok_max_participants);  
                                                    if(ok_max_participants==1)
                                                    {
                                                        draw_games(camp_ales);
                                                        fisier_write(camp_ales_copie,"CampionateTerminate.txt");
                                                    }  
                                                 }   
                                                 break;   

                                        case 3: login_quit=1;
                                                break;        
                                    }
                                }
                              }                                      
                            }    
                            break;  

                    case 3: printf("sunt in case 3\n");
                            stop=1;
                            break;
                            close (client);
                    case 4: printf("[server] case 4\n");
                            // if (read (client,&ok_campionat_terminat, sizeof(ok_campionat_terminat)) <= 0)
                            //  {
                            //   perror ("[server]Eroare la read() de la client.\n");
                            //   close (client);	/* inchidem conexiunea cu clientul */
                            //   continue;		/* continuam sa ascultam */
                            //  }
                            //  printf("[server] ok_campionat_terminat este %d\n",ok_campionat_terminat);
                            break;                   
                }
                printf("stop este %d\n",stop);
            }    

    		close (client);
    		exit(0);
    	}

    }				/* while */
}				/* main */