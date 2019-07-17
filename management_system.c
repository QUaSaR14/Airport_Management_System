#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define SW_MAX 6
#define SW 5
#define FLIGHT 5
#define FLIGHT_ID 6
#define TICKET_ID 9
#define MAX_PASSENGER 5
#define M 3
#define PASS_NAME 15
#define R 6
#define C 10
#define MAX_TICK_IDS 10

typedef enum{FAILURE, SUCCESS} status_code;
typedef enum{WINDOW, MIDDLE, AISLE} seat_type;
typedef enum{ Duplicate,SearchFailure,Success,InsertIt,LessKeys } KeyStatus ;


typedef struct passenger_details
{
    char name[PASS_NAME];
    int age;
    int num_bags,wt_bag,fine,seat_num;
}passenger;

typedef struct ticket_node_tag
{
    char ticket_id[TICKET_ID];
    passenger psngr[MAX_PASSENGER];
    int num_passenger, total_fine;
}ticket;

typedef struct ticket_tree_type
{
    ticket ticket_key[M-1];
    bool isLeaf;
    struct ticket_tree_type *ticket_child[M];
    int n;
}ticket_tree;

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

typedef struct flight_node_tag
{
    char flight_id[FLIGHT_ID];
    int num_of_ticket_ids;
    row *mat;
    ticket_tree *tik_ptr;
}flight;

typedef struct flight_node_tree
{
    flight flt;
    int height;
    struct flight_node_tree *left, *right;
}flight_tree;

typedef struct ServiceWindow
{
    char SW_name[SW];
    int no_of_flight;
    flight_tree *flt_ptr;
}SW_Tree;

SW_Tree SW_arr[SW_MAX];

//B-Tree functions
ticket_tree *insert(ticket key, ticket_tree *root);
status_code search_ticket(char x[], ticket_tree *root, ticket *f);
KeyStatus ins(ticket_tree *r, ticket x, ticket* y, ticket_tree** u);
int searchPos(char x[],ticket *key_arr, int n);
ticket scanTicketData();
passenger scanPassengerDetails();
void printTicketData(ticket tik);
void printPassengerDetails(passenger psgr);
void displayInorder(ticket_tree *root);
//int totalKeys(struct node *ptr);
//void printTotal(struct node *ptr);
//int getMin(struct node *ptr);
//int getMax(struct node *ptr);
//void getMinMax(struct node *ptr);
//int max(int first, int second, int third);
//int maxLevel(struct node *ptr);
//void printMaxLevel(struct node *ptr);
//==========================================================


//AVL Tree functions
flight_tree *rightRotate(flight_tree *y);
flight_tree *leftRotate(flight_tree *x);
int height(flight_tree *temp);
int getBalance(flight_tree *node);
flight_tree *make_flight_tree_node();
flight_tree* InsertFlight(flight_tree *root,flight_tree *temp);
void visit(flight_tree *ptr);
void Inorder(flight_tree *root);
flight_tree *search_flight(flight_tree *root,char str[]);
flight scanFlightData();
void printFlightData(flight flptr);
//===================================================

//SEAT ARRANGEMENT
seat_type which_seat_type(int num);
void create_value_node(int data, int j, row **z);
void create_row_list(row **start, int Row, int column);
void print_chart(row *start);
row *GetSeats();
value *Delete_Seat(row *rptr, value *pptr);
value *find_first_available(row **rptr, int age);
void DisplayAvailableSeats(flight flt);
//====================================================

//SEAT ALLOCATION
status_code allocateBoardingPass(SW_Tree swptr ,char flight_id[], char ticket_id[]);
ticket book_seat(flight flt, ticket tkt);
void storeDataInTicket(ticket tkt, ticket_tree *root, char tk_Id[]);
void printKeysInRange(ticket_tree *ptr, char from[], char to[]);
void RangeDisplayPassengerDetails(flight flt,char from_ticket[], char to_ticket[]);

