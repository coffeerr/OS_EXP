#include<cmath>
#include<ctime>
#include<string>
#include<conio.h>
#include<fstream>
#include<cstdlib>
#include<iostream>
#include<windows.h>
using namespace std;
 
typedef struct UFD
{
	string File_name;         //�ļ���
	int Start;                //�ļ��ڴ��̴洢�ռ����ʼ��ַ
	int Protect;              //�ļ�������
	int File_length;           //�ļ��ĳ���
	int Max_File_length;         //�ļ�����󳤶�
	struct UFD *next;
}UFD,*UFD_ptr;
 
typedef struct MFD
{
	string User_name;
	string User_passwd;
	UFD *Ufd_next;
	int End;
	struct MFD *next;
}MFD,*MFD_ptr;
 
typedef struct AFD
{
	string File_name;
	int File_length;
	int Start;
	int Protect;
	int Pointer;    //�������ָ���ֵȥ����û���Ҫ��  ��дָ��
	int Max_File_length;
	struct AFD *next;
}AFD,*AFD_ptr;

#include"os_fss.h"
#define MaxDisk 512*1024   //һ������512�ֽ�,������1024������,ģ����̵Ĵ�С
int Max_User=10;
int Max_Open=5;
int Max_End=0;
UFD_ptr pufd=NULL;
MFD_ptr pmfd=NULL;
AFD_ptr pafd=NULL;
char User_name[30];   //��ŵ�ǰ�û����û���
char flag='n';
char Disk_Size[MaxDisk];    //����ڴ�ӡ�����ʱ����RWX��ʾ����
 
typedef struct Disk_Table
{
	int Max_length;      //��󳤶�
	int Start;          //��ʼλ��
}Disk_Node;
Disk_Node Disk_Head;
 
void Init_Disk()
{
	Disk_Head.Max_length=MaxDisk;
	Disk_Head.Start=0;
}
 
bool Init_MFD()
{
	pmfd=(MFD *)new MFD;   //��ͷ�ڵ�ĵ�������
	MFD *p=pmfd;
	ifstream ifs("MFD.txt");  //�ļ�������������
	if(!ifs)
	{
		cerr<<"�����޷����ļ�"<<endl;
		p->next=NULL;
		p->Ufd_next=NULL;
		return false;
	}
	while(!ifs.eof())
	{
		p->next=(MFD *)new MFD;
		ifs>>p->next->User_name>>p->next->User_passwd>>p->next->End;  
		if(p->next->End>Max_End)
			Max_End=p->next->End;
		p=p->next;
		p->Ufd_next=NULL;
		p->next=NULL;
	}
	ifs.close();
	return true;
}
 
void Print_UFD()
{
	UFD *p=pufd->next;  
	if(!p)
	{
		cout<<"��Ǹ,���û�û�д����κ��ļ�,���ȴ���!!!"<<endl;
		return ;
	}
	cout<<"�ļ���\t\t����ļ�����\t�ļ�Ȩ��\t��ʼλ��\t�ļ�����"<<endl;
	while(p)
	{
		cout<<p->File_name<<"\t\t"<<p->Max_File_length;
		if(p->Protect==0)
			cout<<"\t\t"<<"---";
		else if(p->Protect==1)
			cout<<"\t\t"<<"r-x";
		else if(p->Protect==2)
			cout<<"\t\t"<<"rwx";
		cout<<"\t\t"<<p->Start;
		cout<<"\t\t"<<p->File_length<<endl;
		p=p->next;
	}
}
 
bool Init_UFD(char *name)
{
	ifstream ifs(name); 
	pufd=(UFD *)new UFD;
	UFD *p=pufd;
	char temp[MaxDisk];
	bool Index=false;
	if(!ifs)
	{
		cerr<<"�����޷����ļ�"<<endl;
		p->next=NULL;
		return false;
	}
	while(!ifs.eof())
	{
		memset(temp,'\0',sizeof(temp));
		p->next=(UFD *)new UFD;
		if(!Index)
		{	
			pmfd->Ufd_next=p->next;
			Index=true;
		}
		ifs>>p->next->File_name>>p->next->Max_File_length>>p->next->Protect>>p->next->Start>>p->next->File_length;
		ifs>>temp;
		if(temp[0]=='#')
			temp[0]='\0';
		for(int i=p->next->Start,j=1;j<=p->next->Max_File_length-1;i++,j++)
		{
			Disk_Size[i]=temp[j-1];
			if(!Disk_Size[i])
				break;
		}
		Disk_Head.Max_length-=p->next->Max_File_length;
	//	Disk_Head.Start+=p->next->Max_File_length;
		p=p->next;
		p->next=NULL;
	}
	ifs.close();	
	return true;
}
 
