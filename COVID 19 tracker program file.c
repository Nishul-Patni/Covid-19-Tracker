#include<stdio.h>
#include<stdlib.h>
#include<conio.h>

//structure for storing patient record
struct patient{
	int age;
	int id;
	char name[40];
	char place[30];
	char status;
};

//global variables
int recsize = sizeof(struct patient);
char pass[20]="LetMeIn";				//password to get access for modifing records 

void modify();			   		//adding and changing patient status
void del();						//called in modify to modifing a patient status
void check_status();			//tells total cases, total active cases, total recovered cases and total deaths.
void tips();					//Print tips
void assesment();				//For a digital assessment test

int main(){
	char choice;
	while(1){
		system("cls");
		printf("\n\n\t\t|||||| MENU ||||||");
		printf("\n\n\t\t1. Digital Corona assessment test");
		printf("\n\t\t2. Check corona status");
		printf("\n\t\t3. Preventive measures and tips to fight against Corona Virus");
		printf("\n\t\t4. Modify Corona records (only for authorised people)");
		printf("\n\t\t5. Exit");
		printf("\n\n\t\tYour choice and press enter: ");
		fflush(stdin);
		scanf("%c",&choice);
		system("cls");
		switch(choice){
			case '1':
				assesment();
				break;
			case '2':
				check_status();
				break;
			case '3':
				tips();
				getch();
				break;
			case '4':
				modify();
				break;
			case '5':
				exit(0);
				break;
			default:
				printf("\t\tWrong Choice.");
				printf("\n\t\tPress any key to go back in Menu");
				getch();
				system("cls");
		}
		
	}
	return 0;
}

//function defination

void tips()
{
	FILE *ftips = fopen("Tips.txt","r");
	if(ftips==NULL){
		printf("\n\n\t\tERROR 404.");
	}
	char c;
	c = fgetc(ftips);
	while(c != EOF){
		printf("%c",c);
		c = fgetc(ftips);	
	}
	fclose(ftips);
}

void modify(){
//	password authorization
	char password[20];
	printf("\n\n\t\tOnly for authorized people");
	printf("\n\t\tEnter password: ");
	fflush(stdin);
	gets(password);

	if(strcmp(password, pass)){
		printf("\t\twrong password. Press any key to return.");
		getch();
		return;
	}
	
	system("cls");
//	now the authorized person can 
//	1. add new patients
//	2. view patients
//	3. delete patients by entering thier p_id
	char choice;
	FILE *fr = fopen("record.dat","ab+");
	struct patient p, pcheck;
	int id;
	char another = 'y';
	int check;
	while(1){
		system("cls");
		printf("\n\n\t\t|||||| WELCOME |||||||");
		printf("\n\t\t1. Add new patient in record");
		printf("\n\t\t2. View record");
		printf("\n\t\t3. Change status of patient record");
		printf("\n\t\t4. Log Out");
		printf("\n\n\t\tYour choice and press enter: ");
		fflush(stdin);
		scanf("%c",&choice);
		system("cls");
		switch(choice){
			case '1':
				another='y';
				while(another=='y'){
					
					printf("\n\n\t\tPatient ID:           ");
					fseek(fr, -recsize, SEEK_END);
					fread(&p, recsize, 1, fr);
					printf("%d",++p.id);
					fseek(fr, recsize, SEEK_END);	
					
					printf("\n\t\tEnter patient name:   ");
					fflush(stdin);
					gets(p.name);
			
				    printf("\t\tEnter Age:            ");
				    scanf("%d",&p.age);
					
					printf("\t\tEnter Place:          ");
					fflush(stdin);
					gets(p.place);
					
					p.status = 'a';
						
					fwrite(&p,recsize,1,fr);		
	        		fflush(stdin);
			        printf("\n\t\tAdd another record(y/n): ");
    		    	scanf("%c",&another);
	    	    	system("cls");
				}
				break;
			case '2':
        		printf("\n\t\t%-6s %-30s %-5s %-20s %-7s","ID", "Name","Age", "Age", "Status"); 
				printf("\n\t\t-----------------------------------------------------------------------\n");
				rewind(fr);
				while(fread(&p,recsize,1,fr)==1)  /// read the file and fetch the record one record per fetch
				{
    	    		printf("\n\t\t%-6d %-30s %-5d %-20s %-2c",p.id, p.name,p.age,p.place, p.status); 
					// print the name, age, place, status and id 
	    		}
	    		getch();
				break;
		
			case '3':
				fclose(fr);
				del();
				fr = fopen("record.dat","ab+");
				break;
		
			case '4':
				return;
				break;
			default:
				printf("\t\tWrong Choice.");
				printf("\n\t\tPress any key to go back in Menu");
				getch();
				system("cls");
		}
	}	
	fclose(fr);
}

