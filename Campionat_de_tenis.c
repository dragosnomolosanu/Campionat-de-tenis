#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Strucuta de jucator
typedef struct Player
{
	char *last_name;
	char *first_name;
	int score;
}Player;

//Structura de player
typedef struct Country
{
	char *name;
	int nr_players;
	int global_score;
	Player *players;
}Country;

//Structura pentru un nod tara

typedef struct node
{
	Country *country;
	float country_score;
	int local_score;
	struct node *prev,*next;
}node;

///Functii pentru citirea din fisier a datelor de tip int, sir de caractere si float (pentru o citire mai simpla a datelor din fisiere)
int getint(FILE **f,int *n)
{
	int temp;
	fscanf(*f,"%d",n);
	temp=*n;
	return temp;
}


char *getsir(FILE **f,char **sir)
{
	fscanf(*f,"%s",*sir);
	char *temp=(char*)malloc(strlen(*sir)*sizeof(char));
	strcpy(temp,*sir);
	return temp;
}


float getfloat(FILE **f,float *n)
{
	float temp;
	fscanf(*f,"%f",n);
	temp=*n;
	return temp;
}


///Functie pentru citirea unei tari din fisier cu toate datele necesare
node *getalldata(FILE **f)
{

	int i,j,sum=0;
	char *nume=(char*)malloc(sizeof(char));
	int numar;

	node *newnode;
	newnode=(node*)malloc(sizeof(node));					//alocam spatiu pentru un nod de tara,
	newnode->country=(Country*)malloc(sizeof(Country));		//respectiv tara

												
	newnode->country->nr_players=numar=getint(f,&numar);
	newnode->country->name=getsir(f,&nume);					//citim din fisier,alocam dinamic si asociem numele echipei

	newnode->country->players=(Player*)malloc(sizeof(Player)*newnode->country->nr_players);		//alocam dinamic numarul de jucatori
			for (j=0;j<(newnode->country->nr_players);j++)
				{
					newnode->country->players[j].last_name=getsir(f,&nume);	//citim din fisier,alocam dinamic si asociem numele si prenumele

					newnode->country->players[j].first_name=getsir(f,&nume);

					newnode->country->players[j].score=getint(f,&numar);		//respectiv scorul fiecarui jucator
				
				}
			for (j=0;j<(newnode->country->nr_players);j++)						/// Tot aici am facut si scorul minim pentru fiecare tara
			{
				sum=sum+newnode->country->players[j].score;
			}
			newnode->country_score=(float)sum/newnode->country->nr_players;
			
	return newnode;
}


float medie(FILE **f)
{
	float medie=0;
	node *newnode;
	int j,sum=0;
	for (j=0;j<(newnode->country->nr_players);j++)
	{
		sum=sum+newnode->country->players[j].score;
	}
	medie=(float)sum/newnode->country->nr_players;
	return medie;
}


//functie pentru adaugarea unei tari
void insertCountry(FILE **f,node **head)					///functie pentru introducerea unei tari
{
	//adaugam santinela la inceputul listei	
	if (*head==0)
	{	
		node *sentinel=(node*)malloc(sizeof(node));			///punem santinela la inceput
		*head=sentinel;
		sentinel->next=sentinel;
		sentinel->prev=sentinel;
	}


	
	node *newnode=getalldata(f);			///Adaugam toata informatia necesara in noul nod
	newnode->next=(*head);
	newnode->prev=(*head)->prev;
	(*head)->prev->next=newnode;
	(*head)->prev=newnode;
	return;
	

}

///functia swap,foarte utila cand o ai :))))
void swap(node **a,node **b)
{
	node *temp=*a;
	*a=*b;
	*b=temp;
}


//functie pentru calcularea celei mai mari puteri a lui 2
int highestpowerof2(int n)
{

    if(n<1)
    {
        return -1;
    }

    int res=1;
    for (int i=0;i<8*sizeof(int);i++)
    {
        int curr=1<<i;
        if (curr>n)
            break;
        res=curr;
    }
    return res;
}

	