void Rename_File()
{
	string File_name;
	UFD *temp=pufd;
	char Index;
SD:
	cout<<"�������ļ������֣�"<<endl;
	cin>>File_name;
	while(temp)
	{
		if(temp->File_name==File_name)
		{
			cout<<"�������µ��ļ�����"<<endl;
			cin>>File_name;
			temp->File_name=File_name;
			break;
		}
		temp=temp->next;
	}
	if(!temp)
	{
		cout<<"��Ǹ,������ļ�������,�޷�����������ļ�����,�Ƿ��������<y/n>?"<<endl;
		cin>>Index;
		if(Index=='y')
			goto SD;
		else
			return ;
	}
	else
		cout<<"�޸ĳɹ�!!!"<<endl;
}
bool User_Check()
{
	string User_passwd;
	int Count=0;
	while(1)
	{
		cout<<"�������û�����";
		cin>>User_name;
INIT:
		cout<<"���������룺";
		cin>>User_passwd;
		MFD *p=pmfd->next;
		char temp[30];
		memset(temp,'\0',sizeof(temp));
		strcpy(temp,User_name);
		Count++;
		while(p)
		{
			if(User_name==p->User_name)
			{
				if(User_passwd==p->User_passwd)
					cout<<"����ɹ�."<<endl;
				else
				{
					cout<<"��Ǹ,�������,��½ʧ��,����������!!!"<<endl;
					if(Count==3)
					{
						cout<<"��������������,ϵͳ���н���!!!"<<endl;
						return false;
					}
					goto INIT;
				}
				strcat(temp,".txt"); 
				Init_UFD(temp);
	//			Print_UFD();
				Disk_Head.Start=Max_End;
				return true;
			}
			p=p->next;
		}
		cout<<"�û���������,����������!!!"<<endl;
	}
//	return false;
}			
void Init_AFD()
{
	pafd=(AFD *)new AFD;
	pafd->next=NULL;
}
 
bool Create_MFD()//�����ļ�����
{
	string User_name;
	char User_passwd[30];
	MFD *p = pmfd;
	cout<<"������Ҫ�������û���: ";
	cin>>User_name;
	cout<<"������"<<User_name<<"�û������룺(Ĭ��Ϊ:admin)";
	getchar();
	cin.getline(User_passwd,30);
	if(User_passwd[0]=='\0')
		strcpy(User_passwd,"admin");
	while(p)
	{
		if(User_name==p->User_name)
		{
			cout<<"���û����Ѵ���!!!"<<endl;
			return false;
		}
		if(!p->next)
			break;
		p= p->next;
	}
	p->next = (MFD *)new MFD;  //β�巨
	p=p->next;
	p->User_name=User_name;
	p->User_passwd=User_passwd;
	p->End=0;
	p->next = NULL;
	p->Ufd_next = NULL;
	Max_User--;
	return true;
}  //Ӧ���� ��ͬ���û��µ�UFD�ļ����ǲ�һ���İ�����ô��������
 
bool Create_UFD()//�����ļ�����
{
	string File_name;
	UFD *p = pufd;
	unsigned int Protect;
	int Max_File_length;
//	AFD *pa = pafd;
	cout<<"������Ҫ�������ļ���: ";
	cin>>File_name;
	cout<<"������Ҫ�������ļ��������ͣ�";
	cin>>Protect;
	cout<<"�������ļ������������"<<endl;
	cin>>Max_File_length;
	while(p)
	{
		if(File_name==p->File_name)
		{
			cout<<"���ļ����Ѵ���!!!"<<endl;
			return false;
		}
		if(!p->next)
			break;
		p= p->next;
	}
	p->next = (UFD *)new UFD;  //β�巨
	p=p->next;
	p->File_name=File_name;
	p->Max_File_length=Max_File_length;
	p->Start=Disk_Head.Start;
	p->Protect = Protect;
//	p->Time_info=      //�ݶ�
	p->File_length = 0;     //�մ����ļ�ʱ���ļ��ǿյ�
	Disk_Head.Start+=Max_File_length;
	p->next = NULL;      //�Ҿ����ⲿ�ִ��������⣻�û�������һ���ļ�����������ļ��ͱ�����
	return true;
}
 
