#include"view.h"

void init()
{
	// 默认当前目录为根目录
	CurDirPtr = (Entry *)malloc(sizeof(Entry));
	char root[11] = "root";
	char exitStr[5] = "exit";
	char cdStr[3] = "cd";
	char dmbrStr[4] = "mbr";
	char dirStr[4] = "dir";
	char treeStr[5] = "tree";
	char mdStr[3] = "md";
	char rdStr[3] = "rd";
	char typeStr[5] = "type";
	char copyStr[5] = "copy";
	char delStr[4] = "del";
	char seditStr[] = "sedit";
	char helpStr[] = "help";
	memcpy(CurDirPtr->DIR_NAME, root, sizeof(root));
	CurDirPtr->EntryBlockNum = ROOT_DIR_Offset;
	strncpy(ComSet[0], exitStr, sizeof(exitStr));
	strncpy(ComSet[1], cdStr, sizeof(cdStr));
	strncpy(ComSet[2], dmbrStr, sizeof(dmbrStr));
	strncpy(ComSet[3], dirStr, sizeof(dirStr));
	strncpy(ComSet[4], treeStr, sizeof(treeStr));
	strncpy(ComSet[5], mdStr, sizeof(mdStr));
	strncpy(ComSet[6], rdStr, sizeof(rdStr));
	strncpy(ComSet[7], typeStr, sizeof(typeStr));
	strncpy(ComSet[8], copyStr, sizeof(copyStr));
	strncpy(ComSet[9], delStr, sizeof(delStr));
	strncpy(ComSet[10], seditStr, sizeof(seditStr));
	strncpy(ComSet[11], helpStr, sizeof(helpStr));
	memcpy(&MBRInfo, ramFDD14 + BS_impBootLength, sizeof(MBRInfo));

	Factive_counter = 0; // 初始化活动文件表中的文件数量
	Fopen_counter = 0; // 初始化当前进程打开文件表中的文件数量
	//AllocClusList = NULL;
	//AllocClusNum = 0;
}

int LoadDisk(char * ptr, FILE * fp)
{
	int count = fread(ptr, 1, DISK_SIZE, fp);
	return count == DISK_SIZE;
}

int FreeDisk(char * ptr, FILE * fp)
{
	fseek(fp, 0, SEEK_SET);
	int count = fwrite(ptr, 1, DISK_SIZE, fp);

	return count == DISK_SIZE;
}
int ClusNum2BlockNum(int ClusNum)
{
	return ClusNum + 31;
}
int num2addr(int BlockNum)
{
	return BlockNum * BYTES_PER_BLOCK;
}

void ReadBlock(int BlockNum, char * buffer, char * ramFDD144)
{
	int address = num2addr(BlockNum);
	memcpy(buffer, &ramFDD144[address], BYTES_PER_BLOCK);
}

void WriteBlock(int BlockNum, char * buffer, char * ramFDD144)
{
	int address = num2addr(BlockNum);
	memcpy(&ramFDD144[address], buffer, BYTES_PER_BLOCK);
}

ushort FindFatClus(ushort ClusNum)
{
	ushort fatNext = 0x0fff;
	ushort ItemNum = ClusNum * 3 / 2;
	char fat[BYTES_PER_BLOCK];
	
	if (ClusNum % 2 == 1)
	{
		ReadBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
		fatNext = *(ushort*)(fat + ItemNum % BYTES_PER_BLOCK);
		fatNext = fatNext >> 4;
	}
	else
	{
		ReadBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
		fatNext = *(ushort*)(fat + ItemNum % BYTES_PER_BLOCK);
		fatNext = fatNext & 0x0fff;
	}
	//printf("%d ", fatNext);
	return fatNext;
}

void WriteFatClus(ushort ClusNum, ushort nextClusNum)
{
	ushort fatNext = 0x0fff;
	ushort ItemNum = ClusNum * 3 / 2;
	char fat[BYTES_PER_BLOCK];

	if (ClusNum % 2 == 1)
	{
		ReadBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
		nextClusNum = nextClusNum << 4;
		fatNext = *(ushort*)(fat + ItemNum % BYTES_PER_BLOCK);
		fatNext = fatNext & 0x000f;
		*(ushort*)(fat + ItemNum % BYTES_PER_BLOCK) = fatNext + nextClusNum;
		WriteBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
	}
	else
	{
		ReadBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
		nextClusNum = nextClusNum & 0x0fff;
		fatNext = *(ushort*)(fat + ItemNum % BYTES_PER_BLOCK);
		fatNext = fatNext & 0xf000;
		*(ushort*)(fat + ItemNum % BYTES_PER_BLOCK) = fatNext + nextClusNum;
		WriteBlock(FAT_Offset + ItemNum / BYTES_PER_BLOCK, fat, ramFDD14);
	}
	
}

