#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include"AI.h"
#define ll long long
using namespace std;

/*enum GameType {
  Winner,  // 争上游
  Hearts   // 红心大战
};
enum Suit {
  Club,     // 梅花
  Diamond,  // 方块
  Heart,    // 红桃
  Spade     // 黑桃
};
struct Card {
  Suit suit;
  // 1 for A, 11 for J, 12 for Q, 13 for K, 14 for small Joker, 15 for the big
  // Joker, 16 for unknown
  unsigned short rank;
};
class card_list
{
	public:
		unsigned short size;
		Card* cards;
};//*/
bool If_Card(const card_list yours,int x,int y)
{
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x&&temp.rank==y) 
		{
			return 1;
		}
	}
	return 0;
}
short Find_Card(const card_list yours,int x,int y)
{
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x&&temp.rank==y) return i;
	}
	return -1;
}
short Count_Suit(const card_list yours,int x)
{
	short ans=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x) ans++;
	}
	return ans;
}
short Trans(const Card x)
{
	return x.suit*13+x.rank-1;
}
short Smallest_Rank(const card_list yours,int x)
{
	int ans=14;bool _=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x)
		{
			_=1;
			ans=min(ans,1*temp.rank);
		}
	}
	if(_==0) return -1;
	return ans;
}
short Largest_Rank(const card_list yours,int x)
{
	int ans=0;bool _=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x)
		{
			_=1;
			ans=max(ans,1*temp.rank);
		}
	}
	if(_==0) return -1;
	return ans;
}
short Special_Largest_Rank(const card_list yours,int x)
{
	int ans=0;bool _=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x&&temp.rank<11)
		{
			_=1;
			ans=max(ans,1*temp.rank);
		}
	}
	if(_==0) return -1;
	return ans;
}
short Special_Largest_Rank2(const card_list yours,int x)
{
	int ans=0;bool _=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==x&&temp.rank!=11)
		{
			_=1;
			ans=max(ans,1*temp.rank);
		}
	}
	if(_==0) return -1;
	return ans;
}
short Count_Smaller(const bool *state,const int x,const int point)
{
	short ans=0;
	for(int i=x*13;i<x*13+point-1;i++)
	{
		if(state[i]) ans++;
	}
	return ans;
}
short Count_Larger(const bool *state,const int x,const int point)
{
	short ans=0;
	for(int i=x*13+point;i<(x+1)*13;i++)
	{
		if(state[i]) ans++;
	}
	return ans;
}
enum Card_type
{
	one,two,three,four,rocket,three_two,dragon,four_two,plane
};
class Type
{
	public:
	int rank;
	int type;
	virtual unsigned short* play_card(card_list yours,unsigned short &size){}
	virtual int impossible_better(int *num){}//1 - impossible,0 - possible if have bomb, -1 possible if don't have bomb
	virtual int check_type(){return 0;}
	virtual int getlast(){return 0;} 
	virtual int getother(){}
	virtual int getlength(){}
};
bool if_bomb(int *num)
{
	for(int i=1;i<=13;i++)
	{
		if(num[i]==4) return 1;
	}
	if(num[14]==1&&num[15]==1) return 1;
	return 0;
}
class One:public Type
{
	public:
		One(int rank)
		{
			this->rank=rank;
			this->type=one;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short;
			int pur;
			if(rank==12) pur=1;
			else if(rank==13) pur=2;
			else if(rank<12) pur=rank+2;
			else pur=rank;
			for(int i=0;i<yours.size;i++)
			{
				if(yours.cards[i].rank==pur)
				{
					temp[0]=i;
					size=1;
					return temp;
				}
			}
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=15;i++)
			{
				if(num[i])
				{
					ok=0;
					break;
				}
			}
			if(!ok) return -1;
			if(if_bomb(num)) return 0;
			return 1;
		}
};
class Two:public Type
{
	public:
		Two(int rank)
		{
			this->rank=rank;
			this->type=two;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			
			unsigned short *temp=new unsigned short [2];
			size=2;
			int pur,cnt=-1;
			if(rank==12) pur=1;
			else if(rank==13) pur=2;
			else if(rank<12) pur=rank+2;
			for(int i=0;i<yours.size;i++)
			{
				if(yours.cards[i].rank==pur)
				{
					
					temp[++cnt]=i;
					if(cnt==1) return temp;
				}
			}
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=15;i++)
			{
				if(num[i]>=2)
				{
					ok=0;
					break;
				}
			}
			if(!ok) return -1;
			if(if_bomb(num)) return 0;
			return 1;
		}
};
class Three:public Type
{
	public:
		Three(int rank)
		{
			this->rank=rank;
			this->type=three;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [3];
			size=3;
			int pur,cnt=-1;
			if(rank==12) pur=1;
			else if(rank==13) pur=2;
			else if(rank<12) pur=rank+2;
			for(int i=0;i<yours.size;i++)
			{
				if(yours.cards[i].rank==pur)
				{
					temp[++cnt]=i;
					if(cnt==2) return temp;
				}
			}
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=15;i++)
			{
				if(num[i]>=3)
				{
					ok=0;
					break;
				}
			}
			if(!ok) return -1;
			if(if_bomb(num)) return 0;
			return 1;
		}
};
class Four:public Type
{
	public:
		Four(int rank)
		{
			this->rank=rank;
			this->type=four;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [4];
			size=4;
			int pur,cnt=-1;
			if(rank==12) pur=1;
			else if(rank==13) pur=2;
			else if(rank<12) pur=rank+2;
			for(int i=0;i<yours.size;i++)
			{
				if(yours.cards[i].rank==pur)
				{
					temp[++cnt]=i;
					if(cnt==3) return temp;
				}
			}
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=15;i++)
			{
				if(num[i]>=4)
				{
					ok=0;
					break;
				}
			}
			if(!ok) return 0;
			if(num[14]==1&&num[15]==1) return 0;
			return 1;
		}
};
class Rocket:public Type
{
	public:
		Rocket(int rank=20)
		{
			this->rank=rank;
			this->type=rocket;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [2];
			size=2;
			int cnt=-1;
			for(int i=0;i<yours.size;i++)
			{
				if(yours.cards[i].rank==14||yours.cards[i].rank==15)
				{
					temp[++cnt]=i;
					if(cnt==1) return temp;
				}
			}
		}
		int impossible_better(int *num)
		{
			return 1;
		}
};
class Three_Two:public Type
{
	public:
		int other;
		Three_Two(int rank,int other)
		{
			this->rank=rank;
			this->other=other;
			this->type=three_two;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [5];
			size=5;
			int pur_num[17],cnt=-1;
			memset(pur_num,0,sizeof(pur_num));
			if(rank<=11) pur_num[rank+2]=3;
			else if(rank<=13) pur_num[rank-11]=3;
			if(other<=11) pur_num[other+2]=2;
			else if(other<=13) pur_num[other-11]=2;
			for(int i=0;i<yours.size;i++)
			{
				if(pur_num[yours.cards[i].rank])
				{
					pur_num[yours.cards[i].rank]--;
					temp[++cnt]=i;
				}
			}
			return temp;
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=15;i++)
			{
				if(num[i]>=3)
				{
					for(int j=1;j<=13;j++)
					{
						if(i==j) continue;
						if(num[j]>=2)
						{
							ok=0;
							break;
						}
					}
					if(!ok) break;
				}
			}
			if(!ok) return -1;
			if(if_bomb(num)) return 0;
			return 1;
		}
		int getother()
		{
			return other;
		}
};
class Dragon:public Type
{
	public:
		int length;
		int width;
		Dragon(int width,int length,int rank)
		{
			this->width=width;
			this->length=length;
			this->rank=rank;
			this->type=dragon;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [width*length];
			size=width*length;
			int pur_num[17],cnt=-1;
			memset(pur_num,0,sizeof(pur_num));
			for(int i=rank;i<=rank+length-1;i++)
			{
				if(i<=11) pur_num[i+2]=width;
				else if(i<=13) pur_num[i-11]=width;
			}
			for(int i=0;i<yours.size;i++)
			{
				if(pur_num[yours.cards[i].rank])
				{
					pur_num[yours.cards[i].rank]--;
					temp[++cnt]=i;
				}
			}
			return temp;
		}
		int impossible_better(int *num)
		{
			bool ok=1;
			for(int i=rank+1;i<=12;i++)
			{
				if(i+length-1>12) break;
				bool if_dragon=1;
				for(int j=i;j<=i+length-1;j++)
				{
					if(num[j]<width)
					{
						if_dragon=0;
						break;
					}
				}
				if(if_dragon)
				{
					ok=0;
					break;
				}
			}
			if(!ok) return -1;
			if(if_bomb(num)) return 0;
			return 1;
		}
		int check_type()
		{
			return width;
		}
		int getlast()
		{
			return rank+length-1;
		}
		int getlength()
		{
			return length;
		}
};
class Four_Two:public Type
{
	public:
		int other1,other2;
		Four_Two(int rank,int other1,int other2)
		{
			this->rank=rank;
			this->other1=other1;
			this->other2=other2;
			this->type=four_two;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [6];
			size=6;
			int pur_num[17],cnt=-1;
			memset(pur_num,0,sizeof(pur_num));
			if(rank<=11) pur_num[rank+2]=4;
			else if(rank<=13) pur_num[rank-11]=4;
			if(other1<=11) pur_num[other1+2]++;
			else if(other1<=13) pur_num[other1-11]++;
			else pur_num[other1]++;
			if(other2<=11) pur_num[other2+2]++;
			else if(other2<=13) pur_num[other2-11]++;
			else pur_num[other2]++;
			for(int i=0;i<yours.size;i++)
			{
				if(pur_num[yours.cards[i].rank])
				{
					pur_num[yours.cards[i].rank]--;
					temp[++cnt]=i;
				}
			}
			return temp;
		}
		int impossible_better(int *num)
		{
			if(if_bomb(num)) return 0;
			return 1;
		}
};
class Plane:public Type
{
	public:
		int other[7];
		int length;
		Plane(int rank,int length,int *a)
		{
			this->rank=rank;
			this->length=length;
			for(int i=1;i<=length;i++)
			{
				other[i]=a[i];
			}
			this->type=plane;
		}
		unsigned short* play_card(card_list yours,unsigned short &size)
		{
			unsigned short *temp=new unsigned short [5*length];
			size=5*length;
			int pur_num[17],cnt=-1;
			memset(pur_num,0,sizeof(pur_num));
			for(int i=rank;i<=rank+length-1;i++)
			{
				if(i<=11) pur_num[i+2]=3;
				else if(i<=13) pur_num[i-11]=3;
			}
			for(int i=1;i<=length;i++)
			{
				if(other[i]<=11) pur_num[other[i]+2]++;
				else if(other[i]<=13) pur_num[other[i]-11]++;
				else pur_num[other[i]]++;
			}
			for(int i=0;i<yours.size;i++)
			{
				if(pur_num[yours.cards[i].rank])
				{
					pur_num[yours.cards[i].rank]--;
					temp[++cnt]=i;
				}
			}
			return temp;
		}
};
bool cmp(const Type *x,const Type *y)
{
	return x->rank<y->rank;
}
static int min_round,max_bomb;
static Type *result[17],*min_result[17];
void calc_round(int *b,int tot)
{
	int a[17],bomb=0;
	for(int i=1;i<=15;i++) a[i]=b[i];
	if(a[14]==1&&a[15]==1)
	{
		result[++tot]=new Rocket();
		bomb++;
		a[14]--;
		a[15]--;
	}
	for(int i=1;i<=13;i++)
	{
		if(a[i]==4)
		{
			a[i]-=4;
			result[++tot]=new Four(i);
			bomb++;
		}
	}
	
	for(int i=1;i<=13;i++)
	{
		if(a[i]==3)
		{
			a[i]-=3;
			bool if_2=0;
			for(int j=1;j<=13;j++)
			{
				if(a[j]==2)
				{
					a[j]-=2;
					result[++tot]=new Three_Two(i,j);
					if_2=1;
					break;
				}
			}
			if(!if_2)
			{
				result[++tot]=new Three(i);
			}
		}
	}
	for(int i=1;i<=15;i++)
	{
		if(a[i]==2)
		{
			a[i]-=2;
			result[++tot]=new Two(i);
		}
		else if(a[i]==1)
		{
			result[++tot]=new One(i);
		}
	}//cout<<"ok";
	if(tot<min_round||(tot==min_round&&bomb>max_bomb))
	{
		min_round=tot;
		max_bomb=bomb;
		for(int i=1;i<=tot;i++)
		{
			//delete min_result[i];
			min_result[i]=result[i];
		}
	}
}
void analysis(int *a,int tot)
{
	if(tot>=min_round) return;
	//cout<<"in"<<endl;
	calc_round(a,tot);
	//cout<<"out"<<endl;
	for(int i=1;i<=12;i++)
	{
		for(int j=1;j<=3;j++)
		if(a[i]>=j)
		{
			int now=0,R=0;
			for(int L=i;L<=12;R=L++)
			{
				if(a[L]<j)break;
				now+=j;a[L]-=j;
				if(now>=5)
				{
					tot++;
					result[tot]=new Dragon(j,L-i+1,i); 
					//cout<<"???"<<j<<" "<<i<<" "<<L<<endl;
					analysis(a,tot);
					tot--;
				}
			}
			for(int L=i;L<=R;L++)a[L]+=j;
		}
	}
}
Type analysis_opponent(card_list cards)
{
	int num[17];
	
	memset(num,0,sizeof(num));
	for(int i=0;i<cards.size;i++)
	{
		if(cards.cards[i].rank<=2) num[cards.cards[i].rank+11]++;
		else if(cards.cards[i].rank<=13) num[cards.cards[i].rank-2]++;
		else num[cards.cards[i].rank]++;
	}
	if(num[14]&&num[15]) return Rocket();
	if(cards.size==1)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]) return One(i);
		}
	}
	if(cards.size==2)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==2) return Two(i);
		}
	}
	if(cards.size==3)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==3) return Three(i);
		}
	}
	if(cards.size==4)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==4) return Four(i);
		}
	}
	if(cards.size==6)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==4)
			{
				int other1=0,other2=0;
				for(int j=1;j<=15;j++)
				{
					if(i==j) continue;
					while(num[j])
					{
						if(other1==0) other1=j;
						else other2=j;
						num[j]--;
					}
				}
				return Four_Two(i,other1,other2); 
			}
		}
	}
	if(cards.size==5)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==3)
			{
				int other=0;
				for(int j=1;j<=15;j++)
				{
					if(i==j) continue;
					if(num[j]==2)
					{
						other=j;
						break;
					}
				}
				return Three_Two(i,other); 
			}
		}
	}
	int num2=0,num3=0;
	for(int i=1;i<=15;i++)
	{
		if(num[i]==2) num2++;
		if(num[i]==3) num3++;
	}
	if(num3==num2)
	{
		for(int i=1;i<=15;i++)
		{
			if(num[i]==3)
			{
				int a[15],cnt=0;
				for(int j=1;j<=15;j++)
				{
					if(num[j]==2)
					a[++cnt]=j;
				}
				return Plane(i,num3,a);
			}
		}
	}
	for(int i=1;i<=12;i++)
	{
		if(num[i])
		{
			int r;
			for(int j=i;j<=12;j++)
			{
				if(num[j]) r=j;
			}
			return Dragon(num[i],r-i+1,i);
		}
	}
}
unsigned short* Play_Winners(const card_list yours, // your cards
const card_list hist_cards[], // the history of cards played
const unsigned short hist_size, // the size of Last_cards
unsigned short &size
)
{
	
	min_round=17;
	max_bomb=0;
	int hand_card[3];
	hand_card[1]=hand_card[2]=18;
	int now_pla=0;
	for(int i=hist_size-1;i>=0;i--)
	{
		now_pla--;
		if(now_pla<0) now_pla+=3;
		hand_card[now_pla]-=hist_cards[i].size;
	}
	/*for(int i=0;i<16;i++)
	{cout<<"ok";
		if(result[i]!=NULL) delete [] result[i];
		if(min_result[i]!=NULL) delete [] min_result[i]; 
	}*/
	int num[17];
	memset(num,0,sizeof(num));
	for(int i=0;i<yours.size;i++)
	{
		if(yours.cards[i].rank==1) num[12]++;
		else if(yours.cards[i].rank==2) num[13]++;
		else if(yours.cards[i].rank<=13) num[yours.cards[i].rank-2]++;
		else num[yours.cards[i].rank]++;
	}
	//for(int i=1;i<17;i++) cout<<num[i]<<" ";
	//cout<<endl;
	analysis(num,0);
	//cout<<min_round<<endl;
	for(int i=1;i<=min_round;i++)
	{
		unsigned short size;
		unsigned short *ans=min_result[i]->play_card(yours,size);
		/*cout<<size<<" ";
		for(int j=0;j<size;j++)
		cout<<yours.cards[ans[j]].rank<<" ";
		cout<<endl;//*/
	}
	int remain_num[17];
	for(int i=1;i<=13;i++) remain_num[i]=4;
	remain_num[14]=1;remain_num[15]=1;
	for(int i=0;i<yours.size;i++)
	{
		if(yours.cards[i].rank==1) remain_num[12]--;
		else if(yours.cards[i].rank==2) remain_num[13]--;
		else if(yours.cards[i].rank<=13) remain_num[yours.cards[i].rank-2]--;
		else remain_num[yours.cards[i].rank]--;
	}
	
	for(int i=0;i<hist_size;i++)
	{
		//cout<<hist_cards[i].size<<endl;
		for(int j=0;j<hist_cards[i].size;j++)
		{
			if(hist_cards[i].cards[j].rank==1) remain_num[12]--;
			else if(hist_cards[i].cards[j].rank==2) remain_num[13]--;
			else if(hist_cards[i].cards[j].rank<=13) remain_num[hist_cards[i].cards[j].rank-2]--;
			else remain_num[hist_cards[i].cards[j].rank]--;
		}
	}
	if(hist_size==0||(hist_size>=2&&hist_cards[hist_size-1].size==0&&hist_cards[hist_size-2].size==0))
	{
		if(min_round==1)
		{
			return min_result[1]->play_card(yours,size);
		}
		int num_small=0,num_bomb=0,cou_card=0;
		for(int i=1;i<=min_round;i++)
		{
			if(min_result[i]->type==four) num_bomb++;
			if(min_result[i]->impossible_better(remain_num)<=0)
			{
				num_small++;
			}
			
		}
		if(num_small<=1)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->impossible_better(remain_num)>0)
				return min_result[i]->play_card(yours,size);
			}
		}
		if(!if_bomb(remain_num))
		{
			int other_small=0;
			int num_card=0,max_card=0;
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->impossible_better(remain_num)<=0)
				{
					if(min_result[i]->type==one)
					{
						num_card++;max_card=max(max_card,1);
					}
					else if(min_result[i]->type==two)
					{
						num_card+=2;max_card=max(max_card,2);
					}
					else other_small++;
				}
			}
			if(other_small==0)
			{
				if(num_card-max_card<=2*num_bomb)
				{
					int num1=0,num2=0;
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==one&&min_result[i]->impossible_better(remain_num)<=0)
						{
							if(!num1) num1=min_result[i]->rank;
							else if(!num2) num2=min_result[i]->rank;
							else break; 
						}
					}
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==two&&min_result[i]->impossible_better(remain_num)<=0)
						{
							if(!num1) num1=min_result[i]->rank;
							if(!num2) num2=min_result[i]->rank;
							else break;
						}
					}
					if(num1&&num2)
					{
						for(int i=1;i<=min_round;i++)
						{
							if(min_result[i]->type==four)
							{
								Type temp=Four_Two(min_result[i]->rank,num1,num2);
								return temp.play_card(yours,size);
							}
						}
					}
				}
			}
			else if(other_small==1)
			{
				if(num_card<=2*num_bomb)
				{
					if(num_card&1)
					{
						for(int i=1;i<=min_round;i++)
						{
							if(min_result[i]->type==one&&min_result[i]->impossible_better(remain_num)<=0)
							{
								return min_result[i]->play_card(yours,size);
							}
						}
					}
					int num1=0,num2=0;
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==one&&min_result[i]->impossible_better(remain_num)<=0)
						{
							if(!num1) num1=min_result[i]->rank;
							else if(!num2) num2=min_result[i]->rank;
							else break; 
						}
					}
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==two&&min_result[i]->impossible_better(remain_num)<=0)
						{
							if(!num1) num1=min_result[i]->rank;
							if(!num2) num2=min_result[i]->rank;
							else break;
						}
					}
					if(num1&&num2)
					{
						for(int i=1;i<=min_round;i++)
						{
							if(min_result[i]->type==four)
							{
								Type temp=Four_Two(min_result[i]->rank,num1,num2);
								return temp.play_card(yours,size);
							}
						}
					}
				}
			}
		}
		int min_rank=233,min_pla;
		sort(min_result+1,min_result+min_round+1,cmp);
		for(int i=1;i<=min_round;i++)
		{
			if(min_result[i]->type!=four&&min_result[i]->type!=rocket)
			{
				bool ok=1;Type temp1=One(1),temp2=Two(1);
				if(min_result[i]->type==two&&(hand_card[1]==2||hand_card[2]==2)&&temp2.impossible_better(remain_num))
				ok=0;
				if(min_result[i]->type==one&&(hand_card[1]==1||hand_card[2]==1)&&temp1.impossible_better(remain_num))
				ok=0;
				return min_result[i]->play_card(yours,size);
			}
		}
		
		return min_result[1]->play_card(yours,size);
	}
	else
	{
		
		Type last;
		if(hist_cards[hist_size-1].size!=0)
		last=analysis_opponent(hist_cards[hist_size-1]);
		else last=analysis_opponent(hist_cards[hist_size-2]);
		int num_small=0,num_bomb=0,cou_card=0;
		for(int i=1;i<=min_round;i++)
		{
			if(min_result[i]->type==four) num_bomb++;
			if(min_result[i]->impossible_better(remain_num)<=0)
			{
				num_small++;
			}
		}
		if(num_small<=1&&num_bomb)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==four||min_result[i]->type==rocket)
				{
					if(min_result[i]->type==four&&last.type==four&&last.rank>min_result[i]->rank) continue;
					return min_result[i]->play_card(yours,size);
				}
			}
		}
		if(!if_bomb(remain_num))
		{
			int other_small=0;
			int num_card=0,max_card=0;
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->impossible_better(remain_num)<=0)
				{
					if(min_result[i]->type==one)
					{
						num_card++;max_card=max(max_card,1);
					}
					else if(min_result[i]->type==two)
					{
						num_card+=2;max_card=max(max_card,2);
					}
					else other_small++;
				}
			}
			num_bomb--;
			if(other_small==0)
			{
				if(num_card-max_card<=2*num_bomb)
				{
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==four||min_result[i]->type==rocket)
						{
							if(min_result[i]->type==four&&last.type==four&&last.rank>min_result[i]->rank) continue;
							return min_result[i]->play_card(yours,size);
						}
					}
				}
			}
			else if(other_small==1)
			{
				if(num_card<=2*num_bomb)
				{
					for(int i=1;i<=min_round;i++)
					{
						if(min_result[i]->type==four||min_result[i]->type==rocket)
						{
							if(min_result[i]->type==four&&last.type==four&&last.rank>min_result[i]->rank) continue;
							return min_result[i]->play_card(yours,size);
						}
					}
				}
			}
			
		}
		int min_rank=233,min_pla=0;
		
		for(int i=1;i<=min_round;i++)
		{
			if(min_result[i]->type==last.type&&min_result[i]->rank>last.rank)
			{
				if(min_result[i]->check_type()!=last.check_type()) continue;
				if(min_result[i]->rank<min_rank)
				{
					min_rank=min_result[i]->rank;
					min_pla=i;
				}
 			}
		}//cout<<"ok";
		if(min_pla) return min_result[min_pla]->play_card(yours,size);
		sort(min_result+1,min_result+min_round+1,cmp);
		bool warning=0;
		if(hist_cards[hist_size-1].size!=0&&hand_card[2]<=3) warning=1;
		if(hist_cards[hist_size-2].size!=0&&hand_card[1]<=3) warning=1;
		if(hand_card[1]<=2||hand_card[2]<=2) warning=1;
		if(warning)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==four||min_result[i]->type==rocket)
				{
					if(min_result[i]->type==four&&last.type==four&&last.rank>min_result[i]->rank) continue;
					return min_result[i]->play_card(yours,size);
				}
			}
		}
		if(last.type==one)
		{
			for(int i=1;i<=min_round;i++)
			{
				//cout<<min_result[i]->type<<" "<<three_two<<endl;
				if((min_result[i]->type==two||min_result[i]->type==three||min_result[i]->type==three_two)&&min_result[i]->rank>=12&&min_result[i]->rank>last.rank)
				return One(min_result[i]->rank).play_card(yours,size);
				if(min_result[i]->type==dragon&&min_result[i]->check_type()==1&&min_result[i]->getlast()>last.rank&&min_result[i]->getlast()-min_result[i]->rank+1>5)
				return One(min_result[i]->getlast()).play_card(yours,size);
			}
			int num_one=0;
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==one&&min_result[i]->rank<=11) num_one++;
			}
			if(num_one>=4)
			{
				for(int i=1;i<=min_round;i++)
				{
					if(min_result[i]->type==rocket) return One(14).play_card(yours,size);
				}
			}
		}
		if(last.type==two)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==three&&min_result[i]->rank>=12&&min_result[i]->rank>last.rank)
				return Two(min_result[i]->rank).play_card(yours,size);
				if(min_result[i]->type==dragon&&min_result[i]->check_type()==2&&min_result[i]->getlast()>last.rank&&min_result[i]->getlast()-min_result[i]->rank+1>3)
				return Two(min_result[i]->getlast()).play_card(yours,size);
				if(min_result[i]->type==three_two&&min_result[i]->getother()>last.rank)
				return Two(min_result[i]->getother()).play_card(yours,size);
			}
		}
		if(last.type==three)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==three_two&&min_result[i]->rank>last.rank)
				return Three(min_result[i]->rank).play_card(yours,size);
				if(min_result[i]->type==dragon&&min_result[i]->check_type()==3&&min_result[i]->getlast()>last.rank&&min_result[i]->getlast()-min_result[i]->rank+1>2)
				return Three(min_result[i]->getlast()).play_card(yours,size);
			}
		}
		if(last.type==dragon)
		{
			for(int i=1;i<=min_round;i++)
			{
				if(min_result[i]->type==dragon&&min_result[i]->check_type()==last.check_type()&&min_result[i]->getlength()-last.getlength()==1)
				return Dragon(min_result[i]->check_type(),last.getlength(),min_result[i]->rank).play_card(yours,size);
			}
		}
		size=0;
		return NULL;
	}
}
unsigned short* Play_Hearts(const card_list Tyours, // your cards
const card_list Thist_cards[], // the history of cards played
const unsigned short hist_size // the size of Last_cards
)
{
	card_list *hist_cards=new card_list [hist_size];
	card_list yours;
	yours.size=Tyours.size;
	yours.cards=new Card [Tyours.size];
	//cout<<"______"<<endl;
	for(int i=0;i<yours.size;i++)
	{
		yours.cards[i]=Tyours.cards[i];
		//cout<<Tyours.cards[i].suit<<" "<<Tyours.cards[i].rank<<endl;
		if(yours.cards[i].rank==1) yours.cards[i].rank=13;
		else yours.cards[i].rank--;
	}
	//cout<<"!!!!!!!!!"<<endl;
	for(int i=0;i<hist_size;i++)
	{
		hist_cards[i].size=Thist_cards[i].size;
		hist_cards[i].cards=new Card;
		hist_cards[i].cards[0].rank=Thist_cards[i].cards[0].rank;
		hist_cards[i].cards[0].suit=Thist_cards[i].cards[0].suit;
		if(hist_cards[i].cards[0].rank==1) hist_cards[i].cards[0].rank=13;
		else hist_cards[i].cards[0].rank--;
	}
	const double eps=1e-10;
	short rank=hist_size%4+1;
	bool vis[55],break_heart=0,have_card[5][5];
	double f[15][5][5];// i cards to k people, j people have had at least one card.
	double g[15][5][5][5];// i cards to l people,k people are chosen and j of them have had at least one card. 
	memset(f,0,sizeof(f));
	
	for(int k=1;k<=3;k++)
	{
		f[0][0][k]=1;
		for(int i=0;i<13;i++)
		{
			for(int j=0;j<=k;j++)
			{
				
				f[i+1][j][k]+=f[i][j][k]*(1.0*j/k);
				f[i+1][j+1][k]+=f[i][j][k]*(1.0*(k-j)/k);
			}
		}
	}
	for(int i=0;i<=13;i++)
	{
		f[i][0][0]=1;
	}
	for(int l=1;l<=3;l++)
	{
		for(int k=1;k<=l;k++)
		{
			g[0][0][k][l]=1;
			for(int i=0;i<13;i++)
			{
				for(int j=0;j<=k;j++)
				{
					g[i+1][j][k][l]+=g[i][j][k][l]*(1.0*(l-k+j)/l);
					g[i+1][j+1][k][l]+=g[i][j][k][l]*(1.0*(k-j)/l);
				}
			}
		}
	}
	for(int i=0;i<=13;i++)
	{
		for(int l=0;l<=3;l++)
		{
			g[i][0][0][l]=1;
		}
	}
	
	memset(have_card,1,sizeof(have_card));
	memset(vis,1,sizeof(vis));
	int round_start=(5-rank)%4;
	if(hist_size!=0)
	for(int i=(hist_size-1)/4-1+(round_start==0?1:0);i>=0;i--)
	{
		int round_color=hist_cards[i*4].cards[0].suit,max_point=0,max_person;
		for(int j=0;j<4;j++)
		{
			if(hist_cards[i*4+j].cards[0].suit==round_color&&hist_cards[i*4+j].cards[0].rank>max_point)
			{
				max_point=max(max_point,1*hist_cards[i*4+j].cards[0].rank);
				max_person=j;
			}
		}
		round_start=(round_start-max_person+4)%4;
		for(int j=0;j<4;j++)
		{
			if(hist_cards[i*4+j].cards[0].suit!=round_color)
			{
				have_card[(j+round_start)%4][round_color]=0;
			}
		}
	}
	round_start=(5-rank)%4;
	for(int i=hist_size-rank+1;i<hist_size;i++)
	{
		int round_color=hist_cards[hist_size-rank+1].cards[0].suit;
		if(hist_cards[i].cards[0].suit!=round_color)
		{
			have_card[round_start][round_color]=0;
			round_start++;if(round_start>3) round_start-=4;
		}
	}
	int num[5];
	memset(num,0,sizeof(num));
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==Spade)
		{
			num[3]++;
		}
		if(temp.suit==Heart)
		{
			num[2]++;
		}
		if(temp.suit==Club)
		{
			num[0]++;
		}
		if(temp.suit==Diamond)
		{
			num[1]++;
		}
	}
	unsigned short *ans;
	ans=new unsigned short;
	
	if(If_Card(yours,Club,1))
	{
		ans[0]=Find_Card(yours,Club,1);
		return ans;
	}
	
	for(int i=0;i<hist_size;i++)
	{
		//cout<<hist_cards[i].cards[0].suit<<" "<<hist_cards[i].cards[0].rank<<endl;
		vis[Trans(hist_cards[i].cards[0])]=0;
		if(hist_cards[i].cards[0].suit==Heart) break_heart=1;
	}
	if(rank!=1)
	{
		int round_color=hist_cards[hist_size-rank+1].cards[0].suit,max_point=0;
		double win_limit=0.3,lose_limit=0.8;
		
		for(int i=hist_size-rank+1;i<hist_size;i++)
		{
			if(hist_cards[i].cards[0].suit==round_color) max_point=max(max_point,1*hist_cards[i].cards[0].rank);
		}
		if(If_Card(yours,Club,13)&&round_color==Club)
		{
			ans[0]=Find_Card(yours,Club,13);
			return ans;
		}
		
		if(num[round_color])
		{
			bool have_point=0;
			for(int i=hist_size-rank+1;i<hist_size;i++)
			{
				if(hist_cards[i].cards[0].suit==Heart||(hist_cards[i].cards[0].suit==Spade&&hist_cards[i].cards[0].rank==11))
				have_point=1;
			}
			if(!have_point)
			{
				bool all=1;
				for(int i=1;i<=4-rank;i++)
				{
					if(!have_card[i][round_color]) all=0;
				}
				int num_now=0;
				for(int i=round_color*13;i<(round_color+1)*13;i++)
				{
					if(!vis[i]) num_now++;
				}
				num_now-=num[round_color];
				double prob=g[num_now][4-rank][4-rank][3];
				if(prob>lose_limit&&all&&(!round_color==Spade||Smallest_Rank(yours,round_color)<11))
				{
					if(round_color!=Spade||!vis[49])
					ans[0]=Find_Card(yours,round_color,Largest_Rank(yours,round_color));
					else ans[0]=Find_Card(yours,round_color,Special_Largest_Rank(yours,round_color));
					return ans;
				}
			}
			int my_point=0,pla=0;
			for(int i=0;i<yours.size;i++)
			{
				Card temp=yours.cards[i];
				if(temp.suit==round_color&&max_point>temp.rank)
				{
					if(temp.rank>my_point)
					{
						my_point=temp.rank;
						pla=i;
					}
				}
			}
			if(my_point)
			{
				ans[0]=Find_Card(yours,round_color,my_point);
				return ans;
			}
			int out_card=Count_Smaller(vis,round_color,Smallest_Rank(yours,round_color));
			if(g[out_card][4-rank][4-rank][3]<win_limit)
			{
				ans[0]=Find_Card(yours,round_color,Smallest_Rank(yours,round_color));
			}
			if(round_color!=Spade)
			ans[0]=Find_Card(yours,round_color,Largest_Rank(yours,round_color));
			else if(num[round_color]!=1||!If_Card(yours,round_color,11))
			ans[0]=Find_Card(yours,round_color,Special_Largest_Rank2(yours,round_color));
			else ans[0]=Find_Card(yours,round_color,11);
			return ans;
		}
		else
		{
			if(If_Card(yours,Spade,11))
			ans[0]=Find_Card(yours,Spade,11);
			return ans;
			double prob=0;int chosen_color;
			for(int i=3;i>=0;i--)
			{
				if(Smallest_Rank(yours,i)==-1) continue;
				if(prob<Count_Larger(vis,i,Largest_Rank(yours,i))/(Count_Larger(vis,i,0)-num[i]))
				{
					prob=Count_Larger(vis,i,Largest_Rank(yours,i))/(Count_Larger(vis,i,0)-num[i]);
					chosen_color=i;
				}
			}
			ans[0]=Find_Card(yours,chosen_color,Largest_Rank(yours,chosen_color));
		}
	}
	else
	{
		int out_heart=13,out_spade=13,out_club=13,out_diamond=13;
		if(break_heart)
		{
			out_heart=Count_Smaller(vis,Heart,Smallest_Rank(yours,Heart));
		}
		out_spade=Count_Smaller(vis,Spade,Smallest_Rank(yours,Spade));
		out_club=Count_Smaller(vis,Club,Smallest_Rank(yours,Club));
		out_diamond=Count_Smaller(vis,Diamond,Smallest_Rank(yours,Diamond));
		int num_person[5];
		memset(num_person,0,sizeof(num_person));
		for(int i=1;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				if(have_card[i][j]) num_person[j]++;
			}
		}
		double prob[5];
		prob[2]=f[out_heart][num_person[Heart]][num_person[Heart]];
		prob[3]=f[out_spade][num_person[Spade]][num_person[Spade]];
		prob[0]=f[out_club][num_person[Club]][num_person[Club]];
		prob[1]=f[out_diamond][num_person[Diamond]][num_person[Diamond]];
		double min_prob=1.1,min_num=15,min_color;
		for(int i=0;i<4;i++)
		{
			if(Smallest_Rank(yours,i)==-1) continue;
			if(i==Heart&&!break_heart) continue;
			if(min_prob>prob[i]||(prob[i]-min_prob>eps&&min_num>num[i]))
			{
				min_prob=prob[i];min_num=num[i];
				min_color=i;
			}
		}
		ans[0]=Find_Card(yours,min_color,Smallest_Rank(yours,min_color));
		return ans;
	}
}