//functie pentru gasirea tarii cu scorul minim.Aici am procedat in felul urmator:
//-minimul porneste de la nodul final al listei, si de fiecare data cand este gasit un element mai mic sau egal cu minimul, acesta in cele din urma preia acel nod
//-explic de ce am facut asta la functia delete:

node *findminim(node **head,int *n)
{
	int i;
	node *aux;
	node *minim=(node*)malloc(sizeof(node));
	minim->country_score=(*head)->country_score;
	for (aux=(*head);aux!=(*head)->next;aux=aux->prev) //
	{
		if (aux->country_score<=minim->country_score) ///
		{
			minim=aux;
		}
		
	}
	return minim;
}


//functie pentru eliberarea de memorie a unei tari
void freememory(node *head)
{
	int j;
	if (head)
	{
		if (head->country)
		{
			for (j=0;j<(head->country->nr_players);j++)
			{
				free(head->country->players[j].last_name);
				free(head->country->players[j].first_name);
			}
			free(head->country->name);
			free(head->country->players);
			free(head->country);
		}
		free(head);
	}
}


///functie de stergere:
///pentru cazul in care ,la ultima stergere din lista sunt mai multe tari cu acelasi scor minim, minimul gasit mai sus, daca parcurg lista in sens normal, returneaza minimul cel mai indepartat de santinela
///ceea ce noi nu dorim, asa ca am parcurs lista in sens invers, iar funnctia returneaza tara cu scor minim, pornind de la santinela
void delete(node *head,int *n)
{
	int q;
	q=highestpowerof2(*n);
	node *temp;
	
	while((*n)>q)
	{
		node *aux=(node*)malloc(sizeof(node));
		aux=findminim(&head->prev,n);
		for (temp=head->next;temp!=head;temp=temp->next);
		{	
		aux->next->prev=aux->prev;
		aux->prev->next=aux->next;
		freememory(aux);
		}
		(*n)--;
	}
}


///CERINTA 3
///Declaram o structura stack,facem functiile de push,pop,isEmptyStack,createfirststack si displaystack. Data din stack am decis sa fie nod de tara precum la lista dublu inlantuita
typedef struct stack
{
	node *countrynode;
	struct stack *next;
}stack;

int isEmptyStack(stack *top)
{
	return top==NULL;
}



void push(stack **top,node **node)						//functia push introduce in stack un nod de tip tara
{
	stack *newnode=(stack*)malloc(sizeof(stack));
	newnode->countrynode=malloc(sizeof(node));
	newnode->countrynode=*node;
	newnode->next=*top;
	*top=newnode;
}

void pop(stack **top)						//functia pop sterge astfel de noduri si elibereaza memoria
{
	stack *temp;
	temp=*top;
	if (*top==NULL)
	{
		exit(1);
	}
	else
	{
		*top=(*top)->next;
		free(temp);
	}
}

void createfirsttack(stack **top,node *newhead)			//functia de creere a stivei initiale
{
	node *temp;
	temp=newhead->next;
	node *aux;
	for (aux=newhead->next;aux!=newhead;aux=temp->next)
	{
		push(top,&temp);
		temp=temp->next;
	}
	push(top,&newhead->prev);
}


///Functia de display . Folosita pentru a verifica ca tarile sunt introduse corect in stack, dar si pentru a afisa tarile castigatoare din stiva WINNER
void displaystack(FILE *f,stack **top)
{
	stack *temp;
	temp=*top;
	if (*top==NULL)
	{
		exit(1);
	}
	else
	{ 
		for(temp=*top;temp!=NULL;temp=temp->next)
		{
		fprintf(f,"\n%s --- %d",temp->countrynode->country->name,temp->countrynode->country->global_score);
		}
	}
}

