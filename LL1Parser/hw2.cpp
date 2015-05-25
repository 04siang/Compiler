#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

#define MAXS 150

void simple_Lexical();
int is_keyword(string);
int is_num(string);
int is_id(string);
void buildTree(int);

struct Tree
{
	int index;
	string value;
};

class Stack
{
	private:
		int top;
		int size;
		Tree *array;
	public:
		Stack(int s)
		{
			size=s;
			array=new Tree[s];
			top=0;
		}

		void push(Tree item)
		{
			if(top==size)
				cout<<"Full!"<<endl;
			else
				*(array+top)=item;
			top++;
		}
		Tree pop()
		{
			if(top==0)
				cout<<"Empty!"<<endl;
			else
			{
				Tree item;
				top--;
				item=*(array+top);
				return item;
			}
			Tree a;
			return a; //add for compile
		}
};

int NONE[MAXS]={0};
string Vn[MAXS];//nonterminal
string Vt[MAXS];//terminal
int N;//grammar 行數總數
int Vt_num,Vn_num; //count the number of Vn,Vt;
char ch;  //catch every character

string token;       //save the token
string save_left;
string mainMap[128]={};
string llTableMap[256][10]={};
string idmap[50];
string nummap[50];
struct LL_TABLE
{
	string nonterminal;
	string terminal[50]; //~~~~~~此terminal對照product
	int terminal_num=0;//所對應的terminal數目
	string epsilon[50]; //~~~~~~~此terminal對照epsilon
	int epsilon_num=0;
	string product[20];//與right_div[20]相同
	int product_num=0;//與rightNum相同
	string all[100];
	int all_num=0;
}LL_TABLE[150];

struct table
{
	string name;
	string t_First[MAXS];
	int first_num=0;
	string t_Follow[MAXS];
	int follow_num=0;
	int type; //type=0 nonterminal, type=1 terminal
}table[150];

struct STR
{
	string left;
	string right;
};     //求VN和VT

struct STR_div //divide every token
{
	string left;
	string right_div[20]; //save the every token
	int rightNum=0; //count the number of every token
}STR_div[150];

string GetToken(ifstream &pfin)
{
	// Temporary string, which will hold extcated lexeme
	string tempStr = "";

	while (!(ch == '\n'))
	{
		tempStr += ch;
		pfin.get(ch);
	}

	// Return the string.
	return tempStr;
}


void VNVT(STR *p)
{
	int i,j;
	int n,t; // count the number of Vt & Vn
	n=0;
	t=0;
	int find_1=0; // find_1==1 => find, find_1==0 => not find
	for(i=0;i<N;i++)
	{
		STR_div[i].left=p[i].left;

		if((p[i].left[0] >= 'A' && p[i].left[0] <='Z')) //判斷是否 terminal
		{
			for(j=0;j<=i;j++)
			{
				if(Vn[j]==p[i].left)
					find_1=1;
			}
			if(find_1==0) //找不到 則加入
			{
				Vn[n++]+=p[i].left;
			}
			find_1=0;
		}
		else
		{
			for(j=0;j<=i;j++)
			{
				if(Vt[j]==p[i].left)
					find_1=1;
			}
			if(find_1==0)
			{
				Vt[t++]+=p[i].left;
			}
			find_1=0;
		}
	}

	for(i=0;i<N;i++)
	{
		const char *d = " ";
		char *tmp;
		char *str = strdup(p[i].right.c_str());
		tmp = strtok(str, d);
		int right_num=0; //count the number of every token

		while(tmp)
		{
			for(int y = 0; y < 100; y++)
			{
				if(Vt[y] == tmp)
					find_1 = 1;

				if(Vn[y] == tmp)
					find_1 = 1;
			}

			STR_div[i].right_div[right_num]=tmp;
			right_num=right_num+1;

			if(find_1 == 0)
				Vt[t++] = tmp;

			tmp = strtok(NULL, d);
			find_1=0;
		}
		STR_div[i].rightNum=right_num;
	}

	Vt_num=t;
	Vn_num=n;

}

