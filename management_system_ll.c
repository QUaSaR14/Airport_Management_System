#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//constants
#define FLIGHT 5
#define FLIGHT_ID 6 //AI266
#define TICKET_ID 9    //AI266014
#define MAX_SIZE 250
#define NEXT_ROW -1
#define END_INPUT -2
#define R 6
#define C 10

int seat_num=0;

typedef enum {FAILURE, SUCCESS} statuscode;
typedef int ItemType;
typedef enum{FALSE,TRUE} Bool;
typedef enum{WINDOW, MIDDLE, AISLE} seat_type;

//Seat arrangement DS

// Node to represent triples
typedef struct value_list
{
	int column_index;
	seat_type type;
	int val;
	struct value_list *next;
}value;

typedef struct row_list
{
	int row_number;
	struct row_list *link_down;
	value *link_right;
}row ;

//passenger DS
typedef struct passenger_detail_tag
{
	char name[20];
	int age;
	int numbag;
	int wtbag;
	int fine;
	int seatnum;
	struct passenger_detail_tag *next;
}passengerdet;

//Ticket Id
typedef struct ticket
{
	char ticketid[TICKET_ID];
	passengerdet *ptr;
	struct ticket *next;
	int numtickets;
	int totalFine;
}ticket_type;

//Flight Data Structure
typedef struct Flight_Node_tag {
	char flight_id[FLIGHT_ID];
    row *mat;
	ticket_type *tptr;
	struct Flight_Node_tag * next;
}Flight_Node;

//Service window Data Structure
typedef struct ServiceWindow_Node_tag {
	char ServiceWindow[5];        //S012
    Flight_Node *flight;
    struct ServiceWindow_Node_tag *prev;
    struct ServiceWindow_Node_tag *next;
} SW_Node;

//==============SPARSE MATRIX==================
seat_type which_seat_type(int num){
	int rem = num%(R);
	seat_type ret_type;
	if(rem == 0 || rem == 1) ret_type = WINDOW;
	else if( rem == 2 || rem == 5) ret_type = MIDDLE;
	else if( rem == 3 || rem == 4) ret_type = AISLE;

	return ret_type;
}

void create_value_node(int data, int j, row **z)
{
	value *temp, *d;

	// Create new node dynamically
	temp = (value*)malloc(sizeof(value));
	temp->column_index = j+1;
	temp->val = data;
	temp->type=which_seat_type(data);
	temp->next = NULL;

	// Connect with row list
	if ((*z)->link_right==NULL)
		(*z)->link_right = temp;
	else
	{
		// d points to data list node
		d = (*z)->link_right;
		while(d->next != NULL)
			d = d->next;
		d->next = temp;
	}
}

// Function to create row list
void create_row_list(row **start, int Row, int column)
{
	int i;
	// For every row, node is created
	for (i = 0; i < Row; i++)
	{
		row *z, *r;

		// Create new node dynamically
		z = (row*)malloc(sizeof(row));
		z->row_number = i+1;
		z->link_down = NULL;
		z->link_right = NULL;
		if (i==0)
			*start = z;
		else
		{
			r = *start;
			while (r->link_down != NULL)
				r = r->link_down;
			r->link_down = z;
		}

		// Firstiy node for row is created,
		// and then travering is done in that row
		int j;
		for (j = 0; j < column; j++)
		{
			int val = (i*R) + (j+1);
			create_value_node(val, j, &z);

		}
	}
}

//Function display data
void print_chart(row *start)
{
	row *r;
	value *z;
	r = start;
	int prev = 0;
	// Traversing row list
	while (r != NULL)
	{
		int count=0;
		if (r->link_right != NULL)
		{
			//printf("%d \n", r->row_number);
			z = r->link_right;

			// Traversing data list
			prev = z->val;
			while (z != NULL)
			{
				if(count==3) { printf("\t"); count = 0; }
				if(prev=z->val || prev+1 == z->val)
				{
					printf("%2d" , z->val);
					switch(z->type){
						case WINDOW :
							printf("W ");
							break;
						case MIDDLE :
							printf("M ");
							break;
						case AISLE :
							printf("A ");
							break;
					}
				}
				else{
					printf("    ");
				}
				prev = z->val;
				z = z->next;
				count++;
			}
          printf("\n");
		}
		r = r->link_down;
	}
}

