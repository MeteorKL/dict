#include <graphics.h>
#include <string.h>

#define MOUSE_SIZE               16   // 16*16��
#define MOUSE_MOVE               0x01 // ����һЩ����
#define MOUSE_LEFT_BUTTON_DOWN   0x02
#define MOUSE_LEFT_BUTTON_UP     0x04
#define MOUSE_RIGHT_BUTTON_DOWN  0x08
#define MOUSE_RIGHT_BUTTON_UP    0x10
#define MOUSE_MIDDLE_BUTTON_DOWN 0x20
#define MOUSE_MIDDLE_BUTTON_UP   0x40
#define Esc 0x011b                    // ��ݼ��ĺ궨��
#define Up 0x4800
#define Down 0x5000
#define Left 0x4b00
#define Right 0x4d00
#define Enter 0x1c0d
#define Delete 0x0e08
#define PgDn 0x5100
#define PgUp 0x4900 
#define End 0x4f00  
#define Home 0x4700 
#define Kong 0x3920
#define Ins 0x5200 
#define Del 0x5300

typedef struct             // �ֵ�ṹ
{
	char entry[20];        /* ��Ŀ, �����19����ĸ*/
	long lib_offset;       /* ���ʽ��͵�ƫ����(��ƫ������ָ�ֵ������ļ�dict.lib�е�fseek����) */
	short int  xlat_len;   /* ���ʽ���ռ�õ��ֽ��� */
	short int  exam_len;   /* ��������ռ�õ��ֽ��� */
} IDX; 
char phonetic[11][16] = /* ��������ŵĵ����� */
{
   0x00,0x00,0x00,0x3C,0x66,0x60,0x60,0x38,
   0x60,0x60,0x60,0x66,0x3C,0x00,0x00,0x00, /* air */
   0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xCC,
   0x0C,0x0C,0x0C,0xCC,0x78,0x00,0x00,0x00, /* gargle */
   0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x66,
   0x1E,0x36,0x66,0x66,0x3E,0x00,0x00,0x00, /* out */
   0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x66,
   0x06,0x06,0x7E,0x66,0x3C,0x00,0x00,0x00, /* alone */
   0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x1C,
   0x1C,0x36,0x36,0x63,0x63,0x00,0x00,0x00, /* cup */
   0x00,0x00,0x00,0x3C,0x66,0x66,0x66,0x7E,
   0x7E,0x66,0x66,0x66,0x3C,0x00,0x00,0x00, /* thin */
   0x00,0x00,0x00,0x76,0x1C,0x7C,0x06,0x3E,
   0x66,0x66,0x66,0x66,0x3C,0x00,0x00,0x00, /* this */
   0x00,0x00,0x00,0x1C,0x30,0x30,0x30,0x30,
   0x30,0x30,0x30,0x30,0x30,0x30,0x30,0xE0, /* ship */
   0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x06,
   0x0C,0x18,0x3C,0x06,0x06,0x06,0x66,0x3C, /* treasure */
   0x00,0x00,0x00,0x00,0x00,0x00,0x6C,0x7E,
   0x66,0x66,0x66,0x66,0x06,0x06,0x66,0x3C, /* sing */
   0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0xBB,
   0x3B,0xFF,0xB8,0xBB,0xEE,0x00,0x00,0x00  /* lack */
};
word cursor_mask0[] = 
{  // ���ͼ�����0
   0x6000, /*0110000000000000*/
   0x7000, /*0111000000000000*/
   0x7800, /*0111100000000000*/
   0x7C00, /*0111110000000000*/
   0x7E00, /*0111111000000000*/
   0x7F00, /*0111111100000000*/
   0x7F80, /*0111111110000000*/
   0x7FC0, /*0111111111000000*/
   0x7FE0, /*0111111111100000*/
   0x7FF0, /*0111111111110000*/
   0x7FF8, /*0111111111111000*/
   0x7FFC, /*0111111111111100*/
   0x7E00, /*0111111000000000*/
   0x7800, /*0111100000000000*/
   0x7000, /*0111000000000000*/
   0x4000, /*0100000000000000*/
};
word cursor_mask1[] = 
{  // ���ͼ�����1
   0x0600, /*0000011000000000*/
   0x0600, /*0000011000000000*/
   0x0600, /*0000011000000000*/
   0x06DA, /*0000011011011010*/
   0x07FF, /*0000011111111111*/
   0xE7FF, /*1110011111111111*/
   0x77FF, /*0111011111111111*/
   0x7FFF, /*0111111111111111*/
   0x3FFF, /*0011111111111111*/
   0x3FFF, /*0011111111111111*/
   0x1FFF, /*0001111111111111*/
   0x1FFF, /*0001111111111111*/
   0x0FFE, /*0000111111111110*/
   0x0FFE, /*0000111111111110*/
   0x07FC, /*0000011111111100*/
   0x07FC, /*0000011111111100*/
};
word search_pic[] =
{  // ����ͼ��
   0x0000, /*0000000000000000*/
   0x1F00, /*0001111100000000*/
   0x3180, /*0011000110000000*/
   0x60C0, /*0110000011000000*/
   0xC060, /*1100000001100000*/
   0xC060, /*1100000001100000*/
   0xC060, /*1100000001100000*/
   0x60C0, /*0110000011000000*/
   0x31C0, /*0011000111000000*/
   0x1FE0, /*0001111111100000*/
   0x0070, /*0000000001110000*/
   0x0038, /*0000000000111000*/
   0x001C, /*0000000000011100*/
   0x000E, /*0000000000001110*/
   0x0006, /*0000000000000110*/
};
struct exp_words  // ��������г��ֵĵ���
{
   int x1,x2, y;  // ���ʿɵ����Χ��x1 - x2��y - y+19��
   char word[20]; // ���ʳ�������
} exp_word[100];  // ������������

void (far *old_mouse_event_handler)(int event, int x, int y); 
volatile int left_click=0,hide_mouse=0;                       // �������Ƿ���������Ƿ�����
struct mouse_cursor                                           // ���ṹ���������꣬�������λ�õı���ͼ
{
   int x, y;
   struct picture *old_img;
} mouse; 

