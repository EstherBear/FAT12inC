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

// MBR�е���Ϣ�ṹ
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

// Ŀ¼��ṹ
struct Entry {
	uchar DIR_NAME[11];
	uchar DIR_Attr;
	uchar reserve[10];
	ushort DIR_WrtTime;
	ushort DIR_WrtDate;
	ushort DIR_FstClus;
	uint DIR_FileSize;
	ushort EntryBlockNum; // Ŀ¼�������߼������ţ������޸�Ŀ¼��
};
typedef struct Entry Entry;

// ϵͳ��ļ�
struct ActiveFile {
	Entry f_entry;
	char path [MAX_PATH];
	int share_counter;
	int index;
}ACTIVE_FILES_LIST[MAX_active_FILES]; // ϵͳ��ļ���
int Factive_counter; // ϵͳ��ļ�����
typedef struct ActiveFile ActiveFile;

// ��ǰ���̴��ļ�
struct OpenFile {
	int posi;
	//int readorwrite;
	ActiveFile * af;
	int index;
}OPEN_FILES_LIST[MAX_open_FILES]; // ��ǰ���̴��ļ���
int Fopen_counter; //��ǰ���̴��ļ�����
typedef struct OpenFile OpenFile;


// global variable
char ComSet[12][MAX_COMMAND]; // ���
char ramFDD14[DISK_SIZE]; // ����
char input[MAX_INPUT]; // ������������
Entry * CurDirPtr; // ��ǰĿ¼ָ��
//ushort* AllocClusList; // �ɷ���ؿ���
//int AllocClusNum; // �ɷ���ؿ�������


// bottom(�ײ�ģ��)
int LoadDisk(char * ptr, FILE * fp); // ����.img�е�����
int FreeDisk(char * ptr, FILE * fp); // д�����ݵ�.img��

void ReadBlock(int BlockNum, char * buffer, char * ramFDD144); // ������
void WriteBlock(int BlockNum, char * buffer, char * ramFDD144); // д����

ushort FindFatClus(ushort ClusNum); // �ҵ�ǰ�غ�ָ�����һ���غ�
ushort* FindFatClusList(ushort head, int * countPtr); // �ҳ�head�����������ؿ���
ushort FindNextEmptyClus(); // �ҵ���һ�����дؿ�
void WriteFatClus(ushort ClusNum, ushort nextClusNum); // ��дFAT���е�ĳһ��
void AddClus(ushort lastClusNum, ushort NewClusNum); // Ϊһ��������һ���ؿ�

int FindPath(char * path, uchar attr, Entry * ans); // ·������
Entry* FindFather(ushort FatherClusNum, int curBlockNum); // FindPath�ĸ����������ҵ�..���Ӧ�ĸ�Ŀ¼������ֺ������߼�������

int ClusNum2BlockNum(int ClusNum); // �غ�->�߼�������

int EntryNameIndex(int BlockNum, Entry e); // ����Ŀ¼������Ӧ�߼�����������
void ChangeEntry(int BlockNum, Entry newEntry, int index); // �޸�Ŀ¼��

void init(); // ��ʼ������


// middle(�в�ģ��)
OpenFile* Open(char* path); // ���ļ�
int Close(OpenFile* fp); // �ر��ļ�

int Read(OpenFile * fp, char * buffer, int size); // ���ļ�
int Write(OpenFile * fp, char * buffer, int size); // д�ļ�
int Seek(OpenFile* fp, long offset, int origin); // ��λ��ǰλ��


// top(����ģ��)
void DisplayMBRInfo(); // չʾMBR�е���Ϣ

void DisplayDir(); // չʾ��ǰĿ¼�е�����Ŀ¼������ļ�����Ŀ¼
void DisplayDirItem(Entry* e); // DisplayDir()�ĸ�������������չʾÿһ��Ŀ¼���е�����

void DisplayTree(); // չʾ�����ļ�ϵͳ������Ŀ¼��
void display(int depth, Entry* e); // DisplayTree()�ĸ�������

int cd(char * path); // ����ĳ��Ŀ¼
int md(char * name, uchar attr, char* path); // �������ļ����߿�Ŀ¼
int rd(char * name, uchar attr, char* path);  // ɾ�����ļ����߿�Ŀ¼

int type(char* path); // ��ʾ�ļ�����
int copy(char**source, int count, char* destination); // �Ӽ��̻��������ļ��и������ݵ�һ�����л����½����ļ�
int sedit(char * path); // ���װ�ı༭�����ṩread, wirte, seek���ܱ༭�ļ�����

void CommandImp(); // �����û���������������غ���

void help(); // �ṩ���װ����ĵ�










