#include"view.h"

// 对输入的命令处理
void CommandImp()
{
	const char s[2] = " ";
	char * token;
	token = strtok(input, s);

	if (strncmp(token, ComSet[1], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		cd(token);
	}
	else if (strncmp(token, ComSet[2], strlen(input)) == 0)
	{
		DisplayMBRInfo(ramFDD14);
	}
	else if (strncmp(token, ComSet[3], strlen(input)) == 0)
	{
		DisplayDir();
	}
	else if (strncmp(token, ComSet[4], strlen(input)) == 0)
	{
		DisplayTree();
	}
	else if (strncmp(token, ComSet[5], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		char ch = '\\';
		char * path = strrchr(token, ch);
		if (path == NULL)
		{
			md(token, 0x10, NULL);
		}
		else
		{
			int len = path - token;
			char * name = path + 1;
			path = (char *)malloc(sizeof(char) * MAX_PATH);
			memset(path, 0, sizeof(path));
			memcpy(path, token, len);
			path[len] = '\0';
			md(name, 0x10, path);
			free(path);
		}
	}
	else if (strncmp(token, ComSet[6], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		char ch = '\\';
		char * path = strrchr(token, ch);
		if (path == NULL)
		{
			rd(token, 0x10, NULL);
		}
		else
		{
			int len = path - token;
			char * name = path + 1;
			path = (char *)malloc(sizeof(char) * MAX_PATH);
			memset(path, 0, sizeof(path));
			memcpy(path, token, len);
			path[len] = '\0';
			rd(name, 0x10, path);
			free(path);
		}
	}
	else if (strncmp(token, ComSet[7], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		type(token);
	}
	else if (strncmp(token, ComSet[8], strlen(input)) == 0)
	{
		const char plus[2] = "+";
		uchar * srcs = strtok(NULL, s);
		if (srcs == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		uchar * des = strtok(NULL, s);
		if (des == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		uchar * src = strtok(srcs, plus);
		uchar ** source = (uchar**)malloc(sizeof(uchar *) * 9);
		for (int i = 0; i < 9; i++)
		{
			source[i] = NULL;
		}
		int count = 0;
		while (src != NULL)
		{	
			if (count >= 9)
			{
				printf("The maximun file you can copy is 9.\n");
				break;
			}
			source[count] = (uchar*)malloc(sizeof(uchar) * (strlen(src)+1));
			strncpy(source[count], src, strlen(src));
			source[count][strlen(src)] = '\0';
			count++;
			src = strtok(NULL, plus);
		}
		copy(source, count, des);
		for (int i = 0; i < 9; i++)
		{
			if (source[i] != NULL)
			{
				free(source[i]);
			}
		}
		free(source);
		
	}
	else if (strncmp(token, ComSet[9], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		char ch = '\\';
		char * path = strrchr(token, ch);
		if (path == NULL)
		{
			rd(token, 0x20, NULL);
		}
		else
		{
			int len = path - token;
			char * name = path + 1;
			path = (char *)malloc(sizeof(char) * MAX_PATH);
			memset(path, 0, sizeof(path));
			memcpy(path, token, len);
			path[len] = '\0';
			rd(name, 0x20, path);
			free(path);
		}
	}
	else if (strncmp(token, ComSet[10], strlen(input)) == 0)
	{
		token = strtok(NULL, s);
		if (token == NULL)
		{
			printf("Too few patameters.\n");
			return;
		}
		sedit(token);
	}
	else if (strncmp(token, ComSet[11], strlen(input)) == 0)
	{
		help();
	}
	else
	{
		printf("The command is invalid.\n");
	}
}

// 展示MBR中的信息
void DisplayMBRInfo()
{
	printf("\nStart to display MBR information:\n");
	
	char buffer[20];

	memcpy(buffer, MBRInfo.BS_OEMName, 8);
	buffer[8] = 0;

	printf("BS_OEMName:         %s\n", buffer);
	printf("BPB_BytesPerSec:    %u\n", MBRInfo.BPB_BytsPerSec);
	printf("BPB_SecPerClus:     %u\n", MBRInfo.BPB_SecPerClus);
	printf("BPB_RsvdSecCnt:     %u\n", MBRInfo.BPB_RsvdSecCnt);
	printf("BPB_NumFATs:        %u\n", MBRInfo.BPB_NumFATs);
	printf("BPB_RootEntCnt:     %u\n", MBRInfo.BPB_RootEntCnt);
	printf("BPB_TotSec16:       %u\n", MBRInfo.BPB_TotSec16);
	printf("BPB_Media:          0x%02x\n", MBRInfo.BPB_Media);
	printf("BPB_FATSz16:        %u\n", MBRInfo.BPB_FATSz16);
	printf("BPB_SecPerTrk:      %u\n", MBRInfo.BPB_SecPerTrk);
	printf("BPB_NumHeads:       %u\n", MBRInfo.BPB_NumHeads);
	printf("BPB_HiddSec:        %u\n", MBRInfo.BPB_HiddSec);
	printf("BPB_TotSec32:       %u\n", MBRInfo.BPB_TotSec32);
	printf("BS_DrvNum:          %u\n", MBRInfo.BS_DrvNum);
	printf("BS_Reserved1:       %u\n", MBRInfo.BS_Reserved1);
	printf("BS_BootSig:         %u\n", MBRInfo.BS_BootSig);
	printf("BS_VolID:           %u\n", MBRInfo.BS_VolID);

	memcpy(buffer, MBRInfo.BS_VolLab, 11);
	buffer[11] = 0;
	printf("BS_VolLab:          %s\n", buffer);

	memcpy(buffer, MBRInfo.BS_FileSysType, 8);
	buffer[11] = 0;
	printf("BS_FileSysType:     %s\n", buffer);
	printf("\n");
}

// 进入目录，path可以是某个目录的相对or绝对路径
int cd(char * path)
{
	Entry e;
	int result = FindPath(path, 0x10, &e);
	if (result == 0)
	{
		printf("The directory doesn't exist.\n");
		return 0;
	}
	else
	{
		CurDirPtr[0] = e;
		//printf("%d\n", e.EntryBlockNum);
		return 1;
	}
}

// 展示某个目录项
void DisplayDirItem(Entry* e)
{
	uchar type[5];
	uchar dir[5] = "DIR";
	uchar file[5] = "FILE";
	uchar name[12] = "";
	memcpy(name, e, 11);
	name[11] = '\0';
	if (e->DIR_Attr == 0x27)
	{
		printf("HIDDEN FILE\n");
		return;
	}
	else if (e->DIR_Attr == 0x10)
	{
		memcpy(type, dir, sizeof(dir));
	}
	else
	{
		memcpy(type, file, sizeof(file));
	}
	
	printf("%11s\t%5s\t%10d\n", name, type, e->DIR_FileSize);

}

void DisplayDir()
{
	// 遍历每一个盘块
	char Block[BYTES_PER_BLOCK];
	char temp[BYTES_PER_ENTRY];
	Entry e;
	char root[11] = "root";
	int MaxEntry;
	char name[] = "FILENAME";
	char type[] = "TYPE";
	char size[] = "SIZE";
	printf("\n%11s\t%5s\t%10s\n\n", name, type, size);
	int initBlockNum;
	if (strncmp(CurDirPtr->DIR_NAME, root, sizeof(root)) == 0)
	{	
		MaxEntry = MBRInfo.BPB_RootEntCnt;
		initBlockNum = ROOT_DIR_Offset;
		for (int i = 0; i < MaxEntry / ENTRY_PER_BLOCK; i++)
		{
			ReadBlock(initBlockNum + i, Block, ramFDD14);
			for (int j = 0; j < ENTRY_PER_BLOCK; j++)
			{
				memcpy(temp, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
				// 若已删除或为0，则不用display
				if ((*temp) == 0 || (*(uchar*)temp) == 0xE5)
				{
					continue;
				}
				memcpy(&e, temp, BYTES_PER_ENTRY);
				DisplayDirItem(&e);

			}
		}
	}
	else
	{
		int count;
		short * ClusList = FindFatClusList(CurDirPtr->DIR_FstClus, &count);
		for (int i = 0; i < count; i++)
		{
			ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
			for (int j = 0; j < ENTRY_PER_BLOCK; j++)
			{
				memcpy(temp, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
				// 若已删除或为0，则不用display
				if ((*temp) == 0 || (*(uchar*)temp) == 0xE5)
				{
					continue;
				}
				memcpy(&e, temp, BYTES_PER_ENTRY);
				DisplayDirItem(&e);

			}
		}
	}
	printf("\n");
	
}
void display(int depth, Entry* e)
{
	char name[ENTRY_Attr_Offset + 1];
	char Block[BYTES_PER_BLOCK];
	memcpy(name, e->DIR_NAME, ENTRY_Attr_Offset);
	name[ENTRY_Attr_Offset] = '\0';
	for (int i = 0; i < depth - 1; i++)
	{
		printf("   ");
	}
	printf("|--%s\n", name);
	int count;
	short * ClusList = FindFatClusList(e->DIR_FstClus, &count);
	for (int i = 0; i < count; i++)
	{
		ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
		for (int j = 0; j < ENTRY_PER_BLOCK; j++)
		{
			char temp[BYTES_PER_ENTRY];
			memcpy(temp, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
			// 若已删除或为0，则不用display
			if ((*temp) == 0 || (*(uchar*)temp) == 0xE5)
			{
				continue;
			}
			Entry next;
			memcpy(&next, temp, BYTES_PER_ENTRY);
			if (next.DIR_Attr == 0x10 && next.DIR_NAME[0] != 0x2E)
			{
				display(depth + 1, &next);
			}
			else if(next.DIR_Attr == 0x20)
			{
				memcpy(name, next.DIR_NAME, ENTRY_Attr_Offset);
				name[ENTRY_Attr_Offset] = '\0';
				for (int i = 0; i < depth; i++)
				{
					printf("   ");
				}
				printf("|--%s\n", name);
			}
			else if (next.DIR_Attr == 0x27)
			{
				for (int i = 0; i < depth; i++)
				{
					printf("   ");
				}
				printf("|--%s\n", "HIDDEN FILE");
			}
			

		}
	}

}
void DisplayTree()
{
	printf("\n\\root\n");
	char Block[BYTES_PER_BLOCK];
	for (int i = 0; i < MBRInfo.BPB_RootEntCnt / ENTRY_PER_BLOCK; i++)
	{
		ReadBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
		for (int j = 0; j < ENTRY_PER_BLOCK; j++)
		{
			char temp[BYTES_PER_ENTRY];
			memcpy(temp, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
			// 若已删除或为0，则不用display
			if ((*temp) == 0 || (*(uchar*)temp) == 0xE5)
			{
				continue;
			}
			Entry e;
			memcpy(&e, temp, BYTES_PER_ENTRY);
			if (e.DIR_Attr == 0x10)
			{
				display(1, &e);
			}
			else if (e.DIR_Attr == 0x20)
			{
				char name[ENTRY_Attr_Offset + 1];
				memcpy(name, e.DIR_NAME, ENTRY_Attr_Offset);
				name[ENTRY_Attr_Offset] = '\0';
				printf("|--%s\n", name);
			}
			else if (e.DIR_Attr == 0x27)
			{
				printf("|--%s\n", "HIDDEN FILE");
			}

		}
	}
	printf("\n");
}

int md(char * name, uchar attr, char* path) 
{
	Entry CreatDir;
	uchar Block[BYTES_PER_BLOCK];
	int MaxBlock;
	ushort * ClusList = NULL;
	Entry newEntry;
	memset(&CreatDir, 0, sizeof(Entry));
	memset(&newEntry, 0, sizeof(Entry));
	// 定位到最后一级子目录，如果失败，返回-2
	int isFound = FindPath(path, 0x10, &CreatDir);
	if (isFound == 0)
	{
		printf("The path you want to create file or dir doesn't exist.\n");
		return -2;
	}

	// 检查是否有同名文件存在，如果有，返回-3
	uchar root[11] = "root";
	int isRoot = (strncmp(CreatDir.DIR_NAME, root, sizeof(root)) == 0); // 要创建目录项的目录是否是根目录
	if (isRoot)
	{
		MaxBlock = MBRInfo.BPB_RootEntCnt / ENTRY_PER_BLOCK;
	}
	else
	{
		ClusList = FindFatClusList(CreatDir.DIR_FstClus, &MaxBlock);
	}
	for (int i = 0; i < MaxBlock; i++)
	{
		if (isRoot)
		{
			ReadBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
		}
		else
		{
			ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
		}
		for (int j = 0; j < ENTRY_PER_BLOCK; j++)
		{
			// 如果是空的或者已删除则跳过。
			if ((*(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				continue;
			}

			// 检查文件名
			int eq = strncmp(name, Block + j * BYTES_PER_ENTRY, strlen(name));
			if (eq == 0)
			{
				printf("There has been the file or dir with same name.\n");
				if (ClusList != NULL)
				{
					free(ClusList);
				}
				return -3;
			}
		}
	}
	// 从目录的第一块开始，寻找一个删除标志的目录项
	//for (int i = 0; i < MaxBlock; i++)
	//{
	//	if (isRoot)
	//	{
	//		ReadBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
	//	}
	//	else
	//	{
	//		ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
	//	}
	//	for (int j = 0; j < ENTRY_PER_BLOCK; j++)
	//	{
	//		// 找到,将首簇号保存到可分配块链尾部，修改目录项
	//		if ((*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
	//		{
	//			memcpy(&newEntry, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
	//			// 将首簇号保存到可分配块链尾部
	//			/*int count = 0;
	//			ushort* deletedClusList = FindFatClusList(newEntry.DIR_FstClus, &count);
	//			ushort* temp = (ushort*)malloc(sizeof(ushort) * (count + AllocClusNum));
	//			memcpy(temp, AllocClusList, AllocClusNum);
	//			memcpy(temp + AllocClusNum, deletedClusList, count);
	//			if (AllocClusList != NULL)
	//			{
	//				free(AllocClusList);
	//			}
	//			AllocClusList = temp;
	//			temp = NULL;
	//			AllocClusNum += count;*/
	//			
	//			// 修改目录项
	//			if (isRoot)
	//			{
	//				newEntry.EntryBlockNum = ROOT_DIR_Offset + i;
	//			}
	//			else
	//			{
	//				newEntry.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
	//			}
	//			
	//			if (attr == 0x10) // 创建的是目录
	//			{
	//				newEntry.DIR_Attr = attr;
	//				newEntry.DIR_FileSize = 0;
	//				newEntry.DIR_FstClus = FindNextEmptyClus();
	//				if (newEntry.DIR_FstClus == 0)
	//				{
	//					printf("There are not any space to create dir now.\n");
	//					if (ClusList != NULL)
	//					{
	//						free(ClusList);
	//					}
	//					return -1;
	//				}

	//				// 在fat表上登记
	//				WriteFatClus(newEntry.DIR_FstClus, 0xfff);
	//				for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
	//				{
	//					newEntry.DIR_NAME[ch] = 0x20;
	//				}
	//				strncpy(newEntry.DIR_NAME, name, strlen(name));

	//				// 创建..和.
	//				Entry Dot, DoubleDot;
	//				memset(&Dot, 0, sizeof(Entry));
	//				memset(&DoubleDot, 0, sizeof(Entry));

	//				for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
	//				{
	//					Dot.DIR_NAME[ch] = 0x20;
	//					DoubleDot.DIR_NAME[ch] = 0x20;
	//				}
	//				Dot.DIR_NAME[0] = 0x2E;
	//				DoubleDot.DIR_NAME[0] = 0x2E;
	//				DoubleDot.DIR_NAME[1] = 0x2E;
	//				memcpy((&Dot) + ENTRY_Attr_Offset, (&newEntry) + ENTRY_Attr_Offset, BYTES_PER_ENTRY - ENTRY_Attr_Offset);
	//				if (isRoot)
	//				{
	//					DoubleDot.DIR_Attr = 0x10;
	//					newEntry.DIR_FileSize = 0;
	//					newEntry.DIR_FstClus = 0;
	//					newEntry.EntryBlockNum = ROOT_DIR_Offset;
	//				}
	//				else
	//				{
	//					memcpy((&DoubleDot) + ENTRY_Attr_Offset, (&CreatDir) + ENTRY_Attr_Offset, BYTES_PER_ENTRY - ENTRY_Attr_Offset);
	//				}
	//				

	//				// 清空新目录的盘块，填写.和..
	//				uchar newDirBlock[BYTES_PER_BLOCK];
	//				ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	//				memset(newDirBlock, 0, BYTES_PER_BLOCK);
	//				memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
	//				memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
	//				WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	//			}
	//			else // 创建的是文件
	//			{
	//				newEntry.DIR_Attr = attr;
	//				newEntry.DIR_FileSize = 0;
	//				for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
	//				{
	//					newEntry.DIR_NAME[ch] = 0x20;
	//				}
	//				strncpy(newEntry.DIR_NAME, name, strlen(name));
	//				newEntry.DIR_NAME[ENTRY_Attr_Offset - 3] = 0x54;
	//				newEntry.DIR_NAME[ENTRY_Attr_Offset - 2] = 0x58;
	//				newEntry.DIR_NAME[ENTRY_Attr_Offset - 1] = 0x54;
	//			}

	//			//写回磁盘
	//			memcpy(Block + j * BYTES_PER_ENTRY, &newEntry, BYTES_PER_ENTRY);
	//			if (isRoot)
	//			{
	//				WriteBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
	//			}
	//			else
	//			{
	//				WriteBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
	//			}
	//			if (ClusList != NULL)
	//			{
	//				free(ClusList);
	//			}
	//			printf("The dir or file has been created.\n");
	//			return 0;
	//		}
	//	}
	//}

	//找不到，寻找第一个空白目录项
	for (int i = 0; i < MaxBlock; i++)
	{
		if (isRoot)
		{
			ReadBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
		}
		else
		{
			ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
		}
		for (int j = 0; j < ENTRY_PER_BLOCK; j++)
		{
			//找到，利用此目录项作为新文件的目录项，填写相关项
			if ((*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				memcpy(&newEntry, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);

				// 修改目录项
				if (isRoot)
				{
					newEntry.EntryBlockNum = ROOT_DIR_Offset + i;
				}
				else
				{
					newEntry.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
				}

				if (attr == 0x10) // 创建的是目录
				{
					newEntry.DIR_Attr = attr;
					newEntry.DIR_FileSize = 0;
					newEntry.DIR_FstClus = FindNextEmptyClus();
					if (newEntry.DIR_FstClus == 0)
					{
						if (ClusList != NULL)
						{
							free(ClusList);
						}
						printf("There are not any space to create a dir now.\n");
						return -1;
					}

					// 在fat表上登记
					WriteFatClus(newEntry.DIR_FstClus, 0xfff);
					for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
					{
						newEntry.DIR_NAME[ch] = 0x20;
					}
					strncpy(newEntry.DIR_NAME, name, strlen(name));

					// 创建..和.
					Entry Dot, DoubleDot;
					memset(&Dot, 0, sizeof(Entry));
					memset(&DoubleDot, 0, sizeof(Entry));

					for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
					{
						Dot.DIR_NAME[ch] = 0x20;
						DoubleDot.DIR_NAME[ch] = 0x20;
					}
					Dot.DIR_NAME[0] = 0x2E;
					DoubleDot.DIR_NAME[0] = 0x2E;
					DoubleDot.DIR_NAME[1] = 0x2E;
					memcpy(&Dot.DIR_Attr, &newEntry.DIR_Attr, sizeof(Entry) - ENTRY_Attr_Offset);
					if (isRoot)
					{
						DoubleDot.DIR_Attr = 0x10;
						DoubleDot.DIR_FileSize = 0;
						DoubleDot.DIR_FstClus = 0;
						DoubleDot.EntryBlockNum = ROOT_DIR_Offset;
					}
					else
					{
						memcpy(&DoubleDot.DIR_Attr, &CreatDir.DIR_Attr, sizeof(Entry) - ENTRY_Attr_Offset);
					}


					// 清空新目录的盘块，填写.和..
					uchar newDirBlock[BYTES_PER_BLOCK];
					ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
					memset(newDirBlock, 0, BYTES_PER_BLOCK);
					memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
					memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
					WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
				}
				else // 创建的是文件
				{
					newEntry.DIR_Attr = attr;
					newEntry.DIR_FileSize = 0;
					for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
					{
						newEntry.DIR_NAME[ch] = 0x20;
					}
					strncpy(newEntry.DIR_NAME, name, strlen(name));
					newEntry.DIR_NAME[ENTRY_Attr_Offset - 3] = 0x54;
					newEntry.DIR_NAME[ENTRY_Attr_Offset - 2] = 0x58;
					newEntry.DIR_NAME[ENTRY_Attr_Offset - 1] = 0x54;
					newEntry.DIR_FstClus = 0;
				}

				//写回磁盘
				memcpy(Block + j * BYTES_PER_ENTRY, &newEntry, BYTES_PER_ENTRY);
				if (isRoot)
				{
					WriteBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
				}
				else
				{
					WriteBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
				}
				if (ClusList != NULL)
				{
					free(ClusList);
				}
				printf("The dir or file has been created.\n");
				return 0;
			}
		}
	}
	
	// 找不到，申请一个空闲数据块（找不到空闲数据块，返回-1），用该块的第一个目录项创建新文件，并且修改当前目录文件，链接上这个数据块
	// 如果是在根目录中创建
	if (isRoot)
	{
		printf("There are not any space to create an entry for file or dir in root now.\n");
		if (ClusList != NULL)
		{
			free(ClusList);
		}
		return -1;
	}
	// 如果是在子目录中创建
	// 申请一个空闲数据块（找不到空闲数据块，返回-1）
	ushort newEntryClus = FindNextEmptyClus();
	if (newEntryClus == 0)
	{
		printf("There are not any space to create an entry for file or dir now.\n");
		if (ClusList != NULL)
		{
			free(ClusList);
		}
		return -1;
	}
	// 修改当前目录文件，链接上这个数据块
	AddClus(ClusList[MaxBlock - 1], newEntryClus);

	// 清空该块，用该块的第一个目录项创建新文件
	uchar newEntryBlock[BYTES_PER_BLOCK];
	ReadBlock(ClusNum2BlockNum(newEntryClus), newEntryBlock, ramFDD14);
	memset(newEntryBlock, 0, BYTES_PER_BLOCK);
	
	// 修改目录项
	newEntry.EntryBlockNum = ClusNum2BlockNum(newEntryClus);
	if (attr == 0x10) // 创建的是目录
	{
		newEntry.DIR_Attr = attr;
		newEntry.DIR_FileSize = 0;
		newEntry.DIR_FstClus = FindNextEmptyClus();
		if (newEntry.DIR_FstClus == 0)
		{
			if (ClusList != NULL)
			{
				free(ClusList);
			}
			printf("There are not any space to create a dir now.\n");
			return -1;
		}

		// 在fat表上登记
		WriteFatClus(newEntry.DIR_FstClus, 0xfff);
		for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
		{
			newEntry.DIR_NAME[ch] = 0x20;
		}
		strncpy(newEntry.DIR_NAME, name, strlen(name));

		// 创建..和.
		Entry Dot, DoubleDot;
		memset(&Dot, 0, sizeof(Entry));
		memset(&DoubleDot, 0, sizeof(Entry));

		for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
		{
			Dot.DIR_NAME[ch] = 0x20;
			DoubleDot.DIR_NAME[ch] = 0x20;
		}
		Dot.DIR_NAME[0] = 0x2E;
		DoubleDot.DIR_NAME[0] = 0x2E;
		DoubleDot.DIR_NAME[1] = 0x2E;
		memcpy((&Dot) + ENTRY_Attr_Offset, (&newEntry) + ENTRY_Attr_Offset, BYTES_PER_ENTRY - ENTRY_Attr_Offset);
		if (isRoot)
		{
			DoubleDot.DIR_Attr = 0x10;
			newEntry.DIR_FileSize = 0;
			newEntry.DIR_FstClus = 0;
			newEntry.EntryBlockNum = ROOT_DIR_Offset;
		}
		else
		{
			memcpy((&DoubleDot) + ENTRY_Attr_Offset, (&CreatDir) + ENTRY_Attr_Offset, BYTES_PER_ENTRY - ENTRY_Attr_Offset);
		}


		// 清空新目录的盘块，填写.和..
		uchar newDirBlock[BYTES_PER_BLOCK];
		ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
		memset(newDirBlock, 0, BYTES_PER_BLOCK);
		memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
		memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
		WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	}
	else // 创建的是文件
	{
		newEntry.DIR_Attr = attr;
		newEntry.DIR_FileSize = 0;
		for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
		{
			newEntry.DIR_NAME[ch] = 0x20;
		}
		strncpy(newEntry.DIR_NAME, name, strlen(name));
		newEntry.DIR_NAME[ENTRY_Attr_Offset - 3] = 0x54;
		newEntry.DIR_NAME[ENTRY_Attr_Offset - 2] = 0x58;
		newEntry.DIR_NAME[ENTRY_Attr_Offset - 1] = 0x54;
		newEntry.DIR_FstClus = 0;
	}

	//写回磁盘
	memcpy(newEntryBlock, &newEntry, BYTES_PER_ENTRY);
	WriteBlock(ClusNum2BlockNum(newEntryClus), newEntryBlock, ramFDD14);
	if (ClusList != NULL)
	{
		free(ClusList);
	}
	printf("The dir or file has been created.\n");
	return 0;

}

int rd(char * name, uchar attr, char* path)
{
	Entry DeleteDir;
	Entry toDelete;
	// 如果是文件检查文件是否已打开（活动文件表），如果已打开，不能删除，返回-1
	if (attr == 0x20 || attr == 0x27)
	{
		for (int i = 0; i < Factive_counter; i++)
		{
			if (strncmp(name, ACTIVE_FILES_LIST[i].f_entry.DIR_NAME, strlen(name)) == 0)
			{
				printf("The file you want to deleted is open.\n");
				return -1;
			}
		}
	}
	// 定位到最后一级子目录，如果定位失败，返回-2
	int isFound = FindPath(path, 0x10, &DeleteDir);
	if (isFound == 0)
	{
		printf("The path you want to delete file or dir doesn't exist.\n");
		return -2;
	}
	// 从目录第一项开始寻找文件或者目录的匹配项，找不到，文件不存在，返回-3
	int EntryIndex = -1;
	uchar root[11] = "root";
	int MaxBlock;
	ushort * ClusList = NULL;
	uchar Block[BYTES_PER_BLOCK];
	int isRoot = (strncmp(DeleteDir.DIR_NAME, root, sizeof(root)) == 0); // 要创建目录项的目录是否是根目录
	if (isRoot)
	{
		MaxBlock = MBRInfo.BPB_RootEntCnt / ENTRY_PER_BLOCK;
	}
	else
	{
		ClusList = FindFatClusList(DeleteDir.DIR_FstClus, &MaxBlock);
	}
	for (int i = 0; i < MaxBlock; i++)
	{
		if (isRoot)
		{
			ReadBlock(ROOT_DIR_Offset + i, Block, ramFDD14);
		}
		else
		{
			ReadBlock(ClusNum2BlockNum(ClusList[i]), Block, ramFDD14);
		}
		for (int j = 0; j < ENTRY_PER_BLOCK; j++)
		{
			// 如果是空的或者已删除则跳过。
			if ((*(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				continue;
			}

			// 检查文件名
			int eq = strncmp(name, Block + j * BYTES_PER_ENTRY, strlen(name));
			int check_attr = (attr == *(uchar*)(Block + j * BYTES_PER_ENTRY + ENTRY_Attr_Offset));
			if (eq == 0 && check_attr == 1)
			{
				EntryIndex = j;
				if (isRoot)
				{
					toDelete.EntryBlockNum = ROOT_DIR_Offset + i;
				}
				else
				{
					toDelete.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
				}
				
				break;
			}
		}
		if (EntryIndex != -1)
		{
			break;
		}
	}
	if (EntryIndex == -1)
	{
		if (ClusList != NULL)
		{
			free(ClusList);
		}
		printf("The file or dir you want to delete doesn't exist.\n");
		return -3;
	}
	// 如果是目录，目录是否为空，不为空不能删除，返回-1
	int delBlockCount;
	uchar delBlock[BYTES_PER_BLOCK];
	isFound = 0;
	memcpy(&toDelete, Block + EntryIndex * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
	ushort* toDeleteClusList = FindFatClusList(toDelete.DIR_FstClus, &delBlockCount);
	// 如果是目录，目录是否为空，不为空不能删除，返回-1
	if (attr == 0x10)
	{
		for (int i = 0; i < delBlockCount; i++)
		{
			ReadBlock(ClusNum2BlockNum(toDeleteClusList[i]), delBlock, ramFDD14);

			for (int j = 0; j < ENTRY_PER_BLOCK; j++)
			{
				// 如果是空的或者已删除或者是.或..则跳过。
				if ((*(delBlock + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(delBlock + j * BYTES_PER_ENTRY)) == 0xE5 || (*(uchar*)(delBlock + j * BYTES_PER_ENTRY)) == 0x2E)
				{
					continue;
				}

				isFound = 1;
				break;
			}
			if (isFound)
			{
				break;
			}
		}
		if (isFound)
		{
			printf("The directory you want to delete doesn't empty.\n");
			if (ClusList != NULL)
			{
				free(ClusList);
			}
			if (toDeleteClusList != NULL)
			{
				free(toDeleteClusList);
			}
			return -1;
		}
	}
	// 找到，第一项置为@，相应的fat表清空
	toDelete.DIR_NAME[0] = 0xE5;
	for (int i = 0; i < delBlockCount; i++)
	{
		WriteFatClus(toDeleteClusList[i], 0);
	}

	// 写回磁盘
	memcpy(Block + EntryIndex * BYTES_PER_ENTRY, &toDelete, BYTES_PER_ENTRY);
	WriteBlock(toDelete.EntryBlockNum, Block, ramFDD14);

	if(ClusList != NULL)
	{
		free(ClusList);
	}
	if (toDeleteClusList != NULL)
	{
		free(toDeleteClusList);
	}
	printf("The dir or file has been deleted.\n");
	return 0;
}

int type(char* path)
{
	OpenFile * fp;
	
	// 调用open, read, close完成文件内容获取
	fp = Open(path);
	// 无法打开相应文件
	if (fp == NULL)
	{
		return -1;
	}
	uchar * buffer = (uchar*) malloc(fp->af->f_entry.DIR_FileSize + 1);
	Read(fp, buffer, fp->af->f_entry.DIR_FileSize);
	buffer[fp->af->f_entry.DIR_FileSize] = '\0';
	printf("%s\n", buffer);
	free(buffer);
	Close(fp);
	return 0;
}

int copy(char**source, int count, char* destination)
{
	// 打开文件
	OpenFile* dfp;
	dfp = Open(destination);
	// 若打开失败，创建文件，再打开文件
	if (dfp == NULL)
	{
		char ch = '\\';
		char * path = strrchr(destination, ch);
		if (path == NULL)
		{
			int result = md(destination, 0x20, NULL);
			if (result < 0)
			{
				return -1;
			}
		}
		else
		{
			int len = path - destination;
			char * name = path + 1;
			path = (char *)malloc(sizeof(char) * MAX_PATH);
			memset(path, 0, sizeof(path));
			memcpy(path, destination, len);
			path[len] = '\0';
			int result = md(name, 0x20, path);
			free(path);
			if (result < 0)
			{
				return -1;
			}			
		}
		dfp = Open(destination);
	}
	// 若是键盘，输入，然后write,close
	if (strncmp(source[0], "con", strlen(source[0])) == 0)
	{
		char ch;
		int count = 0;
		char buffer[BYTES_PER_BLOCK];
		while ((ch = getchar())!=EOF)
		{
			buffer[count++] = ch;
			if (count == BYTES_PER_BLOCK)
			{
				count = 0;
				Write(dfp, buffer, count);
			}
		}
		Write(dfp, buffer, count);
		Close(dfp);
		printf("The data has been written to the file.\n");
		return 0;
	}
	// 若是一定数量的文件，遍历，打开文件，read，write, close
	for (int i = 0; i < count; i++)
	{
		OpenFile* sfp = Open(source[i]);
		if (sfp == NULL)
		{
			printf("The copy of the %dth file is failure.\n", count + 1);
			continue;
		}
		uchar * buffer = (uchar *)malloc(sizeof(uchar) * sfp->af->f_entry.DIR_FileSize);
		Read(sfp, buffer, sfp->af->f_entry.DIR_FileSize);
		Write(dfp, buffer, sfp->af->f_entry.DIR_FileSize);
		Close(sfp);
	}
	// close 目标文件
	Close(dfp);
	printf("The data has been written to the file.\n");
	return 0;
}

int sedit(char * path)
{
	char read[] = "read";
	char write[] = "write";
	char seek[] = "seek";
	char quit[] = "q";
	// 打开文件
	OpenFile *fp = Open(path);
	// 若打开失败，编辑不成功
	if (fp == NULL)
	{
		printf("The edit is failure.\n");
		return -1;
	}
	// 若打开成功，弹出命令表
	printf("\n");
	printf("%-15s%s\n", "read", "Display the file content.");
	printf("%-15s%s\n", "write", "Write the content you input at the current position of the file.");
	printf("%-15s%s\n", "seek", "Change current position of the file.");
	printf("%-15s%s\n", "q", "Quit the edit mode.");
	printf("\n");
	// 当不退出时，接受命令
	char * name = strrchr(path, '\\');
	if (name == NULL)
	{
		name = path;
	}
	else
	{
		name = name + 1;
	}
	printf("%s(posi = %d):> ", name, fp->posi);
	char in[MAX_INPUT];
	gets(in);
	while (strncmp(in, quit, strlen(in) != 0))
	{
		// 可以read、write、seek
		if (strncmp(in, read, strlen(in)) == 0)
		{
			uchar * buffer = (uchar*)malloc(fp->af->f_entry.DIR_FileSize + 1);
			Read(fp, buffer, fp->af->f_entry.DIR_FileSize);
			buffer[fp->af->f_entry.DIR_FileSize] = '\0';
			printf("%s\n", buffer);
			free(buffer);
		}
		else if (strncmp(in, write, strlen(in)) == 0)
		{
			char ch;
			int count = 0;
			char buffer[BYTES_PER_BLOCK];
			while ((ch = getchar()) != EOF)
			{
				buffer[count++] = ch;
				if (count == BYTES_PER_BLOCK)
				{
					count = 0;
					Write(fp, buffer, count);
				}
			}
			Write(fp, buffer, count - 1);
		}
		else
		{
			char s[2] = " ";
			char * token = strtok(in, s);
			if (strncmp(in, seek, strlen(in)) == 0)
			{
				char *offset = strtok(NULL, s);
				Seek(fp, atoi(offset), 1);
			}
			else
			{
				printf("The command you input in invalid. Please type your command again.\n");
			}
			
		}
		printf("%s(posi = %d):> ", name, fp->posi);
		gets(in);
	}
	Close(fp);
	return 0;
}

void help()
{
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
	char help[] = "help";
	printf("\n");
	printf("You can provide either a relative path or absolute path in all implementations refer to file and directory.\n");
	printf("%-15s%s\n", "help", "To get the information of each command.");
	printf("%-15s%s\n", "exit", "To exit the FAT12 system.");
	printf("%-15s%s\n", "mbr", "Check the information on MBR.");
	printf("%-15s%s\n", "tree", "Display the directory tree of the FAT12 system.");
	printf("%-15s%s\n", "dir", "Display all the directories and files in the current directory.");
	printf("%-15s%s\n", "cd", "To enter the directory you want.");
	printf("%-15s%s\n", "md", "Create a directory.");
	printf("%-15s%s\n", "rd", "Delete a directory.");
	printf("%-15s%s\n", "copy", "To create or alter file as the copy of con or other files(<=9).");
	printf("%-15s%s\n", "type", "Display a file's content.");
	printf("%-15s%s\n", "sedit", "A simple version editor to modify your file.");
	printf("%-15s%s\n", "del", "Delete file.");
	printf("\n");
}