void  printPassengerFILE(FILE *fp,passenger psgr)
{
    fprintf(fp,"%s\n", psgr.name);
    fprintf(fp,"%d\n ", psgr.age);
    fprintf(fp,"%d\n", psgr.num_bags);
    fprintf(fp,"%d\n", psgr.wt_bag);
    fprintf(fp,"%d\n%d\n", psgr.fine,psgr.seat_num);
}
void printTicketFile(FILE *fp,ticket tik)
{
    fprintf(fp,"%s\n", tik.ticket_id);
    int i;
    fprintf(fp,"%d\n",tik.num_passenger);
    for(i=0; i<tik.num_passenger; i++)
    {
        printPassengerFILE(fp,tik.psngr[i]);
    }
   // printf("\n--------------------------------");
    fprintf(fp,"%d\n", tik.total_fine);
}
void  Write_ticket_into_file(FILE *fp,ticket_tree *ptr)
{
    if (ptr) {
        if (ptr->n >= 1) {
             Write_ticket_into_file(fp,ptr->ticket_child[0]);
            printTicketFile(fp,ptr->ticket_key[0]);
            Write_ticket_into_file(fp,ptr->ticket_child[1]);
            if (ptr->n == 2) {
              printTicketFile(fp,ptr->ticket_key[1]);
              Write_ticket_into_file(fp,ptr->ticket_child[2]);
            }
        }
    }
}
int count_no_of_tickets(ticket_tree *ptr)
{
    if (ptr) {
            int count=0;
        if (ptr->n >= 1) {
            count+=count_no_of_tickets(ptr->ticket_child[0]);
            count++;
            //printf("%d ", ptr->ticket_key[0]);
            //printTicketData(ptr->ticket_key[0]);
            count+=count_no_of_tickets(ptr->ticket_child[1]);
            if (ptr->n == 2) {
                //printf("%d ", ptr->ticket_key[1]);
                //printTicketData(ptr->ticket_key[1]);
                count++;
                count+=count_no_of_tickets(ptr->ticket_child[2]);
            }
        }
    }
    else
        return 0;
}
void write_finfo_into_file(FILE *fp,flight_tree* fptr)
{
         fprintf(fp,"%s\n%d\n",fptr->flt.flight_id,10);
         row *xptr=fptr->flt.mat;
         while(xptr!=NULL)
		  { value *yptr=xptr->link_right;
		    int count=0;
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
		 // int x=count_no_of_tickets(fptr->flt.tik_ptr);
		  //printf("")
		  fprintf(fp,"%d\n",fptr->flt.num_of_ticket_ids);
		  Write_ticket_into_file(fp,fptr->flt.tik_ptr);

}
void Write_avtree_into_file(FILE *fp,flight_tree *fptr)
{ if(fptr!=NULL)
  {
      if(fptr->left!=NULL)
      {
       Write_avtree_into_file(fp,fptr->left);
      }
      write_finfo_into_file(fp,fptr);
     if(fptr->right!=NULL)
      {
       Write_avtree_into_file(fp,fptr->right);
      }

  }


}
int load_database()
{ int i,x;
  FILE *fp;
  fp=fopen("database.txt","r");
  fscanf(fp,"%d\n",&x);
 // printf("\nx=%d\n",x);
  int x1,cont;
 // scanf("%d",&cont);
  for(i=0;i<x;i++)
  {
        fscanf(fp,"%s\n",SW_arr[i].SW_name);
        fscanf(fp,"%d\n",&x1);
        SW_arr[i].no_of_flight=x1;
         //printf("\nx1=%d\n",x1);
        //scanf("%d",&cont);
        int i1;
        SW_arr[i].flt_ptr=NULL;
        for(i1=0;i1<x1;i1++)
        {

            flight_tree *kptr=(flight_tree*)malloc(sizeof(flight_tree));
            int x2;
            fscanf(fp,"%s\n%d\n",kptr->flt.flight_id,&x2);
            row *start=NULL;
            int l,m,k;
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
              kptr->flt.mat=start;
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
              //printf("Running\n");
             //scanf("%d",&cont);
             //printf("Running\n");
             // scanf("%d",&cont);
              int x10;
             fscanf(fp,"%d\n",&x10);
            //  printf("Running %d %s\n",x10,kptr->flt.flight_id);
             // scanf("%d",&cont);
              kptr->flt.num_of_ticket_ids=x10;
             // printf("Running %d\n",x10,kptr->flt.num_of_ticket_ids);
             // scanf("%d",&cont);
             // printf("Running hj\n");
             // scanf("%d",&cont);
             int i3,i4;
             kptr->flt.tik_ptr=NULL;
             for(i3=0;i3< x10;i3++)
             {  // printf("Running hk\n");
                 // scanf("%d",&cont);
                ticket tik;
                 fscanf(fp,"%s\n", tik.ticket_id);
                  //printf("Running hk\n");
                 // scanf("%d",&cont);
                 fscanf(fp,"%d\n",&(tik.num_passenger));
                  //printf("Running id=%s num=%d\n\n",tik.ticket_id,tik.num_passenger);
                //  scanf("%d",&cont);
                 for(i4=0;i4<tik.num_passenger;i4++)
                 {
                     passenger psgr;
                     fscanf(fp,"%s\n", psgr.name);
                    fscanf(fp,"%d\n ", &(psgr.age));
                    fscanf(fp,"%d\n", &(psgr.num_bags));
                    fscanf(fp,"%d\n", &(psgr.wt_bag));
                    fscanf(fp,"%d\n%d\n", &(psgr.fine),&(psgr.seat_num));
                    tik.psngr[i4]=psgr;
                  }
              fscanf(fp,"%d\n", &(tik.total_fine));
             // printf("Running hkiu\n");
              //scanf("%d",&cont);
               kptr->flt.tik_ptr=insert(tik,kptr->flt.tik_ptr);
             }
		 // int x=count_no_of_tickets(fptr->flt.tik_ptr);
		  //printf("")
		   //printf("Running\n");
         //  scanf("%d",&cont);
		   SW_arr[i].flt_ptr=InsertFlight(SW_arr[i].flt_ptr,kptr);
		   // printf("Running\n");
           //  scanf("%d",&cont);
            //printf("x1=%d\n",x1);
        }

  }
  fclose(fp);
  return x;
}
int main()
{
    flight_tree *root = NULL;
    flight_tree *newptr,*found;
    status_code sc;
    char string[6], str[6], tkid[9],SW_str[5], from[9], to[9];;
    int choice = 1,count=0;
    // int flag = 1;
    count=load_database();

    printf("\n========================WELCOME========================\n\n");
    while(choice)
    {
        printf("\n---------------------------------------\n");
        //printf("enter the choice \n");
        printf("\n1. To Create New Service window\n2. how tree looks like(in order traversal)\n3. To Display Flight Data\n4. To Display Ticket Data\n5. To Get Boarding Pass\n6. To Display Available Seats\n7. To Get Ticket Data in given range of ticket ids\n0.Exit\n");
        printf("\n---------------------------------------\n");
        printf("\nEnter your choice: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                printf("\nEnter name of service window : ");
                scanf("%s",SW_arr[count].SW_name);
                printf("\nEnter number of flights you want to add : ");
                scanf("%d",&SW_arr[count].no_of_flight);
                int i;
                flight_tree *root = NULL;
                for(i=0;i<SW_arr[count].no_of_flight;i++)
                {
                    newptr = make_flight_tree_node();
                    root = InsertFlight(root,newptr);
                }
                SW_arr[count].flt_ptr=root;
                count++;
                break;

            case 2:
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                int flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                        Inorder(SW_arr[i].flt_ptr);
                        flagsw=1;
                    }
                }
                if(flagsw==0) printf("\nPlease enter correct service window \n");
                break;

            case 3:
                //Display Flight Data
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                    //	Inorder(SW_arr[i].flt_ptr);
                        printf("\nEnter the flight id you want to search : ");
                        scanf("%s",string);
                        found=search_flight(SW_arr[i].flt_ptr,string);
                        if(found==NULL)
                            printf("\n!!Flight id not found !!\n");
                        else
                            printFlightData(found->flt);
                        flagsw=1;
                    }
                }
                if(flagsw==0) printf("\nPlease enter correct service window \n");

                break;
            case 4:
                //Display Passenger Details
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                    //	Inorder(SW_arr[i].flt_ptr);
                        printf("\nEnter the ticket ID you want to search : ");
                        scanf("%s", tkid);
                        strncpy(str, tkid, 5);
                        str[5] = '\0';
                        printf("%s", str);
                        //search flight node
                        found = search_flight(SW_arr[i].flt_ptr, str);
                        if(found==NULL)
                            printf("\n!! Flight Not found for corresponding Ticket Id\n");
                        //search ticket node
                        else
                        {
                            ticket tk;
                            sc = search_ticket(tkid, found->flt.tik_ptr, &tk);
                            if(sc)
                                printTicketData(tk);
                            else
                                printf("\n!! Ticket ID not found !!\n");
                        }
                        flagsw=1;
                    }
                }
                if(flagsw==0) printf("\n !! Service window incorrect !! \n");
                break;

            case 5 :
                //Allocate boarding pass
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                        printf("\nEnter the flight id for boarding : ");
                        scanf("%s",string);
                        printf("\nEnter the ticket id for boarding : ");
                        scanf("%s", tkid);
                        sc = allocateBoardingPass(SW_arr[i],string,tkid);
                        printf("\n ------Boarding Successful------\n");
                        flagsw=1;
                    }

                }
                break;

            case 6:
                //display seat chart
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                        printf("\nEnter the flight id to check seat availability : ");
                        scanf("%s",string);
                        found = search_flight(SW_arr[i].flt_ptr, string);
                        DisplayAvailableSeats(found->flt);
                        flagsw=1;
                    }
                }
                break;

            case 7:
                //display passenger details in a range of ticket ids
                printf("\nEnter the name of service window : ");
                scanf("%s",SW_str);
                flagsw=0;
                for(i=0;i<count && flagsw==0;i++)
                {
                    if(strcmp(SW_str,SW_arr[i].SW_name)==0)
                    {
                        printf("\nEnter Flight ID : ");
                        scanf("%s",string);
                        found = search_flight(SW_arr[i].flt_ptr, string);
                        printf("\nTicket Id starting from : ");
                        scanf("%s", from);
                        printf("\nTicket Id ending to : ");
                        scanf("%s", to);
                        RangeDisplayPassengerDetails(found->flt, from, to);
                        flagsw=1;
                    }
                }
                break;


        }
    }
    FILE *fp;
    fp=fopen("database.txt","w");
    fprintf(fp,"%d\n",count);
    int i;
    for(i=0;i<count;i++)
    {
        fprintf(fp,"%s\n",SW_arr[i].SW_name);
        fprintf(fp,"%d\n",SW_arr[i].no_of_flight);
        int j;
        Write_avtree_into_file(fp,SW_arr[i].flt_ptr);
    }
    fclose(fp);
    return 0;

}