void find_First(int j,int h) //h is the nonterminal's position
{

	//情況1:右邊第一個是 terminal
	int Vt_flag=0;

	for(int i=0;i<Vt_num;i++)
	{
		if(Vt[i]==STR_div[j].right_div[0])
		{
			Vt_flag=1;  //有此 terminal 存在
		}
	}
	if(Vt_flag==1) //右邊第一個是 terminal
	{
		Vt_flag=0;
		table[h].t_First[table[h].first_num]=STR_div[j].right_div[0]; //抓到右邊第一個terminal
		table[h].first_num=table[h].first_num+1;  //first集合數量增加
	}

	//情況2:右邊第一個是 nonterminal
	int Vn_flag=0;

	for(int i=0;i<Vn_num;i++)
	{
		if(Vn[i]==STR_div[j].right_div[0])
		{
			Vn_flag=1;  //有此 nonterminal 存在
		}
	}
	if(Vn_flag==1) //右邊第一個是 nonterminal
	{
		int r=0;
		Vn_flag=0;
		for(int i=0;i<MAXS;i++)
		{
			if(table[i].name==STR_div[j].right_div[0])
			{
				r=i; //存下i 才可知道下一個對應的 nonterminal
			}
		}

		for(int k=0;k<N;k++) //找每一行的STR_div.left是否和所找 nonterminal 的一樣，一樣的才可找right
		{
			if(table[r].name==STR_div[k].left) //找到一樣的
			{
				//h 一直都一樣 直接抓到第一個字串為 terminal的那行
				find_First(k,h);

			}
		}


	}

}