row *GetSeats(){
	row *start = NULL;

	//Function creating List of Lists
	create_row_list(&start, C, R);
	return start;
};

value *Delete_Seat(row *rptr, value *pptr){
	value *ptr;
	if(pptr==NULL){
		ptr = rptr->link_right ;
		rptr->link_right = ptr->next;
	//	free(ptr);
	}
	else {
		ptr = pptr->next;
		pptr->next = ptr->next;
	//	free(ptr);
	}
	return pptr;
}

//=======================END OF SPARSE MATRIX=======================

SW_Node *create_node_service_window(SW_Node *lptr)
{
	SW_Node *nptr;
	nptr=(SW_Node*)malloc(sizeof(SW_Node));
	printf("\nEnter name of service window : ");
	scanf("%s",nptr->ServiceWindow);
	nptr->next=lptr;
	nptr->prev=NULL;
	lptr=nptr;
	//printf("\n        SUCCESS           \n");
	return lptr;
}

Flight_Node *create_node_flight(Flight_Node *lptr)
{
	Flight_Node *nptr;
	nptr=(Flight_Node*)malloc(sizeof(Flight_Node));
	printf("\nEnter Flight ID : ");
	scanf("%s",nptr->flight_id);
	nptr->next=lptr;
	nptr->tptr=NULL;
	nptr->mat= GetSeats();
	lptr=nptr;
	return lptr;
}

ticket_type *create_node_ticket(ticket_type *lptr)
{
	ticket_type *nptr;
	nptr=(ticket_type*)malloc(sizeof(ticket_type));
	printf("\nEnter ticket id : ");
	scanf("%s",nptr->ticketid);
	printf("\nEnter number of tickets : ");
	scanf("%d",&nptr->numtickets);
	nptr->totalFine = 0;
	nptr->ptr=NULL;
	nptr->next=lptr;
	lptr=nptr;
	return lptr;
}

passengerdet *create_node_passenger(passengerdet *lptr)
{
	passengerdet *nptr;
	nptr=(passengerdet*)malloc(sizeof(passengerdet));
	printf("\nEnter name : ");
	scanf("%s",nptr->name);
	printf("\nEnter age : ");
	scanf("%d",&nptr->age);
	printf("\nEnter number of bags : ");
	scanf("%d",&nptr->numbag);
	printf("\nEnter weight of bags : ");
	scanf("%d",&nptr->wtbag);
	if(nptr->wtbag >15)
	{
		nptr->fine=(nptr->wtbag - 15)*100;
	}
	else
	{
		nptr->fine=0;
	}
	nptr->seatnum=0;	//as initilly no seat is allocated to the passenger
	nptr->next=lptr;
	nptr->seatnum=0;
	lptr=nptr;
	return lptr;
}

int totalFine(ticket_type *tptr){
	int fine=0;
	if(tptr->ptr!=NULL){
		passengerdet *pass = tptr->ptr;
		while(pass!=NULL){
			fine += pass->fine;
			pass = pass->next;
		}
	}
	return fine;
}

void DisplayFlightData(Flight_Node *lptr,char fl_id[])
{
	Flight_Node *flptr;
	ticket_type *tiptr;
	passengerdet *pptr;
	int flag=0, totFine;
	for(flptr=lptr;flptr!=NULL && flag==0;flptr=flptr->next)
	{
		if(strcmp(fl_id,flptr->flight_id)==0)
		{
			printf("\nDetails of %s are : \n",fl_id);
			flag=1;
			for(tiptr=flptr->tptr;tiptr!=NULL;tiptr=tiptr->next)
			{
				printf("\n*******************************************************\n");
				printf("\n* Ticket id : %s",tiptr->ticketid);
				for(pptr=tiptr->ptr;pptr!=NULL;pptr=pptr->next)
				{
					printf("\n**************************************\n");
					printf("\n* Name of passenger : %s",pptr->name);
					printf("\n* Age of passenger : %d",pptr->age);
					printf("\n* Number of bags this passenger is carrying: %d",pptr->numbag);
					printf("\n* Weight of bags: %d",pptr->wtbag);
					printf("\n* Fine : %d",pptr->fine);
					if(pptr->seatnum == 0) printf("\nCheck-in process incomplete");
					else{
					 printf("\n* Seat : %d ",pptr->seatnum);
					 int type;
					 type = pptr->seatnum % 6;
					 if(type==0 || type==1) printf("WINDOW\n");
					 else if(type==3 || type==4) printf("AISLE\n");
					 else printf("MIDDLE\n");
					}
					printf("\n**************************************\n");
				}
				totFine = totalFine(tiptr);
				printf("\n* Total Fine : %d", totFine);
				printf("\n*******************************************************\n");
				printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			}
		}
	}
	if(flag==0)
	{
		printf("\nWe do not have this flight\n");
	}
}

