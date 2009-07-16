#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include "aes.h"
#include <time.h> 

int main(int argc, char * argv[])
{
    struct aes_ctx aes;
    int32_t retcode = -1;
	
	char szbuf[96]={39,30,70,-110,-110,89,90,73,-38,122,81,-107,-96,103,9,47,-55,70,-95,11,-116,63,47,55,-55,-125,-79,5,-99,-115,71,108,51,-88,-97,-84,-35,-79,-80,49,99,30,-85,-70,-101,63,46,-70,57,-60,-69,-2,-23,-128,106,-111,-101,108,-38,-35,30,-17,109,18,-15,92,-75,-52,86,-67,41,-16,-89,-49,29,-42,-19,77,-15,-82,-81,113,-120,28,8,-71,-5,38,-4,-25,-113,80,12,75,-27,5};
	unsigned char sztmp[100];
	char pkey[] = {124,106,46,-75,98,92,-88,55,-94,13,77,-119,-77,-77,-77,101};

	gen_tabs();
	if(aes_set_key(&aes, (const u8 *)pkey , 16) != 0) {
		printf("can't set key : %s ",pkey);
		return 0;
	}
	printf("Input is :\n");
	for(int j=0;j<96;j++){
		printf("%d,",(char)szbuf[j]);
	}
	printf("\n");
	
    // ¼ÓÃÜ
	retcode = AesEncrypt(&aes,(unsigned char*)szbuf,sztmp,96);
	printf("Encryp Result is :  %d\n", retcode);
	for(int j=0;j<100;j++){
		printf("%d,",(char)sztmp[j]);
	}
	printf("\n");

    // ½âÃÜ
    memset(szbuf, 0, sizeof(szbuf));
	retcode = AesDecrypt(&aes,sztmp,(unsigned char*)szbuf);
	printf("Descrpy Result is : %d\n", retcode);
	for(int j=0;j<96;j++){
		printf("%d,",(char)szbuf[j]);
	}
	printf("\n");
	
	return 0;
}