void __loadds far mouse_event_handler(int event, int x, int y); // ������ж�
void init_mouse(int x,int y);									// ����ʼ��
void draw_mouse(int x, int y);									// �����
void recover_mouse_background(void);							// �ָ���걳��
void save_mouse_background(int x, int y);						// ������걳��
void move_mouse(int x, int y);									// �ƶ����
void close_mouse(void);				              			    // �ر����
IDX *pdic;                   // �ֵ����׸�����ָ��
int entries;                 // �ֵ��еĵ����ܸ���,��114940��
int entry_data[26][2];       // entry_data[i][0]: ('a'+i)��ĸ��ͷ���׸��������ֵ��е����  entry_data[i][1]: ('a'+i)��ĸ��ͷ�ĵ��ʸ���
char translation[2000];      // ����
char example[2000];          // ����
char input_string[20];       // ������ַ���
int possible_word[114941];   // ���ܵĵ���
struct picture *wordpic[26],*mousemode[4],*wordcover,*wordscover,*expcover,*tipscover;   // ת��ΪͼƬ�������
int choose_words=-1,choose_words_all=-1;  // ѡ��ĵ����ڵ�ǰҳ�����λ��ѡ��ĵ��������п��ܵ��ʵ���λ
int exp_n,exp_l;                          // �����е��ʵ�������ÿ�����ʵĳ��ȣ���¼���ʵ�ʱ���õ�
int mouse_type=0;                         // ���ͼ��0Ϊ��ͷ��1Ϊ��ָ
char remb_word[20][20];                   // ��¼������ѯ���ĵ��ʣ����20����ÿ�������20
int remb_x=-1,remb_n=0;                   // �鿴������ѯ���ĵ���ʱ  ��ǰ�鿴�ĵ��ʵ���λ���Ѽ�¼�Ĳ�ѯ���ĵ��ʵ�����
int press_enter=0;                        // �Ƿ�����ѯ�����ʣ� ���뵥�ʺ���enter/�����ѯͼ�� ���� �����ѯ�����еĵ��ʲ����ѯ���ʣ�
int i,m,type;                             // i����ǰ�����ַ����ĳ���  m����ѯ����ʱ�ķ���ֵ����ʾ�鵽�ĵ�������   type�����뵥�ʺ���enter/�����ѯͼ��ʱ��ȷ��������ģ������

void read_dic(IDX **ppdic)        // ���ֵ䣬��� entry_data[26][2]����� entry_data[26][2]��a��ͷ�ĵ�һ���������ļ����ǵڼ������ʣ�a��ͷ�ĵ����м���
{ 
	FILE *fp;
	int len;
	int i, begin;
	fp = fopen("dict.idx", "rb"); // ֻ����ʽ�򿪶������ļ�
	if(fp == NULL)                // û������
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't open the dict.idx");
		return ;
	}
	fseek(fp, 0, SEEK_END);       // �� ��: �ض�λ���ϵ��ļ�ָ��int fseek(FILE *stream, long offset, int fromwhere);���������ļ�ָ��stream��λ�á�
								  // ���ִ�гɹ���stream��ָ����fromwhereΪ��׼��ƫ��offset���ֽڵ�λ�á�
								  // ���ִ��ʧ��(����offset�����ļ������С)���򲻸ı�streamָ���λ�á��ɹ�������0�����򷵻�����ֵ��
	len = ftell(fp);              // ���ú��� ftell() Ҳ�ܷ����֪��һ���ļ��ĳ���fseek(fp, 0L,SEEK_END); len =ftell(fp); ���Ƚ��ļ��ĵ�ǰλ���Ƶ��ļ���ĩβ��Ȼ����ú���ftell()
	fseek(fp, 0, SEEK_SET);       // �� ��:ָ���ļ���ͷ
	entries = len / sizeof(IDX);  // �����ܸ��������ļ����ֽ���/ÿ�����ʽṹ��ռ���ֽ���
	pdic = malloc(entries*sizeof(IDX));
	if(pdic == NULL)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: not enough memory");
		return ;
	}
	fread(pdic, sizeof(IDX), entries, fp);					// pdic   �������ݵ��ڴ��ַ��
															// sizeof(IDX)   Ҫ��д���ֽ�������λ���ֽڣ�
															// entries   Ҫ���ж�д���ٸ�size�ֽڵ�������,ÿ��Ԫ����size�ֽڣ�
															// fp   ������,�ļ�ָ��
	fclose(fp);
	*ppdic = pdic;
	begin = 0;
	for(i=0; i<26; i++)
	{
		while(begin < entries)                              // �ҵ�������ĵ�һ����ĸΪ��ͷ���׸�����
		{
		 if((pdic[begin].entry[0] | 0x20) == 'a'+i)         // 0x20��д��Сд
			break;
		 begin++;                                           // ������ĵ�һ����ĸΪ��ͷ���׸��������ֵ��е����
		}
		entry_data[i][0] = begin;                           // �趨entry_data[i][0]Ϊ����ĸiΪ����ĸ�ĵ������ֵ��е����
		if(i>0)
		 entry_data[i-1][1] = begin - entry_data[i-1][0];   // ��¼�ֵ�����ǰһ����ĸΪ����ĸ�ĵ��ʵĸ���
	}
	entry_data[i-1][1] = entries - begin;                   // ��¼�ֵ��������һ����ĸΪ����ĸ�ĵ��ʵĸ�����entries �ֵ��еĵ����ܸ���
}

void draw_hz(int x,int y,byte *hz)                          // �������
{
	FILE *fp;
	word sixteen_dots;
	byte buf[16][2];
	int i, j;
	long offset;
	fp = fopen("hzk16", "rb");                              // ��16x16�����ֿ��ļ�
	if(fp == NULL)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't open the hzk16");
		return ;
	}
	offset = ((hz[0] - 0xA1) * 94 + (hz[1] - 0xA1)) * 0x20; // ����"��"�ֵ����ƫ��λ��
	fseek(fp, offset, SEEK_SET);                            // �ƶ��ļ�ָ�뵽��ƫ��λ��
	fread(buf, 1, 2*16, fp);                                // ��ȡ32�ֽڵ�������
	fclose(fp);
	for(i=0; i<16; i++)
	{
		sixteen_dots = buf[i][0]<<8 | buf[i][1];
		for(j=0; j<16; j++)
			if( sixteen_dots  &  1<<(15-j) )
				putpixel(x+j, y+i, BLACK);
	}
}

