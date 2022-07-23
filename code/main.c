
// /*KHOA DIEN TU MAT KHAU DUNG VI DIEU KHIEN 8051 */
// 1. Trần Minh Nhật 2014008
// 2. Lê Trung Nguyên 2011701
// 3. Bùi Quốc Khánh 2010325
// 4. Võ Huỳnh Anh Vũ 2015100
#include<reg51.h>
#include<string.h>
#define MAX 16
//khai bao ket noi data lcd
#define lcdport P1

//khai bao ket noi dieu khien lcd
sbit rs=P3^4;
sbit rw=P3^5;
sbit en=P3^6;

//khai bao den led bao hieu
sbit led_green = P3^3;
sbit led_red   = P3^2;

//khai bao ket noi motor
sbit m1=P3^0;
sbit m2=P3^1;

//khao bao ket noi dong cua keypad
sbit r1=P0^0;
sbit r2=P0^1;
sbit r3=P0^2;
sbit r4=P0^3;

//khao ba0 ket noi cot cua keypad
sbit c1=P0^4;
sbit c2=P0^5;
sbit c3=P0^6;

//khai bao ket noi am thanh
sbit buzzer=P0^7;

//khai bao mat khau
char uid[MAX]="12345";
char id[MAX];
char new_id1[MAX];
char new_id2[MAX];

//khai bao chuong trinh con 
void lcdint(); // khoi tao lcd
void lcddis(char *); //in chuoi ki tu ra lcd
void delay(int); //ham delay
void lcdcmd(char); // command mode cho lcd
void lcddata(char); // data mode cho lcd
char lcdkey(); // ham quet ma keypad
char scan_key(); // ham nhan ki tu thu vao
void door_open(); // ham mo cua
void door_close(); // ham dong cua
void sounder(); // ham phat am thanh
void bip_bip();//am thanh ban phim