bool Delete_UFD()  //������Ŀ¼���û��ļ��е��ļ�ɾ��-->������д���ð�    //����ļ��Ѿ����ˣ��ǲ���ɾ����
{
	string File_name;
	cout<<"������Ҫɾ�����ļ�����";
	cin>>File_name;
	UFD *p=pufd,*temp;
	AFD *pa=pafd;
	while(pa->next)
	{
		if(File_name==pa->next->File_name)
		{
			cout<<"��Ǹ,���ļ��ѱ���,���ȹر�,�ٽ���ɾ������!!!"<<endl;
			return false;
		}
	}
	while(p->next)
	{
		if(File_name==p->next->File_name)
		{
			temp=p->next;
			p->next=p->next->next;  //���˵Ҫ����ģ��Ĵ��̿ռ�Ļ���Ӧ���ǻ�����󳤶�
		//	Disk_Head.Max_length+=temp->Max_File_length;    //������ɾ���ˣ��������ļ����ݵ���ʼλ�ò��伴�ɣ���û�£������������Է������ݵ���
			delete temp;
			cout<<"�ļ�ɾ���ɹ�!!!"<<endl;
			return true;
		}
		p=p->next;
	}
	cout<<"��Ǹ,Ҫɾ�����ļ�������!!!"<<endl;
	return false;
}
bool Open()
{
    string File_name;
	unsigned int Protect;
	cout<<"������Ҫ�򿪵��ļ�����";
    cin>>File_name;
    UFD *p=pufd->next;
	AFD *pa=pafd->next;
	while(pa)
	{
		if(File_name==pa->File_name)
		{
			cout<<"�ļ�"<<File_name<<"�Ѿ���!!!"<<endl;
			return true;
		}
		if(!pa->next)
			break;
		pa=pa->next;
	}
	if(!pa)     //����Ҳ��������ߴ򿪵��ļ�Ŀ¼����Ϊ�ձ�
		pa=pafd;
	while(p)
	{
        if(File_name==p->File_name)
		{
			if(!Max_Open)
			{
				cout<<"��Ǹ�����ֻ�ܴ�5���ļ�,���ȹر������򿪵��ļ��ٲ���!!!"<<endl;
				return false;
			}
			if(p->Protect==0)
			{
				cout<<"��Ǹ�����ļ�����ִ���κβ���!!!"<<endl;
				return false;
			}
			cout<<"��ѡ����ʲô���ķ�ʽ���ļ�<1-ֻ��,2-�ɶ���д>��";
			cin>>Protect;
			pa->next=(AFD *)new AFD;
			pa=pa->next;
			pa->File_name=p->File_name;
			pa->Start=p->Start;
			pa->File_length=p->File_length;
			pa->Max_File_length=p->Max_File_length;
			pa->Protect=Protect;
			if(Protect==1)
				pa->Pointer=0;   //Poniterȡ0��ʾ��ʱ�û��޷�д����(û�пռ�ĺ���)
			else
				pa->Pointer=p->File_length;
			pa->next=NULL;
			Max_Open--;
			cout<<"�ļ�"<<File_name<<"�Ѿ���!!!"<<endl;
			return true;
		}
		p=p->next;
	}
	cout<<"��Ǹ,Ҫ�򿪵��ļ�������!!!"<<endl;
	return false;
}
void Close()
{
	string file_name;
	UFD *pu=pufd->next;
	cout<<"������Ҫ�رյ��ļ�����";
	cin>>file_name;
	AFD *p=pafd,*temp;
	while(p&&p->next)
	{
		if(file_name==p->next->File_name)
		{
			temp=p->next;
			p->next=p->next->next;
			if(temp->Protect==2)      //��д���ļ�����Ȩ����д�������Ҳ����������
			{
				while(pu)
				{
					if(temp->File_name==pu->File_name)
					{
						pu->File_length=temp->Pointer;
						break;
					}
					pu=pu->next;
				}
			}
			delete temp;
			cout<<"�ļ�"<<file_name<<"�رճɹ�!!!"<<endl;
			return ;
		}
		p=p->next;
	}
	cout<<"��Ǹ,Ҫ�رյ��ļ�û�б���!!!"<<endl;
}
bool Read_File()  //��Ϊ��д����ͨ���޸������ļ�Ŀ¼��Pointerȥģ�����
{
	string File_name;       //��Ҫ���ļ��Ļ��������ļ��϶���������/���ļ�Ŀ¼���棬�ǰ�
	unsigned int length;
	AFD *p=pafd->next;
	cout<<"������Ҫ�����ļ�����";
	cin>>File_name;
	cout<<"������Ҫ��ȡ�ĳ��ȣ�";  //��ȡ�Ļ���ʵ�����ļ��ĳ��Ȳ�����ı�
	cin>>length;          //����ȡ�ĳ��Ȳ����ܱ����ȥ,Ϊ�˷��㣨����û��Ƕ����ݵĻ���pointerʼ�ղ��䣩
	while(p)
	{
		if(File_name==p->File_name)
		{
			for(int i=p->Start,j=1;j<=length;i++,j++)
				cout<<Disk_Size[i];
			cout<<endl;
			cout<<"�ļ�"<<File_name<<"��ȡ�ɹ�!!!"<<endl;
			return true;
		}
		p=p->next;
	}
	cout<<"��ȡʧ��,�ļ�û�д򿪹�!!!"<<endl;
	return false;
}
bool Write_File()  //д�Ļ�����Ȼ�������ļ�Ŀ¼
{
	string File_name;
	unsigned int length;
	AFD *p=pafd->next;
	char temp[MaxDisk]={'\0'};
	cout<<"������Ҫд���ļ�����";
	cin>>File_name;
	while(p)
	{
		if(File_name==p->File_name)
		{
			if(p->Protect!=2)
			{
				cout<<"�ļ�"<<File_name<<"����д"<<endl;
				return false;
			}
			cout<<"������Ҫд��ĳ��ȣ�"<<endl;
			cin>>length;
			if(p->Pointer+length<=p->Max_File_length)
			{
				cout<<"��д��ָ�����ȵ����ݣ�<�Իس���Ϊ������>"<<endl;
				getchar();
				gets(temp);
				for(int i=p->Start+p->File_length,j=1;j<=strlen(temp);i++,j++)
					Disk_Size[i]=temp[j-1];
				p->Pointer += length;
				cout<<"�ļ�"<<File_name<<"д��ɹ�!!!"<<endl;
			}
			else
			{
				cout<<"��д������ݴ�С�������,�ѳ����ļ��ķ�������,д��ʧ��!!!"<<endl;
				return false;
			}
			return true;
		}
		p=p->next;
	}
	cout<<"д��ʧ��,�ļ�û�д򿪹�!!!"<<endl;
	return false;
}
void Destroy_Space()
{
	MFD *pm=pmfd;
	while(pm)
	{
		pmfd=pmfd->next;
		delete pm;
		pm=pmfd;
	}
	AFD *pa=pafd;
	while(pa)
	{
		pafd=pafd->next;
		delete pa;
		pa=pafd;
	}
	UFD *pu=pufd;
	while(pu)
	{
		pufd=pufd->next;
		delete pu;
		pu=pufd;
	}
}
 