//Functie pentru afisarea jucatorilor unui stack. Pentru cerinta 4, am pus jucatorii din ultimele 4 tari ramase in turneu intr-un al doilea stack. Cu aceasta functie verific daca s-au introdus corect
void displaysecondstack(FILE *f,stack **top)
{
	int j;
	stack *temp;
	temp=*top;
	if (*top==NULL)
	{
		exit(1);
	}
	else
	{ 
		for(temp=*top;temp!=NULL;temp=temp->next)
		{
			for (j=0;j<(temp->countrynode->country->nr_players);j++)
			{		
			fprintf(f,"\n%s %s %d",temp->countrynode->country->players[j].first_name,temp->countrynode->country->players[j].last_name,temp->countrynode->country->players[j].score);
			}
		}
	}
}


///Urmeaza tot ceea ce tine de creerea unei cozi:nod de coada,enqueue,dequeue,displayqueue. Asemenea stack-ului, am decis ca data din structura coada sa fie tot un nod din lista dublu inlantuita
typedef struct queue
{
	node *countrynode;
	struct queue *next;
}queue;

void enqueue(queue **front,queue **rear,stack **node)	//functie de inserare de nod in coada
{
	queue* newnode=(queue*)malloc(sizeof(queue));
	newnode->countrynode=malloc(sizeof(node));
	newnode->countrynode=(*node)->countrynode;
	newnode->next=NULL;
	if (*front==NULL && *rear==NULL)
	{
		*front=*rear=newnode;
	}
	else
	{
		(*rear)->next=newnode;
		*rear=newnode;
	}
}

void dequeue(queue **front,queue **rear)	//functie de stergere de nod, mai precis doar de eliberare de memorie
{
	queue *temp;
	temp=*front;
	if (*front==NULL && *rear==NULL)
	{
		exit(1);
	}
	else
	{
		*front=(*front)->next;
		free(temp);
	}
	temp=temp->next;
}

//Functia principala de creere a cozilor. Aceasta functie preia din stack-ul initiar tari in pereche de cate 2 pentru meciuri

void createqueue(queue **front,queue **rear,stack **top,int *n)  
{
	*front=NULL;
	*rear=NULL;
	int i;
	stack *temp;
	temp=*top;
	enqueue(front,rear,&temp);		//pun tara in coada
	*top=(*top)->next;				//si scot tara respectiva din stiva
	pop(&temp);
	enqueue(front,rear,&temp);
	*top=(*top)->next;
	pop(&temp);
}

//functie de afiseare a unei cozi, folosita pentru a verifica daca informatia este pusa corect
void displayqueue(queue **front,queue **rear,FILE **f)
{
	queue *temp;
	if (*front==NULL && *rear==NULL)
	{
		exit(1);
	}
	else
	{
		temp=*front;
		while(temp!=NULL)
		{
			fprintf(*f,"%s\n",temp->countrynode->country->name);
			temp=temp->next;
		}
	}
}
///Functia de punere in stiva initiala a tarilor din stiva WINNER
void pushintstack(stack **top, stack **WinnerStack)
{
	stack *newnode=(stack*)malloc(sizeof(stack));
	newnode->countrynode=malloc(sizeof(node));
	newnode->countrynode=(*WinnerStack)->countrynode;
	newnode->next=*top;
	*top=newnode;
}

///Functia log in baza 2 din n, folosita pentru a numerota etapele din campionat
double Log2(int n)
{
	return log( n )/log ( 2 );
}


///Functia meciuri este cea mai complexa functie din cod. Aici au loc urmatoarele operatii:
///-au loc meciurile dintre tari, pe etape
/// Se pun in stiva WINNER tarile cu scor local mai mare


///Pentru cerinta 4 am facut o a doua stiva in care sunt pusi jucatorii din ultimele 4 tari ramase in turneu. 
///Din pacate, nu am reusit sa le introduc in arbore...
typedef struct treenode
{
	char *last_name;
	char *first_name;
	int score;
	struct treenode *left,*right;
}treenode;

