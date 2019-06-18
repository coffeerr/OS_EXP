
#include<string.h>
#include<stdlib.h>
#include<iostream.h>
#include<iomanip.h>
 
#if _MSC_VER>1000
#pragma once
#endif	//_MSC_VER>1000
 
extern int disk_block[10000];
extern int disk_empty;
typedef struct UFD		//�洢�ļ���Ϣ
{
	char name[10];		//�ļ���
	int attribute;		//����
	int length;			//����
	int a[10];		//Ϊ�ļ��������10���ռ�
	int *p1;			//һ��������100���ռ�
	int (*p2)[100];		//����������100*100���ռ�
	struct UFD *next;
}UFD;
typedef struct DIR		//�洢Ŀ¼��Ϣ
{
	DIR* above;			//��һ���
	char name[10];
	int length;
	DIR *next;			//��һ���
	UFD *File_head;		//��Ŀ¼�µ��ļ�ָ��
	DIR *Dir_head;		//��Ŀ¼��Ŀ¼����ָ��
}DIR;
 
class Cuse				//��������û�Ŀ¼����
{
	DIR *now;			//��ǰĿ¼
	UFD *Fhead;			//�ļ���ͷ���
	DIR *Dhead;			//��Ŀ¼��Ŀ¼��ͷ���
	char code[10];		//����
	char name[10];		//�û���
	int length;			//�û��ռ��С
	int status;			//�Ƿ��ÿռ�
public:
	void set_status(int);
	int dele_user();
	int dis_file();		//��ʾ�ļ���ռ�����
	int dis_dir(DIR *d);//��ǰ·��
	int get_length();
	char const *get_name();
	char const *get_code();
	int get_status();
	int set_user(char *,char *);//�����û���������
	DIR *get_now();
	int dele_file(UFD *f);		//ɾ���ļ�
	int dele_dir(DIR*);			//ɾ��Ŀ¼
	Cuse();						//����
	~Cuse();					//����
	int goback();				//�����ϼ�
	int dis_now();				//��ʾ��ǰĿ¼
	int new_file();
	int new_dir();
	int open_dir();
	int open_file();
	int first_dele_file();		//ɾ���ļ���ǰ���ֹ���
	int first_dele_dir();		//ɾ��Ŀ¼��ǰ���ֹ���
	int set_code();
};
 
class Cdisk{					//�û���
public:
	Cuse user[5];		//�û��������Ϊ5
	char code[10];
	int dis_disk();
	int first_dele_user();
	int dele_user(int);
	int new_user();				//�鿴��ǰ�û������ռ�ʹ��������󴴽����û�
	int set_code();				//����������
	int login(char);				//��½
	Cdisk();
	virtual~Cdisk();			//�麯��������
};
 
int disk_block[10000];
int disk_empty;
Cdisk::Cdisk()		//������̵��࣬���캯��
{
	int i=0;
	char code[10]="123456";
	for(i=0;i<10000;i++)		//��ʼ�����д��̿�Ϊ����
		disk_block[i]=0;
	//this->user[0].set_user("student","123");//Ĭ��һ���û�
	disk_empty=10000;
	cout.setf(ios::left);	//���������ʽ
}
Cdisk::~Cdisk()		//����
{
}
 
int Cdisk::dele_user(int i)		//Cdisk��dele_user�Ĺ���
{
	Cuse C;
	C=user[i];
	user[i].dele_user();		//����Cuse��ĳ�Ա���� int dele_user()
	return 1;
}
 
