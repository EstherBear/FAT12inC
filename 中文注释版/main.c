#include"view.h"

int main()
{
	FILE* fat12;
	// ��ӳ���ļ�
	fat12 = fopen("test.img", "rb+");//ֻ��������
	if (fat12 == NULL)
	{
		printf("The img file cannot open.");
		exit(0);
	}
	
	// ����ӳ���ļ�
	if (!LoadDisk(ramFDD14, fat12))
	{
		printf("Load disk error");
	}
	// ����
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
	
	// д��ӳ���ļ�
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
	// �ر�ӳ���ļ�
	fclose(fat12);
	
}