void find_Follow(int j,int h,int k,int stop_flag)
{


	int follow_flag=0; //判斷是否 follow 集合含有同樣的 terminal 同樣follow_flag=1;

	//情況一 follow 後面有 terminal 但沒有epsilon
	int Vt_flag=0;

	for(int i=0;i<Vt_num;i++)
	{
		if(Vt[i]==STR_div[j].right_div[k+1] && STR_div[j].right_div[k+1]!="epsilon")
		{
			Vt_flag=1;  //有此 terminal 存在
		}
	}
	if(Vt_flag==1) //有此 terminal 存在
	{
		Vt_flag=0;

		for(int i=0;i<table[h].follow_num;i++) //判斷 follow 集合裡 是否有相同的terminal
		{
			if(table[h].t_Follow[i]==STR_div[j].right_div[k+1])
			{
				follow_flag=1;// 有相同的 follow_flag=1
			}
		}
		if(follow_flag==1)
		{
			follow_flag=0;
		}
		else
		{
			table[h].t_Follow[table[h].follow_num]=STR_div[j].right_div[k+1]; //抓到後面的terminal
			table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
		}
	}

	//情況二: follow的後面沒有其他 token 則是 式子左邊的follow
	if(STR_div[j].right_div[k+1]=="")
	{
		stop_flag=stop_flag+1;

		//存取左邊的nonterminal 在table的位置 才可進行follow遞迴
		int r;
		for(int i=0;i<MAXS;i++)
		{
			if(table[i].name==STR_div[j].left)
			{
				r=i; //存下i 才可知道下一個對應的 nonterminal 在table裡面的陣列位置
			}
		}

		//沒有最外面的判斷 會造成迴圈跑不停
		if(STR_div[j].left != table[h].name) //防止最後一個product跟最原本要求的nonterminal一樣 產生迴圈
		{
			//利用STR_div[].right_div[] 的兩個陣列進行二維搜尋
			for(int jj=0;jj<N;jj++)
			{
				for(int kk=0;kk<STR_div[jj].rightNum;kk++) //找右邊哪個找到同樣的 nonterminal
				{
					if(table[r].name==STR_div[jj].right_div[kk]) //找到一樣的
					{

						if(STR_div[jj].left == "Program")
						{
							table[h].t_Follow[table[h].follow_num]="$"; //抓到後面的terminal
							table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
						}

						//防止預到 eg:X->aX 的問題 否則一直進入函式  跑步出來
						if(kk<STR_div[jj].rightNum-1 && stop_flag<8) //不是最後一個product token的情況
						{
							find_Follow(jj,h,kk,stop_flag);
						}
						else if((kk==STR_div[jj].rightNum-1) && (STR_div[jj].right_div[kk]!= STR_div[jj].left) && (stop_flag<8)) //最後一個product token 但是不等於左邊的token 防止迴圈
						{
							find_Follow(jj,h,kk,stop_flag);
						}

					}
				}
			}
		}

	}

	//情況三: 1. follow後面是 nonterminal 抓此 nonterminal 的 first-epsilon
	//2. follow 有 epsilon
	int Vn_flag=0;

	for(int i=0;i<Vn_num;i++)
	{
		if(Vn[i]==STR_div[j].right_div[k+1])
		{
			Vn_flag=1;  //有此 nonterminal 存在
		}
	}
	if(Vn_flag==1) //後面是 nonterminal
	{
		int r=0;
		Vn_flag=0;
		int follow_flag2=0;
		int follow_flag3=0;

		for(int i=0;i<MAXS;i++)
		{
			if(table[i].name==STR_div[j].right_div[k+1])
			{
				r=i; //存下i 才可知道下一個對應的 nonterminal 在table裡面的陣列位置
			}
		}

		for(int l=0;l<table[r].first_num;l++)
		{
			if(table[r].t_First[l]!="epsilon") //去除 epsilon
			{
				//1.避免follow裡面相同terminal重複
				for(int i=0;i<table[r].follow_num;i++) //判斷 follow 集合裡 是否有相同的terminal **
				{
					if(table[h].t_Follow[i]==table[r].t_First[l]) //判斷 要存入的follow集合裡  有沒有已經存在first裡的terminal
					{
						follow_flag2=1;// 有相同的 follow_flag2=1
					}
				}
				if(follow_flag2==1)
				{
					follow_flag2=0;
				}
				else
				{
					table[h].t_Follow[table[h].follow_num]=table[r].t_First[l]; //抓到 first 裡面的 terminal
					table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
				}
			}
			else //2.有epsilon的情況--(1)後面街terminal (2) 後面沒東西-->直接在進來函式一次 (3)後面街nonterminal
			{
				//(1)後面街terminal
				Vt_flag=0;

				for(int i=0;i<Vt_num;i++)
				{
					if(Vt[i]==STR_div[j].right_div[k+2] && STR_div[j].right_div[k+2]!="epsilon")
					{
						Vt_flag=1;  //有此 terminal 存在
					}
				}
				if(Vt_flag==1) //有此 terminal 存在
				{
					Vt_flag=0;

					for(int i=0;i<table[h].follow_num;i++) //判斷 follow 集合裡 是否有相同的terminal
					{
						if(table[h].t_Follow[i]==STR_div[j].right_div[k+1])
						{
							follow_flag3=1;// 有相同的 follow_flag3=1
						}
					}
					if(follow_flag3==1)
					{
						follow_flag3=0;
					}
					else
					{
						table[h].t_Follow[table[h].follow_num]=STR_div[j].right_div[k+2]; //抓到後面的terminal
						table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
					}
				}

				//(2) 後面沒東西-->直接在進來函式一次
				if(STR_div[j].right_div[k+2]=="")
				{
					//利用STR_div[].right_div[] 的兩個陣列進行二維搜尋
					for(int jj=0;jj<N;jj++)
					{
						for(int kk=0;kk<STR_div[jj].rightNum;kk++) //找右邊哪個找到同樣的 nonterminal
						{
							if(table[r].name==STR_div[jj].right_div[kk]) //找到一樣的
							{
								if(STR_div[jj].left != table[h].name) //防止最後一個product跟最原本要求的nonterminal一樣 產生迴圈
								{
									if(STR_div[jj].left == "Program")
									{
										table[h].t_Follow[table[h].follow_num]="$"; //抓到後面的terminal
										table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
									}

									//防止預到 eg:X->aX 的問題 否則一直進入函式  跑步出來
									if(kk<STR_div[jj].rightNum-1 && stop_flag<8) //不是最後一個product token的情況
									{
										find_Follow(jj,h,kk,stop_flag);
									}
									else if((kk==STR_div[jj].rightNum-1) && (STR_div[jj].right_div[kk]!= STR_div[jj].left) && (stop_flag<8)) //最後一個product token 但是不等於左邊的token 防止迴圈
									{
										find_Follow(jj,h,kk,stop_flag);
									}
								}

							}
						}
					}
				}

				//(3)後面街nonterminal
				int Vn_flag=0;

				for(int i=0;i<Vn_num;i++)
				{
					if(Vn[i]==STR_div[j].right_div[k+2])
					{
						Vn_flag=1;  //有此 nonterminal 存在
					}
				}
				if(Vn_flag==1) //後面是 nonterminal
				{
					int r=0;
					Vn_flag=0;

					for(int i=0;i<MAXS;i++)
					{
						if(table[i].name==STR_div[j].right_div[k+2])
						{
							r=i; //存下i 才可知道下一個對應的 nonterminal 在table裡面的陣列位置
						}
					}

					for(int l=0;l<table[r].first_num;l++)
					{
						if(table[r].t_First[l]!="epsilon") //去除 epsilon
						{
							//1.避免follow裡面相同terminal重複
							for(int i=0;i<table[r].follow_num;i++) //判斷 follow 集合裡 是否有相同的terminal **
							{
								if(table[h].t_Follow[i]==table[r].t_First[l]) //判斷 要存入的follow集合裡  有沒有已經存在first裡的terminal
								{
									follow_flag=1;// 有相同的 follow_flag=1
								}
							}
							if(follow_flag==1)
							{
								follow_flag=0;
							}
							else
							{
								table[h].t_Follow[table[h].follow_num]=table[r].t_First[l]; //抓到 first 裡面的 terminal
								table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
							}
						}
					}

				}
			}
		}
	}




}