void del(){
	char another='y';
	FILE *fr, *ft;
	int id, check, status;
	struct patient p;
	while(another=='y'){
		fr = fopen("record.dat", "rb");
		ft = fopen("temp.dat", "wb+");
		
//		printing active and recovered patients
        printf("\n\t\t%-6s %-30s %-5s %-20s %-7s","ID", "Name","Age", "Age", "Status"); 
		printf("\n\t\t-----------------------------------------------------------------------\n");
		while(fread(&p, recsize, 1, fr)==1){
			if(p.status!='d')
   	    		printf("\n\t\t%-6d %-30s %-5d %-20s %-2c",p.id, p.name,p.age,p.place, p.status); 
		}
		
		printf("\n\n\n\t\tEnter Patient ID: ");
		scanf("%d",&id);
		check=0;
		rewind(fr);
		while(fread(&p, recsize, 1, fr)==1){
			if(p.id==id){
   	    		printf("\n\t\tPatient record-:");
				printf("\n\t\tP.ID    :%d\n\t\tName    :%s\n\t\tAge     :%d\n\t\tPlace   :%s\n\t\tStatus  %c:",p.id, p.name,p.age,p.place, p.status); 
				if(p.status=='a'){
					printf("\n\n\t\tPress 1 (If Recovered)\n\t\t2 (If Died)\n\t\t: ");
					scanf("%d",&status);
					if(status==1)
						p.status = 'r';
					else if(status==2)
						p.status = 'd';
					else{
						printf("\n\t\tWrong input. Press any key to rerutn back");
						getch();
						return;
					}
					check=1;
				}
				else if(p.status=='r'){
					p.status = 'a';
					check=1;
				}
				
			}
			fwrite(&p, recsize, 1, ft);		
		}
		
		if(check==1)
			printf("\n\t\tStatus Updated succfully.");
		else
			printf("\n\t\tWrong Id entered.");
				
		fclose(fr);
		fclose(ft);
		remove("record.dat");
		rename("temp.dat","record.dat");
		fclose(fr);
		fclose(ft);
		printf("\n\n\n\t\tAdd another record(y/n) ");
	   	fflush(stdin);
       	scanf("%c",&another);
	    system("cls");	
	}
	
}

void check_status(){
	FILE *fr=fopen("record.dat", "rb");
	struct patient p;
	int total=0, active=0, recovered=0, death=0;
	while(fread(&p,recsize,1,fr)==1){
		if(p.status=='a')
			active++;
		else if(p.status=='r')
			recovered++;
		else
			death++;
		total++;	
	}
	printf("\n\n\n\t\t==> Total cases             : %d", total);
	printf("\n\n\t\t==> Total active cases      : %d", active);
	printf("\n\n\t\t==> Total recovered cases   : %d", recovered);
	printf("\n\n\t\t==> Total death             : %d", death);
	
	fclose(fr);
	getch();
}

