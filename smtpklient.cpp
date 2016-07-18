  #include <stdio.h>
#include <unistd.h> 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream> 
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>


using namespace std;
int spracuj(string cestat, vector<string> *emailu);
int main (int argc, char **argv) {
  int parametry,hlavni_soked,  cislo_portu=25,cas=0;
  int res;
  bool cestat(true);
  char msgg[400],msggo[400],prijmana_sprava[400];
  string ip="127.0.0.1";
  string cesta,sprava="HELO xkonde03";
  struct sockaddr_in sin;
  vector<string> emaily;
  emaily.push_back(sprava);
  
 while ((parametry = getopt(argc, argv, "a:p:i:w:")) != -1) 
    switch (parametry) {
      
      case 'a':
	ip=optarg;
      
      break;
      
      case 'p':
	cislo_portu=atoi(optarg);
      break;
      
      case 'i':
	cestat=false;
	cesta=optarg;
      break;
      
      case 'w':
	cas=atoi(optarg);
	if(cas>3600)
	{
	  cerr << "error vic nez hodina "<< endl;
	  return EXIT_FAILURE;
	}
      break;
    
      
    }
    
    if(cestat)
    {
      cerr << "error neni zadan soubor "<< endl;
      return EXIT_FAILURE;
    }
    
    cout<< ip<<endl;
    cout<< cas<<endl;
    spracuj(cesta,&emaily);
     for(int i = 0; i < emaily.size(); i++)
	{
		cout  << emaily[i] << 	endl;
	}
    
    
   
    //////////////////////////////////////////////////////////////*
    
    if ( (hlavni_soked = socket(PF_INET, SOCK_STREAM, 0 ) ) < 0) //vytvoreni soketu
   { 
    cerr << "error on soket "<< endl;  
      return EXIT_FAILURE;
    }
    char * newip = new char [ip.length()+1];
    strcpy (newip, ip.c_str());
    //nastaveni parametru
  sin.sin_family = PF_INET;
  sin.sin_port = htons(cislo_portu);  
 // res = inet_pton(PF_INET,"127.0.0.1",&sin.sin_addr);
  sin.sin_addr.s_addr = inet_addr(newip);
  
  
  //cout<<"aloha";
  if (connect (hlavni_soked, (struct sockaddr *)&sin, sizeof(sin) ) < 0 )
  {
    cerr << "error on connect "<< endl; 
    return EXIT_FAILURE;
  }
  
	for(int z=0;z< emaily.size(); z++){
	sprava=emaily[z];
	 char * msg = new char [sprava.length()+1]; //prevedeni na char
	strcpy (msg, sprava.c_str());
	cout<<msg<<endl;
	if ( write(hlavni_soked, msg, strlen(msg) +1) < 0 ) //poslani spravy
	{  
	  cerr << "error on write "<< endl;
	  return EXIT_FAILURE; 
	}
	 //velikost prijmane spravy prida nulu
	  bzero(prijmana_sprava,sizeof(prijmana_sprava)); //velikost prijmane spravy
	    
	    if ( read(hlavni_soked, prijmana_sprava, sizeof(prijmana_sprava) ) <0)  //cteni spravy
	    {  
	      cerr << "error on read "<< endl;         
	      return EXIT_FAILURE;
	    }
	    cout<<prijmana_sprava<<endl;
	
	}
	
	
	
 
 
     if (close(hlavni_soked) < 0) {  //uzavreni soketu
    cerr << "error on close "<< endl;   
    return EXIT_FAILURE;
  }
    return EXIT_SUCCESS;
}

int spracuj(string cestat, vector<string> *emailu){
  int c=0;
  int i=0;
  char neco;
  bool mezera=true;
  FILE *soubor;
  string email,email2;
  //vector <string> emailu;
  char * cestachar = new char [cestat.length()+1];
    strcpy (cestachar, cestat.c_str());
    cout<<cestat<<endl;
    
     if ((soubor = fopen(cestachar, "r")) == NULL) {
        cerr << "error nelz otevirt soubor" <<endl;
        return EXIT_FAILURE;
    }
   neco = fgetc(soubor);
   
	while (neco != EOF) {

		switch (i) {
		  case 0:
		     email="";
		    email=email+"MAIL FROM:<xkonde03@isa.local> ";
		    emailu->push_back(email);
		      email="";
		   
		  case 7:
		    email=email+"RCPT TO:<"+neco;
		    i=1;
		    break;
		    
		  case 1:
		
		    if(neco==',')
		    {
		      i=2;
		      email=email+">";
		      emailu->push_back(email);
		      email="";
		      break;
		    }
		    if(neco==' '){
		      i=4;
		      email=email+">";
		      emailu->push_back(email);
		      email="";
		      break;
		    }
		    email=email+neco;
		    break;
		  
		  case 2:
		    email=email+"RCPT TO:<"+neco;
		    i=3;
		   break;
		   
		  case 3:
		    
		      if(neco==',')
			{
			  i=2;
			  email=email+">";
			  emailu->push_back(email);
			  email="";
			  break;
		      }
		      if(neco==' '){
			i=4;
			email=email+">";
			emailu->push_back(email);
			email="";
			break;
		      }
		      email=email+neco;
			  break;
		  case 4:
		    email=email+"Data: "+neco;
		    i=5;
		    break;
		    
		    case 5:
		    email=email+neco;
		    if(neco=='\n'){
		     email=email+".";
		     emailu->push_back(email);
		     email="";
		      i=0;
		    }
		    
		    break;
	    }
      neco = fgetc(soubor);  
    } 
//  

 
    //cout<<"******"<<endl<<email<<"/////"<<endl;
     //cout <<emailu.back();
    fclose(soubor);
}