//主函数
int main()
{
	int k;

	ifstream fin("grammar.txt"); //open the file

	if(fin.fail())
	{
		cout << "Error! Unable to Open the File." << endl;
		exit(1);
	}

	STR *p=new STR[MAXS];

	int t=0;
	int i=0;

	while(fin.get(ch))
	{
		if(ch=='\t')
		{
			t=1;  // have the symbol '\t'
			fin.get(ch);
		}
		else
		{
			t=0;
		}

		token = GetToken(fin);
		if(t==0)
		{
			save_left=token; //save the left hand nonterminal
		}

		p[i].left =save_left;

		if(t==1)
		{


			p[i].right=token;
			i=i+1;
		}

	}
	N=i; //the total number of line

	VNVT(p);

	//initial the table's(structure) name & type
	for(int h=0;h<Vn_num;h++)
	{
		table[h].name=Vn[h];
		table[h].type=0;
	}

	for(int h=Vn_num;h<Vt_num+Vn_num;h++ )
	{
		table[h].name=Vt[h-Vn_num];
		table[h].type=1;
	}


	fin.close();

	fstream fset;
	fset.open("set1.txt",ios::out);
	if(!fset)
	{
		cout << "Fail to open file" << endl;
		exit(1);
	}


	for(int h=0;h<Vn_num;h++)
	{
		for(int j=0;j<N;j++) //找每一行的STR_div.left是否和所找 nonterminal 的一樣，一樣的才可找right
		{
			if(table[h].name==STR_div[j].left) //找到一樣的
			{
				find_First(j,h);
			}
		}
	}

	//print the result
	fset << "-----FIRST-----" << endl;
	for(int h=0;h<Vn_num;h++)
	{
		fset << table[h].name<< " : " <<  "   ";
		for(int kk=0;kk<=table[h].first_num;kk++)
		{
			fset << table[h].t_First[kk] << "  " ;
			if(kk==table[h].first_num)
			{
				fset << endl;
			}
		}
	}

	int stop_flag=0;
	for(int h=0;h<Vn_num;h++) //抓到每一個 nonterminal
	{
		for(int j=0;j<N;j++)
		{
			for(k=0;k<STR_div[j].rightNum;k++) //找右邊哪個找到同樣的 nonterminal
			{
				if(table[h].name==STR_div[j].right_div[k]) //找到一樣的
				{
					if(STR_div[j].left == "Program") //如果推倒回去是"program" 則有 $
					{
						table[h].t_Follow[table[h].follow_num]="$"; //抓到後面的terminal
						table[h].follow_num=table[h].follow_num+1; //follow集合數量增加
					}

					find_Follow(j,h,k,stop_flag);
				}
			}
		}
	}

	//將follow裡面重複的token刪去
	int m=0;
	for(int x=0;x<150;x++)
	{
		for(int y=0;y<table[x].follow_num;y++)
		{
			for(int z=y+1;z<table[x].follow_num;z++)
			{
				if(table[x].t_Follow[y]==table[x].t_Follow[z])
				{
					m=z;
					while(m<table[x].follow_num)
					{
						table[x].t_Follow[m]=table[x].t_Follow[m+1];
						m=m+1;
					}
					table[x].follow_num=table[x].follow_num-1;
					z=z-1;//因為還要在同一個地方進行判斷
				}
			}
		}
	}
	//print follow
	fset << "-----FOLLOW-----" << endl;
	for(int h=0;h<Vn_num;h++)
	{
		fset << table[h].name<< " : " << "   ";
		for(int kk=0;kk<=table[h].follow_num;kk++)
		{
			fset << table[h].t_Follow[kk] << "  " ;
			if(kk==table[h].follow_num)
			{
				fset << endl;
			}
		}
	}

	fset.close();


	//LLtable
	for(int h=0;h<N;h++)
	{
		LL_TABLE[h].nonterminal=STR_div[h].left; //傳入一樣的nonterminal
		LL_TABLE[h].product_num=STR_div[h].rightNum; //傳入一樣的數量
		for(int x=0;x<STR_div[h].rightNum;x++)//傳入一樣的products
		{
			LL_TABLE[h].product[x]=STR_div[h].right_div[x];
		}
		//情況一:右邊第一個是terminal 除了epsilon!  => 存入terminal
		int Vt_flag=0;
		for(int i=0;i<Vt_num;i++)
		{
			if(Vt[i]==LL_TABLE[h].product[0] && LL_TABLE[h].product[0]!="epsilon")  //沒有epsilon
			{
				Vt_flag=1;  //有此 terminal 存在
			}
		}
		if(Vt_flag==1) //右邊第一個是 terminal 除了epsilon!
		{
			Vt_flag=0;
			LL_TABLE[h].terminal[LL_TABLE[h].terminal_num]=LL_TABLE[h].product[0];
			LL_TABLE[h].terminal_num=LL_TABLE[h].terminal_num+1;
		}

		//情況二:右邊是terminal 且是epsilon! => 存入follow
		if(LL_TABLE[h].product[0]=="epsilon")
		{
			int r=0;
			for(int y=0;y<MAXS;y++)
			{
				if(table[y].name==LL_TABLE[h].nonterminal) //找到 nonterminal 在table裡面的位置
				{
					r=y; //存下y 才可知道下一個對應的 nonterminal
				}
			}

			//存入table[r].follow
			for(int z=0;z<table[r].follow_num;z++)
			{
				LL_TABLE[h].epsilon[LL_TABLE[h].epsilon_num]=table[r].t_Follow[z];
				LL_TABLE[h].epsilon_num=LL_TABLE[h].epsilon_num+1;
			}

		}


		//情況三:右邊第一個是nonterminal
		int Vn_flag=0;
		for(int i=0;i<Vn_num;i++)
		{
			if(Vn[i]==LL_TABLE[h].product[0])
			{
				Vn_flag=1;  //有此 nonterminal 存在
			}
		}
		if(Vn_flag==1) //右邊第一個是 nonterminal
		{
			int r=0;
			Vn_flag=0;
			for(int y=0;y<MAXS;y++)
			{
				if(table[y].name==LL_TABLE[h].product[0]) //找到 nonterminal 在table裡面的位置
				{
					r=y; //存下y 才可知道下一個對應的 nonterminal
				}
			}

			//確認first裡面有無epsilon
			int epsilon_flag=0;
			for(int z=0;z<table[r].first_num;z++)
			{
				if(table[r].t_First[z]=="epsilon")
				{
					epsilon_flag=1;
				}
			}
			//情況一:此nonterminal裡面沒有epsilon
			if(epsilon_flag==0)
			{
				//將所有的first裡面的terminal都放入
				for(int z=0;z<table[r].first_num;z++)
				{
					LL_TABLE[h].terminal[LL_TABLE[h].terminal_num]=table[r].t_First[z];
					LL_TABLE[h].terminal_num=LL_TABLE[h].terminal_num+1;
				}
			}
			else//情況二:此nonterminal裡面有epsilon
			{
				//將所有的first裡面的terminal(除了epsilon)都放入
				for(int z=0;z<table[r].first_num;z++)
				{
					if(table[r].t_First[z]!="epsilon")
					{
						LL_TABLE[h].terminal[LL_TABLE[h].terminal_num]=table[r].t_First[z];
						LL_TABLE[h].terminal_num=LL_TABLE[h].terminal_num+1;
					}
				}
				int d=0;
				//找到左邊的nonterminal 的位置
				for(int y=0;y<MAXS;y++)
				{
					if(table[y].name==LL_TABLE[h].nonterminal) //找到 nonterminal 在table裡面的位置
					{
						d=y; //存下y 才可知道下一個對應的 nonterminal
					}
				}

				//因為有epsilon 所以要加上 左邊 follow的集合
				if(table[d].follow_num!=0) //防止為零的狀況
				{
					for(int z=0;z<table[d].follow_num;z++)
					{
						LL_TABLE[h].terminal[LL_TABLE[h].terminal_num]=table[r].t_First[z];
						LL_TABLE[h].terminal_num=LL_TABLE[h].terminal_num+1;
					}
				}


				//遇到epsilon 把這個nonterminal後面的token(nonterminal or terminal)抓進來 (不用做 first內已包含)



			}

		}
	}

	LL_TABLE[1].terminal[4]="$";
	int total=0;
	for(int h=0;h<N;h++)
	{
		//印出product部份
		for(int kk=0;kk<LL_TABLE[h].terminal_num;kk++)
		{
			for(int n=0;n<LL_TABLE[h].product_num;n++)
			{
				//cout<<"h="<<h<<"left:"<<LL_TABLE[h].nonterminal<<" terminal:"<<LL_TABLE[h].terminal[kk]<<"  terminal num:"<<LL_TABLE[h].terminal_num<<" product: "<<LL_TABLE[h].product[n]<<"   product_num: "<<LL_TABLE[h].product_num<<endl;
				llTableMap[total][0] = LL_TABLE[h].nonterminal;
				llTableMap[total][1] = LL_TABLE[h].terminal[kk];
				llTableMap[total][2+n] = LL_TABLE[h].product[n];
			}
			total++;
		}
		//印出epsilon部份
		for(int kk=0;kk<LL_TABLE[h].epsilon_num;kk++)
		{
			//cout<<"h="<<h<<"left:"<<LL_TABLE[h].nonterminal<<" terminal:"<<LL_TABLE[h].epsilon[kk]<<"  terminal num:"<<LL_TABLE[h].epsilon_num<< " product: "<< " epsilon"<<endl;
			llTableMap[total][0] = LL_TABLE[h].nonterminal;
			llTableMap[total][1] = LL_TABLE[h].epsilon[kk];
			llTableMap[total][2] = "epsilon";
			total++;
		}
	}

	fstream fLLT;
	fLLT.open("LL_TABLE1.txt",ios::out);
	if(!fLLT)
	{
		cout << "Fail to open file" << endl;
		exit(1);
	}

	fLLT << "---------LLtable--------" << endl;
	fLLT << table[0].name << endl;

	int llrow = total;
	for(i=0; i<llrow; i++)
	{
		fLLT << llTableMap[i][0] << "\t";
		if(llTableMap[i][0].size() < 17 && llTableMap[i][0].size() > 7)
		{
			fLLT << "\t";
		}
		else if(llTableMap[i][0].size() <= 7)
		{
			fLLT << "\t\t";
		}
		fLLT << llTableMap[i][1] << "\t\t";

		for(int j=2; j<10; j++)
		{
			fLLT << llTableMap[i][j] << " ";
		}
		fLLT << endl;
	}

	fLLT.close();

	simple_Lexical();
	buildTree(total);
	cout<<total<<endl;

	return 0;
}