int Cdisk::dis_disk()		//��������Ϣ
{
	int i=0;
	cout<<setw(14)<<"�û���"<<setw(14)<<"ռ�ÿռ��С"<<endl;
	for(i=0;i<5;i++)
		if(user[i].get_status()==1)		//���ڵ��û�����Ϣ
			cout<<setw(14)<<user[i].get_name()<<setw(14)<<user[i].get_length()<<endl;
		cout<<"���ÿռ䣺"<<10000-disk_empty<<endl<<"ʣ��ռ䣺"<<disk_empty<<endl;
		return 1;
}
int Cdisk::login(char b)		//��½
{
	char n[10],c[10];
	int i;
	if(b=='1')
	{
		cout<<"�û�������Ա"<<endl;
		cout<<"���룺Ĭ��\n"<<endl;
		system("pause");
		return 1;
	}
	else
	{
		if(!user[0].get_status())	//��ǰ�������û�
		{
			i=0;
			cout<<"��ǰ�û�Ϊ�գ���ӭע�ᣡ"<<endl;
			user[i].set_status(1);		//Ϊ���û�����Ȩ��
			cout<<"�������û�����"<<endl;
			cin>>n;
			cout<<"���������룺"<<endl;
			cin>>c;
			user[i].set_user(n,c);		//����Cuse�ĳ�Ա�����������û���������
 
			cout<<"��ϲ�������û��ɹ���"<<endl;
			return i;
		}
		else
		{
			cout<<"�û���:";
			cin>>n;
			cout<<"���룺";
			cin>>c;
			cout<<endl;
			for(i=0;i<5;i++)	//�����Ƿ���ڴ��û�
			{
				if(user[i].get_status())		//���ڷ��Ƚ�
					if(!strcmp(n,user[i].get_name()))	//���ʱΪ0�����ж�Ϊƥ��
						if(!strcmp(c,user[i].get_code()))		//����ƥ��
						{
							cout<<"��½�ɹ���"<<endl;
							cout<<"��ӭ"<<user[i].get_name()<<"��½"<<endl;
							return i;
						}
						else
						{
							cout<<"�������"<<endl;
							return -1;
						}
			}
			cout<<"���û������ڣ�"<<endl;
			return -1;
		}
	}
}
 
int Cdisk::set_code()		//����������
{
	char temp1[10],temp2[10];
	cout<<"������ԭ����"<<endl;
	cin>>temp1;
	if(strcmp(temp1,code))		//��������Ե�ǰ�û����в�������ֱ��code
	{
		cout<<"ԭ�������"<<endl;
		return 0;
	}
	while(1)
	{
		cout<<"�����������룺"<<endl;
		cin>>temp1;
		cout<<"���ٴ����������룺"<<endl;
		cin>>temp2;
		if(strcmp(temp1,temp2))
		{
			cout<<"�������벻��ͬ��������!"<<endl;
			break;
		}
		cout<<"�������óɹ���"<<endl;
		strcpy(code,temp1);		//����������
		break;
	}
	return 1;
}
 
int Cdisk::new_user()		//׼���������û�
{
	char n[10],c[10];
	int i=0,j;
	for(i=0;i<5;i++)
		if(user[i].get_status()==0)		//�Ƿ��д��û�������δ����
			break;
		if(i==5)
		{
			cout<<"�Ѿ��ﵽ����û�5���������ٴ�����"<<endl;
			return 0;
		}
		user[i].set_status(1);		//Ϊ���û�����Ȩ��
		cout<<"�������û�����"<<endl;
		cin>>n;
		if(i>0)			//���������û�����
		{
			for(j=0;j<i-1;j++)
				if(!strcmp(user[j].get_name(),n))
				{
					cout<<"���û����Ѵ��ڣ�"<<endl;
					return 0;
				}
		}
		cout<<"���������룺"<<endl;
		cin>>c;
		user[i].set_user(n,c);		//����Cuse�ĳ�Ա�����������û���������
		cout<<"��ϲ�������û��ɹ���"<<endl;
		return 1;
}
 
int Cdisk::first_dele_user()		//ɾ���û�
{
	char n[10],c;
	int i;
	cout<<"��������Ҫɾ�����û���"<<endl;
	cin>>n;
	for(i=0;i<5;i++)		//�ڲ����û���ͬʱ���õ��û����i
		if(!strcmp(user[i].get_name(),n)&&user[i].get_status())
			break;		//�ҵ�������
		if(i==5)
		{
			cout<<"�����������û������ڣ�"<<endl;
			return 0;
		}
		cout<<"ȷ��ɾ�����û���ȷ��Y��ȡ�������"<<endl;
		cin>>c;
		if(c!='Y'&&c!='y')
		{
			cout<<"�Ѿ�ȡ��ɾ����"<<endl;
			return 0;
		}
		this->dele_user(i);
		cout<<"�û�ɾ���ɹ�"<<endl;
		return 1;
}
 
Cuse::Cuse()		//���캯������ʼ����Ա
{
	status=0;		//�û�Ȩ�������Ƿ񱻴������
	length=0;		//�ռ�
	now=0;			//��ǰĿ¼
	Fhead=0;		//�ļ�
	Dhead=0;		//Ŀ¼
}
 
