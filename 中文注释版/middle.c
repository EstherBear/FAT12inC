#include "view.h"

OpenFile* Open(char* path)
{
	// 获得文件控制信息：目录项（路径分析）
	Entry entry_of_file;
	
	int isFound = FindPath(path, 0x20,  &entry_of_file);
	if (isFound == 0)
	{
		printf("The file doesn't exist.\n");
		return NULL;
	}
	else
	{
	// 把文件信息存进内存，活动文件表, 并返回打开文件表的索引
		if (Fopen_counter == MAX_open_FILES)
		{
			printf("The open file list of the current process has been full.\n");
			return NULL;
		}
		for (int i = 0; i < Factive_counter; i++)
		{
			if (strncmp(path, OPEN_FILES_LIST[i].af->path, MAX_PATH) == 0)
			{
				printf("The file has been opened in the current process.\n");
			}
		}
		// 查找当前文件是否在活动文件表已打开
		int AFindex = -1;
		for (int i = 0; i < Factive_counter; i++)
		{
			if (strncmp(path, ACTIVE_FILES_LIST[i].path, MAX_PATH) == 0)
			{
				AFindex = i;
				break;
			}
		}
		if (AFindex != -1)
		{
			ACTIVE_FILES_LIST[AFindex].share_counter++;
			OpenFile* fp = &OPEN_FILES_LIST[Fopen_counter];
			fp->index = Fopen_counter;
			Fopen_counter++;
			fp->posi = 0;
			fp->af = &ACTIVE_FILES_LIST[AFindex];
			
			// 返回文件入口
			return fp;
		}
		else
		{
			if (Factive_counter == MAX_active_FILES)
			{
				printf("The active file list of the system has been full.\n");
				return NULL;
			}
			else
			{
				ACTIVE_FILES_LIST[Factive_counter].share_counter = 1;
				ACTIVE_FILES_LIST[Factive_counter].f_entry = entry_of_file;
				ACTIVE_FILES_LIST[Factive_counter].index = Factive_counter;
				memcpy(ACTIVE_FILES_LIST[Factive_counter].path, path, MAX_PATH);
				OpenFile* fp = &OPEN_FILES_LIST[Fopen_counter];
				fp->index = Fopen_counter;
				fp->posi = 0;
				fp->af = &ACTIVE_FILES_LIST[Factive_counter];
				Fopen_counter++;
				Factive_counter++;
				// 返回文件入口
				return fp;
			}
		}	
	}
}

int Close(OpenFile* fp)
{
	fp->af->share_counter--;
	if (fp->af->share_counter == 0)
	{
		// update dir and data
		// free active_item
		for (int i = fp->af->index; i < Factive_counter-1; i++)
		{
			ACTIVE_FILES_LIST[i] = ACTIVE_FILES_LIST[i + 1];
		}
		Factive_counter--;
		
	}
	// free open_item
	for (int i = fp->index; i < Fopen_counter - 1; i++)
	{
		OPEN_FILES_LIST[i] = OPEN_FILES_LIST[i + 1];
	}
	Fopen_counter--;
	return 1;
}

int Read(OpenFile * fp, char * buffer, int size)
{

	int b0, b1;
	ushort * ClusList;
	char block[BYTES_PER_BLOCK];
	int count = 0;
	ClusList = FindFatClusList(fp->af->f_entry.DIR_FstClus, &count);
	if (fp->posi == fp->af->f_entry.DIR_FileSize && size != 0)
	{
		printf("Wrning: You are at the end of the file.\n");
		return 0;
	}
	// 计算读操作的起止相对块号
	if (fp->posi + size <= fp->af->f_entry.DIR_FileSize)
	{
		b0 = (fp->posi) / BYTES_PER_BLOCK;
		b1 = ((fp->posi) + size - 1) / BYTES_PER_BLOCK;
	}
	else
	{
		printf("Warning: The file size is smaller than the size you want to read.");
		size = fp->af->f_entry.DIR_FileSize - (fp->posi);
		b0 = (fp->posi) / BYTES_PER_BLOCK;
		b1 = (fp->af->f_entry.DIR_FileSize - 1) / BYTES_PER_BLOCK;
	}
	// 文件寻址
	if (b0 == b1) // 同块
	{
		ReadBlock(ClusNum2BlockNum(ClusList[b0]), block, ramFDD14);
		memcpy(buffer, &block[fp->posi % BYTES_PER_BLOCK], size);
	}
	else // 不同块
	{
		char * cur = buffer;
		for (int i = b0; i <= b1; i++)
		{	
			ReadBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);

			// 提取
			if (i == b0)
			{
				memcpy(cur, &block[fp->posi % BYTES_PER_BLOCK], BYTES_PER_BLOCK - (fp->posi % BYTES_PER_BLOCK));
				cur = cur + BYTES_PER_BLOCK - (fp->posi % BYTES_PER_BLOCK);
			}
			else if (i == b1)
			{
				memcpy(cur, block, (fp->posi + size) % BYTES_PER_BLOCK);
				cur += (fp->posi + size) % BYTES_PER_BLOCK;
			}
			else
			{
				memcpy(cur, &block[fp->posi % BYTES_PER_BLOCK], BYTES_PER_BLOCK);
				cur += BYTES_PER_BLOCK;
			}
		}
	}
	
	// 释放内存，修改posi， 返回size
	if (ClusList != NULL)
	{
		free(ClusList);
	}
	fp->posi += size;
	return size;
}

