#include <graphics.h>
#include <string.h>

#define MOUSE_SIZE               16   // 16*16点
#define MOUSE_MOVE               0x01 // 鼠标的一些参数
#define MOUSE_LEFT_BUTTON_DOWN   0x02
#define MOUSE_LEFT_BUTTON_UP     0x04
#define MOUSE_RIGHT_BUTTON_DOWN  0x08
#define MOUSE_RIGHT_BUTTON_UP    0x10
#define MOUSE_MIDDLE_BUTTON_DOWN 0x20
#define MOUSE_MIDDLE_BUTTON_UP   0x40
#define Esc 0x011b                    // 快捷键的宏定义
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

typedef struct             // 字典结构
{
	char entry[20];        /* 条目, 单词最长19个字母*/
	long lib_offset;       /* 单词解释的偏移量(该偏移量是指字典正文文件dict.lib中的fseek距离) */
	short int  xlat_len;   /* 单词解释占用的字节数 */
	short int  exam_len;   /* 单词例句占用的字节数 */
} IDX; 
char phonetic[11][16] = /* 各音标符号的点阵定义 */
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
{  // 鼠标图像点阵0
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
{  // 鼠标图像点阵1
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
{  // 搜索图标
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
struct exp_words  // 保存解释中出现的单词
{
   int x1,x2, y;  // 单词可点击范围：x1 - x2；y - y+19；
   char word[20]; // 单词长度限制
} exp_word[100];  // 单词数量限制

void (far *old_mouse_event_handler)(int event, int x, int y); 
volatile int left_click=0,hide_mouse=0;                       // 鼠标左键是否点击，鼠标是否隐藏
struct mouse_cursor                                           // 鼠标结构，包括坐标，鼠标所在位置的背景图
{
   int x, y;
   struct picture *old_img;
} mouse; 

void __loadds far mouse_event_handler(int event, int x, int y); // 鼠标类中断
void init_mouse(int x,int y);									// 鼠标初始化
void draw_mouse(int x, int y);									// 画鼠标
void recover_mouse_background(void);							// 恢复鼠标背景
void save_mouse_background(int x, int y);						// 保存鼠标背景
void move_mouse(int x, int y);									// 移动鼠标
void close_mouse(void);				              			    // 关闭鼠标
IDX *pdic;                   // 字典中首个单词指针
int entries;                 // 字典中的单词总个数,共114940个
int entry_data[26][2];       // entry_data[i][0]: ('a'+i)字母开头的首个单词在字典中的序号  entry_data[i][1]: ('a'+i)字母开头的单词个数
char translation[2000];      // 翻译
char example[2000];          // 解释
char input_string[20];       // 输入的字符串
int possible_word[114941];   // 可能的单词
struct picture *wordpic[26],*mousemode[4],*wordcover,*wordscover,*expcover,*tipscover;   // 转化为图片快速输出
int choose_words=-1,choose_words_all=-1;  // 选择的单词在当前页面的排位，选择的单词在所有可能单词的排位
int exp_n,exp_l;                          // 解释中单词的数量，每个单词的长度，记录单词的时候用到
int mouse_type=0;                         // 鼠标图像，0为箭头，1为手指
char remb_word[20][20];                   // 记录曾经查询过的单词，最多20个，每个长度最长20
int remb_x=-1,remb_n=0;                   // 查看曾经查询过的单词时  当前查看的单词的排位，已记录的查询过的单词的数量
int press_enter=0;                        // 是否曾查询过单词（ 输入单词后按下enter/点击查询图标 或者 点击查询解释中的单词才算查询单词）
int i,m,type;                             // i：当前输入字符串的长度  m：查询单词时的返回值，表示查到的单词数量   type：输入单词后按下enter/点击查询图标时精确搜索还是模糊搜索

void read_dic(IDX **ppdic)        // 打开字典，填充 entry_data[26][2]，填充 entry_data[26][2]，a开头的第一个单词在文件中是第几个单词，a开头的单词有几个
{ 
	FILE *fp;
	int len;
	int i, begin;
	fp = fopen("dict.idx", "rb"); // 只读方式打开二进制文件
	if(fp == NULL)                // 没有内容
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't open the dict.idx");
		return ;
	}
	fseek(fp, 0, SEEK_END);       // 功 能: 重定位流上的文件指针int fseek(FILE *stream, long offset, int fromwhere);函数设置文件指针stream的位置。
								  // 如果执行成功，stream将指向以fromwhere为基准，偏移offset个字节的位置。
								  // 如果执行失败(比如offset超过文件自身大小)，则不改变stream指向的位置。成功，返回0，否则返回其他值。
	len = ftell(fp);              // 利用函数 ftell() 也能方便地知道一个文件的长。fseek(fp, 0L,SEEK_END); len =ftell(fp); 首先将文件的当前位置移到文件的末尾，然后调用函数ftell()
	fseek(fp, 0, SEEK_SET);       // 功 能:指向文件开头
	entries = len / sizeof(IDX);  // 单词总个数等于文件总字节数/每个单词结构所占的字节数
	pdic = malloc(entries*sizeof(IDX));
	if(pdic == NULL)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: not enough memory");
		return ;
	}
	fread(pdic, sizeof(IDX), entries, fp);					// pdic   接收数据的内存地址；
															// sizeof(IDX)   要读写的字节数，单位是字节；
															// entries   要进行读写多少个size字节的数据项,每个元素是size字节；
															// fp   输入流,文件指针
	fclose(fp);
	*ppdic = pdic;
	begin = 0;
	for(i=0; i<26; i++)
	{
		while(begin < entries)                              // 找到以输入的第一个字母为开头的首个单词
		{
		 if((pdic[begin].entry[0] | 0x20) == 'a'+i)         // 0x20大写变小写
			break;
		 begin++;                                           // 以输入的第一个字母为开头的首个单词在字典中的序号
		}
		entry_data[i][0] = begin;                           // 设定entry_data[i][0]为以字母i为首字母的单词在字典中的序号
		if(i>0)
		 entry_data[i-1][1] = begin - entry_data[i-1][0];   // 纪录字典中以前一个字母为首字母的单词的个数
	}
	entry_data[i-1][1] = entries - begin;                   // 纪录字典中以最后一个字母为首字母的单词的个数；entries 字典中的单词总个数
}

void draw_hz(int x,int y,byte *hz)                          // 输出中文
{
	FILE *fp;
	word sixteen_dots;
	byte buf[16][2];
	int i, j;
	long offset;
	fp = fopen("hzk16", "rb");                              // 打开16x16点阵字库文件
	if(fp == NULL)
	{
		putimage(470,5, tipscover,COPY_PUT);
		setcolor(RED);
		outtextxy(470,5,"Warning: can't open the hzk16");
		return ;
	}
	offset = ((hz[0] - 0xA1) * 94 + (hz[1] - 0xA1)) * 0x20; // 计算"我"字点阵的偏移位置
	fseek(fp, offset, SEEK_SET);                            // 移动文件指针到该偏移位置
	fread(buf, 1, 2*16, fp);                                // 读取32字节点阵数据
	fclose(fp);
	for(i=0; i<16; i++)
	{
		sixteen_dots = buf[i][0]<<8 | buf[i][1];
		for(j=0; j<16; j++)
			if( sixteen_dots  &  1<<(15-j) )
				putpixel(x+j, y+i, BLACK);
	}
}

int putstring(char* string,int x,int y)                     // 输出解释中的字符串
{
	byte *hz;
	int j,k,n,soundmark=0;
    char put_char[2],*p;
	hz=malloc(sizeof(byte)*2);
	for(n=0;n<strlen(string);n++)                           // 一个一个字符输出
	{
        if(string[n]==91)                                   // 从‘[’开始输出音标
			soundmark=1;
		else if(string[n]==93)                              // 从‘]’结束输出音标
			soundmark=0;
		if(soundmark)                                       // []中间音标内容输出
		{
			if(string[n]>=47 && string[n]<=57)              // 输出非ascll字符的音标
			{
				for(j=0; j<16; j++)
					for(k=0; k<8; k++)
						if(phonetic[string[n]-0x2F][j]  &  1<<(7-k))
							putpixel(x+k, y+j, BLACK);
				x+=8;
			}
			else if( (string[n]>=33 && string[n]<=126) ||string[n]==' ' || (string[n]>=1&&string[n]<=6)|| string[n]==31 || string[n]==59) // 输出ascll字符
			{
				put_char[0]=string[n];
				put_char[1]='\0';
				outtextxy(x,y,put_char);
				x=x+8;
			}
			else if(string[n]=='\r' && string[n+1]=='\n')                // 换行
			{
				x=175;
				y+=20;
				n++;
			}
			else                                                         // 输出中文
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
		else                                                             // 其他输出
		{
			if( (string[n]>=33 && string[n]<=126) ||string[n]==' ')
			{
				put_char[0]=string[n];
				put_char[1]='\0';
				outtextxy(x,y,put_char);
				x=x+8;
				if(x>=780)                                               // 输出的字符到达右边界
				{
					if( (string[n]|0x20)>=97 && (string[n]|0x20)<=122 )  // 输出的字符为英文字母
					{
						
						do
						{
						   x=x-8;
						   setfillstyle(SOLID_FILL,WHITE);
						   bar(x,y,x+7,y+19);
						   n--;
						}while( (string[n]|0x20)>=97 && (string[n]|0x20)<=122 );        // 当为英文单词时整个单词移到下一行
						n--;
					}
					x=175;
					y+=20;
				}
				else if( ( string[n]>=65 && string[n]<=90) || (string[n]>=97 && string[n]<=122) || string[n]==39 )  //如果当前输出字符是单词
				{
					if( n>=1 && !(string[n-1]>=65 && string[n-1]<=90) && !(string[n-1]>=97 && string[n-1]<=122) && string[n-1]!=39 )  //开始记录单词
					{
						exp_l=0;
						exp_word[exp_n].word[exp_l]=string[n]|0x20;
						exp_word[exp_n].x1=x-8;
						exp_word[exp_n].y=y;
						exp_l=1;
						if( !(string[n+1]>=65 && string[n+1]<=90) && !(string[n+1]>=97 && string[n+1]<=122) && string[n+1]!=39 )   //结束记录单个字母的单词
						{
							exp_word[exp_n].word[exp_l]='\0';
							exp_word[exp_n].x2=x;
							exp_n++;
							exp_l=0;
						}
					}
					else if( exp_l>=1 && !(string[n+1]>=65 && string[n+1]<=90) && !(string[n+1]>=97 && string[n+1]<=122) && string[n+1]!=39 )  //结束记录单词
					{
						exp_word[exp_n].word[exp_l]=string[n];
						exp_l++;
						exp_word[exp_n].word[exp_l]='\0';
						exp_word[exp_n].x2=x;
						exp_n++;
						exp_l=0;
					}
					else if( exp_l>=1 )                          // 记录单词
					{
						exp_word[exp_n].word[exp_l]=string[n]|0x20;
						exp_l++;
					}
				}
			}
			else if(string[n]=='\r' && string[n+1]=='\n')        // 换行
			{
				x=175;
				y+=20;
				n++;
			}
			else                                                 // 汉字
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

void show_explination(int m)                          // 翻译和解释页面
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
	if(fp == NULL)                                    // 没有内容
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

int precise_search(void)                              // 精确搜索
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

int strcomp(char *src,char *dst)                      // 模糊搜索的单词比较 *表示任意个字符（包括0个字符）  ？表示单个字符
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

int vague_search(char* input_string)                     // 模糊搜索
{
	FILE *fp;
	int y,j,k,n=0,c,*p;
	char *ptr, entry[20];
	memset(possible_word, -1, sizeof(possible_word));
	if(input_string[0]>='a'&&input_string[0]<='z')       // 以字母开头
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
	else                                                 // 以 ? * 开头
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

void draw_pic(int x,int y,word *p,int color)         // 画图标
{
   int k, j;
   for(k=0; k<15; k++)
		for(j=0; j<16; j++)
			if(p[k]  &  1<<(16-1-j))
				putpixel(x+j, y+k, color);
}

void remember(char* string)                          // 记录输入过的单词
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

void show_back_forword_word(int type)                // 后退前进按钮按下
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

void init(void)                              // 背景，数据的初始化
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

void inputs(char input_char)              // 输入字符函数
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

void del_char(void)                         // 清除字符函数
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

void keypress_enter()                                                 // 按下enter键，查询单词解释
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

void update_num(void)                                                 // 所选中的可能单词的排位信息更新
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

void mouse_hide(void)                                                 // 鼠标隐藏
{
    hide_mouse=1;
	recover_mouse_background(); 
}

void mouse_recover(void)                                              // 鼠标恢复
{
    save_mouse_background(mouse.x, mouse.y);
	recover_mouse_background();
	hide_mouse=0;
	draw_mouse(mouse.x, mouse.y);
}

void keypress_down(void)                                                                          // 按下Down键，可能单词列表下移
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

void keypress_up(void)                                                                         // 按下Up键，可能单词列表上移
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

void keypress_ins(void)                                                                               // 按下Ins键，本页首单词
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

void keypress_del(void)                                                                 // 按下Del键，本页末单词
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

void keypress_pgup(void)                                                              // 按下PgUp键，到达上一页
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
 
void keypress_pgdn(void)                                                   // 按下PaDn键，到下一页
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

void keypress_home(void)                                                 // 按下HOME键，到达首页
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

void keypress_end(void)                                                 // 按下end键，到达尾页
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

void mouse_change(void)                                                                        // 鼠标的图像变化
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

void search_exp_word(void)                                                 // 点击查询解释中的单词
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

void search_possible_word(void)                                                     // 点击查询可能单词列表中的单词                   
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

void mouse_click(void)                                                       // 左键点击
{
	if(mouse.x>=404&&mouse.x<420&&mouse.y>=5&&mouse.y<=19)                   // 点击搜索按钮
		keypress_enter();
	else if(mouse.x>=180&&mouse.x<188&&mouse.y>=5&&mouse.y<=19)              // 查看查询过的单词，向前翻页
		show_back_forword_word(0);
	else if(mouse.x>=195&&mouse.x<203&&mouse.y>=5&&mouse.y<=19)              // 查看查询过的单词，向后翻页
		show_back_forword_word(1);
	if(mouse.x>=174&&mouse.x<=796&&mouse.y>=54&&mouse.y<=594)                // 点击查询解释中的单词
	    search_exp_word();
	else if((mouse.x>=3&&mouse.x<=169&&mouse.y>=75&&mouse.y<=75+20*25+19)&&possible_word[choose_words_all-choose_words+(mouse.y-75)/20]!=-1&&m>0) // 点击查询可能单词列表中的单词
	    search_possible_word();
	else if(mouse.x>=86-4-8*7-18&&mouse.x<86-4-8*7-4&&mouse.y>=55&&mouse.y<75)      // 首页  
	    keypress_home();														    
	else if(mouse.x>=86-4-8*7&&mouse.x<32&&mouse.y>=55&&mouse.y<75)				    // 前一页
	    keypress_pgup();														      
	else if(mouse.x>=86+4+8*6&&mouse.x<86+4+8*7&&mouse.y>=55&&mouse.y<75)           // 后一页
	    keypress_pgdn();
	else if(mouse.x>=86+4+8*6+12&&mouse.x<86+4+8*6+26&&mouse.y>=55&&mouse.y<75)     // 尾页
		keypress_end();
}

int main(void)
{
	char input_char;
	long key;
	int quit=0,driver=0, mode=VESA_800x600x8bit;
    initgraph(&driver, &mode, "");           // 初始化图形模式
	cleardevice();                           // 清屏
	init();                                  // 初始化一些数据
	init_mouse(_width/2,_height/2);          // 初始化鼠标，显示在屏幕中央
	draw_pic(404,5,search_pic,0X37);         // 绘制搜索按钮
	read_dic(&pdic);                         // 读取字典文件
	i=0;                                     // 当前输入的字符串长度，初始化为0
	while (!quit)                            // 当没有按下退出键（ESC）时
	{ 
		mouse_change();                      // 检测鼠标的图片是否要变化
		if(left_click==1)                    // 若鼠标左键单击
		{
			left_click=0;
			mouse_click();                   // 检测是否要做相应动作
		}
		if(bioskey(1))                       // 当有按键在缓存中时
		{
			key = bioskey(0);
			input_char=key & 0xFF;           // 输入字符的ascll码为按键的低8位
			if( ((input_char>='a')&&(input_char<='z')) || (input_char>='A')&&(input_char<='Z') || input_char=='*' || input_char=='?'|| input_char==39)        // 输入字符
				inputs(input_char);
			switch (key)
			{
				case Esc   : quit = 1;        break;       // 退出
				case Enter : keypress_enter();break;       // 确认
				case Delete: del_char();      break;       // 删除字符
				case Down  : keypress_down(); break;       // 下移
				case Up    : keypress_up();   break;       // 上移
				case Ins   : keypress_ins();  break;       // 该页面首单词
				case Del   : keypress_del();  break;       // 该页面末单词
				case PgUp  : keypress_pgup(); break;       // 前一页
				case PgDn  : keypress_pgdn(); break;       // 后一页
				case Home  : keypress_home(); break;       // 首页
				case End   : keypress_end();  break;       // 尾页
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

void __loadds far mouse_event_handler(int event, int x, int y)	// 鼠标事件函数
{																// 本函数是用来处理鼠标事件的回调函数，即它是由操作系统来调用，而不是由main()来
																// 调用。当鼠标状态发生变化时，如鼠标移动、左右键被按下或释放，则不管当前CPU在做
																// 什么事情，操作系统都会要求CPU暂停正在做的工作，而转去调用本函数。因此本函数非
																// 常类似中断处理函数。注意鼠标处理函数必须加上__loadds及far修饰，其中__loadds用
																// 来强制编译器在本函数的开头生成对数据段寄存器DS进行赋值的代码，而far则强制编译
																// 器在程序末尾生成retf远返回指令以代替retn近返回指令。
   #pragma aux mouse_event_handler parm [EAX] [ECX] [EDX];		//#pragma aux是编译指示语句,它规定参数event由寄存器EAX传递, x由ECX传递, y由EDX传递
   if(event & MOUSE_MOVE)
      if(hide_mouse==0)
		  move_mouse(x, y);
   if(event & MOUSE_LEFT_BUTTON_DOWN)
	  left_click=1;
}

void save_mouse_background(int x, int y)  // 保存鼠标背景
{
   getimage(x-4, y, x-4+MOUSE_SIZE-1, y+MOUSE_SIZE-1, mouse.old_img);
   mouse.x = x;
   mouse.y = y;
}

void draw_mouse(int x, int y)             // 画鼠标
{
   if(hide_mouse==0)
   {
	   if(mouse_type==0)
		   draw_pic(x-4,y,cursor_mask0,0x36);
	   else
		   draw_pic(x-4,y,cursor_mask1,0x2A);
   }
}

void recover_mouse_background(void)       // 恢复鼠标背景
{
   putimage(mouse.x-4, mouse.y, mouse.old_img, COPY_PUT);
}

void move_mouse(int x, int y)             // 移动鼠标
{
   recover_mouse_background();
   save_mouse_background(x, y);
   draw_mouse(x, y);
}

void init_mouse(int x,int y)              // 鼠标初始化
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