//take input of ticket and passenger
ticket scanTicketData()
{
    ticket tik;
    tik.total_fine =0;
    printf("\nEnter Ticket ID : ");
    scanf("%s", tik.ticket_id);
    printf("\nEnter Number of passengers : ");
    scanf("%d", &tik.num_passenger);
    int i;
    for(i=0; i<tik.num_passenger && tik.num_passenger<MAX_PASSENGER; i++)
    {
        tik.psngr[i] = scanPassengerDetails();
        tik.total_fine += tik.psngr[i].fine;
    }
    return tik;
}

passenger scanPassengerDetails()
{
    passenger passgr;
    printf("\nPassenger Name : ");
    scanf("%s", passgr.name);
    printf("\nPassenger Age : ");
    scanf("%d", &passgr.age);
    printf("\nNumber of Bags : ");
    scanf("%d", &passgr.num_bags);
    printf("\nTotal Weight of bags : ");
    scanf("%d", &passgr.wt_bag);
    if(passgr.wt_bag >15)
    {
        passgr.fine =(passgr.wt_bag - 15)*100;
    }
    else
    {
        passgr.fine=0;
    }
    passgr.seat_num = 0;
    return passgr;
}

//display ticket data and passenger data
void printTicketData(ticket tik)
{
    printf("\n==========================================");
    printf("\n* Ticket ID : %s", tik.ticket_id);
    int i;
    for(i=0; i<tik.num_passenger; i++)
    {
        printPassengerDetails(tik.psngr[i]);
    }
    printf("\n--------------------------------");
    printf("\n* Total Fine : %d", tik.total_fine);
    printf("\n==========================================\n\n");
}

