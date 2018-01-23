/********************************************************************************
*						      			       				   
													SHA Secure Hash Algorithm				                                
*															Parallel 1.0 Version															
* Authors:           	Yuchen Liang and Syed Muhammad Zeeshan Iqbal
* Supervisor:					Hakan Grahn	 						
* Section:          	Computer Science of BTH,Sweden
* GCC Version:				4.2.4								
* Environment: 				Kraken(Ubuntu4.2.4) with Most 8 Processors 				
* Start Date:       	27th October 2009					
* End Date:         	23th November 2009			
*		
*********************************************************************************/


/* NIST Secure Hash Algorithm */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include "sha.h"

#define HB_ENERGY_IMPL
#include <heartbeats/hb-energy.h>
#include <heartbeats/heartbeat-accuracy-power.h>
#include <poet/poet.h>
#include <poet/poet_config.h>

#define PREFIX "SHA"
#define USE_POET // Power and performance control

heartbeat_t* heart;
poet_state* state;
static poet_control_state_t* control_states;
static poet_cpu_state_t* cpu_states;
unsigned int num_runs = 1000;
char* path;

#define MAX_NO_FILES 16
#define MAX_WORKERS 8
#define PRINT 0


typedef struct Params {
	int s_index;
	int e_index;
	int no;
	char *name;
} parameters;

struct fileData{
	  BYTE* data;
	  int size;
}Tdata[MAX_NO_FILES];

char* in_file_list[]={"in_file1.asc","in_file2.asc","in_file3.asc","in_file4.asc","in_file5.asc","in_file6.asc","in_file7.asc","in_file8.asc","in_file9.asc","in_file10.asc","in_file11.asc","in_file12.asc","in_file13.asc","in_file14.asc","in_file15.asc","in_file16.asc",NULL};
char* out_file_list[]={"out_file1.txt","out_file2.txt","out_file3.txt","out_file4.txt","out_file5.txt","out_file6.txt","out_file7.txt","out_file8.txt",NULL};


int no_files=16;
int no_workers=8;

pthread_attr_t string_attr;
pthread_mutex_t string_mutex;
pthread_t workers[MAX_WORKERS]; 

static int partition_size;
static parameters paramsArr[MAX_WORKERS];

void hb_poet_init() {
    float min_heartrate;
    float max_heartrate;
    int window_size;
    double power_target;
    unsigned int nstates;

    if(getenv(PREFIX"_MIN_HEART_RATE") == NULL) {
      min_heartrate = 0.0;
    }
    else {
      min_heartrate = atof(getenv(PREFIX"_MIN_HEART_RATE"));
    }
    if(getenv(PREFIX"_MAX_HEART_RATE") == NULL) {
      max_heartrate = 100.0;
    }
    else {
      max_heartrate = atof(getenv(PREFIX"_MAX_HEART_RATE"));
    }
    if(getenv(PREFIX"_WINDOW_SIZE") == NULL) {
      window_size = 30;
    }
    else {
      window_size = atoi(getenv(PREFIX"_WINDOW_SIZE"));
    }
    if(getenv(PREFIX"_POWER_TARGET") == NULL) {
      power_target = 70;
    }
    else {
      power_target = atof(getenv(PREFIX"_POWER_TARGET"));
    }

    printf("init heartbeat with %f %f %d\n", min_heartrate, max_heartrate, window_size);

    heart = heartbeat_acc_pow_init(window_size, 100, "heartbeat.log",
                                   min_heartrate, max_heartrate,
                                   0, 100,
                                   1, hb_energy_impl_alloc(), power_target, power_target);
    if (heart == NULL) {
      fprintf(stderr, "Failed to init heartbeat.\n");
      exit(1);
    }
#ifdef USE_POET
    if (get_control_states(NULL, &control_states, &nstates)) {
      fprintf(stderr, "Failed to load control states.\n");
      exit(1);
    }
    if (get_cpu_states(NULL, &cpu_states, &nstates)) {
      fprintf(stderr, "Failed to load cpu states.\n");
      exit(1);
    }
    state = poet_init(heart, nstates, control_states, cpu_states, &apply_cpu_config, &get_current_cpu_state, 1, "poet.log");
    if (state == NULL) {
      fprintf(stderr, "Failed to init poet.\n");
      exit(1);
    }
#endif
   printf("heartbeat init'd\n");

}

void hb_poet_finish() {
#ifdef USE_POET
    poet_destroy(state);
    free(control_states);
    free(cpu_states);
#endif
    heartbeat_finish(heart);
    printf("heartbeat finished\n");
}

void readFilesData(){
	FILE *fin;
	int index;
	char file[200];
	 
	 for ( index=0; index<no_files; index++){
	      strcpy(file, path);
	      strcat(file, in_file_list[index]);
	      if ((fin = fopen(file, "rb")) == NULL) {
					printf("Error opening %s for reading\n", in_file_list[index]);
					}		
	 			// Calculate File Size
    		fseek(fin, 0, SEEK_END);
				Tdata[index].size = ftell(fin);
				fseek(fin, 0, SEEK_SET);
								
		    Tdata[index].data = (BYTE *) calloc(sizeof(BYTE),Tdata[index].size+5);
		    
   			if (Tdata[index].size != fread(Tdata[index].data, sizeof(BYTE), Tdata[index].size, fin))			{
						free(Tdata[index].data);
			  		printf("Error: Could not read file!\n");
	   		}
		}
printf("File reading completed!\n");
}


