#include <asf.h>
#include <stdio.h>
#include <string.h>
#include "myDefine.h"



int findKeyword(const char *haystack, const char *keyword) {
	int haystackLength = strlen(haystack);
	int keywordLength = strlen(keyword);

	for (int i = 0; i <= haystackLength - keywordLength; i++) 
	{
		int j;
		for (j = 0; j < keywordLength; j++) 
		{
			if (haystack[i + j] != keyword[j]) 
			{
				break;
			}
		}
		if (j == keywordLength) 
		{
			return i; // 找到关键字，返回位置
		}
	}

	return -1; // 未找到关键字
}