void printPassengerDetails(passenger psgr)
{
    printf("\n***********************************");
    printf("\n* Name        : %s", psgr.name);
    printf("\n* Age         : %d", psgr.age);
    printf("\n* Total Bags  : %d", psgr.num_bags);
    printf("\n* Total Weight: %d", psgr.wt_bag);
    printf("\n* Fine        : %d", psgr.fine);
    if(psgr.seat_num!=0)
    {
        printf("\n* Seat Number : %d", psgr.seat_num);
        int type;
        type = which_seat_type(psgr.seat_num);
        if(type==0) printf("WINDOW");
        else if(type==1) printf("MIDDLE");
        else printf("AISLE");
    }
    else
        printf("\n* !! Check In Process InComplete !!");
    printf("\n***********************************\n");
}

//B-TREE PART
ticket_tree *insert(ticket key, ticket_tree *root)
{
    ticket_tree *newnode;
    ticket upKey;
    KeyStatus value;
    value = ins(root, key, &upKey, &newnode);
    if (value == Duplicate)
        printf("Key already available\n");
    else if (value == InsertIt)
    {
        ticket_tree *uproot = root;
        root=(ticket_tree*)malloc(sizeof(ticket_tree));
        root->n = 1;
        root->ticket_key[0] = upKey;
        root->ticket_child[0] = uproot;
        root->ticket_child[1] = newnode;
    }
    return root;
}/*End of insert()*/

