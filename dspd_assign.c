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



int main()
{
    flight_tree *root = NULL;
    flight_tree *newptr,*found;
    status_code sc;
    char string[6], str[6], tkid[9];
    int choice = 1;
    // int flag = 1;
    while(choice)
    {
    printf("enter the choice \n");
    printf("1. Inserting a flight\n2. how tree looks like\n3. searching any flight\n4. searching any ticket id\n0.Exit\n");
    scanf("%d",&choice);
     switch(choice)
       {
            case 1:
                newptr = make_flight_tree_node();
                root = InsertFlight(root,newptr);
                break;

            case 2:
                Inorder(root);
                break;

            case 3:
                //Display Flight Data
                printf("\nEnter the flight id you want to search : ");
                scanf("%s",string);
                found=search_flight(root,string);
                if(found==NULL)
                    printf("\n!!Flight id not found !!\n");
                else
                    printFlightData(found->flt);
                break;
            case 4:
                //Display Passenger Details
                printf("\nEnter the ticket ID you want to search : ");
                scanf("%s", tkid);
                strncpy(str, tkid, 5);
                str[5] = '\0';
                printf("%s", str);
                //search flight node
                found = search_flight(root, str);
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
                break;
        }
    }

    /*ticket_tree *root=NULL;
    ticket tckt[4];
    passenger pasgr1;
    //pasgr1 = scanPassengerDetails();
    for(int i=0; i<4; i++)
    {
        tckt[i] = scanTicketData();
        root = insert(tckt[i], root);
    }
    //printTicketData(tckt1);
    displayInorder(root);*/

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
    for(int i=0; i<tik.num_passenger && tik.num_passenger<MAX_PASSENGER; i++)
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
    for(int i=0; i<tik.num_passenger; i++)
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
        printf("\n* Seat Number : %d", psgr.seat_num);
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
	flt.mat = NULL; //to add availability matrix
	printf("\nEnter number of ticket IDS : ");
	scanf("%d", &flt.num_of_ticket_ids);
	flt.tik_ptr = NULL;
	//scan ticket data
	ticket tik;
	for(int i=0; i<flt.num_of_ticket_ids; i++)
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
    printf("\n#######################################################\n\n");
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

