#define _CRT_SECURE_NO_WARNINGS
#pragma pack(1)
#define DISK_SIZE 512*2880
#define BS_impBootLength 3
#define BYTES_PER_BLOCK 512
#define MAX_open_FILES 10
#define MAX_active_FILES 100
#define MAX_PATH 100
#define ROOT_DIR_Offset (9+9+1)
#define FAT_Offset 1
#define ENTRY_Attr_Offset 11
#define BYTES_PER_ENTRY 32
#define ENTRY_PER_BLOCK 16
#define BLOCK_PER_FAT 9
#define MAX_CLUS_NUM 2848
#define MAX_INPUT 200
#define MAX_COMMAND 20
#define FIRST_CLUS_OFFSET 26
#define MAX_FROM_CON 1000
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
// I can do this all day.

typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;

// MBR中的信息结构
struct MBRInfo {
	char BS_OEMName[8];
	
	ushort BPB_BytsPerSec;
	uchar BPB_SecPerClus;
	ushort BPB_RsvdSecCnt;
	uchar BPB_NumFATs;
	ushort BPB_RootEntCnt;
	ushort BPB_TotSec16;
	uchar BPB_Media;
	ushort BPB_FATSz16;
	ushort BPB_SecPerTrk;
	ushort BPB_NumHeads;
	uint BPB_HiddSec;
	uint BPB_TotSec32;
	
	uchar BS_DrvNum;
	uchar BS_Reserved1;
	uchar BS_BootSig;
	uint BS_VolID;
	char BS_VolLab[11];
	char BS_FileSysType[8];
} MBRInfo;

// 目录项结构
struct Entry {
	uchar DIR_NAME[11];
	uchar DIR_Attr;
	uchar reserve[10];
	ushort DIR_WrtTime;
	ushort DIR_WrtDate;
	ushort DIR_FstClus;
	uint DIR_FileSize;
	ushort EntryBlockNum; // 目录项所在逻辑扇区号，方便修改目录项
};
typedef struct Entry Entry;

// 系统活动文件
struct ActiveFile {
	Entry f_entry;
	char path [MAX_PATH];
	int share_counter;
	int index;
}ACTIVE_FILES_LIST[MAX_active_FILES]; // 系统活动文件表
int Factive_counter; // 系统活动文件数量
typedef struct ActiveFile ActiveFile;

// 当前进程打开文件
struct OpenFile {
	int posi;
	//int readorwrite;
	ActiveFile * af;
	int index;
}OPEN_FILES_LIST[MAX_open_FILES]; // 当前进程打开文件表
int Fopen_counter; //当前进程打开文件数量
typedef struct OpenFile OpenFile;


// global variable
char ComSet[12][MAX_COMMAND]; // 命令集
char ramFDD14[DISK_SIZE]; // 软盘
char input[MAX_INPUT]; // 存放输入的命令
Entry * CurDirPtr; // 当前目录指针
//ushort* AllocClusList; // 可分配簇块链
//int AllocClusNum; // 可分配簇块链长度


// bottom(底层模块)
int LoadDisk(char * ptr, FILE * fp); // 加载.img中的数据
int FreeDisk(char * ptr, FILE * fp); // 写回数据到.img中

void ReadBlock(int BlockNum, char * buffer, char * ramFDD144); // 读扇区
void WriteBlock(int BlockNum, char * buffer, char * ramFDD144); // 写扇区

ushort FindFatClus(ushort ClusNum); // 找当前簇号指向的下一个簇号
ushort* FindFatClusList(ushort head, int * countPtr); // 找出head引导的整条簇块链
ushort FindNextEmptyClus(); // 找到下一个空闲簇块
void WriteFatClus(ushort ClusNum, ushort nextClusNum); // 填写FAT表中的某一项
void AddClus(ushort lastClusNum, ushort NewClusNum); // 为一条链增加一个簇块

int FindPath(char * path, uchar attr, Entry * ans); // 路径分析
Entry* FindFather(ushort FatherClusNum, int curBlockNum); // FindPath的辅助函数，找到..项对应的父目录项的名字和所在逻辑扇区号

int ClusNum2BlockNum(int ClusNum); // 簇号->逻辑扇区号

int EntryNameIndex(int BlockNum, Entry e); // 计算目录项在相应逻辑扇区的索引
void ChangeEntry(int BlockNum, Entry newEntry, int index); // 修改目录项

void init(); // 初始化函数


// middle(中层模块)
OpenFile* Open(char* path); // 打开文件
int Close(OpenFile* fp); // 关闭文件

int Read(OpenFile * fp, char * buffer, int size); // 读文件
int Write(OpenFile * fp, char * buffer, int size); // 写文件
int Seek(OpenFile* fp, long offset, int origin); // 定位当前位置


// top(顶层模块)
void DisplayMBRInfo(); // 展示MBR中的信息

void DisplayDir(); // 展示当前目录中的所有目录项，包括文件和子目录
void DisplayDirItem(Entry* e); // DisplayDir()的辅助函数，用来展示每一个目录相中的内容

void DisplayTree(); // 展示整个文件系统的整棵目录树
void display(int depth, Entry* e); // DisplayTree()的辅助函数

int cd(char * path); // 进入某个目录
int md(char * name, uchar attr, char* path); // 创建空文件或者空目录
int rd(char * name, uchar attr, char* path);  // 删除空文件或者空目录

int type(char* path); // 显示文件内容
int copy(char**source, int count, char* destination); // 从键盘或者其它文件中复制内容到一个已有或者新建的文件
int sedit(char * path); // 简易版的编辑器，提供read, wirte, seek功能编辑文件内容

void CommandImp(); // 处理用户输入的命令并调用相关函数

void help(); // 提供简易帮助文档










