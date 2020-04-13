# FAT12inC
This is a simple version of FAT12 system in C.  
  
You can see all the declaraions of functions and global variables in `view.h`. 
And `top.c` `middle.c` `bottom.c` respectively provide the implementations of the __top__, __middle__ and __bottom__ modules of the FAT12 system while `main.c` and `test.img` are for testing. 
  
## Commands provided

| Command       | Description   |
| ------------- | ------------- |
| help          |To get the information of each command.
| mbr           |Check the information on MBR.
| dir [pattern] |Display all the directories and files in the current directory of the order you provide.
| tree [path] |Display the directory tree of the path you provide.
| cd [path]|To enter the directory you want.
|md [path]|Create a directory.
|rd [path]|Delete a directory.
|copy [path]filename[+…] [path]filename|To create or modify a file as the copy of con or other files(<=9).
|type [path]filename|Display a file's content.
|del [path]filename|Delete a file.
|sedit [path]filename|A simple version editor to modify your file.
|exit|To exit the FAT12 system.
  
## Note
This is an assignment for my os class and anybody can refer to this work for your practice.   
  
But please just do not copy the whole projects directly. My motivation is to provide a possible version for anyone who has difficulty in finishing the whole system on their own.
 
