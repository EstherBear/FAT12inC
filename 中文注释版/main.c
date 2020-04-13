#include"view.h"

int main()
{
	FILE* fat12;
	// 打开映像文件
	fat12 = fopen("test.img", "rb+");//只读二进制
	if (fat12 == NULL)
	{
		printf("The img file cannot open.");
		exit(0);
	}
	
	// 加载映像文件
	if (!LoadDisk(ramFDD14, fat12))
	{
		printf("Load disk error");
	}
	// 测试
	init();
	char *curName;
	const char s[2] = " ";
	curName = strtok(CurDirPtr->DIR_NAME, s);
	printf("%s:> ", curName);
	gets(input);
	while (strncmp(input, ComSet[0], strlen(input) != 0))
	{
		CommandImp();
		curName = strtok(CurDirPtr->DIR_NAME, s);
		printf("%s:> ", curName);
		gets(input);
	}
	
	// 写回映像文件
	printf("Do u want to save the change to the img file?\n");
	char ch = 0;
	scanf("%c", &ch);
	if (ch == 'y')
	{
		if (!FreeDisk(ramFDD14, fat12))
		{
			printf("Free disk error");
			printf(ferror(fat12));
		}
	}
	scanf("%c", &ch);
	// 关闭映像文件
	fclose(fat12);
	
}