int putstring(char* string,int x,int y)                     // ��������е��ַ���
{
	byte *hz;
	int j,k,n,soundmark=0;
    char put_char[2],*p;
	hz=malloc(sizeof(byte)*2);
	for(n=0;n<strlen(string);n++)                           // һ��һ���ַ����
	{
        if(string[n]==91)                                   // �ӡ�[����ʼ�������
			soundmark=1;
		else if(string[n]==93)                              // �ӡ�]�������������
			soundmark=0;
		if(soundmark)                                       // []�м������������
		{
			if(string[n]>=47 && string[n]<=57)              // �����ascll�ַ�������
			{
				for(j=0; j<16; j++)
					for(k=0; k<8; k++)
						if(phonetic[string[n]-0x2F][j]  &  1<<(7-k))
							putpixel(x+k, y+j, BLACK);
				x+=8;
			}
			else if( (string[n]>=33 && string[n]<=126) ||string[n]==' ' || (string[n]>=1&&string[n]<=6)|| string[n]==31 || string[n]==59) // ���ascll�ַ�
			{
				put_char[0]=string[n];
				put_char[1]='\0';
				outtextxy(x,y,put_char);
				x=x+8;
			}
			else if(string[n]=='\r' && string[n+1]=='\n')                // ����
			{
				x=175;
				y+=20;
				n++;
			}
			else                                                         // �������
			{
				hz[0]=string[n];
				n++;
				hz[1]=string[n];
				draw_hz(x,y,hz);
				x+=16;
				if( x>=780 ||  ((string[n+1]|0x20)>=97 && (string[n+1]|0x20)<=122) )
				{
					x=175;
					y+=20;
				}
			}
		}
		else                                                             // �������
		{
			if( (string[n]>=33 && string[n]<=126) ||string[n]==' ')
			{
				put_char[0]=string[n];
				put_char[1]='\0';
				outtextxy(x,y,put_char);
				x=x+8;
				if(x>=780)                                               // ������ַ������ұ߽�
				{
					if( (string[n]|0x20)>=97 && (string[n]|0x20)<=122 )  // ������ַ�ΪӢ����ĸ
					{
						
						do
						{
						   x=x-8;
						   setfillstyle(SOLID_FILL,WHITE);
						   bar(x,y,x+7,y+19);
						   n--;
						}while( (string[n]|0x20)>=97 && (string[n]|0x20)<=122 );        // ��ΪӢ�ĵ���ʱ���������Ƶ���һ��
						n--;
					}
					x=175;
					y+=20;
				}
				else if( ( string[n]>=65 && string[n]<=90) || (string[n]>=97 && string[n]<=122) || string[n]==39 )  //�����ǰ����ַ��ǵ���
				{
					if( n>=1 && !(string[n-1]>=65 && string[n-1]<=90) && !(string[n-1]>=97 && string[n-1]<=122) && string[n-1]!=39 )  //��ʼ��¼����
					{
						exp_l=0;
						exp_word[exp_n].word[exp_l]=string[n]|0x20;
						exp_word[exp_n].x1=x-8;
						exp_word[exp_n].y=y;
						exp_l=1;
						if( !(string[n+1]>=65 && string[n+1]<=90) && !(string[n+1]>=97 && string[n+1]<=122) && string[n+1]!=39 )   //������¼������ĸ�ĵ���
						{
							exp_word[exp_n].word[exp_l]='\0';
							exp_word[exp_n].x2=x;
							exp_n++;
							exp_l=0;
						}
					}
					else if( exp_l>=1 && !(string[n+1]>=65 && string[n+1]<=90) && !(string[n+1]>=97 && string[n+1]<=122) && string[n+1]!=39 )  //������¼����
					{
						exp_word[exp_n].word[exp_l]=string[n];
						exp_l++;
						exp_word[exp_n].word[exp_l]='\0';
						exp_word[exp_n].x2=x;
						exp_n++;
						exp_l=0;
					}
					else if( exp_l>=1 )                          // ��¼����
					{
						exp_word[exp_n].word[exp_l]=string[n]|0x20;
						exp_l++;
					}
				}
			}
			else if(string[n]=='\r' && string[n+1]=='\n')        // ����
			{
				x=175;
				y+=20;
				n++;
			}
			else                                                 // ����
			{
				hz[0]=string[n];
				n++;
				hz[1]=string[n];
				draw_hz(x,y,hz);
				x+=16;
				if( x>=780 )
				{
					x=175;
					y+=20;
				}
			}
		}
	}
	return y;
}

void show_explination(int m)                          // ����ͽ���ҳ��
{
    FILE *fp;
	int y,j;
	exp_n=0;
	putimage(174,54, expcover,COPY_PUT);
	memset(example, 0, sizeof(example));
	memset(translation, 0, sizeof(translation));
	j=0;
	while(exp_word[j].x2!=-1)
	{
	    memset(exp_word[j].word, 0, sizeof(exp_word[j].word));
		exp_word[j].x2=-1;
        exp_word[j].y=-20;
		j++;
	}
	setcolor(BLACK);
	outtextxy(175,54,pdic[m].entry);
	fp = fopen("dict.lib", "rb");
	if(fp == NULL)                                    // û������
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't open the dict.lib");
		return ;
	}
	fseek(fp, pdic[m].lib_offset, SEEK_SET);
	fread(translation, 1, pdic[m].xlat_len, fp);
	fread(example, 1, pdic[m].exam_len, fp);
	fclose(fp);
	y=putstring(translation,175,84);
	putstring(example,175,y+30);
}

int precise_search(void)                              // ��ȷ����
{
	int l,m,r,k,y,j;
	char entry[20];
	l=0;
	r=entries;
	m=(l+r)/2;
	j=0;
	while(pdic[m].entry[j]!='\0')
	{
		entry[j] = pdic[m].entry[j] | 0x20;
		j++;
	}
	entry[j]='\0';
	while(strcmp(input_string,entry)!=0)
	{
		if(strcmp(input_string,entry)<0)
			r=m-1;
		else
		    l=m+1;
		if(l>r)
		    return 0;
		m=(l+r)/2;
		j=0;
        while(pdic[m].entry[j]!='\0')
		{
		    entry[j] = pdic[m].entry[j] | 0x20;
			j++;
		}
		entry[j]='\0';
	}
	show_explination(m);
	return 1;
}