Cuse::~Cuse()		//�������������ռ�õ��ڴ�
{
	disk_empty+=length;
	length=0;
	UFD *f=Fhead;
	DIR *d=Dhead;
	while(f!=0)		//�ļ�
	{
		if(f->next==0)
		{
			this->dele_file(f);
			f=0;
			break;
		}
		while(f->next->next!=0)
			f=f->next;
		this->dele_file(f->next);
		f->next=0;
		f=Fhead;
	}
	while(d!=0)		//Ŀ¼
	{
		if(d->next==0)
		{
			this->dele_dir(d);
			d=0;
			break;
		}
		while(d->next->next!=0)
			d=d->next;
		this->dele_dir(d->next);
		d->next=0;
		d=Dhead;
	}
}
 
 
int Cuse::new_file()		//�������ļ�
{
	int i=0,j=0;
	UFD *f,*p=0;
	DIR *D;
 
	p=new UFD;				//����һ���µ��ļ��ṹ��
	if(p==0)
	{
		cout<<"�޿����ڴ�ռ䣬�����ļ�ʧ�ܣ�"<<endl;
		return 1;
	}
	cout<<"�����뽨�����ļ�����";
	cin>>p->name;
	if(now==0)			//��Ŀ¼�µ��ļ���
		f=Fhead;
	else				//��ǰĿ¼�µ��ļ���
		f=now->File_head;
	while(f!=0)			//����Ƿ��ļ�����
	{
		if(!strcmp(p->name,f->name))
		{
			cout<<"���ļ��Ѵ��ڣ�"<<endl;
			return 0;	//�˳�
		}
		f=f->next;
	}
	cout<<"\n"<<"���ȣ�";
	cin>>p->length;
	cout<<"\n"<<"����(0��ֻ����1����д)��";
	cin>>p->attribute;
	cout<<endl;
	if(p->length>disk_empty)		//�ռ䲻��
	{
		cout<<"�ļ�̫�󣬿ռ䲻�㣬��ǰ�ռ�Ϊ��"<<disk_empty<<endl;
		delete p;
		return 0;
	}
	disk_empty=disk_empty-p->length;	//ʣ������̿�
	//
	for(i=0;i<p->length&&i<10;i++)		//�ļ���Сʱ��ֱ�ӵ�ַ���ļ������̿��
		for(j;j<10000;j++)				//λʾͼ��
			if(disk_block[j]==0)		//����
			{
				p->a[i]=j;				//�õ��˿ռ�
				disk_block[j]=1;		//���Ϊ�ѷ����ȥ
				j++;
				break;					//����Ѱ�ң�Ϊ�ļ�������һ�ռ�
			}
			p->p1=0;					//һ������
			p->p2=0;					//��������
			if(p->length>10)			//����10����һ������
			{
				p->p1=new int[100];		//Ϊһ����������100���ռ�
				for(i=10;i<p->length&&i<110;i++)
					for(j;j<10000;j++)		//j������ǰ���ֵ
						if(disk_block[j]==0)
						{
							(p->p1)[i-10]=j;
							disk_block[j]=1;
							j++;
							break;
						}
						if(p->length>110)			//����110�����ö�������
						{
							p->p2=new int[100][100];	//��һ�������Ļ����ϣ�2ά
							for(i=110;i<p->length;i++)
								for(j;j<10000;j++)		//j������ǰ���ֵ
									if(disk_block[j]==0)
									{
										int m=(i-110)/100;		//��
										int k=(i-110)%100;		//��
										p->p2[m][k]=j;
										disk_block[j]=1;
										j++;
										break;
									}
						}
			}
			if(now==0)	//��Ŀ¼�µ��ļ�
			{
				p->next=Fhead;	//��̽��ָ��ͷ������ָ����ǰ
				Fhead=p;	//�½����ͷ
			}
			else
			{
				p->next=now->File_head;
				now->File_head=p;
			}
			length+=p->length;	//�û��ܿռ��С����
			if(now!=0)			//��Ŀ¼�¿ռ��С����
			{
				now->length+=p->length;
				D=now->above;	//��һ��Ŀ¼
				while(D!=0)		//�ϼ�Ŀ¼����Ŀ¼��ʵ�֣��ռ�����
				{
					D->length+=p->length;
					D=D->above;		//������
				}
			}
 
			return 0;
}
 