void assesment(){
	int point=0;
	char ans;
	printf("\n\n\t\t\t\t| | | | | | DIGITAL CORONA ASSESSMENT | | | | | |");
	printf("\n\n\t\t-------------------------------------------------------------------------------------");
	printf("\n\n\t\t==> Are you experiencing any of the following symptoms -:");
	printf("\n\t\tCough, Fever, Difficulty in breathing, Loss of senses of smeel and taste");
	printf("\n\t\tPress y if yes, n if no: ");
	fflush(stdin);
	scanf("%c",&ans);
	if(ans=='y' || ans=='Y'){
		printf("\n\n\t\tDo you experiencing more than one symptoms");
		printf("\n\t\tPress y if yes, n if no: ");
		fflush(stdin);
		scanf("%c",&ans);
		if(ans=='y' || ans=='Y'){
			printf("\n\n\t\t==>If the information provided by you is accurate,%s",
			"\n\t\t    it indicates that you are either unwell or at risk.");	
			printf("\n\t\t    We recommend you to take corona test in the nearest hospital of pathelogy lab.");
			printf("\n\n\t\tPress any key to return.");
			getch();
			return;
		}
		else if(ans=='n' || ans=='N')
			point++;
		else{
			printf("\n\n\t\tWrong input. Press any key to retutn.");
			getch();
			return;
		}
	}
	
	printf("\n\n\t\tHave you ever had any of the following -:");
	printf("\n\t\tDiabetes, Hypertension, Lung disease, Heart Disease, Kidney Disorder");
	printf("\n\t\tPress y if yes and n if no: ");
	fflush(stdin);
	scanf("%c",&ans);
	if(ans=='y' || ans=='Y')
		point++;
	
	printf("\n\n\t\tHave you traveled anywhere Internationally in last 28-25 days?");
	printf("\n\t\tPress y if yes and n if no: ");
	fflush(stdin);
	scanf("%c",&ans);
	if(ans=='y' || ans=='Y'){
		system("cls");
		FILE *ftravelled = fopen("travelled.txt","r");
		if(ftravelled==NULL){
			printf("ERROR. File not found.");
			exit(0);
		}
		char c;
		c = fgetc(ftravelled);
		while(c != EOF){
			printf("%c",c);
			c = fgetc(ftravelled);	
		}
		fclose(ftravelled);	
		printf("\n\n\t\tPress any key to return.");
		getch();
		return;
	}

	printf("\n\t\tWhich of the following apply to you?");
	printf("\n\t\t1. I have recently intracted or lived with someone who has tested COVID-19 positive.");
	printf("\n\t\t2. I am a COVID-19 worker and I examined a COVID-19 confirmed case without protective gear.");
	printf("\n\t\t3. None of the above");
	printf("\n\t\tEnter your input: ");
	fflush(stdin);
	scanf("%c",&ans);
	if(ans=='1' || ans=='2')
		point+=2;
	else if(ans=='3' || point<3){
		system("cls");
		printf("\n\n\t\tYour infection risk is low.%s%s%s%s%s",
		"\n\t\tWe recommend that you stay at home to avoid any chance of exposure to the Novel Coronavirus.",
		"\n\n\t\tRetake the Self-Assesment Test if you develop symptoms or come in contact with a COVID-19 confirmed patient.",
		"\n\n\t\tDo visit",
		"\n\t\thttps://www.mohfw.gov.in/ for more information",
		"\n\n\t\tTHANK YOU for using our self assessment test.");
		getch();
		return;
	}
	else{
		printf("\n\n\t\tWrong input. Press any key to return.");
		getch();
		return;
	}
	
	if(point==3 || point==4){		
		system("cls");
		printf("\n\n\t\t==>If the information provided by you is accurate,%s",
		"\n\t\tit indicates that you are either unwell or at risk.");	
		printf("\n\t\t   We recommend you to take corona test in the nearest hospital of pathelogy lab.");
		printf("\n\n\t\tPress any key to return.");
		getch();
		return;
	}
							
}