int strcomp(char *src,char *dst)                      // ģ�������ĵ��ʱȽ� *��ʾ������ַ�������0���ַ���  ����ʾ�����ַ�
{
    int ret = *src - *dst;
	if(*src=='?')
	{
		if(!*dst)
			return ret = *src - *dst;
		do
		{
			if(!*dst)
				return ret = *src - *dst;
			++src;
			++dst;
		}while(*src=='?');
		ret = *src - *dst;
		if(!*dst&&*src!='*')
			return ret;
	}
	if(*src=='*')
	{
		do
		{
			if(*src=='?')
			{
				if(!*dst)
					return ret = *src - *dst;
				++dst;
			}
			++src;
		}while( *src=='?' || *src=='*');
		if(!*dst)
			return ret = *src - *dst;
		if(!*src)
			return 0;
		do
		{
			ret = *src - *dst;
			if( !ret )
			{
				if((ret=strcomp(src,dst))==0)
					return ret;
			}
			++dst;
		}while( *src && *dst );
	}
	while ( !ret && *src && *dst )
	{
		++src;
		++dst;
		ret = *src - *dst;
		if(*src=='?')
		{
		    if(!*dst)
				return ret = *src - *dst;
			do
		    {
				if(!*dst)
				    return ret = *src - *dst;
				++src;
				++dst;
		    }while(*src=='?');
			ret = *src - *dst;
			if(!*dst&&*src!='*')
				return ret;
		}
		if(*src=='*')
		{
		    do
		    {
				if(*src=='?')
				{
					if(!*dst)
						return ret = *src - *dst;
					++dst;
				}
				++src;
		    }while( *src=='?' || *src=='*');
			if(!*dst)
				return ret = *src - *dst;
			if(!*src)
			    return 0;
			do
			{
				ret = *src - *dst;
				if( !ret )
				{
				    if((ret=strcomp(src,dst))==0)
						return ret;
				}
				++dst;
			}while( *src && *dst );
		}
	}
    return ret;
} 

int vague_search(char* input_string)                     // ģ������
{
	FILE *fp;
	int y,j,k,n=0,c,*p;
	char *ptr, entry[20];
	memset(possible_word, -1, sizeof(possible_word));
	if(input_string[0]>='a'&&input_string[0]<='z')       // ����ĸ��ͷ
	{
		c=input_string[0]-'a';
		for(k=0;k<entry_data[c][1];k++)
		{
		    j=0;
			while(pdic[entry_data[c][0]+k].entry[j]!='\0')
			{
				entry[j] = pdic[entry_data[c][0]+k].entry[j] | 0x20;
				j++;
			}
			entry[j]='\0';
			if(strcomp(input_string,entry)==0)
			{
				if (n>=0&&n<=25)
				{
					setcolor(BLACK);
					outtextxy(3,75+20*n,pdic[entry_data[c][0]+k].entry);
					getimage(3,75+20*n,169,94+20*n, wordpic[n]);
				}
				possible_word[n]=entry_data[c][0]+k;
				n++;
			}
		}
	}
	else                                                 // �� ? * ��ͷ
	{
		for(k=0;k<entries;k++)
		{
			j=0;
			while(pdic[k].entry[j]!='\0')
			{
				entry[j] = pdic[k].entry[j] | 0x20;
				j++;
			}
			entry[j]='\0';
			if(strcomp(input_string,entry)==0)
			{
				if (n>=0&&n<=25)
				{
					setcolor(BLACK);
					outtextxy(3,75+20*n,pdic[k].entry);
					getimage(3,75+20*n,169,94+20*n, wordpic[n]);
				}
				possible_word[n]=k;
				n++;
			}
		}
	}
	setfillstyle(SOLID_FILL,WHITE);
	bar(32,55,137,74);
	setcolor(BLACK);
	outtextxy(74,55,"0/");
	itoa(n,ptr,10);
	outtextxy(86+4,55,ptr);
	return n;
}

void draw_pic(int x,int y,word *p,int color)         // ��ͼ��
{
   int k, j;
   for(k=0; k<15; k++)
		for(j=0; j<16; j++)
			if(p[k]  &  1<<(16-1-j))
				putpixel(x+j, y+k, color);
}

void remember(char* string)                          // ��¼������ĵ���
{
	strcpy(remb_word[remb_n],string);
	if(remb_n>=1)
		if(strcmp(remb_word[remb_n],remb_word[remb_n-1])==0)
		{
			memset(remb_word[remb_n], 0, sizeof(remb_word[remb_n]));
			return ;
		}
	remb_n++;
	if(remb_n==20)
		for(remb_n=0;remb_n<=18;remb_n++)
			strcpy(remb_word[remb_n],remb_word[remb_n+1]);
	else
		remb_x++;
}

void show_back_forword_word(int type)                // ����ǰ����ť����
{
    char input_show_word[20];
	int j;
	putimage(470,5, tipscover,COPY_PUT);
	if(type==0)
	    if(remb_x>=1)
		    remb_x--;
	    else
		{
			setcolor(RED);
			outtextxy(470,5,"Warning: no more former words!");
			return ;
		}
	if(type==1)
        if(remb_x<remb_n-1)
            remb_x++;
	    else
		{
			setcolor(RED);
			outtextxy(470,5,"Warning: no more latter words!");
			return ;
		}
	setfillstyle(SOLID_FILL,WHITE);
	putimage(210,5, wordcover,COPY_PUT);
	putimage(3,75, wordscover,COPY_PUT);
	bar(32,55,137,74);
	strcpy(input_string,remb_word[remb_x]);
	setcolor(BLACK);
	outtextxy(210,5,input_string);
	j=0;
	while(input_string[j]!='\0')
	{
		input_show_word[j]=input_string[j];
		j++;
	}
	input_show_word[j]='*';
	j++;
	input_show_word[j]='\0';
	m=vague_search(input_show_word);
	if(m==0)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't find it!");
	}
	else
	{
	    precise_search();
	}
	choose_words=-1;
	choose_words_all=-1;
	press_enter=1;
}