int Cuse::new_dir()		//������Ŀ¼
{
	DIR *p,*h;
	cout<<"��������Ŀ¼�����֣�"<<endl;
	p=new DIR;
	cin>>p->name;		//Ŀ¼��
	p->Dir_head=0;		//Ŀ¼�µ�Ŀ¼��Ϊ��
	p->length=0;		//
	p->File_head=0;		//Ŀ¼�µ��ļ�Ϊ��
	if(now==0)			//��ǰΪ��Ŀ¼
		h=Dhead;	//��һ��ʱ��h=0��ָ��Ŀ¼��
	else
		h=now->Dir_head;//��ǰ��Ŀ¼��
	while(h!=0)	//��Ŀ¼�´���������Ŀ¼
	{
		if(!strcmp(h->name,p->name))
		{
			cout<<"��Ŀ¼�Ѵ��ڣ�"<<endl;
			return 0;
		}
		h=h->next;
	}
	if(now==0)		//��ǰΪ��Ŀ¼
	{
		p->above=0;		//��Ŀ¼��Ŀ¼����һ���Ϊ0
		p->next=Dhead;		//��ԭĿ¼������Ŀ¼���棨Dhead��ʼΪ0��
		Dhead=p;		//Dheadʼ��ָ������Ŀ¼���
	}
	else
	{
		p->above=now;		//��ǰĿ¼Ϊ��Ŀ¼����һ���
		p->next=now->Dir_head;	//���������Ŀ¼
		now->Dir_head=p;	//����Ŀ¼��
	}
	cout<<"Ŀ¼�����ɹ�"<<endl;
	return 1;
}
 
int Cuse::goback()		//���Ϸ���
{
	if(now==0)
	{
		cout<<"������Ŀ¼���������ϣ�"<<endl;
		return 0;
	}
	now=now->above;		//��һ���
	return 1;
}
 
int Cuse::open_dir()	//��Ŀ¼
{
	char name[10];
	DIR *p;
	if(now==0)	//��ǰ��Ŀ¼
		p=Dhead;
	else
		p=now->Dir_head;	//pָ��Ŀ¼��
	cout<<"��������Ҫ�򿪵�Ŀ¼����"<<endl;
	cin>>name;
	//int flag=0;
	while(p!=0)
	{
		if(strcmp(p->name,name)==0)
		{
			now=p;	//�ҵ�Ŀ¼��now���
			return 1;
		}
		p=p->next;
	}
	cout<<"��ǰû��Ŀ¼"<<endl;
	return 0;
}
 
int Cuse::first_dele_file()	//ɾ���ļ���ǰ�湤��
{
	char temp[10],a[5];
	cout<<"��Ҫɾ�����ļ�����"<<endl;
	cin>>temp;
	UFD *f=Fhead;	//�����ļ�ͷָ��
	UFD *above=0;
	if(now!=0)
		f=now->File_head;	//��ǰĿ¼�µ������ļ�
	while(f!=0)
	{
		if(!strcmp(f->name,temp))
			break;	//�ҵ�������
		above=f;	//��ǵ�һ���ļ�
		f=f->next;
	}
	if(f==0)
	{
		cout<<"���ļ�������"<<endl;
		return 0;
	}
	cout<<"ȷ��ɾ��"<<f->name<<"�ļ��𣿰�0ȷ����������ȡ��"<<endl;
	cin>>a;
	if(a[0]!='0')
	{
		cout<<"��ȡ��ɾ���ļ���"<<endl;
		return 0;
	}
	disk_empty+=f->length;	//���մ������ļ��Ŀռ��С
	if(now==0)	//��Ŀ¼
	{
		if(f==Fhead)
			Fhead=Fhead->next;
		else
			above->next=f->next;
	}
	else
	{
		DIR *d=now;
		while(d!=0)
		{
			d->length=f->length;
			d=d->above;
		}
		if(f==now->File_head)
			now->File_head=now->File_head->next;
		else
			above->next=f->next;
	}
	length-=f->length;
	this->dele_file(f);
	cout<<"ɾ���ɹ�"<<endl;
	return 1;
}
 
