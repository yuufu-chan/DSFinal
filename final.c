#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct queue
{
    int item;
    int type;
    int arrival_time;
    struct queue *next;
}q;

int *ask_type_amount();
int *ask_type_time();
q *generate_data(int);
q *organize_data(q*);
q *assign(q *,int ,int ,int );
void enqueue(q **,q *);
q *dequeue(q **);
int proccess(q *,int,int);


int main()
{

    srand(time(NULL));

    //user input
    //allow user to determine data amount and processing time of 3 types
    int amount,*time;
    amount=ask_data_amount();

    time=malloc( 3*sizeof(int));
    time=ask_type_time();

    //generating data
    q *data;
    data=generate_data(amount);

    //organizing data
    q *order;
    order=organize_data(data);

    //write organized data into file
    char filename[10];
    FILE * filepointer;

    printf("Please enter the name of file to store: ");
    scanf("%s", filename);
    filepointer = fopen(filename, "w");
    print_f(order,filepointer);
    fclose(filepointer);

    //assigning and processing data
    assign(order,*time,*(time+1),*(time+2));

}

int ask_data_amount()
{
    int amount;
    printf("data amount: ");
    scanf("%d",&amount);
    return amount;
}

int *ask_type_time()
{
    int *time,i;
    time=malloc( 3*sizeof(int));
    for(i=0;i<3;i++)
    {
        printf("type %d processing time: ",i+1);
        scanf("%d",time+i);
    }
    return time;
}


//generate set
q *generate_data(int num_of_data)
{
    q *set=malloc(sizeof(q)),*ptr;
    int i;
    for(i=0;i<num_of_data;i++)
    {
        //generate first node of set
        if(i==0)
        {
            set->item=rand()%1000;//generate 0~999
            set->type=rand()%3+1;//generate 1~3
            set->arrival_time=rand()%20;//generate 0~19
            set->next=NULL;
            //printf("%d ",set->item);
            ptr=set;
        }
        //generate others node of set
        else
        {
            ptr->next=malloc(sizeof(q));
            ptr=ptr->next;
            ptr->item=rand()%1000;//generate 0~999
            ptr->type=rand()%3+1;//generate 1~3
            ptr->arrival_time=rand()%20;//generate 0~19
            //printf("%d ",ptr->item);
            ptr->next=NULL;

        }
    }
    print(set);
    return set;
}

/*print link list*/
void print(q *L)
{
    while(L->next!=NULL)
    {
        printf("%d ",L->item);
        L=L->next;
    }
    printf("%d \n",L->item);
}

void print_f(q *L,FILE * fp)
{
    while(L->next!=NULL)
    {
        fprintf(fp,"%d %d %d\n",L->item, L->type, L->arrival_time);
        L=L->next;
    }
    fprintf(fp,"%d %d %d\n",L->item, L->type, L->arrival_time);
}



q *organize_data(q *set)
{
    printf("!!");
    int i=0;
    q *order=malloc(sizeof(q)),*before,*after,*ptr,*noneed;
    //order list
    while(set!=NULL)
    {
        ptr=malloc(sizeof(q));
        ptr->item=set->item;
        ptr->type=set->type;
        ptr->arrival_time=set->arrival_time;
        ptr->next=NULL;
        noneed=set;
        set=set->next;
        free(noneed);
        noneed=NULL;
        if(i==0)
        {
            order=ptr;
            i++;
        }
        else if(ptr->arrival_time<=after->arrival_time)
        {
            ptr->next=order;
            order=ptr;
        }
        else if(ptr->arrival_time>after->arrival_time)
        {
            while(after!=NULL)
            {
                if(ptr->arrival_time>after->arrival_time)
                {
                    before=after;
                    after=after->next;
                }
                else
                {
                    before->next=ptr;
                    ptr->next=after;
                    break;
                }
            }
            if(after==NULL)
            {
                before->next=ptr;
            }
        }
        after=order;
    }
    print(order);
    return order;
}


int proccess(q *Q,int time,int self)
{
    if(Q!=NULL){
        if(self<time){
            self++;
            printf(" item%d processing\n",Q->item);
        }
        else if(self==time){
            self=0;
            printf(" item%d completed\n",Q->item);
        }
    }
    else{
        self=0;
    }
    return self;
}

void enqueue(q **Q,q *order)
{
    q *n,*t;
    n=malloc(sizeof(q));
    n->item=order->item;
    n->type=order->type;
    n->arrival_time=order->arrival_time;
    n->next=NULL;

    if(*Q==NULL)
        *Q=n;
    else{
        t=*Q;
        while(t->next!=NULL)
            t=t->next;
        t->next=n;
    }

    return 0;
}
q *dequeue(q **Q)
{
    q *t;

    if((*Q)->next!=NULL){
        t=(*Q);
        (*Q)=(*Q)->next;
    }
    else
    {
        t=(*Q);
        (*Q)=NULL;
    }
    return t;
}
q *assign(q *order,int time1,int time2,int time3)
{
    int sec=0,self_time1=0,self_time2=0,self_time3=0,   //total time and processing time for each type
    many1=0,many2=0,many3=0,    //amount of item in queue of each type
    average1=0,average2=0,average3=0;   //Average amount of time spent in a queue for each type of items
    q *Q1,*Q2,*Q3;  //queue waiting of each type
    Q1=Q2=Q3=NULL;
    q *P1,*P2,*P3;  //item processing of each type
    P1=P2=P3=NULL;
    int check_pro=0;    //equal to 1 if all tasks are finished



    while(check_pro==0)
    {
        printf("Time %d\n",sec);

        //update processing condition
        self_time1=proccess(P1,time1,self_time1);
        self_time2=proccess(P2,time2,self_time2);
        self_time3=proccess(P3,time3,self_time3);

        //assign arriving item into right queue
        if(order!=NULL){
            while(sec==order->arrival_time){
                if(order!=NULL){
                    switch(order->type){
                        case 1:
                            enqueue(&Q1,order);
                            printf(" enqueue item%d to q1\n",order->item);
                            //print(Q1);
                            many1++;
                            break;
                        case 2:
                            enqueue(&Q2,order);
                            printf(" enqueue item%d to q2\n",order->item);
                            //print(Q2);
                            many2++;
                            break;
                        case 3:
                            enqueue(&Q3,order);
                            printf(" enqueue item%d to q3\n",order->item);
                            //print(Q3);
                            many3++;
                            break;
                    }
                    if(order->next!=NULL){
                        order=order->next;
                    }
                    else{
                        order=NULL;
                        break;
                    }

                }
            }
        }

        //no item need to be processing if no item waiting in queue
        if(self_time1==0 && Q1==NULL){
            P1=NULL;
        }
        if(self_time2==0 && Q2==NULL){
            P2=NULL;
        }
        if(self_time3==0 && Q3==NULL){
            P3=NULL;
        }

        //dequeue item into processing if there is item waiting in queue
        if(self_time1==0 && Q1!=NULL){
            P1=dequeue(&Q1);
            many1--;
            printf(" dequeue q1\n");
        }
        if(self_time2==0 && Q2!=NULL){
            P2=dequeue(&Q2);
            many2--;
            printf(" dequeue q2\n");
        }
        if(self_time3==0 && Q3!=NULL){
            P3=dequeue(&Q3);
            many3--;
            printf(" dequeue q3\n");
        }

        //finish if all item is completed
        if(self_time1==0 && self_time2==0 && self_time3==0 && order==NULL && P1==NULL && P2==NULL && P3==NULL) {
            check_pro=1;
        }

        sec++;
    }
    printf("done\n");

}