void main(){
    int n_enter, n_chmod, n_lock, n, n_2;
	char key;
	led_green=0;
    led_red=0;
    buzzer=0;
	
	P2=0x00; // port 2 la ngo ra
	P1=0xff; // port 1 la ngo vao

	lcdint(); 
	lcddis("    WELCOME!    ");
	lcdcmd(0xc0);
	lcddis("|--- HCMUT ---| ");
	delay(100);
	lcdint();
	lcddis("CUA DA KHOA !!");
	delay(100);
	n_lock=0;

	while(1){
		lcdcmd(0x01);  //xoa hien thi lcd
		lcdcmd(0x02);	//4bit mode lcd
		lcddis("NHAP MAT KHAU:*");
		lcdcmd(0xc0);	// di chuyen con tro den dong thu 2
		lcddis("DOI MAT KHAU: #");
		n_chmod=0;
		while(n_chmod<1){
			key=scan_key();
			delay(100);
			n_chmod++;
		}
		if(key=='*'){
			//mode nhap mat khau
			lcdint();
			lcddis("NHAP MAT KHAU: ");
			lcdcmd(0xc0);	// di chuyen con tro den dong thu 2
			n_enter=0;
			while(n_enter<MAX){ //lcd 16x2 => n_max=16
				unsigned char i;
				for(i=0;i<MAX;i++){
					id[i]=0;
				}
				do{
					key=scan_key();
					delay(100);
					if(n_enter==16){
						break;
					}
					if(key != '*'){
						id[n_enter]=key;
					}else{
						break;
					}
					n_enter++;
				}while(key != '*');
				if(key=='*'){
					break;
				}
				n_enter++;
			}
			lcdcmd(0x01); // xoa hien thi lcd
			lcdcmd(0x02);	// 4bit mode lcd
			if(strcmp(uid,id)==0){
				lcddis("NHAP THANH CONG!");
				n_lock=0;
    		    led_green = 1;
    		    led_red   = 0;
				delay(200);
				door_open(); //mo cua
				delay(200);
				door_close(); //dong cua
				lcdcmd(0x01); //xoa hien thi lcd
				lcdcmd(0x02);	//4bit mode lcd
			}
			else{
				n_lock++;
				lcddis("SAI MAT KHAU");
    		    led_green = 0;
    		    led_red   = 1;
				sounder();	//phat am thanh
				led_green = 0;
    		    led_red   = 0;
				delay(100);
				
				lcdcmd(0x01); //xoa hien thi lcd
				lcdcmd(0x02); //4bit mode lcd
				if(n_lock!=5){
					lcddis("VUI LONG THU LAI");
					delay(200);
					lcdcmd(0x01); //xoa hien thi lcd
					lcdcmd(0x02); //4bit mode lcd
				}else{
					lcddis("KHOA VINH VIEN!!");
					while(1){
						delay(100);
					}
				}
			}
		}else if(key=='#'){
			//mode doi mat khau
			lcdint();
			lcddis("NHAP MAT KHAU CU");
			lcdcmd(0xc0);
			n_enter=0;
			while(n_enter<MAX){ //lcd 16x2 => n_max=16
				unsigned char i;
				for(i=0;i<MAX;i++){
					id[i]=0;
				}
				do{
					key=scan_key();
					delay(100);
					if(n_enter==16){
						break;
					}
					if(key != '*'){
						id[n_enter]=key;
					}else{
						break;
					}
					n_enter++;
				}while(key != '*');
				if(key=='*'){
					break;
				}
				n_enter++;
			}
			lcdcmd(0x01); // xoa hien thi lcd
			lcdcmd(0x02);	// 4bit mode lcd
			if(strcmp(uid,id)==0){
				unsigned char i;
				for(i=0;i<MAX;i++){
					new_id1[i]=0;
				}
				lcddis("NHAP MATKHAU MOI");
				lcdcmd(0xc0);
				n=0;
				while(n<MAX){
					do{
						key=scan_key();
						delay(100);
						if(n==16){
							break;
						}
						if(key != '*'){
							new_id1[n]=key;
						}else{
							break;
						}
						n++;
					}while(key != '*');
					if(key=='*'){
						break;
					}
					n++;
				}
				for(i=0;i<MAX;i++){
					new_id2[i]=0;
				}
				lcdint();
				lcddis("NHAP MK LAN 2:");
				lcdcmd(0xc0);

				n_2=0;
				while(n_2<MAX){
					do{
						key=scan_key();
						delay(100);
						if(n_2==16){
							break;
						}
						if(key != '*'){
							new_id2[n_2]=key;
						}else{
							break;
						}
						n_2++;
					}while(key != '*');
					if(key=='*'){
						break;
					}
					n_2++;
				}
				lcdint();
				if(strcmp(new_id1,new_id2)==0){
					unsigned char i;
					lcddis("DOI THANH CONG!");
					delay(100);
					lcdint();
					for(i=0;i<MAX;i++){
						uid[i]=0;
					}
					for(i=0;i<n_2;i++){
						uid[i]=new_id2[i];
					}
					
				}else{
					lcddis("KHONG HOP LE!");
					lcdint;
				}
				
			}else{
				lcddis("SAI MAT KHAU!");
				delay(100);
				lcdint();
			}
		}
		
	}
	
}
//------------------------
// khoi tao lcd
void lcdint(){
	lcdcmd(0x38);	//  khoi tao lcd 8bit
	delay(2);
	lcdcmd(0x01);	// xoa hien thi lcd
	delay(2);
	lcdcmd(0x80);	// di chuyen con tro den dong 1
	delay(2);
	lcdcmd(0x0e);	//con tro nhap nhay
	delay(2);
}
//-------------------------
//ham delay
void delay(int x)
{
	int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<1275;j++);
}
//-----------------------
//ham delay ms
void delay_ms(int x)
{
	int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<123;j++);
}
//-------------------------
//command mode 
void lcdcmd(char A){
	lcdport=A;	// gui ma code den lcd port
	rs=0;		
	rw=0;	
	en=1; 
	delay(1);
	en=0;	//tao canh xuong
}
//------------------------------
// hien thi chuoi ki tu ra lcd
void lcddis(char *p){
	while(*p!='\0'){
		lcddata(*p);
		delay(5);
		p++;
	}
}
//--------------------
//data mode lcd
void lcddata(char value)
{
	lcdport=value; 
	rs=1;	
	rw=0;	
	en=1;	
	delay(1);
	en=0;	//canh xuong
}
//------------------------
// ham phat am thanh
void sounder(){
	int i;
	for(i=0;i<5;i++){
		buzzer=1; //buzzer on
		delay(40);
		buzzer=0; //buzzer of
	}
}
//-------------------------
//ham mo cua
void door_open(){
	lcdcmd(0x01); //xoa hien thi man hinh lcd
	lcdcmd(0x02); // quay ve vi tri home
	lcddis("DANG MO CUA...");
	m1=1; //Motor on
	m2=0; //motor off
}
//--------------------------
//ham dong cua
void door_close(){
	lcdcmd(0x01); 
	lcdcmd(0x02);
	lcddis("DANG DONG CUA...");
	m1=0;
	m2=0;
	delay(20);
	m1=0;
	m2=1;
	delay(200);
	m1=0;
	m2=0;
	led_green=0;
	led_red=0;
}
//---------------------------
//ham am thanh ban phim
void bip_bip(){
	int i;
	for(i=0;i<5;i++){
		buzzer=1; //buzzer on
		delay(1);
		buzzer=0; //buzzer of
	}
}

//--------------------------
//ham nhan mot ki tu da duoc nhan
char scan_key(){
	char b='n';
	while(b=='n'){
		b=lcdkey();
	}
	if(b!='n'){
		bip_bip();
	}
	return b;
}

//--------------------------
//ham quet ki tu cua keypad
/*
r1----------1-----2-----3
			|	  |     |
r2----------4-----5-----6
			|	  |     |
r3----------7-----8-----9
			|	  |     |
r2----------*-----0-----#
			|	  |     |
c1~~~~~~~~~~|     |     |
c2~~~~~~~~~~~~~~~~|     |
c3~~~~~~~~~~~~~~~~~~~~~~|
*/
char lcdkey(){
	c1=c2=c3=1; 
	r1=r2=r3=r4=0; 
	
	//quet hang thu 1
	r1=0;
	r2=r3=r4=1;
	if(c1==0){
		delay_ms(50); //chong rung phim
		if(c1==0){
			lcddata('*');
			delay(2);
			return '1';
		}
		
	}
	if(c2==0){
		delay_ms(50); //chong rung phim
		if(c2==0){
			lcddata('*');
			delay(2);
			return '2';
			
		}
	}
	if(c3==0){
		delay_ms(50); //chong rung phim
		if(c3==0){
			lcddata('*');
			delay(2);
			return '3';
		}
	}

	//quet hang thu 2
	r2=0;
	r1=r3=r4=1;
	
	if(c1==0){
		delay_ms(50);
		if(c1==0){
			lcddata('*');
			delay(2);
			return '4';
		}
		
	}

	
	if(c2==0){
		delay_ms(50);
		if(c2==0){
			lcddata('*');
			delay(2);
			return '5';
		}
		
	}

	
	if(c3==0){
		delay_ms(50);
		if(c3==0){
			lcddata('*');
			delay(2);
			return '6';
		}
		
	}

	//quet dong thu 3
	r3=0;
	r1=r2=r4=1;
	
	if(c1==0){
		delay_ms(50);
		if(c1==0){
			lcddata('*');
			delay(2);
			return '7';
		}
		
	}
	
	if(c2==0){
		delay_ms(50);
		if(c2==0){
			lcddata('*');
			delay(2);
			return '8';
		}
		
	}
	if(c3==0){
		delay_ms(50);
		if(c3==0){
			lcddata('*');
			delay(2);
			return '9';
		}
		
	}

	//quet dong thu 4
	r4=0;
	r1=r2=r3=1;
	if(c1==0){
		delay_ms(50);
		if(c1==0){
			lcddata(' ');
			delay(2);
			return '*';
		}
		
	}

	if(c2==0){
		delay_ms(50);
		if(c2==0){
			lcddata('*');
			delay(2);
			return '0';
		}
		
	}


	if(c3==0){
		delay_ms(50);
		if(c3==0){
			lcddata(' ');
			delay(2);
			return '#';
		}
		
	}
	return 'n'; //n - no key
}
//-------------------