///Aici am initializat primul jucator din a doua stiva
treenode *initTree (stack **SecondStackTop)
{
	
	treenode *root=(treenode*)malloc(sizeof(treenode));
	root->first_name=(char*)malloc(sizeof(char));
	root->last_name=(char*)malloc(sizeof(char));
	root->left=root->right=0;
	root->first_name=(*SecondStackTop)->countrynode->country->players[0].first_name;
	root->last_name=(*SecondStackTop)->countrynode->country->players[0].last_name;
	root->score=(*SecondStackTop)->countrynode->country->players[0].score;
	return root;
}


///Aici am incercat sa implementez iterativ functia de adaugare in arbore, doar ca aceasta imi punea aleatoriu 3 jucatori din a doua stiva
///Iar implementata recursiv primieam segmentation fault
treenode *insert(treenode *root,stack **SecondStackTop)
{
	int j;	
	if (root==NULL)
		{
			root=initTree(SecondStackTop);
		}
		
		else
		{	
		for (j=1;j<((*SecondStackTop)->countrynode->country->nr_players);j++)
			{	
			if ((*SecondStackTop)->countrynode->country->players[j].score < (*SecondStackTop)->countrynode->country->players[0].score)
				{
					root->left=(treenode*)malloc(sizeof(treenode));
					root->left->first_name=(*SecondStackTop)->countrynode->country->players[j].first_name;
					root->left->last_name=(*SecondStackTop)->countrynode->country->players[j].last_name;
					root->left->score=(*SecondStackTop)->countrynode->country->players[j].score;
					
				}

			else if ((*SecondStackTop)->countrynode->country->players[j].score > root->score)
				{
					root->right=(treenode*)malloc(sizeof(treenode));
					root->right->first_name=(*SecondStackTop)->countrynode->country->players[j].first_name;
					root->right->last_name=(*SecondStackTop)->countrynode->country->players[j].last_name;
					root->right->score=(*SecondStackTop)->countrynode->country->players[j].score;
					
				}

			}
		}
	
		
	return root;
		
}


///Functie de creere a BST-ului
treenode *createBST(treenode *root,stack *SecondStackTop)
{
	int j;
	root=NULL;
	stack *temp;
	temp=SecondStackTop;
	while(temp!=NULL)
	{
		root=insert(root,&temp);
		temp=temp->next;
	}
	return root;
}

void inorderreversed(FILE *g, treenode *root)		///Pentru a afisa jucatori in ordine descrescatoare din BST, am decis sa folosesc un inorder "intors".
{
	
	{
		if (root)
		{
		inorderreversed(g,root->right);
		fprintf(g,"\n%s %s %d",root->first_name,root->last_name,root->score);
		inorderreversed(g,root->left);
		}
	}
}