int Cuse::dele_file(UFD *f)	//����ʵ��ɾ���ļ�
{
	int i=0,m;
	for(i=0;i<10&&i<f->length;i++)	//�����ļ�����ռ䣬���ÿ���
	{
		m=f->a[i];
		disk_block[m]=0;
	}
	if(f->p1!=0)	//����һ������
	{
		for(i=10;i<110&&i<f->length;i++)
		{
			m=f->p1[i-10];
			disk_block[m]=0;
		}
		delete[](f->p1);
	}
	if(f->p2!=0)	//���ն�������
	{
		for(i=110;i<f->length;i++)
		{
			m=(f->p2)[(i-110)/100][(i-110)%100];
			disk_block[m]=0;
		}
		delete[](f->p2);
		delete f;
	}
	f=0;	//Ҫɾ�����ļ�Ϊ����
	return 1;
}
 
int Cuse::first_dele_dir()	//ɾ��Ŀ¼��ǰ��
{
	char n[10];
	char c;
	DIR *p,*above=0;
	p=Dhead;	//ָ���Ŀ¼�µ�Ŀ¼��
	if(now!=0)
		p=now->Dir_head;	//ָ��ǰĿ¼�µ�Ŀ¼��
	cout<<"Ҫɾ����Ŀ¼����"<<endl;
	cin>>n;
	while(p!=0)
	{
		if(!strcmp(p->name,n))
			break;	//�ҵ�Ҫɾ����Ŀ¼������
		above=p;	//����ǰһ���
		p=p->next;
	}
	if(p==0)
	{
		cout<<"û�����Ŀ¼��"<<endl;
		return 0;
	}
	cout<<"ȷ��ɾ����ǰĿ¼����Ŀ���ϵ�������Ϣ�𣿰�0ȷ����������ȡ��"<<endl;
	cin>>c;
	if(c!='0')
		return 0;
	disk_empty+=p->length;	//���մ��̿ռ�
	if(now==0)
	{
		if(p==Dhead)	//����Ǹ�Ŀ¼��ͷ��㣬ֱ���ƶ�Dhead
			Dhead=Dhead->next;
		else
			above->next=p->next;//�м���Ŀ��
	}
	else
	{
		if(p==now->Dir_head)
			now->Dir_head=now->Dir_head->next;
		else
			above->next=p->next;
		above=now;	//��ǰĿ¼
		while(above!=0)
		{
			above->length-=p->length;//��above��ǵ�ǰĿ¼�����д�С����
			above=above->above;	//����һ��
		}
	}
	length-=p->length;	//��Ŀ¼��С����
	this->dele_dir(p);
	p=0;
	cout<<"ɾ���ɹ���"<<endl;
	return 1;
}
 
int Cuse::dele_dir(DIR *p)	//����ʵ��ɾ��Ŀ¼�Ĺ���
{
	int flag=0;
	DIR *d=p->Dir_head;	//��ǰĿ¼�µ�Ŀ¼����
	UFD *f=p->File_head;	//��ǰĿ¼�µ��ļ���
	if(f!=0)
	{
		while(p->File_head->next!=0)//ɾ��Ŀ¼����ļ�
		{
			f=p->File_head;
			while(f->next->next!=0)
				f=f->next;
			this->dele_file(f->next);
			f->next=0;
		}
		if(p->File_head->next==0)
		{
			this->dele_file(p->File_head);//ɾ��ͷ�ļ�
			p->File_head=0;
		}
	}
	if(d!=0)	//ɾ��Ŀ¼�µ�Ŀ¼
	{
		while(p->Dir_head->next!=0)
		{
			d=p->Dir_head;
			while(d->next->next!=0)
				d=d->next;
			this->dele_dir(d->next);
			d->next=0;
		}
		if(p->Dir_head->next==0)
		{
			this->dele_dir(p->Dir_head);//ɾ��Ŀ¼��ͷ���
			p->Dir_head=0;
		}
	}
	delete p;	//�ͷ�pռ�õ��ڴ�
	p=0;		//��0
	return 1;
}
 