void Save_MFD()
{
	ofstream ofs;        //�ļ������������
	ofs.open("MFD.txt");
	if(!ofs.is_open())
	{
		cout<<"The MFD can't open!!!"<<endl;
		if(flag=='y')
			cout<<"���ڱ�����Ŀ¼�û�����Ϣ..."<<endl;
		else
			ofs.close();  //��������
	}
	MFD *p=pmfd->next;
	while(p)
	{
		if(p->User_name==User_name)
			p->End=Disk_Head.Start;
		ofs<<p->User_name<<" "<<p->User_passwd<<" "<<p->End;
		if(p->next)
			ofs<<"\n";	
		p=p->next;
	}
	ofs.close();
}
 
void Save_UFD(char *name)
{
	ofstream ofs;        //�ļ������������
	char temp[MaxDisk]={'\0'};
	ofs.open(name);
	if(!ofs.is_open())
	{
		cout<<"The "<<name<<"�ļ� can't open!!!"<<endl;
		ofs.close();  //�������������������õĺ����Ϊ�Ҳ�û�������˳���κ���������
	}
	UFD *p=pufd->next;
	while(p)
	{
		memset(temp,'\0',sizeof(temp));
		for(int i=p->Start,j=1;j<=p->Max_File_length;i++,j++)
		{
			if(Disk_Size[i])
				temp[j-1]=Disk_Size[i];
			else
				break;
		}
		if(!temp[0])        //����ļ�û�����ݵĻ������ÿո��ʾ���ļ�û���κ�����
			temp[0]='#'; 
		ofs<<p->File_name<<" "<<p->Max_File_length<<" "<<p->Protect<<" "<<p->Start<<" "<<p->File_length<<" "<<temp;
		if(p->next)
			ofs<<"\n";
		p=p->next;
	}
	ofs.close();
}
 