void meciuri(FILE *g,queue *front,queue *rear,stack *FirstStackTop,stack *WinnerStack,int *n)
{	
		front=NULL;
		rear=NULL;
		int max1;
		char *sir1;								///Definim toate variabilele de care avem nevoie
		int max2;
		char *sir2;
		WinnerStack=NULL;
		int x;
		int i,j;
		int u;
		int v=Log2(*n);
		stack *SecondStackTop=NULL;
		for (u=1;u<=v;u++)					//U porneste de la 1 pana la ultima etapa
		{	
			if (u!=1)
			{	
			fprintf(g,"\n");
			}
			fprintf(g,"\n====== ETAPA %d ======",u);
			for (x=0;x<(*n)/2;x++)
			{	
			createqueue(&front,&rear,&FirstStackTop,n);		///Se pun tarile in cozi si incep meciurile
			fprintf(g,"\n");
			fprintf(g,"\n%s %d ----- %s %d",front->countrynode->country->name,front->countrynode->country->global_score,rear->countrynode->country->name,rear->countrynode->country->global_score);	
			for (i=0;i<(front->countrynode->country->nr_players);i++)
				{
				for (j=0;j<(rear->countrynode->country->nr_players);j++)
					{
						fprintf(g,"\n%s %s %d vs %s %s %d", front->countrynode->country->players[i].last_name, front->countrynode->country->players[i].first_name, front->countrynode->country->players[i].score,rear->countrynode->country->players[j].last_name,rear->countrynode->country->players[j].first_name,rear->countrynode->country->players[j].score);	
						if (front->countrynode->country->players[i].score > rear->countrynode->country->players[j].score)
							{
								front->countrynode->country->players[i].score=front->countrynode->country->players[i].score+5;					///In functie de rezultat se actualizeaza punctele pentru jucatori,
								front->countrynode->local_score = front->countrynode->local_score+3;											///respectiv scorul local pentru tara
							}
							else if (front->countrynode->country->players[i].score == rear->countrynode->country->players[j].score)
								{
									front->countrynode->country->players[i].score=front->countrynode->country->players[i].score+2;
									front->countrynode->local_score=front->countrynode->local_score+1;
									rear->countrynode->country->players[j].score=rear->countrynode->country->players[j].score+2;
									rear->countrynode->local_score=rear->countrynode->local_score+1;
								}
							else	if (front->countrynode->country->players[i].score < rear->countrynode->country->players[j].score)
								{
									rear->countrynode->country->players[j].score=rear->countrynode->country->players[j].score+5;
									rear->countrynode->local_score = rear->countrynode->local_score+3;
								}	
						
						}
					}

					if (front->countrynode->local_score > rear->countrynode->local_score)
					{
					push(&WinnerStack,&front->countrynode);
					}
					else if (front->countrynode->local_score < rear->countrynode->local_score)
					{
							push(&WinnerStack,&rear->countrynode);
					}

					else if (front->countrynode->local_score == rear->countrynode->local_score)
					{


							//Aici, pnetru ca in ultimele zile a fost o confuzie legata de tarile cu scor local elgal, mi-am facut 4 auxiliare, max1,max2,sir1,sir2 cu care sa introduc in stiva castigatoare
							for (i=0;i<(front->countrynode->country->nr_players);i++)		/// tara cu jucatorul cel mai mic din punct de vedere lexicografic
							{																///dupa ce a aparut update-ul pe site, m-am conformat cu acela, dar am preferat sa nu sterg pentru eventuale		
								for (j=0;j<(rear->countrynode->country->nr_players);j++)	///teste
								{	
									sir1=(char*)malloc(sizeof(char));
									sir2=(char*)malloc(sizeof(char));
									strcpy(sir1,front->countrynode->country->players[0].last_name);
									strcpy(sir2,rear->countrynode->country->players[0].last_name);
									max1=front->countrynode->country->players[0].score;
									max2=rear->countrynode->country->players[0].score;
									if (front->countrynode->country->players[i].score>max1)
									{
											max1=front->countrynode->country->players[i].score;
											strcpy(sir1,front->countrynode->country->players[i].last_name);
									}
									if (rear->countrynode->country->players[j].score>max2)
									{
											max2=rear->countrynode->country->players[j].score;
											strcpy(sir2,rear->countrynode->country->players[j].last_name);
									}
								}
							}
							
						if (max1>max2)
							{
								push(&WinnerStack,&front->countrynode);				///Tara cu scor local mai mare ste pusa in stiva, altfel se pune tara a carui jucator are scorul personal mai mare
																					 
							}
							else if(max1<max2)
							{
								push(&WinnerStack,&rear->countrynode);
							}
							else if (max1==max2)									///Daca exista 2 jucatori cu acelasi scor personal, atunci este pusa tara care este scoasa prima din stiva initiala
							{
								push(&WinnerStack,&front->countrynode);
								
							}
						
							
					}
					front->countrynode->country->global_score=front->countrynode->country->global_score+front->countrynode->local_score;	///Actualizam si scorurile globale ale tarilor
						rear->countrynode->country->global_score=rear->countrynode->country->global_score+rear->countrynode->local_score;	
				}
			stack *temp,*temp2;
			fprintf(g,"\n");
			fprintf(g, "\n=== WINNER ===");
			displaystack(g,&WinnerStack);  ///Afisam stiva WINNER
			for (temp2=WinnerStack;temp2!=NULL;temp2=temp2->next)
			{
				temp2->countrynode->local_score=0;		///Si actualizam scorurile locale ale tarilor cu 0(Aici am avut ptobleme mari)
			}
			temp=WinnerStack;
			while(temp!=NULL)
			{
				pushintstack(&FirstStackTop,&temp);			///Si punem tarile din stiva WINNER inapoi in cea initiala
				WinnerStack=WinnerStack->next;
				pop(&temp);
			}
			(*n)=(*n)/2;
		}
		/*									
		fprintf(g,"\n");										
		fprintf(g,"\n====== CLASAMENT JUCATORI ======");
		displaysecondstack(g,&SecondStackTop);			///Afisez jucatorii din ultimele 4 tari ramase in turneu din stiva in care i-am introdus
		pushinarray(&v,&SecondStackTop);
		fprintf(g,"\n");
		fprintf(g,"\nAsa arata clasamentul din arbore:");		///Aici am incercat sa creez arborele, fara succes
		treenode *root;
		*/
			
}		
	