int Cuse::dis_now()	//��ʾ��ǰĿ¼
{
	DIR *d=Dhead;
	UFD *f=Fhead;
	if(now!=0)
	{
		d=now->Dir_head;	//��ǰĿ¼�µ�Ŀ¼��
		f=now->File_head;
	}
	if(d==0&&f==0)
	{
		cout<<"��ǰĿ¼Ϊ��"<<endl;
		return 0;
	}
	cout<<"��ǰĿ¼��С��";
	if(now==0)
		cout<<length;
	else
		cout<<now->length;
	cout<<endl;
	if(d==0)
		cout<<"��ǰĿ¼��û��Ŀ¼"<<endl;
	else
	{
		cout<<"��ǰĿ¼�°�������Ŀ¼��"<<endl;
		cout<<setw(14)<<"Ŀ¼����"<<setw(14)<<"Ŀ¼��С"<<endl;
		while(d!=0)
		{
			cout<<setw(14)<<d->name<<setw(14)<<d->length<<endl;
			d=d->next;
		}
	}
	if(f==0)
		cout<<"��ǰĿ¼��û���ļ�"<<endl;
	else
	{
		cout<<"��ǰĿ¼�°��������ļ�:"<<endl;
		cout<<setw(14)<<"�ļ�����"<<setw(14)<<"�ļ���С"<<setw(14)<<"�ļ�����"<<endl;
		while(f!=0)
		{
			cout<<setw(14)<<f->name<<setw(14)<<f->length<<setw(14)<<f->attribute<<endl;
			f=f->next;
		}
	}
	return 1;
}
 
int Cuse::open_file()	//���ļ�
{
	char n[10];
	cout<<"������Ҫ�򿪵��ļ���"<<endl;
	cin>>n;
	UFD *f=Fhead;	//�ļ�ͷָ��
	if(now!=0)
		f=now->File_head;	//ָ���ļ�
	while(f!=0)
	{
		if(!strcmp(f->name,n))
		{
			cout<<"�ļ��򿪳ɹ�"<<endl;
			return 1;
		}
		f=f->next;
	}
	cout<<"��ǰĿ¼�޴��ļ�"<<endl;
	return 0;
}
 
int Cuse::set_code()	//��������
{
	char a1[10],a2[10];
	cout<<"������ԭ����"<<endl;
	cin>>a1;
	if(strcmp(a1,code))
	{
		cout<<"�������"<<endl;
		return 0;
	}
	while(1)
	{
		cout<<"�����������룺"<<endl;
		cin>>a1;
		cout<<"�ٴ�����������:"<<endl;
		cin>>a2;
		if(strcmp(a1,a2))
			cout<<"�����������벻ͬ�������䣡"<<endl;
		else
		{
			strcpy(code,a1);
			cout<<"�����޸ĳɹ���"<<endl;
			break;
		}
	}
	return 1;
}
 
DIR *Cuse::get_now()	//�õ���ǰĿ¼·��
{
	return now;
}
 
int Cuse::set_user(char *n,char *c)//�����û�������
{
	strcpy(name,n);
	strcpy(code,c);
	status=1;
	return 1;
}
 
void Cuse::set_status(int b)//��Ƿ���
{
	status=b;
}
 
int Cuse::get_status()//̽ȡ�Ƿ����
{
	return status;
}
 
const char* Cuse::get_code()//�õ�����
{
	return code;
}
 
const char* Cuse::get_name()//�õ�����
{
	return name;
}
 
int Cuse::get_length()//�õ�����
{
	return length;
}
 
int Cuse::dis_dir(DIR *d)//��ʾĿ¼
{
	if(d==0)
		return 0;
	if(d->above!=0)
		this->dis_dir(d->above);//�ݹ���ô˹���
	cout<<"	"<<d->name<<'\n';
	return 0;
}
 
int Cuse::dis_file()//�鿴�ļ�
{
	int i;
	char n[10];
	UFD *f=Fhead;
	if(now!=0)
		f=now->File_head;
	cout<<"������Ҫ�鿴���ļ�����"<<endl;
	cin>>n;
	while(f!=0)
	{
		if(!strcmp(n,f->name))	//�ҵ����ļ�������
			break;
		f=f->next;
	}
	if(f==0)
	{
		cout<<"��ǰĿ¼��û���ļ�"<<endl;
		return 0;
	}
	if(f->attribute==0)
		cout<<"��Ϊֻ���ļ���";
	else
		cout<<"��Ϊ��д�ļ���";
	cout<<"ռ��Ӳ�̿�����£�"<<endl;
	for(i=0;i<f->length&&i<10;i++)		//ֱ�Ӵ�ŵ�
	{
		cout<<setw(6)<<f->a[i];
		if((i+1)%10==0)
			cout<<endl;
	}
	for(i=10;i<f->length&&i<110;i++)	//һ��������ŵ�
	{
		cout<<setw(6)<<f->p1[i-10];
		if((i+1)%10==0)
			cout<<endl;
	}
	for(i=110;i<f->length;i++)	//����������ŵ�
	{
		cout<<setw(6)<<f->p2[(i-110)/100][(i-110)%100];
		if((i+1)%10==0)
			cout<<endl;
	}
	cout<<endl;
	return 1;
}
 