ushort* FindFatClusList(ushort head, int * countPtr)
{
	if (head == 0)
	{
		(*countPtr) = 0;
		return NULL;
	}
	(*countPtr) = 1;
	ushort NextClus = FindFatClus(head);
	while (NextClus != 0x0fff)
	{
		(*countPtr)++;
		NextClus = FindFatClus(NextClus);
	}
	ushort* ClusList = (ushort*)malloc(sizeof(ushort)*(*countPtr));
	int i = 0;
	ClusList[i] = head;
	NextClus = FindFatClus(head);
	while (NextClus != 0x0fff)
	{
		ClusList[++i] = NextClus;
		NextClus = FindFatClus(NextClus);
	}
	
	
	return ClusList;
}

ushort FindNextEmptyClus()
{
	ushort ClusNum;
	//if (AllocClusNum == 0)
	//{
		ClusNum = 2;
		while (FindFatClus(ClusNum)!=0)
		{
			ClusNum++;
			if (ClusNum > MAX_CLUS_NUM)
			{
				return 0;
			}
		}
		return ClusNum;
	//}
	
	/*else
	{
		ClusNum = AllocClusList[0];
		AllocClusNum--;
		if (AllocClusNum == 0)
		{
			free(AllocClusList);
			AllocClusList = NULL;
		}
		else
		{
			ushort* temp = AllocClusList;
			AllocClusList = &AllocClusList[1];
			free(temp);
			temp = NULL;
		}
		return ClusNum;
	}*/
}

// 为一个簇块链添加簇块
void AddClus(ushort lastClusNum, ushort NewClusNum)
{
	WriteFatClus(lastClusNum, NewClusNum);
	WriteFatClus(NewClusNum, 0xfff);
}

int EntryNameIndex(int BlockNum, Entry e)
{
	char block[BYTES_PER_BLOCK];
	ReadBlock(BlockNum, block, ramFDD14);
	for (int i = 0; i < ENTRY_PER_BLOCK; i++)
	{
		if (strncmp(block + (i * BYTES_PER_ENTRY), e.DIR_NAME, 11) == 0)
		{
			
			return i;
		}
	}
	return -1;
}



void ChangeEntry(int BlockNum, Entry newEntry, int index)
{
	char block[BYTES_PER_BLOCK];
	ReadBlock(BlockNum, block, ramFDD14);	
	memcpy(block + (index * BYTES_PER_ENTRY), &newEntry, BYTES_PER_ENTRY);
	WriteBlock(BlockNum, block, ramFDD14);	
}