void DisplayPassengerDetails(Flight_Node *lptr,char tick_id[])
{
	Flight_Node *flptr;
	ticket_type *tptr;
	passengerdet *pptr;
	int flag=0;
	for(flptr=lptr;flptr!=NULL;flptr=flptr->next)
	{
		for(tptr=flptr->tptr;tptr!=NULL && flag==0;tptr=tptr->next)
		{
			if(strcmp(tick_id,tptr->ticketid)==0)
			{
				for(pptr=tptr->ptr;pptr!=NULL;pptr=pptr->next)
				{

					printf("\n******************************************************************\n");
					printf("\n*	Ticket Id : %s",tptr->ticketid);
					printf("\n* Name of passenger : %s",pptr->name);
					printf("\n* Age of passenger : %d",pptr->age);
					printf("\n* Number of bags this passenger is carrying : %d",pptr->numbag);
					printf("\n* Weight of bags: %d",pptr->wtbag);
					printf("\n* Fine : %d",pptr->fine);
					if(pptr->seatnum == 0) printf("\n* !!! Check-in process incomplete !!!");
					else
					{
					 printf("\n* Seat : %d  ",pptr->seatnum);
					 int type;
					 type = pptr->seatnum % 6;
					 if(type==0 || type==1) printf("WINDOW\n");
					 else if(type==3 || type==4) printf("AISLE\n");
					 else printf("MIDDLE\n");
					}
					printf("\n******************************************************************\n");
				}
					flag=1;
			}
			//printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		}
	}
	if(flag==0)
	{
		printf("\nSorry the ticket id you are searching for is not here\n");
	}
}

SW_Node *addFlightServiceWindow(){
	Flight_Node *fptr;
	ticket_type *lptr;
	passengerdet *pptr;
	fptr=NULL;
	int m,k;
	SW_Node *serptr;
	serptr=NULL;
	serptr=create_node_service_window(serptr);
	printf("\n\nEnter number of flights you want : ");
	scanf("%d",&m);
	for(k=0;k<m;k++)
	{
		lptr=NULL;
		fptr=create_node_flight(fptr);
		printf("\n\nEnter number of ticket id's that you want to add in database : ");
		int n,i,j;
		scanf("%d",&n);
		for(i=0;i<n;i++)
		{
			pptr=NULL;
			lptr=create_node_ticket(lptr);
			for(j=0;j<lptr->numtickets;j++)
			{
				pptr=create_node_passenger(pptr);
			}
			lptr->ptr=pptr;
		}
		fptr->tptr=lptr;
	}
	serptr->flight=fptr;
	printf("\n        SUCCESS           \n");
	return serptr;					//this is done assuming only single service window
}

//=======================SEAT ALLOCATION=============================
value *find_first_available(row **rptr, int age){
	row *nptr, *row_ptr;
	value *vptr, *pvptr=NULL, *start, *seat_ptr=NULL;
	int flag = 0;
	seat_num=0;
	row_ptr = *rptr;
	while(row_ptr->link_right==NULL){
		row_ptr = row_ptr->link_down;
	}
	start = row_ptr->link_right;
	nptr = row_ptr;

	if(age>60){
		while(nptr!=NULL && flag==0){
			vptr = nptr->link_right;
			pvptr = vptr;
			vptr = vptr->next;
			while(vptr!=NULL && flag==0){
				if(vptr->val%R==3 || vptr->val%R==4){
					seat_ptr = vptr;
					seat_num=vptr->val;
					Delete_Seat(nptr, pvptr);
					flag=1;
				}
				else {
					pvptr = pvptr->next;
					vptr = vptr->next;
				}
			}
			if(flag==0) nptr = nptr->link_down;
		}
		if(seat_ptr==NULL){
			seat_ptr = start;
			seat_num=start->val;
			Delete_Seat(nptr, pvptr);
			nptr = row_ptr;
		}
	}
	else if(age<=15){
		while(nptr!=NULL && flag==0){
				vptr = nptr->link_right;
				pvptr = vptr;
				vptr = vptr->next;
				while(vptr!=NULL && flag==0){
					if(pvptr->val%6==1 || vptr->val%6==0){
						seat_ptr = vptr;
//						if(pvptr->val%6==1) seat_num=pvptr->val;
//						else seat_num=vptr->val;
//						Delete_Seat(nptr, pvptr);
//						flag=1;
						if(pvptr->val%6==1){
						 seat_num=pvptr->val;
						 seat_ptr=pvptr;
						// pvptr=NULL;
						 Delete_Seat(nptr, NULL);}
						else {
						seat_num=vptr->val;
						Delete_Seat(nptr, pvptr);}
						flag=1;
					}

					else {
						pvptr = pvptr->next;
						vptr = vptr->next;
					}
				}
				if(flag==0) nptr = nptr->link_down;
			}
		if(seat_ptr == NULL){
			seat_ptr = start;
			seat_num=start->val;
			Delete_Seat(nptr, pvptr);
			nptr = row_ptr;
		}
	}
	else{
		seat_ptr = start;
		seat_num=start->val;
		Delete_Seat(nptr, pvptr);
	}
//	*rptr = nptr;
	return seat_ptr;
}

void book_seat(Flight_Node *flptr, ticket_type *tiptr){
		ticket_type *tptr;
		tptr = tiptr;
		value *seat_ptr;
		row *row_ptr,*rptr;
		passengerdet *passngr;
		passngr=tptr->ptr;
		row_ptr = flptr->mat;
		int num;
		//call find available and get seat of first passenger
	//	seat_ptr = find_first_available(&row_ptr, passngr->age);
		//update passenger structure
	//	passngr->seatnum = seat_ptr->val;
		while(passngr!=NULL)
		{
			seat_ptr=find_first_available(&row_ptr,passngr->age);
			passngr->seatnum=seat_ptr->val;
			passngr=passngr->next;
		}
	return;
}
//=======================END OF SEAT ALLOCATION======================

//=======================BOARDING PROCESS===========================
void allocateBoardingPass(Flight_Node *fptr ,char flight_id[], char ticket_id[]){
    Flight_Node *flptr;
    ticket_type *tiptr;

    flptr = fptr;

    int flag=0;
    while(flptr!=NULL && flag==0){
        if(strcmp(flight_id, flptr->flight_id) == 0){
            flag=1;
            tiptr = flptr->tptr;
            while(tiptr!=NULL && flag==1){
                if(strcmp(ticket_id, tiptr->ticketid) == 0 && tiptr->ptr->seatnum == 0){
                    flag=2;
                    //printf("\n\nTICKET\n\n");

                }
                else {
                    tiptr = tiptr->next;
                }
            }
        }
        else {
            flptr = flptr->next;
        }
    }
    if(flag==0){
        printf("Flight with ID %s not found\n", flight_id);
    }
    else if(flag==1){
        printf("Ticket with ID %s not found or Seat is already alloted\n", ticket_id);
    }
    else {
        //get seat chart
        //print_chart(seats);
        book_seat(flptr, tiptr);
        tiptr->totalFine = totalFine(tiptr);
        printf("       Boarding SuccessFull!!!       \n");
    }

    return;
}


//=======================END OF BOARDING PROCESS====================

void DisplayAvailableSeats(SW_Node *swptr, char fl_id[]){
    Flight_Node *flptr;
    flptr = swptr->flight;
    int flag=0;
    while(flptr!=NULL && flag==0){
		if(strcmp(fl_id, flptr->flight_id) == 0){
			print_chart(flptr->mat);
			flag=1;
		}
		else{
			flptr = flptr->next;
		}
    }
    return;
}

// =======  Load data from service_window.txt ====== //
void load_database(SW_Node **swptr)
{
  FILE *fp;
  fp=fopen("service_windows.txt","r");
  int x,i,j,k,l,m,n,o,p,y,z;
  fscanf(fp,"%d\n",&x);
  for(i=0;i<x;i++)
  { SW_Node *nptr=(SW_Node*)malloc(sizeof(SW_Node));
    nptr->prev=NULL;
    nptr->next=NULL;
    char sw[5];
    fscanf(fp,"%s\n",nptr->ServiceWindow);
    fscanf(fp,"%d\n",&y);
    nptr->flight=NULL;
    for(j=0;j<y;j++)
	{ Flight_Node *kptr=(Flight_Node*)malloc(sizeof(Flight_Node));
	  fscanf(fp,"%s\n",kptr->flight_id);
	  fscanf(fp,"%d\n",&z);
	  row *start=NULL;
	  for (l = 0; l < C; l++)
	   {
		row *z, *r;

		// Create new node dynamically
		z = (row*)malloc(sizeof(row));
		z->row_number = i+1;
		z->link_down = NULL;
		z->link_right = NULL;
		if (l==0)
			start = z;
		else
		{
			r = start;
			while (r->link_down != NULL)
				r = r->link_down;
			r->link_down = z;
		}
	  }
	  kptr->mat=start;
	  row *rr=start;
	  value *pp;
	  for(l=0;l< C;l++)
	  { fscanf(fp,"%d\n",&m);
	    pp=NULL;
		for(k=0;k<m;k++)
		{  value *vptr=(value*)malloc(sizeof(value));
		   fscanf(fp,"%d\n%d\n%d\n",&(vptr->column_index),&(vptr->type),&(vptr->val));
		   vptr->next=NULL;
		   if(pp==NULL)
		   { rr->link_right=vptr;
		   	 pp=vptr;
		   }
		   else
		   { pp->next=vptr;
		     pp=vptr;
		   }
		}
	  	rr=rr->link_down;
	  }
	  kptr->tptr=NULL;
      fscanf(fp,"%d\n",&n);
      for(z=0;z<n;z++)
       { ticket_type *tiptr=(ticket_type*)malloc(sizeof(ticket_type));
         fscanf(fp,"%s\n",tiptr->ticketid);
         fscanf(fp,"%d\n",&o);
         int x1;
         tiptr->ptr=NULL;
         for(x1=0;x1<o;x1++)
		 { passengerdet *pptr=(passengerdet*)malloc(sizeof(passengerdet));
           fscanf(fp,"%s\n%d\%d\n%d\n%d\n%d\n",pptr->name,&(pptr->age),&(pptr->numbag),&(pptr->wtbag),&(pptr->fine),&(pptr->seatnum));
		   pptr->next=tiptr->ptr;
           tiptr->ptr=pptr;
         }
         fscanf(fp,"%d\n%d\n",&(tiptr->numtickets),&(tiptr->totalFine));

         tiptr->next=kptr->tptr;
         kptr->tptr=tiptr;
       }

	  kptr->next=nptr->flight;
	  nptr->flight=kptr;
	}
	if(*swptr==NULL)
	{ *swptr=nptr;
	}
	else
    { (*swptr)->prev=nptr;
    	nptr->next=*swptr;
		*swptr=nptr;
    }
  }
}
int main() {

	SW_Node *swptr=NULL;
    Flight_Node *fptr;
    int x=1;
	statuscode sc;
	char fl_id[6], tick_id[9];
	//fptr = swptr->flight;
    load_database(&swptr);
	printf("\n========================== WELCOME ==========================");

	while(x){
		printf("\n\n 1. Create New Service Window \n 2. Get Boarding Passes \n 3. Show Flight Data \n 4. Show Passenger Details \n 5. Show Available Seat \n 0. Exit\n\n");
		scanf("%d", &x);

		switch(x){
			case 1:
				swptr=addFlightServiceWindow();
				fptr = swptr->flight;
				break;

			case 2:
				if(swptr==NULL) {
					printf("No service window is present....");
				}
				else {
					fptr = swptr->flight;
					printf("\n For boarding passes : \n");
					printf("\nEnter flight Id : ");
					scanf("%s",fl_id);
					printf("\nEnter the ticket Id : ");
					scanf("%s",tick_id);
					allocateBoardingPass(fptr, fl_id, tick_id);
				}
				break;

			case 3:
				if(swptr==NULL) {
					printf("No service window is present....");
				}
				else {
					fptr = swptr->flight;
					printf("\nEnter flight Id : ");
					scanf("%s",fl_id);
					DisplayFlightData(fptr,fl_id);
				}
				break;

			case 4:
				if(swptr==NULL) {
					printf("No service window is present....");
				}
				else{
					printf("\nEnter the ticket id : ");
					scanf("%s",tick_id);
					fptr = swptr->flight;
					DisplayPassengerDetails(fptr,tick_id);
				}
				break;

			case 5:
				if(swptr==NULL) {
					printf("No service window is present....");
				}
				else {
					printf("\nEnter flight id : ");
					scanf("%s",fl_id);
					DisplayAvailableSeats(swptr, fl_id);
				}
				break;
		}
	}

	printf("\n\n Thanks for visiting\n Have a nice journey ahead :)\n\n");

	//----- Storing Data in service_window.txt ----- //
	FILE *fp;
	fp=fopen("service_windows.txt","w");
	SW_Node *nptr=swptr;
	int count=0;
	while(nptr!=NULL)
	{ count++;
	  nptr=nptr->next;
	}
	fprintf(fp,"%d\n",count);
	nptr=swptr;
	while(nptr!=NULL)
	{
		fprintf(fp,"%s\n",nptr->ServiceWindow);
		Flight_Node *kptr=nptr->flight;
		count=0;
		while(kptr!=NULL)
		{ count++;
		  kptr=kptr->next;
		}
		fprintf(fp,"%d\n",count);
		kptr=nptr->flight;
		while(kptr!=NULL)
		{ fprintf(fp,"%s\n",kptr->flight_id);
		  row *xptr=kptr->mat;
		  fprintf(fp,"%d\n",10);
		  while(xptr!=NULL)
		  { value *yptr=xptr->link_right;
		    count=0;
		  	while(yptr!=NULL)
			{ count++;
			  yptr=yptr->next;
			}
		    fprintf(fp,"%d\n",count);
		    yptr=xptr->link_right;
		    while(yptr!=NULL)
			{ fprintf(fp,"%d\n%d\n%d\n",yptr->column_index,yptr->type,yptr->val);
			  yptr=yptr->next;
			}
		  	xptr=xptr->link_down;
		  }
		  ticket_type *tiptr=kptr->tptr;
		  count=0;
		  while(tiptr!=NULL)
		  { count++;
		  	tiptr=tiptr->next;
		  }
		  fprintf(fp,"%d\n",count);
		 // printf("count=%d\n",count);
		  tiptr=kptr->tptr;
		  while(tiptr!=NULL)
		  { fprintf(fp,"%s\n",tiptr->ticketid);
			passengerdet *pptr=tiptr->ptr;
			count=0;
			while(pptr!=NULL)
			{ pptr=pptr->next;
			  count++;
			}
			fprintf(fp,"%d\n",count);
			pptr=tiptr->ptr;
			while(pptr!=NULL)
			{  fprintf(fp,"%s\n%d\n%d\n%d\n%d\n%d\n",pptr->name,pptr->age,pptr->numbag,pptr->wtbag,pptr->fine,pptr->seatnum);
               pptr=pptr->next;
			}
			fprintf(fp,"%d\n%d\n",tiptr->numtickets,tiptr->totalFine);
		  	tiptr=tiptr->next;
		  }
		  kptr=kptr->next;
		}

	  nptr=nptr->next;
	}
	fclose(fp);
    return 0;
}