void init(void)                              // ���������ݵĳ�ʼ��
{
    int j;
	char *ptr;
	for(j=0;j<=25;j++)
	    wordpic[j]=malloc(imagesize(3,75,169,95));
	memset(possible_word, -1, sizeof(possible_word));
	memset(example, 0, sizeof(example));
    memset(translation, 0, sizeof(translation));
	for(j=0;j<20;j++)
		memset(remb_word[j], 0, sizeof(remb_word[j]));
	for(j=0;j<100;j++)
	{
	    memset(exp_word[j].word, 0, sizeof(exp_word[j].word));
		exp_word[j].x2=-1;
        exp_word[j].y=-20;
	}
	setfillstyle(SOLID_FILL,0x4F);
	bar(0,0,799,599);
	setfillstyle(SOLID_FILL,WHITE);
	bar(174,5,796,19);
	bar(3,25,169,595);
	bar(174,25,796,595);
	setfillstyle(SOLID_FILL,LIGHTGRAY);
	bar(420,5,423,19);
	bar(3,51,169,53);
	bar(174,51,796,53);
	setfillstyle(SOLID_FILL,0x35);
	bar(404,5,419,19);
	setfillstyle(SOLID_FILL,0x4F);
	bar(174,5,209,19);
	setcolor(WHITE);
	outtextxy(3,5,"Dictionary  By:Meteor");
	setcolor(BLACK);
	outtextxy(33,30,"possible words");
	outtextxy(385,30,"translations and examples");
	setcolor(RED);
	outtextxy(424,5,"tips:");
	setfillstyle(SOLID_FILL,WHITE);
	bar(32,55,86-5,74);
	setcolor(BLACK);
	outtextxy(74,55,"0/0");
	*ptr=17;
	*(ptr+1)='\0';
	setcolor(0x37);
	outtextxy(86-4-8*7,55,ptr);
	outtextxy(86-4-8*7-12,55,ptr);
	outtextxy(86-4-8*7-18,55,ptr);
	setcolor(0x34);
	outtextxy(180,5,ptr);
	*ptr=16;
	*(ptr+1)='\0';
	setcolor(0x37);
	outtextxy(86+4+8*6,55,ptr);
	outtextxy(86+4+8*6+12,55,ptr);
	outtextxy(86+4+8*6+18,55,ptr);
	setcolor(0x34);
	outtextxy(195,5,ptr);
	wordcover=malloc(imagesize(210,5,370,19));
	getimage(210,5,370,19, wordcover);
    wordscover=malloc(imagesize(3,75,169,594));
	getimage(3,75,169,594, wordscover);
	tipscover=malloc(imagesize(470,5,796,19));
	getimage(470,5,796,19, tipscover);
	expcover=malloc(imagesize(174,54,796,594));
	getimage(174,54,796,594, expcover);
}

void inputs(char input_char)              // �����ַ�����
{
	int j,input_show;
	char input_word[2],input_show_word[20];
	if(press_enter==1)
	{
		i=0;
		press_enter=0;
		memset(input_string, 0, sizeof(input_string));
		setfillstyle(SOLID_FILL,WHITE);
		putimage(210,5, wordcover,COPY_PUT);
		putimage(3,75, wordscover,COPY_PUT);
		bar(32,55,137,74);
	}
	choose_words=-1;
	choose_words_all=-1;
	if(i==19)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning! the word you input is too long");
	}
	else
	{
		input_word[0]=input_char;
		input_string[i]=input_char | 0x20;
		input_word[1]='\0';
		setcolor(BLACK);
		outtextxy(210+i*8,5,input_word);
		i++;
	}
	input_string[i]='\0';
	if(i<19)
	{
		input_show=0;
		j=0;
		while(input_string[j]!='\0')
		{
			if(input_string[j]=='*' || input_string[j]=='?')
				input_show=1;
			input_show_word[j]=input_string[j];
			j++;
		}
		setfillstyle(SOLID_FILL,WHITE);
		putimage(3,75, wordscover,COPY_PUT);
		bar(32,55,137,74);
		if(input_show==0)
		{
			input_show_word[j]='*';
			j++;
			input_show_word[j]='\0';
			m=vague_search(input_show_word);
		}
	}
}

void del_char(void)                         // ����ַ�����
{
	int j,input_show;
	char input_show_word[20];
	press_enter=0;
	choose_words=-1;
	choose_words_all=-1;
	putimage(470,5, tipscover,COPY_PUT);
	if(i>=1)
		i--;
	setfillstyle(SOLID_FILL,WHITE);
	bar(210+i*8,5,210+(i+1)*8,19);
	input_string[i]='\0';
	if(i<19)
	{
		input_show=0;
		j=0;
		while(input_string[j]!='\0')
		{
			if(input_string[j]=='*' || input_string[j]=='?')
				input_show=1;
			input_show_word[j]=input_string[j];
			j++;
		}
		if(input_show==0)
		{
			input_show_word[j]='*';
			j++;
			input_show_word[j]='\0';
			setfillstyle(SOLID_FILL,WHITE);
			putimage(3,75, wordscover,COPY_PUT);
			bar(32,55,137,74);
			m=vague_search(input_show_word);
		}
	}
}

void keypress_enter()                                                 // ����enter������ѯ���ʽ���
{
    int j;
	if(choose_words==-1)
	{
		type=0;
		j=0;
		while(input_string[j]!='\0')
		{
			if(input_string[j]=='*' || input_string[j]=='?')
			{
				type=1;
				break;
			}
			j++;
		}
		remember(input_string);
		if(type==0)
			m=precise_search();
		else
			m=vague_search(input_string);
		if(m==0)
		{
			putimage(470,5, tipscover,COPY_PUT);
			setcolor(RED);
			outtextxy(470,5,"Warning: can't find it!");
		}
		press_enter=1;
	}
}

void update_num(void)                                                 // ��ѡ�еĿ��ܵ��ʵ���λ��Ϣ����
{
	char *ptr;
	ptr=malloc(sizeof(int));
	setfillstyle(SOLID_FILL,WHITE);
	bar(32,55,86-5,74);
	itoa(choose_words_all+1,ptr,10);
	outtextxy(86-4-strlen(ptr)*8,55,ptr);
	free(ptr);
	ptr=NULL;
}

void mouse_hide(void)                                                 // �������
{
    hide_mouse=1;
	recover_mouse_background(); 
}

void mouse_recover(void)                                              // ���ָ�
{
    save_mouse_background(mouse.x, mouse.y);
	recover_mouse_background();
	hide_mouse=0;
	draw_mouse(mouse.x, mouse.y);
}