KeyStatus ins(ticket_tree *ptr, ticket key, ticket *upKey,ticket_tree **newnode)
{
    ticket_tree *newPtr, *lastPtr;
    int pos, i, n,splitPos;
    ticket newKey, lastKey;
    KeyStatus value;
    if (ptr == NULL)
    {
        *newnode = NULL;
        *upKey = key;
        return InsertIt;
    }
    n = ptr->n;
    pos = searchPos(key.ticket_id, ptr->ticket_key, n);
    if (pos < n && strcmp(key.ticket_id ,ptr->ticket_key[pos].ticket_id)==0)
        return Duplicate;
    value = ins(ptr->ticket_child[pos], key, &newKey, &newPtr);
    if (value != InsertIt)
        return value;
    /*If keys in node is less than M-1 where M is order of B tree*/
    if (n < M - 1)
    {
        pos = searchPos(newKey.ticket_id, ptr->ticket_key, n);
        /*Shifting the key and pointer right for inserting the new key*/
        for (i=n; i>pos; i--)
        {
            ptr->ticket_key[i] = ptr->ticket_key[i-1];
            ptr->ticket_child[i+1] = ptr->ticket_child[i];
        }
        /*Key is inserted at exact location*/
        ptr->ticket_key[pos] = newKey;
        ptr->ticket_child[pos+1] = newPtr;
        ++ptr->n; /*incrementing the number of keys in node*/
        return Success;
    }/*End of if */
    /*If keys in nodes are maximum and position of node to be inserted is last*/
    if (pos == M - 1)
    {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else /*If keys in node are maximum and position of node to be inserted is not last*/
    {
        lastKey = ptr->ticket_key[M-2];
        lastPtr = ptr->ticket_child[M-1];
        for (i=M-2; i>pos; i--)
        {
            ptr->ticket_key[i] = ptr->ticket_key[i-1];
            ptr->ticket_child[i+1] = ptr->ticket_child[i];
        }
        ptr->ticket_key[pos] = newKey;
        ptr->ticket_child[pos+1] = newPtr;
    }
    splitPos = (M - 1)/2;
    (*upKey) = ptr->ticket_key[splitPos];

    (*newnode)=(ticket_tree*)malloc(sizeof(ticket_tree));/*Right node after split*/
    ptr->n = splitPos; /*No. of keys for left splitted node*/
    (*newnode)->n = M-1-splitPos;/*No. of keys for right splitted node*/
    for (i=0; i < (*newnode)->n; i++)
    {
        (*newnode)->ticket_child[i] = ptr->ticket_child[i + splitPos + 1];
        if(i < (*newnode)->n - 1)
            (*newnode)->ticket_key[i] = ptr->ticket_key[i + splitPos + 1];
        else
            (*newnode)->ticket_key[i] = lastKey;
    }
    (*newnode)->ticket_child[(*newnode)->n] = lastPtr;
    return InsertIt;
}/*End of ins()*/

int searchPos(char key[], ticket *key_arr, int n)
{
    int pos=0;
    while (pos < n && strcmp(key,key_arr[pos].ticket_id)>0)
        pos++;
    return pos;
}/*End of searchPos()*/


status_code search_ticket(char key[], ticket_tree *root, ticket *tkt)
{
    int pos, i, n;
    status_code sc=FAILURE;
    ticket_tree *ptr = root;
    while (ptr)
    {
        n = ptr->n;
        //for (i=0; i < ptr->n; i++)
        //    printNode(ptr->ticket_key[i]);

        pos = searchPos(key, ptr->ticket_key, n);
        if (pos < n && strcmp(key, ptr->ticket_key[pos].ticket_id) == 0)
        {
            //printNode(key);
            *tkt = ptr->ticket_key[pos];
            sc = SUCCESS;
        }
        ptr = ptr->ticket_child[pos];
    }
    //printf("Not found \n");
    return sc;
}/*End of search()*/

void displayInorder(ticket_tree *ptr) {
    if (ptr) {
        if (ptr->n >= 1) {
            displayInorder(ptr->ticket_child[0]);
            //printf("%d ", ptr->ticket_key[0]);
            printTicketData(ptr->ticket_key[0]);
            displayInorder(ptr->ticket_child[1]);
            if (ptr->n == 2) {
                //printf("%d ", ptr->ticket_key[1]);
                printTicketData(ptr->ticket_key[1]);
                displayInorder(ptr->ticket_child[2]);
            }
        }
    }
}

//AVL TREE PART
//******************************************************************************************************************
flight_tree *rightRotate(flight_tree *y)
{
    flight_tree *x = y->left;
    flight_tree *T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}
flight_tree *leftRotate(flight_tree *x)
{
    flight_tree *y = x->right;
    flight_tree *T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}
int height(flight_tree *temp)									//function to calculate the height of the subtree with root node of thatsubtree as temp
{
    int retval,leftht,rightht;
    if(temp == NULL)
    {
         retval = -1;
    }
    else if(temp->left == NULL && temp->right == NULL)
    {
        retval = 0;
    }
       else
       {
	        leftht = height(temp->left);
	        rightht = height(temp->right);
	        if(leftht>rightht)
	        {
	       		retval = 1+leftht;
	        }
	        else
	        {
	        	retval = 1+rightht;
	        }
        }

   return retval;
}

int getBalance(flight_tree *node)							//Assuming height of left sub tree +ve and height of left subtree -ve
{
	if(node == NULL)
        return 0;
     //printf("\nRETVAL left=%d\n",height(node->left));
    // printf("\nretval rright=%d\n",height(node->right));
    return (height(node->left)-height(node->right));
}

/*ticket tik_init()
{
    ticket tkt;
    tkt.ticket_id = '\0';
    tkt.total_fine = 0;
    tkt.num_passenger = 0;
}*/

flight scanFlightData()
{
    flight flt;
    printf("\nEnter the flight id : ");
	scanf("%s", flt.flight_id);
	flt.mat = GetSeats(); //to add availability matrix
	printf("\nEnter number of ticket IDS : ");
	scanf("%d", &flt.num_of_ticket_ids);
	flt.tik_ptr = NULL;
	//scan ticket data
	ticket tik;
	int i;
	for(i=0; i<flt.num_of_ticket_ids; i++)
    {
        tik = scanTicketData();
        //insert into tree
        flt.tik_ptr = insert(tik, flt.tik_ptr); // to add ticket tree pointer
        //tik = tik_init();
    }
	return flt;
}

void printFlightData(flight flt)
{
    printf("\n#######################################################\n");
    printf("\n* Flight ID : %s", flt.flight_id);
    displayInorder(flt.tik_ptr);
    printf("\n#######################################################\n\n");
}

flight_tree *make_flight_tree_node()
{
	flight_tree *fltptr;
	fltptr=(flight_tree*)malloc(sizeof(flight_tree));
	fltptr->flt = scanFlightData();
	fltptr->left=NULL;
	fltptr->right=NULL;
	return fltptr;
}

flight_tree* InsertFlight(flight_tree *root,flight_tree *temp)
{
   int r,k;
   flight_tree *p = root;
    if(p == NULL)
    {
        p = temp;
    }
    else
    {
          k = strcmp(p->flt.flight_id,temp->flt.flight_id);

            if(k>0)
            {
                p->left = InsertFlight(p->left,temp);
		  	    printf("PIYUSH\n");
            }
            else
           {
                p->right = InsertFlight(p->right,temp);
            }

  		// printf("ravi");
  	   int balance;
       balance = getBalance(p);
       printf("\nBALANCE=%d\n",balance);
       //printf("ravi");
      int m,n;
	   if(balance>1)
	    {  printf("DHAKAR\n");
	        m = strcmp(p->left->flt.flight_id,temp->flt.flight_id);

		  if(m>0)
		    {
		     	p = rightRotate(p);
	            printf("MADHUR\n");
		    }
	        else
	        {
	         	p->left = leftRotate(p->left);
			 	p = rightRotate(p);
	       }
	    }
	   if(balance < -1)
	    {
		     m = strcmp(p->right->flt.flight_id,temp->flt.flight_id);

			if(m<0)
		   {
		        p = leftRotate(p);
            }
            else
	        {
               p->right = rightRotate(p->right);
		       p = leftRotate(p);
            }
		}
   }
   return p;
}

void visit(flight_tree *ptr)
{
  if(ptr!=NULL)
   {
    	printf("The flight id is %s\n",ptr->flt.flight_id);
   }
}


void Inorder(flight_tree *root)
{  flight_tree *ptr;
   ptr = root;
  if(ptr!=NULL)
   {
      Inorder(ptr->left);
      visit(ptr);
      Inorder(ptr->right);
   }
}

//ASSUMING THE SERVICE WINDOW IS KNOWN TO US THROUGH MAIN FUNCTION
flight_tree *search_flight(flight_tree *root,char str[])
{
	flight_tree *p=root;
	int flag=0;
	while(p!=NULL && flag==0)
	{
		int j=strcmp(p->flt.flight_id,str);
		if(j==0)
		{
			flag=1;
		}
		else
		{
			if(j>0)
			{
				p=p->left;
			}
			else
			{
				p=p->right;
			}
		}
	}
	if(flag==1)
	{

	}
	else
	{
		p=NULL;
	}
	return p;
}
//******************************************************************************************************************

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

	// Traversing row list
	while (r != NULL)
	{
		if (r->link_right != NULL)
		{
			//printf("%d \n", r->row_number);
			z = r->link_right;

			// Traversing data list
			while (z != NULL)
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
				z = z->next;
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

//=======================SEAT ALLOCATION=============================
value *find_first_available(row **rptr, int age){
	row *nptr, *row_ptr;
	value *vptr, *pvptr=NULL, *start, *seat_ptr=NULL;
	int flag = 0;
	int seat_num=0;
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
				if(vptr->val%6==3 || vptr->val%6==4){
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

ticket book_seat(flight flt, ticket tkt){
		value *seat_ptr;
		row *row_ptr,*rptr;
		row_ptr = flt.mat;
		int num, i=0;
		while(i<tkt.num_passenger)
		{
			seat_ptr=find_first_available(&row_ptr,tkt.psngr[i].age);
			//printf("\nGOT SEAT");
			printf("\n%d", seat_ptr->val);
			tkt.psngr[i].seat_num=seat_ptr->val;
			i++;
		}
	return tkt;
}
//=======================END OF SEAT ALLOCATION======================

void storeDataInTicket(ticket tkt, ticket_tree *root, char key[])
{
    int pos, i, n;
    ticket_tree *ptr = root;
    while (ptr)
    {
        n = ptr->n;
        //for (i=0; i < ptr->n; i++)
        //    printNode(ptr->ticket_key[i]);

        pos = searchPos(key, ptr->ticket_key, n);
        if (pos < n && strcmp(key, ptr->ticket_key[pos].ticket_id) == 0)
        {
            //printNode(key);
            ptr->ticket_key[pos] = tkt;
        }
        ptr = ptr->ticket_child[pos];
    }
}

//=======================BOARDING PROCESS===========================
status_code allocateBoardingPass(SW_Tree swptr ,char flight_id[], char ticket_id[]){
    flight_tree *flptr;
    //flight flt_node;
    ticket tkt;
    status_code sc=SUCCESS;
    flptr = search_flight(swptr.flt_ptr, flight_id);
    if(flptr==NULL)
    {
        printf("\n !! Flight Id %s not found !!", flight_id);
        sc=FAILURE;
    }
    else
    {
        printf("\nflight search complete");
        sc = search_ticket(ticket_id, flptr->flt.tik_ptr, &tkt);
        if(sc)
        {
            printf("\nticket search complete");
            tkt = book_seat(flptr->flt, tkt);
            //printf("%d",tkt.psngr[0].seat_num);
            storeDataInTicket(tkt, flptr->flt.tik_ptr, ticket_id);
            //printf("       Boarding SuccessFull  !!!       \n");
        }
        else
            printf("\n !! Ticket Id %s not found !!", ticket_id);
            sc=FAILURE;
    }
    return sc;

}


//=======================END OF BOARDING PROCESS====================

void DisplayAvailableSeats(flight flt){
    printf("\n\n=============================\n");
    print_chart(flt.mat);
    printf("\n=============================\n\n");
}

void RangeDisplayPassengerDetails(flight flt, char from_tik[], char to_tik[])
{
    printKeysInRange(flt.tik_ptr, from_tik, to_tik);
}

void printKeysInRange(ticket_tree *ptr, char from[], char to[])
{
    if(ptr)
    {
        for(int i=0; i<ptr->n; i++)
        {
            if(strcmp(ptr->ticket_key[i].ticket_id, from)>0 && strcmp(ptr->ticket_key[i].ticket_id, to)<0)
            {
                //printf("%d", ptr->keys[i]);
                printTicketData(ptr->ticket_key[i]);
                printKeysInRange(ptr->ticket_child[i], from, to);
                printKeysInRange(ptr->ticket_child[i+1], from, to);
            }
            else if(strcmp(ptr->ticket_key[i].ticket_id, from)<=0)
            {
                printKeysInRange(ptr->ticket_child[i+1], from, to);
            }
            else if(strcmp(ptr->ticket_key[i].ticket_id, to)>=0)
            {
                printKeysInRange(ptr->ticket_child[i], from, to);
            }
        }
        //return;
    }
    return ;
}