void compute_digest(int index){
	
	   SHA_INFO sha_info;
    
	    	sha_stream(&sha_info, Tdata[index].data,Tdata[index].size);
				
				if (PRINT)			{
				   sha_print(&sha_info);	
				}
				 		 
}

void *parallel_SHA_Dig_Calc(void *params){
	
			 int index;
			 parameters *param=(parameters *)params;
       
       if (PRINT)
       printf("Worker number %d: started calculating digest.\n",param->no);
       for (index=param->s_index; index<param->e_index; index++){
       	     compute_digest(index);
        }
        
        if (PRINT)
        printf("Worker number %d: finished calculating digest.\n",param->no);
}

void init_workers(){
	
	pthread_attr_init(&string_attr);
	pthread_mutex_init(&string_mutex,NULL);
	pthread_attr_setdetachstate(&string_attr,PTHREAD_CREATE_JOINABLE);
	
}

void createWorkers(){
	long index;
	for ( index= 0 ; index<no_workers; index++){
		    pthread_create(&workers[index],&string_attr,parallel_SHA_Dig_Calc,(void *)&paramsArr[index]);
	}
	
}
void waitForWorkersToFinish(){
	int index;
	    for ( index= 0 ; index<no_workers; index++)
	    	   pthread_join(workers[index],NULL);
	    	   
 }                                               
 
void divide_into_sub_tasks(){

	  int i;
	  int s_index=0;
	  
	  
	  
	  for (i=0;i<no_workers;i++) {
			   paramsArr[i].s_index=s_index;
			   paramsArr[i].e_index=s_index+partition_size;
			   paramsArr[i].name="Worker";
			   paramsArr[i].no=i+1;
			   s_index+=partition_size;
			      
	  }
 
	  // It handles even not of jobs and odd no of workers vice versa.
	   paramsArr[i-1].e_index=paramsArr[i-1].e_index+(no_files%no_workers);
	   	  
}

  void show_sub_tasks() {

		int i;
			for (i=0;i<no_workers;i++) {
				   
				   printf("%d   ",paramsArr[i].s_index);
				   printf("%d   ",paramsArr[i].e_index);
				   printf("%s ",paramsArr[i].name);
				   printf("%d \n ",paramsArr[i].no);
		  }
}

void init(){
	  partition_size= MAX_NO_FILES/no_workers;
	  divide_into_sub_tasks();
	  if (PRINT)
	  		show_sub_tasks();
}   

void parallel_process(){
	 		printf("*******  Digest Calucation:Parallel Process  *******\n");
            int i;

            hb_poet_init();
       
	    init();
	    init_workers();
	    for (i = 0; i < num_runs; i++) {
	      createWorkers();
	      waitForWorkersToFinish();
	      if (i >= 12 && i % 2 == 0) {
	      heartbeat_acc(heart, i, 1);
#ifdef USE_POET
	      poet_apply_control(state);
#endif
	      }
	    }

	    hb_poet_finish();
}

void sequential_process(){
	
	      int index;
       
       printf("*******  Digest Calucation:Sequential Process  *******\n");
       
       for (index=0; index<no_files; index++){
       	if (PRINT){
       	    printf("-------*******************************-------------------\n");
       	    printf("Computing Digest for file: %s\n",in_file_list[index]);
       	  }
       	    
       	       compute_digest(index);
       	   
       	  if (PRINT){     
	       	    printf("Digest written to  file: %s\n",out_file_list[index]);
	       	    printf("-------*******************************-------------------\n");
       	  }
      }
	
}


int main(int argc,char *argv[])
{
	char *token;
      if (argc<2 ||argc>4){
      	  printf("|-----------------------------------------------------------------------|\n");
      	  printf("	Error: Insufficient Parameters.                             \n");
      	  printf("	Maximum Workers are 8. Number of workers should be even!\n");
      	  printf("	Commands to run!                             \n");
      	  printf("	Command Format: OjbectFileName -Sequential(S)/Parallel(P) -Workers -Runs!\n");
      	  printf("	Example: sequential SHA: ' ./sha -S   '!                             \n");
      	  printf("	Example: parallel SHA  : ' ./sha -P -2 -1000'!\n");
      	  printf("|---------------------------------------------------------------------- |\n");
      	  exit(0);
      }else {
      	    
      	    token=argv[1];
            // get current path (where inputs files are)
            char* lslash = strrchr(argv[0], '/');
            if (lslash) {
              *(lslash + 1) = '\0';
              path=argv[0];
            } else {
              path = "";
            }
      	    if (*token=='-') {
      	    	   token++;
      	    	  if (strcmp(token,"S")==0){
      	    	  	    readFilesData();
			      	    	   sequential_process();
			      	    }else if (strcmp(token, "P")==0){
			      	    	     token= argv[2];
			      	    	     token++;
			      	    	     if (isdigit(*token)!=0){
			      	    	         no_workers= atoi(token);
			      	    	    		 /*if (no_workers%2 != 0 || no_workers >MAX_WORKERS)
			      	    	    		     printf("ERROR: Number of worker should be even and no more than 8\n");
			      	    	    		 else {*/
			      	    	    		 	   readFilesData();
			      	                   parallel_process();	
			      	               //}
			      	          }else{
			      	          	printf("ERROR: Invalid number of workers\n");
			      	          }
			      	  	}else {
			      	  		  printf("ERROR: Unknown Parameters!\n");
			      	  	}//end-if	  	
      	  }else {
      	  	 printf("ERROR: Unknown Parameters!\n");
      	  }
      	  	
      }
    return(0);
}