void keypress_down(void)                                                                          // ����Down�������ܵ����б�����
{
    int j;
	if(possible_word[choose_words_all+1]!=-1)
	{
		mouse_hide();
		choose_words_all++;
		if(choose_words+1<=25)
		{
			if(choose_words>=0)
			{
				remember(input_string);
				setfillstyle(SOLID_FILL,WHITE);
				bar(3,75+choose_words*20,169,94+choose_words*20);
				setcolor(BLACK);
				outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all-1]].entry);
			}
			choose_words++;
			setfillstyle(SOLID_FILL,78);
			bar(3,75+choose_words*20,169,94+choose_words*20);
			setcolor(BLACK);
			outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		}
		else
		{
			setfillstyle(SOLID_FILL,WHITE);
			bar(3,75+20*25,169,95+20*25);
			setcolor(BLACK);
			outtextxy(3,75+20*25,pdic[possible_word[choose_words_all-choose_words+24]].entry);
			for(j=0;j<=24;j++)
				getimage(3,95+20*j,169,115+20*j, wordpic[j]);
			setfillstyle(SOLID_FILL,WHITE);
			bar(3,75+20*25,169,95+20*25);
			setcolor(BLACK);
			outtextxy(3,75+20*25,pdic[possible_word[choose_words_all-choose_words+25]].entry);
			getimage(3,75+20*25,169,95+20*25, wordpic[25]);
			setfillstyle(SOLID_FILL,78);
			bar(3,75+20*25,169,95+20*25);
			outtextxy(3,75+20*25,pdic[possible_word[choose_words_all-choose_words+25]].entry);
			for(j=0;j<=24;j++)
				putimage(3,75+20*j, wordpic[j],COPY_PUT);
		}
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_up(void)                                                                         // ����Up�������ܵ����б�����
{
	int j;
	if(choose_words_all>0)
	{
		mouse_hide();
		choose_words_all--;
		if(choose_words-1>=0)
		{
			setfillstyle(SOLID_FILL,WHITE);
			bar(3,75+choose_words*20,169,95+choose_words*20);
			setcolor(BLACK);
			outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all+1]].entry);
			choose_words--;
			setfillstyle(SOLID_FILL,78);
			bar(3,75+choose_words*20,169,95+choose_words*20);
			setcolor(BLACK);
			outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		}
		else
		{
			setfillstyle(SOLID_FILL,WHITE);
			bar(3,75,169,95);
			setcolor(BLACK);
			outtextxy(3,75,pdic[possible_word[choose_words_all-choose_words+1]].entry);
			for(j=1;j<=25;j++)
				getimage(3,55+20*j,169,75+20*j, wordpic[j]);
			setfillstyle(SOLID_FILL,WHITE);
			bar(3,75,169,95);
			setcolor(BLACK);
			outtextxy(3,75,pdic[possible_word[choose_words_all-choose_words]].entry);
			getimage(3,75,169,95, wordpic[0]);
			setfillstyle(SOLID_FILL,78);
			bar(3,75,169,95);
			outtextxy(3,75,pdic[possible_word[choose_words_all-choose_words]].entry);
			for(j=1;j<=25;j++)
				putimage(3,75+20*j, wordpic[j],COPY_PUT);
		}
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_ins(void)                                                                               // ����Ins������ҳ�׵���
{
	if(choose_words_all>=0)
	{
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		choose_words_all-=choose_words;
		choose_words=0;
		setfillstyle(SOLID_FILL,78);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_del(void)                                                                 // ����Del������ҳĩ����
{
	if(choose_words_all>=0)
	{
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		choose_words_all=choose_words_all-choose_words+25;
		choose_words=25;
		setfillstyle(SOLID_FILL,78);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_pgup(void)                                                              // ����PgUp����������һҳ
{
    int j;
	if(choose_words_all>=0&&m>26&&choose_words_all-choose_words>=26)
	{
		mouse_hide();
		choose_words_all-=26;
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75,169,595);
		setcolor(BLACK);
		for(j=0;j<=25;j++)
		{
			outtextxy(3,75+20*j,pdic[possible_word[choose_words_all-choose_words+j]].entry);
			getimage(3,75+20*j,169,95+20*j, wordpic[j]);
		}
		setfillstyle(SOLID_FILL,78);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}
 
void keypress_pgdn(void)                                                   // ����PaDn��������һҳ
{
    int j;
	if(choose_words_all>=0&&m>26&&choose_words_all-choose_words<=m-26*2)
	{
		mouse_hide();
		choose_words_all+=26;
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75,169,595);
		setcolor(BLACK);
		for(j=0;j<=25;j++)
		{
			outtextxy(3,75+20*j,pdic[possible_word[choose_words_all-choose_words+j]].entry);
			getimage(3,75+20*j,169,95+20*j, wordpic[j]);
		}
		setfillstyle(SOLID_FILL,78);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_home(void)                                                 // ����HOME����������ҳ
{
    int j;
	if(m>26&&choose_words_all>=0&&choose_words_all!=0)
	{
		mouse_hide();
		choose_words=0;
		choose_words_all=0;
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75,169,595);
		setcolor(BLACK);
		for(j=0;j<=25;j++)
		{
			outtextxy(3,75+20*j,pdic[possible_word[j]].entry);
			getimage(3,75+20*j,169,95+20*j, wordpic[j]);
		}
		setfillstyle(SOLID_FILL,78);
		bar(3,75,169,95);
		setcolor(BLACK);
		outtextxy(3,75,pdic[possible_word[0]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void keypress_end(void)                                                 // ����end��������βҳ
{
    int j;
	if(m>26&&choose_words_all>=0&&choose_words_all!=m-1)
	{
		mouse_hide();
		choose_words=25;
		choose_words_all=m-1;
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75,169,595);
		setcolor(BLACK);
		for(j=0;j<=25;j++)
		{
			outtextxy(3,75+20*j,pdic[possible_word[choose_words_all-choose_words+j]].entry);
			getimage(3,75+20*j,169,95+20*j, wordpic[j]);
		}
		setfillstyle(SOLID_FILL,78);
		bar(3,75+20*25,169,95+20*25);
		setcolor(BLACK);
		outtextxy(3,75+20*25,pdic[possible_word[choose_words_all-choose_words+25]].entry);
		update_num();
		mouse_recover();
		show_explination(possible_word[choose_words_all]);
	}
}

void mouse_change(void)                                                                        // ����ͼ��仯
{
	int j,k;
	if(mouse.x>=404&&mouse.x<420&&mouse.y>=5&&mouse.y<=19)
		mouse_type=1;
	else if(mouse.x>=180&&mouse.x<188&&mouse.y>=5&&mouse.y<=19)
		mouse_type=1;
	else if(mouse.x>=195&&mouse.x<203&&mouse.y>=5&&mouse.y<=19)
		mouse_type=1;
	else if(mouse.x>=174&&mouse.x<=796&&mouse.y>=54&&mouse.y<=594)
	{
		k=0;
		for(j=0;j<exp_n;j++)
			if(mouse.x>=exp_word[j].x1&&mouse.x<exp_word[j].x2&&mouse.y>=exp_word[j].y&&mouse.y<exp_word[j].y+19)
			{
				mouse_type=1;
				k=1;
				break;
			}
		if(k==0)
			mouse_type=0;
	}
	else if((mouse.x>=3&&mouse.x<=169&&mouse.y>=75&&mouse.y<=75+20*25+19)&&possible_word[choose_words_all-choose_words+(mouse.y-75)/20]!=-1&&m>0)
		mouse_type=1;
	else if(mouse.x>=86-4-8*7-18&&mouse.x<86-4-8*7-4&&mouse.y>=55&&mouse.y<75)
		mouse_type=1;
	else if(mouse.x>=86-4-8*7&&mouse.x<32&&mouse.y>=55&&mouse.y<75)
		mouse_type=1;
	else if(mouse.x>=86+4+8*6&&mouse.x<86+4+8*7&&mouse.y>=55&&mouse.y<75)
		mouse_type=1;
	else if(mouse.x>=86+4+8*6+12&&mouse.x<86+4+8*6+26&&mouse.y>=55&&mouse.y<75)
		mouse_type=1;
	else
		mouse_type=0;
}

void search_exp_word(void)                                                 // �����ѯ�����еĵ���
{
	int j,k;
	char input_show_word[20];
	mouse_hide();
	for(k=0;k<exp_n;k++)
	{
		if(mouse.x>=exp_word[k].x1&&mouse.x<exp_word[k].x2&&mouse.y>=exp_word[k].y&&mouse.y<exp_word[k].y+19)
		{
			setfillstyle(SOLID_FILL,WHITE);
			putimage(210,5, wordcover,COPY_PUT);
			putimage(3,75, wordscover,COPY_PUT);
			bar(32,55,137,74);
			strcpy(input_string,exp_word[k].word);
			remember(input_string);
			setcolor(BLACK);
			outtextxy(210,5,input_string);
			j=0;
			while(input_string[j]!='\0')
			{
				input_show_word[j]=input_string[j];
				j++;
			}
			input_show_word[j]='*';
			j++;
			input_show_word[j]='\0';
			m=vague_search(input_show_word);
			precise_search();
			if(m==0)
			{
				putimage(470,5, tipscover,COPY_PUT);
				setcolor(RED);
				outtextxy(470,5,"Warning: can't find it!");
			}
			choose_words=-1;
			choose_words_all=-1;
			press_enter=1;
			break;
		}
	}
	mouse_recover();
}

void search_possible_word(void)                                                     // �����ѯ���ܵ����б��еĵ���                   
{
	mouse_hide();
	if(choose_words>=0)
	{
		setfillstyle(SOLID_FILL,WHITE);
		bar(3,75+20*choose_words,169,95+20*choose_words);
		setcolor(BLACK);
		outtextxy(3,75+20*choose_words,pdic[possible_word[choose_words_all]].entry);
	}
	else
	{
		remember(input_string);
	}
	putimage(174,54, expcover,COPY_PUT);
	choose_words_all=choose_words_all-choose_words+(mouse.y-75)/20;
	choose_words=(mouse.y-75)/20;
	setfillstyle(SOLID_FILL,78);
	bar(3,75+20*((mouse.y-75)/20),169,95+20*((mouse.y-75)/20));
	setcolor(BLACK);
	outtextxy(3,75+20*((mouse.y-75)/20),pdic[possible_word[choose_words_all]].entry);
	update_num();
	mouse_recover();
	show_explination(possible_word[choose_words_all]);
}

void mouse_click(void)                                                       // ������
{
	if(mouse.x>=404&&mouse.x<420&&mouse.y>=5&&mouse.y<=19)                   // ���������ť
		keypress_enter();
	else if(mouse.x>=180&&mouse.x<188&&mouse.y>=5&&mouse.y<=19)              // �鿴��ѯ���ĵ��ʣ���ǰ��ҳ
		show_back_forword_word(0);
	else if(mouse.x>=195&&mouse.x<203&&mouse.y>=5&&mouse.y<=19)              // �鿴��ѯ���ĵ��ʣ����ҳ
		show_back_forword_word(1);
	if(mouse.x>=174&&mouse.x<=796&&mouse.y>=54&&mouse.y<=594)                // �����ѯ�����еĵ���
	    search_exp_word();
	else if((mouse.x>=3&&mouse.x<=169&&mouse.y>=75&&mouse.y<=75+20*25+19)&&possible_word[choose_words_all-choose_words+(mouse.y-75)/20]!=-1&&m>0) // �����ѯ���ܵ����б��еĵ���
	    search_possible_word();
	else if(mouse.x>=86-4-8*7-18&&mouse.x<86-4-8*7-4&&mouse.y>=55&&mouse.y<75)      // ��ҳ  
	    keypress_home();														    
	else if(mouse.x>=86-4-8*7&&mouse.x<32&&mouse.y>=55&&mouse.y<75)				    // ǰһҳ
	    keypress_pgup();														      
	else if(mouse.x>=86+4+8*6&&mouse.x<86+4+8*7&&mouse.y>=55&&mouse.y<75)           // ��һҳ
	    keypress_pgdn();
	else if(mouse.x>=86+4+8*6+12&&mouse.x<86+4+8*6+26&&mouse.y>=55&&mouse.y<75)     // βҳ
		keypress_end();
}

int main(void)
{
	char input_char;
	long key;
	int quit=0,driver=0, mode=VESA_800x600x8bit;
    initgraph(&driver, &mode, "");           // ��ʼ��ͼ��ģʽ
	cleardevice();                           // ����
	init();                                  // ��ʼ��һЩ����
	init_mouse(_width/2,_height/2);          // ��ʼ����꣬��ʾ����Ļ����
	draw_pic(404,5,search_pic,0X37);         // ����������ť
	read_dic(&pdic);                         // ��ȡ�ֵ��ļ�
	i=0;                                     // ��ǰ������ַ������ȣ���ʼ��Ϊ0
	while (!quit)                            // ��û�а����˳�����ESC��ʱ
	{ 
		mouse_change();                      // �������ͼƬ�Ƿ�Ҫ�仯
		if(left_click==1)                    // ������������
		{
			left_click=0;
			mouse_click();                   // ����Ƿ�Ҫ����Ӧ����
		}
		if(bioskey(1))                       // ���а����ڻ�����ʱ
		{
			key = bioskey(0);
			input_char=key & 0xFF;           // �����ַ���ascll��Ϊ�����ĵ�8λ
			if( ((input_char>='a')&&(input_char<='z')) || (input_char>='A')&&(input_char<='Z') || input_char=='*' || input_char=='?'|| input_char==39)        // �����ַ�
				inputs(input_char);
			switch (key)
			{
				case Esc   : quit = 1;        break;       // �˳�
				case Enter : keypress_enter();break;       // ȷ��
				case Delete: del_char();      break;       // ɾ���ַ�
				case Down  : keypress_down(); break;       // ����
				case Up    : keypress_up();   break;       // ����
				case Ins   : keypress_ins();  break;       // ��ҳ���׵���
				case Del   : keypress_del();  break;       // ��ҳ��ĩ����
				case PgUp  : keypress_pgup(); break;       // ǰһҳ
				case PgDn  : keypress_pgdn(); break;       // ��һҳ
				case Home  : keypress_home(); break;       // ��ҳ
				case End   : keypress_end();  break;       // βҳ
			}
		}
	}
	close_mouse();
	free(pdic);
	pdic=NULL;
	free(mouse.old_img);
	mouse.old_img=NULL;
	cleardevice(); 
	closegraph();
	return 0;
}

void __loadds far mouse_event_handler(int event, int x, int y)	// ����¼�����
{																// ��������������������¼��Ļص��������������ɲ���ϵͳ�����ã���������main()��
																// ���á������״̬�����仯ʱ��������ƶ������Ҽ������»��ͷţ��򲻹ܵ�ǰCPU����
																// ʲô���飬����ϵͳ����Ҫ��CPU��ͣ�������Ĺ�������תȥ���ñ���������˱�������
																// �������жϴ�������ע����괦�����������__loadds��far���Σ�����__loadds��
																// ��ǿ�Ʊ������ڱ������Ŀ�ͷ���ɶ����ݶμĴ���DS���и�ֵ�Ĵ��룬��far��ǿ�Ʊ���
																// ���ڳ���ĩβ����retfԶ����ָ���Դ���retn������ָ�
   #pragma aux mouse_event_handler parm [EAX] [ECX] [EDX];		//#pragma aux�Ǳ���ָʾ���,���涨����event�ɼĴ���EAX����, x��ECX����, y��EDX����
   if(event & MOUSE_MOVE)
      if(hide_mouse==0)
		  move_mouse(x, y);
   if(event & MOUSE_LEFT_BUTTON_DOWN)
	  left_click=1;
}

void save_mouse_background(int x, int y)  // ������걳��
{
   getimage(x-4, y, x-4+MOUSE_SIZE-1, y+MOUSE_SIZE-1, mouse.old_img);
   mouse.x = x;
   mouse.y = y;
}

void draw_mouse(int x, int y)             // �����
{
   if(hide_mouse==0)
   {
	   if(mouse_type==0)
		   draw_pic(x-4,y,cursor_mask0,0x36);
	   else
		   draw_pic(x-4,y,cursor_mask1,0x2A);
   }
}

void recover_mouse_background(void)       // �ָ���걳��
{
   putimage(mouse.x-4, mouse.y, mouse.old_img, COPY_PUT);
}

void move_mouse(int x, int y)             // �ƶ����
{
   recover_mouse_background();
   save_mouse_background(x, y);
   draw_mouse(x, y);
}

void init_mouse(int x,int y)              // ����ʼ��
{
   union REGS r;
   struct SREGS sr;
   r.w.ax = 0;      /* check mouse driver */
   int86(0x33, &r, &r);
   if(r.w.ax != 0xFFFF)
      return ;
   r.w.ax = 0x0007; /* set horizontal range */
   r.w.cx = 4;
   r.w.dx = _width-9;
   int86(0x33, &r, &r);
   r.w.ax = 0x0008; /* set vertical range */
   r.w.cx = 0;
   r.w.dx = _height-1;
   int86(0x33, &r, &r);
   r.w.ax = 0x0004; /* position cursor */
   r.w.cx = x;
   r.w.dx = y;
   int86(0x33, &r, &r);
   r.w.ax = 2;      /* hide cursor */
   int86(0x33, &r, &r);
   r.w.ax = 0x0014; /* install mouse event handler */
   r.w.cx = MOUSE_MOVE | MOUSE_LEFT_BUTTON_DOWN | MOUSE_LEFT_BUTTON_UP |
            MOUSE_RIGHT_BUTTON_DOWN | MOUSE_RIGHT_BUTTON_UP; /* monitor these events */
   r.x.edx = FP_OFF(mouse_event_handler);
   memset(&sr, 0, sizeof(sr));
   sr.es   = FP_SEG(mouse_event_handler);
   int86x(0x33, &r, &r, &sr);
   old_mouse_event_handler = MK_FP(sr.es, r.x.edx);
   mouse.old_img = malloc(imagesize(0, 0, MOUSE_SIZE-1, MOUSE_SIZE-1));
   save_mouse_background(x, y);
   move_mouse(x, y);
}

void close_mouse(void)
{
   union REGS r;
   struct SREGS sr;
   r.w.ax = 0x0014; /* install mouse event handler */
   r.w.cx = 0;      // DO NOT monitor any events!!!
   r.x.edx = FP_OFF(old_mouse_event_handler);
   memset(&sr, 0, sizeof(sr));
   sr.es   = FP_SEG(old_mouse_event_handler);
   int86x(0x33, &r, &r, &sr);
}