int main(int argc,char *argv[])
{
	FILE *f,*g,*h;
	int i,j,n;
	node *head=NULL;
	f=fopen(argv[1],"r");
	g=fopen(argv[2],"r");
	h=fopen(argv[3],"w");
	
	if (f==NULL)
		{
			printf("Fisierul de cerinte nu poate fi deschis!");		///Verificam fiecare fisier in parte 
			exit(1);
		}
	
	if (g==NULL)
	{
		printf("Fisierul de citire nu poate fi deschis!");
		exit(1);
	}
	
	if (h==NULL)
	{
		printf("Fisierul de scriere nu poate fi deschis!");
		exit(1);
	}

	fscanf(g,"%d",&n);

	int k;
	int v[5];
	for (k=0;k<n;k++)				///Pentru o rezolvare mai precisa a cerintelor, am facut un vector de 5 elemente care primeste numerele din cerinte.in
	{
		fscanf(f,"%d",&v[1]);
		fscanf(f,"%d",&v[2]);
		fscanf(f,"%d",&v[3]);
		fscanf(f,"%d",&v[4]);
		fscanf(f,"%d",&v[5]);
	}

	for (i=0;i<n;i++)
	{	
		insertCountry(&g,&head);
	}

	if (v[2]==1)				///Daor de la cerinta 2 in sus stergem tari
		{		
			delete(head,&n);
		}

	for (i=0;i<n;i++)
	{	
		fputs(head->next->country->name,h);			///Afisam tarile in fisierul rezultate.out
		fprintf(h,"\n");
		head=head->next;
	}	
	
	if (v[3]==1)
	{	
	node *newhead=head->next;   ///Datorita algoritmului de eliberare de stergere de nod  de la cerinta 2,santinela din head a devenid head->next. Am incercat sa rezolv problema de eliberare, fara succes
							///asa ca am decis sa-mi iau un nou nod,newhead,pentru a nu avea probleme de confuzie.
	///Asadar, NEWHEAD ESTE SANTINELA!!!!(ca sa nu uit :))) )

	stack *FirstStackTop=NULL;
	createfirsttack(&FirstStackTop,newhead);
	queue *front;
	queue *rear;
	stack *WinnerStack;
	stack *SecondStackTop;
	meciuri(h,front,rear,FirstStackTop,WinnerStack,&n);			///Functia pentru meciuri
	}
				
	return 0;				
}	

	