unsigned short* Exchange_Hearts(const card_list yours)
{
	bool if_q=0,if_k=0,if_a=0,vis[15];
	memset(vis,0,sizeof(vis));
	int num_spade=0,num_heart=0,num_club=0,num_diamond=0;
	int small_spade=0,small_heart=0,small_club=0,small_diamond=0;
	for(int i=0;i<yours.size;i++)
	{
		Card temp=yours.cards[i];
		if(temp.suit==Spade&&temp.rank==12) if_q=1;
		if(temp.suit==Spade&&temp.rank==13) if_k=1;
		if(temp.suit==Spade&&temp.rank==1) if_a=1;
		if(temp.suit==Spade)
		{
			if(temp.rank<=4&&temp.rank>=2) small_spade++;
			num_spade++;
		}
		if(temp.suit==Heart)
		{
			if(temp.rank<=4&&temp.rank>=2) small_heart++;
			num_heart++;
		}
		if(temp.suit==Club)
		{
			if(temp.rank<=4&&temp.rank>=2) small_club++;
			num_club++;
		}
		if(temp.suit==Diamond)
		{
			if(temp.rank<=4&&temp.rank>=2) small_diamond++; 
			num_diamond++;
		}
	}
	int tot=-1;
	short* card_id;
	card_id=new short[15];
	if(if_q&&num_spade-if_q-if_k-if_a<=4)
	card_id[++tot]=Find_Card(yours,Spade,12);
	if(if_k&&num_spade-if_q-if_k-if_a<=4)
	card_id[++tot]=Find_Card(yours,Spade,13);
	if(if_a&&num_spade-if_q-if_k-if_a<=4)
	card_id[++tot]=Find_Card(yours,Spade,1);
	if(num_heart-small_heart<=2-tot)
	{
		for(int i=5;i<=13;i++)
		{
			if(If_Card(yours,Heart,i)) card_id[++tot]=Find_Card(yours,Heart,i);
		}
		if(If_Card(yours,Heart,1)) card_id[++tot]=Find_Card(yours,Heart,1);
	}
	else if(num_heart<7)
	{
		if(If_Card(yours,Heart,12)) card_id[++tot]=Find_Card(yours,Heart,12);
		if(If_Card(yours,Heart,13)) card_id[++tot]=Find_Card(yours,Heart,13);
		if(If_Card(yours,Heart,1)) card_id[++tot]=Find_Card(yours,Heart,1);
	}
	if(num_diamond-small_diamond<=2-tot)
	{
		for(int i=13;i>=5;i--)
		{
			if(If_Card(yours,Diamond,i)) card_id[++tot]=Find_Card(yours,Diamond,i);
		}
		if(If_Card(yours,Diamond,1)) card_id[++tot]=Find_Card(yours,Diamond,1);
	}
	if(num_club-small_club-If_Card(yours,Club,1)<=2-tot)
	{
		for(int i=13;i>=5;i--)
		{
			if(If_Card(yours,Club,i)) card_id[++tot]=Find_Card(yours,Club,i);
		}
	}
	if(If_Card(yours,Club,2)) card_id[++tot]=Find_Card(yours,Club,2);
	for(int i=3;i>=0;i--)
	{
		if(i==Spade) continue;
		if(tot>=2) break;
		if(If_Card(yours,i,1)&&!vis[Find_Card(yours,i,1)]) card_id[++tot]=Find_Card(yours,i,1);
	}
	for(int j=13;j>=2;j--)
	{
		for(int i=3;i>=0;i--)
		{
			if(tot>=2) break;
			if(i==Spade) continue;
			if(If_Card(yours,i,j)&&!vis[Find_Card(yours,i,j)]) card_id[++tot]=Find_Card(yours,i,j);
		}
		
	}
	unsigned short* ans;
	ans=new unsigned short[3];
	for(int i=0;i<=2;i++) ans[i]=card_id[i];
	return ans;
}