Entry* FindFather(ushort FatherClusNum, int curBlockNum)
{
	Entry* e = (Entry*)malloc(sizeof(Entry));
	if (FatherClusNum == 0)
	{
		
		uchar name[11] = "root";
		memcpy(e, name, sizeof(name));
		e->EntryBlockNum = ROOT_DIR_Offset;
		return e;
	}
	else
	{
		uchar block[BYTES_PER_BLOCK];
		ReadBlock(ClusNum2BlockNum(FatherClusNum), block, ramFDD14);
		ushort GrandpaClusNum = *(ushort*)(block + BYTES_PER_ENTRY + FIRST_CLUS_OFFSET);
		int GrandpaBlockNum;
		int MAX_Block;
		if (GrandpaClusNum == 0)
		{
			GrandpaBlockNum = ROOT_DIR_Offset;
			MAX_Block = MBRInfo.BPB_RootEntCnt / ENTRY_PER_BLOCK;
			for (int i = 0; i < MAX_Block; i++)
			{
				ReadBlock(GrandpaBlockNum + i, block, ramFDD14);
				for (int j = 0; j < ENTRY_PER_BLOCK; j++)
				{
					if (*(ushort*)(block + j * BYTES_PER_ENTRY + FIRST_CLUS_OFFSET) == FatherClusNum)
					{
						memcpy(e, block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
						e->EntryBlockNum = GrandpaBlockNum + i;
						return e;
					}
				}
			}
		}
		else
		{
			GrandpaBlockNum = ClusNum2BlockNum(GrandpaClusNum);
			ushort* ClusList  = FindFatClusList(GrandpaClusNum, &MAX_Block);
			for (int i = 0; i < MAX_Block; i++)
			{
				ReadBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);
				for (int j = 0; j < ENTRY_PER_BLOCK; j++)
				{
					if (*(ushort*)(block + j * BYTES_PER_ENTRY + FIRST_CLUS_OFFSET) == FatherClusNum)
					{
						memcpy(e, block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
						e->EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
						return e;
					}
				}
			}
			if (ClusList != NULL)
			{
				free(ClusList);
			}
		}
	}
}

int FindPath(char * _path, uchar attr, Entry * ans)
{
	
	char path[MAX_PATH];
	if (_path == NULL)
	{
		memcpy(ans, CurDirPtr, sizeof(Entry));
		return 1;
	}
	memcpy(path, _path, MAX_PATH);
	const char s[2] = "\\";
	ushort CurBlockNum;
	char * token = NULL;
	char root[11] = "root";
	Entry CurEntry;
	memset(&CurEntry, 0, sizeof(Entry));
	
	if (path[0] == '\\')
	{
		
		memcpy(CurEntry.DIR_NAME, root, sizeof(root));
		CurBlockNum = ROOT_DIR_Offset;
	}
	else
	{
		CurEntry = *(CurDirPtr);
		CurBlockNum = CurDirPtr->EntryBlockNum;
	}
	token = strtok(path, s);
	char * temp;
	while (token != NULL)
	{
		char Sec[BYTES_PER_BLOCK];
		int MaxEntry;
		ushort * ClusList = NULL;
		int isRoot = (strncmp(CurEntry.DIR_NAME, root, sizeof(root)) == 0);
		temp = strtok(NULL, s);
		if (isRoot)
		{
			MaxEntry = MBRInfo.BPB_RootEntCnt;
		}
		else
		{	
			int count = 0;
			ClusList = FindFatClusList(CurEntry.DIR_FstClus, &count);
			MaxEntry = ENTRY_PER_BLOCK * count;
		}
		
		// 遍历当前目录的盘块, 遍历每个盘块中的每一个目录项
		int isFound = 0;
		for (int i = 0; i < MaxEntry / ENTRY_PER_BLOCK; i++)
		{
			if (isRoot)
			{
				ReadBlock(ROOT_DIR_Offset + i, Sec, ramFDD14);
			}
			else
			{
				ReadBlock(ClusNum2BlockNum(ClusList[i]), Sec, ramFDD14);
			}
			
			for (int j = 0; j < ENTRY_PER_BLOCK; j++)
			{
			// 检查是否有匹配(检查文件名+属性)的目录项，如果找到，跳出循环，否则，路径非法，返回NULL。
				int addr = j * BYTES_PER_ENTRY; 
				int addr_attr = addr + ENTRY_Attr_Offset;
				// 如果是空的或者已删除则跳过。
				if ((*(Sec + addr)) == 0 || (*(uchar*)(Sec + addr)) == 0xE5)
				{
					continue;
				}
				
				// 检查文件名
				// 计算文件名长度
				int len;
				for (len = 0; len < 11; len++)
				{
					if (Sec[addr + len] == 0x20)
					{
						break;
					}
				}
				int eq = strncmp(token, Sec + addr, strlen(token)); 
				
				// 检查属性
				int check_attr = 1;
				if (temp == NULL) // 如果已经是最后一项
				{
					if (attr != Sec[addr_attr]) // 检查是否符合想找的目录项的属性
					{
						check_attr = 0;
					}
				}
				else
				{
					check_attr = (Sec[addr_attr] == 0x10); // 检查是否是目录文件
				}

				// 当前目录指向匹配的子目录。
				if (eq == 0 && check_attr)
				{
					isFound = 1;
					if (*(ushort*)token == 0x2E2E) // 如果是..
					{
						int curBlockNum;
						if (isRoot)
						{
							curBlockNum = ROOT_DIR_Offset + i;
						}
						else
						{
							curBlockNum = ClusNum2BlockNum(ClusList[i]);
						}
						ushort FatherClusNum = *(ushort*)(Sec + addr + FIRST_CLUS_OFFSET);
						memcpy(&CurEntry, FindFather(FatherClusNum, curBlockNum), sizeof(Entry));
					}
					else if (*(uchar*)token != 0x2E) // 如果不是..,也不是..
					{
						memcpy(&CurEntry, Sec + addr, BYTES_PER_ENTRY);
						if (isRoot)
						{
							CurEntry.EntryBlockNum = ROOT_DIR_Offset + i;
						}
						else
						{
							CurEntry.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
						}
					}
					break;
				}
			}
			if (isFound)
			{
				break;
			}
		}
		if (!isFound)
		{
			return 0;
		}
		token = temp;
		if (ClusList != NULL)
		{
			free(ClusList);
		}
	}
	memcpy(ans, &CurEntry, sizeof(Entry));
	return 1;
	
}





