#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<windows.h>
#define INSERT 1
#define DELETE 2
#define UPDATE 3
#define APPEND 4
using namespace std;
struct node {
	char str[100];
	int line;
	int opera;
	struct node *next;
};
class stack{
	private:
		int top;
		struct node *head;
	public:
		stack(){
			this->top=0;
			head=NULL;
		}
		int Top(){
			return this->top;
		}
		void print(void);
		void push(struct node *n1);
		struct node * pop();	
		
};
void stack :: push(struct node *n1){
	top++;
	if(head==NULL){
		head=n1;
	}else{
		n1->next=head;
		head=n1;
	}
}
struct node* stack :: pop(){
	if(head==NULL){
		return NULL;
	}
	 top--;
	 struct node *temp=head;
	 head=head->next;
	 temp->next=NULL;
	 return temp;
}  
void stack :: print(){
	struct node *tr=head;
	struct node *n1;
	stack temp;
	while(Top()!=0){
		n1=this->pop();
	    temp.push(n1);
	}
	while(temp.Top()!=0){
		n1=temp.pop();
		cout<<n1->str<<endl;
		push(n1);
	}
}
stack undo,redo,text,temp;
struct node * create(char *str,int line_no,int opera){
	struct node *n1;
	n1=new node;
	if(str==NULL){
		n1->str[0]='\0';
	}else{
	strcpy(n1->str,str);
	}
	n1->line=line_no;
	n1->opera=opera;
	n1->next=NULL;
	return n1;
}
void input(char *str){
	gets(str);
	fflush(stdin);
}
void ClearRedo(){
	struct node *n1;
	while(redo.Top()!=0){
		n1=redo.pop();
		delete n1;
	}
}
void insert(char *str){
 	struct node *n1,*n2;
 	int t=text.Top()+1;
 	n1=create(str,t,INSERT);
 	n2=create(NULL,t,INSERT);
 	text.push(n1);
 	undo.push(n2);
	ClearRedo();
}
void Delete(int n){
	int i=1,j=1;
	struct node *n1;
	while(text.Top()!=n){
		n1=text.pop();
		(n1->line)--;
		temp.push(n1);
	}
	n1=text.pop();
	n1->opera=DELETE;
	undo.push(n1);
	while(temp.Top()!=0){
		n1=temp.pop();
		text.push(n1);
	}
	 ClearRedo();
}
void Append(int n,char *str){
 	int i=1,j;
 	struct node *n1,*n2;
 	if(text.Top()<n || n<=0){
 		return;
	 }
 	while(text.Top()!=n){
 		n1=text.pop();
 		temp.push(n1);
	 }
	 n1=text.pop();
	 n2=new node;
	 strcpy(n2->str,n1->str);
	 n2->line=n1->line;
	 n2->opera=APPEND;
	 strcat(n1->str,str);
	 text.push(n1);
	 undo.push(n2);
	 while(temp.Top()!=0){
	 	n1=temp.pop();
	 	text.push(n1);
	 }
	  ClearRedo();
}
void Update(int n,char *str){
	struct node *n1,*n2;
	while(text.Top()!=n){
		n1=text.pop();
		temp.push(n1);
	}
	n1=text.pop();
	n2=new node;
	strcpy(n2->str,n1->str);
	n2->line=n1->line;
	n2->opera=UPDATE;
	strcpy(n1->str,str);
	undo.push(n2);
	text.push(n1);
	while(temp.Top()!=0){
		n1=temp.pop();
		text.push(n1);
	}
	 ClearRedo();
}
void Undo(){
	if(undo.Top()==0){
		cout<<"Not Perform This Operation\n";
		return ;
	}
	struct node *current;
	current=undo.pop();
	if(current->opera==INSERT){
		struct node *n1;
	 	n1=text.pop();
	 	n1->opera=INSERT;
	 	redo.push(n1);
	}
	if(current->opera==DELETE){
		struct node *n1; 
		while(text.Top()!=current->line-1){
			n1=text.pop();
			temp.push(n1);
		}
		text.push(current);
		redo.push(create(NULL,current->line,DELETE));
		while(temp.Top()!=0){
			n1=temp.pop();
			(n1->line)++;
			text.push(n1);
		}
	}
	if(current->opera==APPEND){
		struct node *n1;
		while(text.Top()!=current->line){
			n1=text.pop();
			temp.push(n1);
		}
		n1=text.pop();
		n1->opera=APPEND;
		redo.push(n1);
		text.push(current);
		while(temp.Top()!=0){
			n1=temp.pop();
			text.push(n1);
		}
	}
	if(current->opera==UPDATE){
		struct node *n1;
		while(text.Top()!=current->line){
			n1=text.pop();
			temp.push(n1);
		}
		n1=text.pop();
		n1->opera=UPDATE;
		redo.push(n1);
		text.push(current);
		while(temp.Top()!=0){
			n1=temp.pop();
			text.push(n1);
		}
	}
}
void Redo(){
	if(redo.Top()==0){
		cout<<"You can't Redo";
		return ;
	}
	struct node *current=redo.pop();
	if(current->opera==INSERT){
	 	undo.push(create(NULL,current->line,INSERT));
		text.push(current);
	}
	if(current->opera==DELETE){
		struct node *n1;
		while(text.Top()!=current->line){
			n1=text.pop();
			(n1->line)--;
			temp.push(n1);
		}
		n1=text.pop();
		n1->opera=DELETE;
		undo.push(n1);
		while(temp.Top()!=0){
			n1=temp.pop();
			text.push(n1);
		}
	}
	if(current->opera==APPEND || current->opera==UPDATE){
		struct node *n1;
		while(text.Top()!=current->line){
			n1=text.pop();
			temp.push(n1);
		}
		n1=text.pop();
		if(current->opera==APPEND){
		n1->opera=APPEND;
		}else{
				n1->opera=UPDATE;	
		}
		undo.push(n1);
		text.push(current);
		while(temp.Top()!=0){
			n1=temp.pop();
			text.push(n1);
		}
	}
}
void store(char *str){
	struct node *n1;
	if(text.Top()<=0){
		cout<<"Empty-Data ";
		return;
	}
	cout<<"Please Wait Data Tranfering On File ....  ";
	FILE *fp;
	while(text.Top()!=0){
		n1=text.pop();
		temp.push(n1);
	}
	fp=fopen(str,"w");
	while(temp.Top()!=0){
		Sleep(900);
		n1=temp.pop();
		fputs(n1->str,fp);
		fputs("\n",fp);
		text.push(n1);
	}
	cout<<" Done :)"<<endl;
	fclose(fp);	
}
void Show_List(){
 		cout<<"\n_____________________________________________________________________\n";
	    cout<<"Welcome Text-Editor Console Program's\n";
 		cout<<"1. Insert Data\n";
 		cout<<"2. Up-date Date\n";
 		cout<<"3. Append Data\n";
 		cout<<"4. Search Data\n";
 		cout<<"5. Delete Line\n";
 		cout<<"6. Display All Data\n";
 		cout<<"7. Undo\n";
 		cout<<"8. Redo\n";
 		cout<<"9. Save Data Permanatally\n";
 		cout<<"10.Exit\n";
 		cout<<"_____________________________________________________________________\n";
}
void search(char *str){
	struct node *n1;
	while(text.Top()!=0){
		n1=text.pop();
		temp.push(n1);
	}
//	cout<<str;
	cout<<"Finding..."<<endl;
	while(temp.Top()!=0){
		n1=temp.pop();
		if(n1->str==str){
		     cout<<str;
		}
		text.push(n1);
	}
	getch();
}
int main(){
 		int choise,n;
 		char str[100];
 		while(1){
 		cout<<"\nDone Press Key For Reset :";
 		system("pause");
 		fflush(stdin);
 		system("cls");
 		Show_List();
 		cout<<"Select Any One at a Time : ";
 		cin>>choise;
 		fflush(stdin);
 		switch(choise){
 			case 1:
 				cout<<"Put text : ";
 				input(str);
 				insert(str);
 				break;
 			case 2:
 				text.print();
 				cout<<"Row No. ";
 				cin>>n;
 				fflush(stdin);
 				input(str);
 				Update(n,str);
 				break;
 			case 3:
 				text.print();
 				cout<<"Row No. ";
 				cin>>n;
 				fflush(stdin);
 				input(str);
 				Append(n,str);
 				break;
 			case 4:
 				 cout<<"Finding Data :";
 				 input(str);
				 search(str);
 				 break;
 			case 5:
 				text.print();
 				cout<<"Row No. ";
 				cin>>n;
 				fflush(stdin);
 				Delete(n);
 				break;
 			case 6:
 				text.print();
 				break;
 			case 7:
 				Undo();
 				break;
 			case 8:
 				Redo();
 				break;
 			case 9:
 				cout<<"Enter File Name : ";
 				input(str);
 				store(str);
 				break;
 			case 10:
 				return 0;
 			default :
 				cout<<"\nIn-vailid Choise\n ";
		 }
	}
	return 0;	
}
