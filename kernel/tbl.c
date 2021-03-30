#include "tbl.h"
#include "types.h"
#include "memlayout.h"
#include "tabl.h"

//za rukovanje tabelom
static ushort *crtt = (ushort*)P2V(0xb8000);
static int pocetak=57;
static int pflag=57;
static int selected=0;


extern int bojaConsol;
int visTbl=0;

#define BLACK  138
#define BLUE   218
#define GREEN  298
#define AQUA   378
#define RED    458
#define PURPLE 538
#define YELLOW 618
#define WHITE  698

//Za FG -->BG
#define FB(x) (x +11)



static void select(int broj,int boja){
	for(int i=0;i<10;i++){
		crtt[broj+i]=(crtt[broj+i] & 0xff) | boja;	
	}

}
void printTbl(){
	int i=0;
	pflag=pocetak;
	while(i<230){
		uTable[i]=(crtt[pflag]&0xff);
		crtt[pflag++]=table[i] |0x0f00;

		if(!(pflag%80)){
			pflag+=pocetak;		
		}
		i++;

	}
	selected=GREEN;
	select(selected,0xf000);
	visTbl=1;

}
#define W 0
#define A 1
#define S 2
#define D 3
																		
static void increment(int flag){
	int tren=0;
	if(flag==W)
		tren= (selected-80)>80 ? selected-80 : 7*80+selected;	
	else if(flag==A || flag==D)
			tren= ((selected%80))>58 ? selected-11 : selected+11; 
		else if(flag== S)
				tren=(selected+80)<770 ? selected+80: selected-7*80;
			


	if(tren!=0){
		select(selected,0x0f00);
		select(tren,0xf000);
		selected=tren;
	}
}

void workOfTbl(int c){

switch(c){
	case 'w': 
		increment(W);
		break;
	case 'a':
		increment(A);
		break;
	case 's':
		increment(S);
		break;
	case 'd':
		increment(D);
		break;
	case 'e':
		setNewBoju(0);
		break;
	case 'r':
		setNewBoju(1);
		break;
}

}




void clostTbl(){
	int i=0;
	pflag=pocetak;
	while(i<230){
		crtt[pflag++]=(uTable[i]&0xff) |bojaConsol;  

		if(!(pflag%80))
			pflag+=pocetak;		
		
		i++;
	}
	visTbl=0;
}


static int retBojaTbl(int flag){
	int test=bojaConsol;
	int boja=0;
	//vrvt ima lepsi nacin za pronalazenje ovoga 
	// atm nmg da smislim
	int nizB[16]={
		 0x0000, 0x0100, 0x0200, 0x0300, 0x0400, 0x0500, 0x0600, 0x0700,
		 0x0000, 0x1000, 0x2000, 0x3000, 0x4000, 0x5000, 0x6000, 0x7000
	};
	int nizA[16]={BLACK,    BLUE,    GREEN,    AQUA,    RED,     PURPLE,   YELLOW,    WHITE,
		   FB(BLACK),FB(BLUE),FB(GREEN),FB(AQUA),FB(RED),FB(PURPLE),FB(YELLOW),FB(WHITE)
	};

	for(int i=0;i<16;i++){
		if(nizA[i]==selected){
			boja=i;
			break;
		}	
	}

	if(boja<8)
		test= (test &0xf000);		
	   else
		test=(test & 0x0f00);
		
		
	test|=nizB[boja]; 
	
	if(flag){
	   if(boja<8)
		test|=0x0800;
	   else
		test|=0x8000;	
	}


return test;
}




//flag je za svetliju il tamniju boju
void setNewBoju(int flag){

	if(retBojaTbl(flag)==bojaConsol)
		return;

	int j=0;
	bojaConsol=retBojaTbl(flag);

	for(int i=0; i<2000;i++){
		j=i%80;	
		if(i<=840 && j>=57 && j<=79  )continue;
		crtt[i]=(crtt[i]&0xff) |bojaConsol;
	}

}

void oboji(int pos){

for(int i=pos%80,k=0;i<80;i++,k++){
	crtt[pos+k]= (crtt[pos+k]&0xff) | bojaConsol;	
}	

}