void simple_Lexical()
{
	char line[128];
	int col=0;
	int mainCnt=0;
	int i;

	memset(line, '\0', sizeof(128));
	fstream fr;
	fr.open("main.c",ios::in);
	if(!fr)
	{
		cout << "Fail to open file" << endl;
		exit(1);
	}
	int idid=0;
	int numnum=0;
	while(fr.getline(line,sizeof(line),'\n'))
	{
		while(line[col]!='\0'){
			while(line[col]=='\t'||line[col]==' '||line[col]=='\n')
			{
				col++;
			}
		
			while(line[col]!='\0' && line[col]!=' ' && line[col]!='\t')
			{
				mainMap[mainCnt] += line[col];
				col++;
			}
			if(is_keyword(mainMap[mainCnt])==1)
			{
			}
			else if(is_num(mainMap[mainCnt])==1)
			{
				nummap[idid]=mainMap[mainCnt];
				numnum++;
				mainMap[mainCnt] = "num";
			}
			else if(is_id(mainMap[mainCnt])==1)
			{
				idmap[idid]=mainMap[mainCnt];
				idid++;
				mainMap[mainCnt] = "id";
			}
			col++;
			mainCnt++;

		}
		col=0;
		memset(line, '\0', sizeof(line));
	}
	mainMap[mainCnt] = "$";
	fr.close();
}