int Write(OpenFile * fp, char * buffer, int size)
{
	int b0, b1;
	ushort * ClusList;
	char block[BYTES_PER_BLOCK];
	int count = 0;
	int changeFileSize = 0;
	int changeFirstClus = 0;
	ClusList = FindFatClusList(fp->af->f_entry.DIR_FstClus, &count);
	
	// 计算写操作的起止相对块号
	b0 = (fp->posi) / BYTES_PER_BLOCK;
	b1 = ((fp->posi) + size - 1) / BYTES_PER_BLOCK;
	if (fp->posi + size > fp->af->f_entry.DIR_FileSize)
	{
		changeFileSize = 1;
	}

	// 簇块分配
	if (count == 0) // 文件还没有分配簇块
	{
		ClusList = (short *)malloc(sizeof(ushort) * (b1 + 1));
		changeFirstClus = 1;
		count = b1 + 1;
		for (int i = b0; i <= b1; i++)
		{
			ClusList[i] = FindNextEmptyClus();
			if (i == b0)
			{
				if (ClusList[b0] == 0)
				{
					printf("There is not any new block for the file to write.\n");
					if (ClusList != NULL)
					{
						free(ClusList);
					}
					return 0;
				}
				WriteFatClus(ClusList[b0], 0xfff);

			}
			else
			{
				if (ClusList[i] == 0)
				{
					printf("There are not enough blocks to store data.\n");
					size = i * BYTES_PER_BLOCK;
					b1 = i - 1;
					break;
				}
				AddClus(ClusList[i - 1], ClusList[i]);
			}
			count++;
		}
	}
	else if(b1 > (count - 1))
	{
		ushort* temp = (ushort*)malloc(sizeof(ushort) * (b1 + 1));
		memcpy(temp, ClusList, count * sizeof(ushort));
		for (int i = count; i <= b1; i++)
		{
			temp[i] = FindNextEmptyClus();
			if (temp[i] == 0)
			{
				printf("There are not enough blocks to store data.\n");
				size = (BYTES_PER_BLOCK- (fp->posi % BYTES_PER_BLOCK)) + (i - 1 - b0 ) * BYTES_PER_BLOCK;
				b1 = i - 1;
				break;
			}
			AddClus(temp[i - 1], temp[i]);
			count++;
		}
		free(ClusList);
		ClusList = temp;
		temp = NULL;
	}

	// 组块
	if (b0 == b1) // 同块
	{
		ReadBlock(ClusNum2BlockNum(ClusList[b0]), block, ramFDD14);
		memcpy(&block[fp->posi % BYTES_PER_BLOCK], buffer, size);
		WriteBlock(ClusNum2BlockNum(ClusList[b0]), block, ramFDD14);
	}
	else // 不同块
	{
		char * cur = buffer;
		for (int i = b0; i <= b1; i++)
		{
			ReadBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);

			if (i == b0)
			{
				memcpy(&block[fp->posi % BYTES_PER_BLOCK], cur, BYTES_PER_BLOCK - (fp->posi % BYTES_PER_BLOCK));
				cur = cur + BYTES_PER_BLOCK - (fp->posi % BYTES_PER_BLOCK);
				WriteBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);
			}
			else if (i == b1)
			{
				memcpy(block, cur, (fp->posi + size) % BYTES_PER_BLOCK);
				cur += (fp->posi + size) % BYTES_PER_BLOCK;
				WriteBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);
			}
			else
			{
				memcpy(block, cur, BYTES_PER_BLOCK);
				cur += BYTES_PER_BLOCK;
				WriteBlock(ClusNum2BlockNum(ClusList[i]), block, ramFDD14);
			}
		}
	}

	// 修改文件的目录项（文件大小+首簇号）
	if (changeFileSize)
	{
		fp->af->f_entry.DIR_FileSize = fp->posi + size;
		ChangeEntry(fp->af->f_entry.EntryBlockNum, fp->af->f_entry, EntryNameIndex(fp->af->f_entry.EntryBlockNum, fp->af->f_entry));
	}
	if (changeFirstClus)
	{
		fp->af->f_entry.DIR_FstClus = ClusList[0];
		ChangeEntry(fp->af->f_entry.EntryBlockNum, fp->af->f_entry, EntryNameIndex(fp->af->f_entry.EntryBlockNum, fp->af->f_entry));
	}

	// 释放内存，修改posi， 返回size
	if (ClusList != NULL)
	{
		free(ClusList);
	}
	fp->posi += size;
	return size;
}

// 1:SEEK_SET, 0:SEEK_CUR, -1: SEEK_END
int Seek(OpenFile* fp, long offset, int origin)
{
	int cur;
	if(origin == 1)
	{
		cur = 0;
	}
	else if (origin == 0)
	{
		cur = fp->posi;
	}
	else
	{
		cur = fp->af->f_entry.DIR_FileSize;
	}
	cur += offset;
	if (cur < 0 || cur > fp->af->f_entry.DIR_FileSize)
	{
		printf("Your seek is out of range.\n");
		return 0;
	}
	else
	{
		fp->posi = cur;
		return 1;
	}

}