/*int main()
{
	srand(1);
	card_list yours[3]; 
	Card temp[55];
	int cnt=-1;
	for(int i=0;i<4;i++)
	{
		for(int j=1;j<=13;j++)
		{
			cnt++;
			temp[cnt].suit=(Suit)i;temp[cnt].rank=j;
		}
	}
	temp[52].rank=14;temp[53].rank=15;
	//for(int)
	for(int i=1;i<=10000;i++)
	{
		swap(temp[rand()%54],temp[rand()%54]);
	}
	for(int i=0;i<3;i++)
	{
		yours[i].cards=new Card [18];
		yours[i].size=18;
		for(int j=0;j<18;j++)
		{
			yours[i].cards[j]=temp[i*18+j];
		}
	}
	card_list hist_cards[55];
	int tot=-1;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<18;j++)
		cout<<yours[i].cards[j].rank<<" ";
		cout<<endl; 
	}
	while(1)
	{
		for(int i=0;i<3;i++)
		{
			cout<<i<<"start:"<<endl;
			unsigned short size;
			unsigned short *ans=Play_Winners(yours[i],hist_cards,tot+1,size);
			
			cout<<"play cards:"<<endl;
			for(int j=0;j<size;j++)
			{
				//cout<<ans[j]<<endl;
				cout<<yours[i].cards[ans[j]].rank<<" ";
			}
			cout<<endl;
			tot++;
			hist_cards[tot].cards=new Card [size];
			hist_cards[tot].size=size;
			bool vis[19];
			memset(vis,0,sizeof(vis));
			for(int j=0;j<size;j++)
			{
				hist_cards[tot].cards[j]=yours[i].cards[ans[j]];
				vis[ans[j]]=1;
			}
			int cnt=-1;
			for(int j=0;j<yours[i].size;j++)
			{
				if(!vis[j])
				yours[i].cards[++cnt]=yours[i].cards[j];
			}
			yours[i].size=cnt+1;
			cout<<"remain:"<<yours[i].size<<endl;
			for(int j=0;j<yours[i].size;j++)
			cout<<yours[i].cards[j].rank<<" ";
			cout<<endl<<endl;
			if(yours[i].size==0)
			{
				cout<<i<<"wins";
				return 0;
			}
		}
	}
}//*/
/*int main()
{
	card_list yours;
	yours.size=13;yours.cards=new Card [15];
	for(int i=0;i<13;i++)
	{
		int x,y;
		cin>>x>>y;
		yours.cards[i].suit=(Suit)x;
		yours.cards[i].rank=y;
	}
	unsigned short *ans=Exchange_Hearts(yours);
	card_list hist_cards[55];
	int rank,tot=-1;
	cin>>rank;
	for(int i=1;i<=13;i++)
	{
		for(int j=0;j<=3;j++)
		{
			if(rank==j)
			{
				unsigned short *ans=Play_Hearts(yours,hist_cards,tot+1);
				cout<<yours.cards[ans[0]].suit<<" "<<yours.cards[ans[0]].rank<<endl;
				tot++;
				yours.size--;
				
				hist_cards[tot].cards=new Card;
				hist_cards[tot].cards[0].suit=yours.cards[ans[0]].suit;
				hist_cards[tot].cards[0].rank=yours.cards[ans[0]].rank;
				for(int l=ans[0];l<yours.size;l++) yours.cards[l]=yours.cards[l+1];
			}
			else
			{
				int x,y;
				cin>>x>>y;
				tot++;
				hist_cards[tot].cards=new Card;
				hist_cards[tot].cards[0].suit=(Suit)x;
				hist_cards[tot].cards[0].rank=y;
			}
		}
		int round_color=hist_cards[tot-3].cards[0].suit;
		int pla,max_val=0; 
		for(int j=0;j<=3;j++)
		{
			if(round_color!=hist_cards[tot-(3-j)].cards[0].suit) continue;
			if(hist_cards[tot-(3-j)].cards[0].rank==1)
			hist_cards[tot-(3-j)].cards[0].rank=14;
			if(max_val<hist_cards[tot-(3-j)].cards[0].rank)
			{
				 
				max_val=hist_cards[tot-(3-j)].cards[0].rank;
				pla=j;
			}
			if(hist_cards[tot-(3-j)].cards[0].rank==14)
			hist_cards[tot-(3-j)].cards[0].rank=1;
			
		}
		rank=(4+rank-pla)%4;
	}
}*/
