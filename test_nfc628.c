/*
 * 
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX_SIZE 261

int strToHex(char *ch,unsigned char *hex)
{
	unsigned int high,low;
	int tmp = 0;
	unsigned char *p = hex;	

	if(ch == NULL || hex == NULL){
		return -1;
	}

	printf("[command]:%s\n",ch);

	while(*ch){
		high = low = 0;
		if((*ch >= '0')&&(*ch <= '9'))
			high = *ch - '0';
		else if((*ch >= 'A')&&(*ch <= 'F'))
			high = *ch - 'A' + 10;
		else
			return -1;

		if((*(ch+1) >= '0')&&(*(ch+1) <= '9'))
			low = *(ch+1) - '0';
		else if((*ch >= 'A')&&(*ch <= 'F'))
			low = *(ch+1) - 'A' + 10;
		else
			return -1;	

		*p = ((high << 4)&0xf0) + (low&0x0f);
		p++;
		ch = ch + 2;	
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int fd, i, wrlen, rdlen;
	unsigned char *cmd;
	
	if(argc != 2){
		printf("usage: ./test_nfc628 [COMMAND]\n");
		return -1;
	}
	
	wrlen = strlen(argv[1])/2;

	cmd = (unsigned char*)malloc(wrlen);
	
	if(strToHex(argv[1], cmd) < 0){
		printf("命令格式错误，字母应为大写\n");
		return -1;
	}	
	

	fd = open("/dev/bnsem628", O_RDWR);
	if (fd < 0) {
		printf("cannot open /dev/nfc628\n");
		return -1;
	}

	if(write(fd, cmd, wrlen) < 0){
		printf("write faild\n");
	}

	sleep(1);

	rdlen = read(fd, cmd, MAX_SIZE);
	if(rdlen < 0){
		printf("read faild\n");
	}

	printf("[read data]");
	for(i = 0; i < rdlen; i++){
		printf("%02x", cmd[i]);
	}
	printf("\n");

	close(fd);
	return 0;
}