int Cuse::dele_user()//ɾ���û�
{
	length=0;	//�û��ռ���0
	Fhead=0;
	Dhead=0;
	now=0;
	status=0;
	return 1;
}
 
 
void main()
{
	char c;
	Cdisk D;		//����Ա��Ķ���
	int i=1,n,flag=1,t=0;
 
	while(flag)
	{
		cout<<"*******************��ӭʹ���ļ�ϵͳ**************************"<<endl;
		cout<<endl;
		cout<<"***                 1.����Ա��½"<<endl;
		cout<<"***                 2.�û���½"<<endl;
		cout<<"***                 3.�˳�ϵͳ"<<endl;
		cout<<endl;
		cout<<"*************************************************************"<<endl;
 
		cout<<"\n������ѡ��";
		cin>>c;
		switch(c)
		{
		case '1':
			n=D.login(c);
			flag=1;
			system("cls");
			cout<<"����Ա��½�ɹ���"<<endl;
			while(flag)
			{
				cout<<"*************************************************************"<<endl;
				cout<<"***1.�����û�"<<endl;
				cout<<"***2.ɾ���û�"<<endl;
				cout<<"***3.�鿴��ǰ�û�"<<endl;
				cout<<"***4.�޸�����"<<endl;
				cout<<"***5.���ص�½����"<<endl;
				cout<<"*************************************************************"<<endl;
				cout<<"��ѡ��";
				cin>>c;
				cout<<endl;
				switch(c){
				case '1':
					D.new_user();
					break;
				case '2':
					D.first_dele_user();
					break;
				case '3':
					D.dis_disk();
					break;
				case '4':
					{
						if(t==0)	//t����ǣ���ֹ�ظ������������
							strcpy(D.code,"123");
						D.set_code();
						t++;
						break;
					}
				case '5':
					{
						flag=0;
						system("cls");
					}
					break;
				default:
					cout<<"������1-5��"<<endl;
				}
			}
			flag=1;
			break;
		case '2':
			n=D.login(c);
			if(n==-1)
				break;
			while(flag)
			{
				cout<<"*************************************************************"<<endl;
				cout<<"***1.�����ļ�"<<endl;
				cout<<"***2.ɾ���ļ�"<<endl;
				cout<<"***3.����Ŀ¼"<<endl;
				cout<<"***4.ɾ��Ŀ¼"<<endl;
				cout<<"***5.��Ŀ¼"<<endl;
				cout<<"***6.������һ��Ŀ¼"<<endl;
				cout<<"***7.�鿴��ǰĿ¼"<<endl;
				cout<<"***8.�޸�����"<<endl;
				cout<<"***9.�鿴�ļ�"<<endl;
				cout<<"***0.��ȫ�˳�"<<endl;
				cout<<"*************************************************************"<<endl;
				cout<<"�û���"<<D.user[n].get_name()<<'\n'<<"Ŀ¼Ϊ:\n	root\n";
				D.user[n].dis_dir(D.user[n].get_now());
				cout<<endl;
				cout<<"������ѡ��";
				cin>>c;
				cout<<endl;
				switch(c)
				{
				case '1':
					D.user[n].new_file();
					break;
				case '2':
					D.user[n].first_dele_file();
					break;
				case '3':
					D.user[n].new_dir();
					break;
				case '4':
					D.user[n].first_dele_dir();
					break;
				case '5':
					D.user[n].open_dir();
					break;
				case '6':
					D.user[n].goback();
					break;
				case '7':
					D.user[n].dis_now();
					break;
				case '8':
					D.user[n].set_code();
					break;
				case '9':
					D.user[n].dis_file();
					break;
				case '0':
					{
						flag=0;
						system("cls");
					}
					break;
				default:
					cout<<"������0-9"<<endl;
				}
			}
			flag=1;
			break;
		case '3':
			flag=0;
			break;
		default:
			cout<<"������1-3��"<<endl;
			}
		}
}