void Quit_System()
{
	AFD *pa=pafd->next;
	UFD *pu=pufd->next;
	while(pa)
	{
		if(pa->Protect==2)  //2��ʾ��д   //1��ʾֻ��  //0��ʾ���ɲ���
		{
			while(pu)
			{
				if(pa->File_name==pu->File_name)
				{
					pu->File_length=pa->Pointer;
					break;
				}
				pu=pu->next;
			}
		}
		pa=pa->next;
	}
	char temp[30];
	strcpy(temp,User_name);
	strcat(temp,".txt");
	Save_MFD();
	Save_UFD(temp);
	Print_UFD();
	Destroy_Space();
}
 
void Print_Help()
{
	system("color 0b");
	cout<<"************************�����ļ�ϵͳ��ʾ*************************"<<endl;
	cout<<"*\t\t����			  ˵��			*"<<endl; 
	cout<<"*\t\tlogin			��¼ϵͳ		*"<<endl; 
	cout<<"*\t\tcreate			�����ļ�		*"<<endl; 
	cout<<"*\t\tdelete			ɾ���ļ�		*"<<endl; 
	cout<<"*\t\topen			���ļ�		*"<<endl; 
	cout<<"*\t\tclose			�ر��ļ�		*"<<endl; 
	cout<<"*\t\tread			��ȡ�ļ�		*"<<endl; 
	cout<<"*\t\twrite			д���ļ�		*"<<endl;
	cout<<"*\t\tls		        ��ʾĿ¼		*"<<endl;
	cout<<"*\t\trename			�����ļ�		*"<<endl;
	cout<<"*\t\thelp			�����˵�		*"<<endl;
	cout<<"*\t\tcls 			�����Ļ		*"<<endl;
	cout<<"*\t\tlogout 			�л��û�		*"<<endl;
	cout<<"*\t\tquit			�˳�ϵͳ		*"<<endl; 
	cout<<"*****************************************************************"<<endl; 
}
void System_Init()
{
Start:
	Print_Help();
	Init_Disk();
	if(!Init_MFD())
	{
		int num;
		cout<<"��Ŀ¼��δ�������Ƿ񴴽�<y/n>!!!"<<endl;
		cin>>flag;
		cout<<"���������������û��ļ��еĸ���(1-10)��"<<endl;
		cin>>num;
		if(flag=='y')
		{
			while(num--)
			{
				Create_MFD();
				if(!Max_User)
				{	
					cout<<"��ϵͳ,���֧��10���û�,���������û�ʧ��,��ɾ�������û�,��������!!!"<<endl;
					return ;
				}
			}
			Save_MFD();
			cout<<"��Ŀ¼�Ѿ��������,�û�����Ŀ¼�ѵõ�����,�Ƿ����<y/n>!!!"<<endl;
			cin>>flag;
			if(flag=='y')
				goto Start;
		}
	}
	return ;
}
 
 
void File_System()
{
	while(1)
	{
		string Command;
		cout<<"���������";
		cin>>Command;
		if(Command=="create")
			Create_UFD();
		else if(Command=="delete")
			Delete_UFD();
		else if(Command=="open")
			Open();
		else if(Command=="close")
			Close();
		else if(Command=="read")
			Read_File();
		else if(Command=="write")
			Write_File();
		else if(Command=="quit")
		{ 
			Quit_System();
			break;
		}
		else if(Command=="ls")
			Print_UFD();
		else if(Command=="cls")
			system("cls");
		else if(Command=="login")
		{
			if(!User_Check())
				break;
			Init_AFD();
		}
		else if(Command=="rename")
			Rename_File();
		else if(Command=="logout")
		{
			cout<<"�û��ǳ��ɹ�!!!"<<endl;
			Quit_System();
			System_Init();
			File_System();
		}
		else
			Print_Help();
	}
}
int main()   //�ұ��棬�Ҳ������ҵ�ָ����Ҳ���氡�����ǲ��Եİ�
{
	System_Init();
	File_System();
	return 0;
}