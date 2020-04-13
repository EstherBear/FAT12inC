#include"view.h"

// ������������
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

// չʾMBR�е���Ϣ
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

// ����Ŀ¼��path������ĳ��Ŀ¼�����or����·��
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

// չʾĳ��Ŀ¼��
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
	// ����ÿһ���̿�
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
				// ����ɾ����Ϊ0������display
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
				// ����ɾ����Ϊ0������display
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
			// ����ɾ����Ϊ0������display
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
			// ����ɾ����Ϊ0������display
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
	// ��λ�����һ����Ŀ¼�����ʧ�ܣ�����-2
	int isFound = FindPath(path, 0x10, &CreatDir);
	if (isFound == 0)
	{
		printf("The path you want to create file or dir doesn't exist.\n");
		return -2;
	}

	// ����Ƿ���ͬ���ļ����ڣ�����У�����-3
	uchar root[11] = "root";
	int isRoot = (strncmp(CreatDir.DIR_NAME, root, sizeof(root)) == 0); // Ҫ����Ŀ¼���Ŀ¼�Ƿ��Ǹ�Ŀ¼
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
			// ����ǿյĻ�����ɾ����������
			if ((*(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				continue;
			}

			// ����ļ���
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
	// ��Ŀ¼�ĵ�һ�鿪ʼ��Ѱ��һ��ɾ����־��Ŀ¼��
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
	//		// �ҵ�,���״غű��浽�ɷ������β�����޸�Ŀ¼��
	//		if ((*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
	//		{
	//			memcpy(&newEntry, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
	//			// ���״غű��浽�ɷ������β��
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
	//			// �޸�Ŀ¼��
	//			if (isRoot)
	//			{
	//				newEntry.EntryBlockNum = ROOT_DIR_Offset + i;
	//			}
	//			else
	//			{
	//				newEntry.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
	//			}
	//			
	//			if (attr == 0x10) // ��������Ŀ¼
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

	//				// ��fat���ϵǼ�
	//				WriteFatClus(newEntry.DIR_FstClus, 0xfff);
	//				for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
	//				{
	//					newEntry.DIR_NAME[ch] = 0x20;
	//				}
	//				strncpy(newEntry.DIR_NAME, name, strlen(name));

	//				// ����..��.
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

	//				// �����Ŀ¼���̿飬��д.��..
	//				uchar newDirBlock[BYTES_PER_BLOCK];
	//				ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	//				memset(newDirBlock, 0, BYTES_PER_BLOCK);
	//				memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
	//				memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
	//				WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	//			}
	//			else // ���������ļ�
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

	//			//д�ش���
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

	//�Ҳ�����Ѱ�ҵ�һ���հ�Ŀ¼��
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
			//�ҵ������ô�Ŀ¼����Ϊ���ļ���Ŀ¼���д�����
			if ((*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				memcpy(&newEntry, Block + j * BYTES_PER_ENTRY, BYTES_PER_ENTRY);

				// �޸�Ŀ¼��
				if (isRoot)
				{
					newEntry.EntryBlockNum = ROOT_DIR_Offset + i;
				}
				else
				{
					newEntry.EntryBlockNum = ClusNum2BlockNum(ClusList[i]);
				}

				if (attr == 0x10) // ��������Ŀ¼
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

					// ��fat���ϵǼ�
					WriteFatClus(newEntry.DIR_FstClus, 0xfff);
					for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
					{
						newEntry.DIR_NAME[ch] = 0x20;
					}
					strncpy(newEntry.DIR_NAME, name, strlen(name));

					// ����..��.
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


					// �����Ŀ¼���̿飬��д.��..
					uchar newDirBlock[BYTES_PER_BLOCK];
					ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
					memset(newDirBlock, 0, BYTES_PER_BLOCK);
					memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
					memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
					WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
				}
				else // ���������ļ�
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

				//д�ش���
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
	
	// �Ҳ���������һ���������ݿ飨�Ҳ����������ݿ飬����-1�����øÿ�ĵ�һ��Ŀ¼������ļ��������޸ĵ�ǰĿ¼�ļ���������������ݿ�
	// ������ڸ�Ŀ¼�д���
	if (isRoot)
	{
		printf("There are not any space to create an entry for file or dir in root now.\n");
		if (ClusList != NULL)
		{
			free(ClusList);
		}
		return -1;
	}
	// ���������Ŀ¼�д���
	// ����һ���������ݿ飨�Ҳ����������ݿ飬����-1��
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
	// �޸ĵ�ǰĿ¼�ļ���������������ݿ�
	AddClus(ClusList[MaxBlock - 1], newEntryClus);

	// ��ոÿ飬�øÿ�ĵ�һ��Ŀ¼������ļ�
	uchar newEntryBlock[BYTES_PER_BLOCK];
	ReadBlock(ClusNum2BlockNum(newEntryClus), newEntryBlock, ramFDD14);
	memset(newEntryBlock, 0, BYTES_PER_BLOCK);
	
	// �޸�Ŀ¼��
	newEntry.EntryBlockNum = ClusNum2BlockNum(newEntryClus);
	if (attr == 0x10) // ��������Ŀ¼
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

		// ��fat���ϵǼ�
		WriteFatClus(newEntry.DIR_FstClus, 0xfff);
		for (int ch = 0; ch < ENTRY_Attr_Offset; ch++)
		{
			newEntry.DIR_NAME[ch] = 0x20;
		}
		strncpy(newEntry.DIR_NAME, name, strlen(name));

		// ����..��.
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


		// �����Ŀ¼���̿飬��д.��..
		uchar newDirBlock[BYTES_PER_BLOCK];
		ReadBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
		memset(newDirBlock, 0, BYTES_PER_BLOCK);
		memcpy(newDirBlock, &Dot, BYTES_PER_ENTRY);
		memcpy(newDirBlock + BYTES_PER_ENTRY, &DoubleDot, BYTES_PER_ENTRY);
		WriteBlock(ClusNum2BlockNum(newEntry.DIR_FstClus), newDirBlock, ramFDD14);
	}
	else // ���������ļ�
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

	//д�ش���
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
	// ������ļ�����ļ��Ƿ��Ѵ򿪣���ļ���������Ѵ򿪣�����ɾ��������-1
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
	// ��λ�����һ����Ŀ¼�������λʧ�ܣ�����-2
	int isFound = FindPath(path, 0x10, &DeleteDir);
	if (isFound == 0)
	{
		printf("The path you want to delete file or dir doesn't exist.\n");
		return -2;
	}
	// ��Ŀ¼��һ�ʼѰ���ļ�����Ŀ¼��ƥ����Ҳ������ļ������ڣ�����-3
	int EntryIndex = -1;
	uchar root[11] = "root";
	int MaxBlock;
	ushort * ClusList = NULL;
	uchar Block[BYTES_PER_BLOCK];
	int isRoot = (strncmp(DeleteDir.DIR_NAME, root, sizeof(root)) == 0); // Ҫ����Ŀ¼���Ŀ¼�Ƿ��Ǹ�Ŀ¼
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
			// ����ǿյĻ�����ɾ����������
			if ((*(Block + j * BYTES_PER_ENTRY)) == 0 || (*(uchar*)(Block + j * BYTES_PER_ENTRY)) == 0xE5)
			{
				continue;
			}

			// ����ļ���
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
	// �����Ŀ¼��Ŀ¼�Ƿ�Ϊ�գ���Ϊ�ղ���ɾ��������-1
	int delBlockCount;
	uchar delBlock[BYTES_PER_BLOCK];
	isFound = 0;
	memcpy(&toDelete, Block + EntryIndex * BYTES_PER_ENTRY, BYTES_PER_ENTRY);
	ushort* toDeleteClusList = FindFatClusList(toDelete.DIR_FstClus, &delBlockCount);
	// �����Ŀ¼��Ŀ¼�Ƿ�Ϊ�գ���Ϊ�ղ���ɾ��������-1
	if (attr == 0x10)
	{
		for (int i = 0; i < delBlockCount; i++)
		{
			ReadBlock(ClusNum2BlockNum(toDeleteClusList[i]), delBlock, ramFDD14);

			for (int j = 0; j < ENTRY_PER_BLOCK; j++)
			{
				// ����ǿյĻ�����ɾ��������.��..��������
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
	// �ҵ�����һ����Ϊ@����Ӧ��fat�����
	toDelete.DIR_NAME[0] = 0xE5;
	for (int i = 0; i < delBlockCount; i++)
	{
		WriteFatClus(toDeleteClusList[i], 0);
	}

	// д�ش���
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
	
	// ����open, read, close����ļ����ݻ�ȡ
	fp = Open(path);
	// �޷�����Ӧ�ļ�
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
	// ���ļ�
	OpenFile* dfp;
	dfp = Open(destination);
	// ����ʧ�ܣ������ļ����ٴ��ļ�
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
	// ���Ǽ��̣����룬Ȼ��write,close
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
	// ����һ���������ļ������������ļ���read��write, close
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
	// close Ŀ���ļ�
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
	// ���ļ�
	OpenFile *fp = Open(path);
	// ����ʧ�ܣ��༭���ɹ�
	if (fp == NULL)
	{
		printf("The edit is failure.\n");
		return -1;
	}
	// ���򿪳ɹ������������
	printf("\n");
	printf("%-15s%s\n", "read", "Display the file content.");
	printf("%-15s%s\n", "write", "Write the content you input at the current position of the file.");
	printf("%-15s%s\n", "seek", "Change current position of the file.");
	printf("%-15s%s\n", "q", "Quit the edit mode.");
	printf("\n");
	// �����˳�ʱ����������
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
		// ����read��write��seek
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