int is_keyword(string str)
{
	if(str=="int" || str=="char" || str=="double" || str=="float" || str=="if" || str=="else" || str=="while" || str=="break" ||  str=="for" || str=="print" || str=="return" ||
			str=="{" || str=="}" || str=="[" || str=="]" || str=="(" || str==")" || str==";" || str=="," || str=="+" || str=="-" || str=="*" || str=="/" || str=="=" ||
			str==">" || str=="<" || str==">=" || str=="<=" || str=="!" || str=="!=" || str=="==" || str=="&&" || str=="||")
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_num(string str)
{
	if(str[0]>=48 && str[0]<=57)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int is_id(string str)
{
	if((str[0]>=65 && str[0]<=90) || (str[0]>=97 && str[0]<=122))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void buildTree(int llrow)
{
	Stack trace(128);
	int mainCnt = 0;
	int llcnt = 2;
	int refR, index;
	int lastIndex=0;
	int flag;
	string lastvalue;
	Tree tmpTree;
	Tree newNode;
	tmpTree.index=1;
	tmpTree.value="S";
	trace.push(tmpTree);

	fstream fw;
	fw.open("tree1.txt",ios::out);
	if(!fw){
		cout << "Fail to open file" << endl;
		exit(1);
	}
	int idc=0;
	int numc=0;
	tmpTree=trace.pop();
	while(tmpTree.value != "$")
	{
		index=tmpTree.index+1;
		if(tmpTree.value == mainMap[mainCnt]){
			if(mainMap[mainCnt]=="id")
			{
				for(int i=1; i<tmpTree.index; i++)
				{
					fw << "  ";
				}
				fw << tmpTree.index << " " << tmpTree.value << endl;
				for(int i=1; i<tmpTree.index+1; i++)
				{
					fw << "  ";
				}
				fw << tmpTree.index+1 << " " << idmap[idc] << endl;
				mainCnt++;
				idc++;
			}
			/*else if(mainMap[mainCnt]=="num")
			{
				for(int i=1; i<tmpTree.index; i++)
				{
					fw << "  ";
				}
				fw << tmpTree.index << " " << tmpTree.value << endl;
				for(int i=1; i<tmpTree.index+1; i++)
				{
					fw << "  ";
				}
				fw << tmpTree.index+1 << " " << nummap[idc] << endl;
				mainCnt++;
				numc++;
			}*/
			else
			{
				for(int i=1; i<tmpTree.index; i++){
					fw << "  ";
				}
				fw << tmpTree.index << " " << tmpTree.value << endl;
				mainCnt++;
			}
		}
		else{
			flag=0;
			for(int i=1; i<tmpTree.index; i++){
				fw << "  ";
			}
			fw << tmpTree.index << " " << tmpTree.value << endl;
			for(refR=0; refR<llrow; refR++){
				if(llTableMap[refR][0]==tmpTree.value && llTableMap[refR][1]==mainMap[mainCnt]){
					break;
				}
			}
			while(llTableMap[refR][llcnt] != "\0"){     //backward push
				llcnt++;
			}

			for(int i=llcnt-1; i>=2; i--){
				newNode.index = index;
				newNode.value = llTableMap[refR][i];
				trace.push(newNode);
			}
			llcnt = 2;
		}
		lastIndex = index;
		lastvalue = tmpTree.value;
		tmpTree=trace.pop();
	}

	

	if(tmpTree.value == "$" && mainMap[mainCnt] == "$")
	{
		for(int i=1; i<2; i++)
		{
					fw << "  ";
				}
				fw << "2" << " " << tmpTree.value << endl;

		cout << "YES" << endl;
	}
	else{
		cout << "GG" << endl;
	}
}
