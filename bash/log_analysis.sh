#!/bin/sh

# ��ѯ��Ԫ�����ѵ�code��time
cat 2009-06-16* | grep "|UserIBPay|.*Subject_ID = 4" | awk -F"," '{printf "%s %s\n", $5, $8}'

# ��ѯ����ʧ�ܵ���Ԫ�����ѵ�code��time
cat 2009-06-1* | grep "|UserIBPay|-.*Subject_ID = 4" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq

# ��ѯ����ʧ�ܵĽ�Ԫ�����ѵ�code��time
cat 2009-06-1* | grep "|UserIBPay|-.*Subject_ID = 3" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq
# ��Ԫ�����Ѽ�¼ -1454 ���� -1451 Code
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s\n", $15}' | sort | uniq
# ��Ԫ�����Ѽ�¼ -1454 ���� -1451 Code Time Subject DeitailID
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s %s %s %s\n", $15, $18, $20, $24}' | sort | uniq
# ��Ԫ�����Ѽ�¼ -1454 ���� -1451 Code Time Subject Price DiscountPrice DeitailID
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s %s %s%s%s%s\n", $15, $18, $20, $22, $23, $24}' | sort | uniq

# ��time_tת���ɿɶ�ʱ��
grep "UserIBPay.*|7119351"  2009-06-2* | awk -F"[ |=,]" '{printf "%s %s %s %04s %s\n", $25, $35, strftime("%Y-%m-%d_%H:%M:%S",$40), $64, $69}' | cat -n

### ͳ�Ʋ������Ϊ0�ļ�¼
awk -F"[| ]" '{if ($9 == 0 && $8 != "Gateway" && $8 != "N/A") printf "%s %s %-20s %5s %11s %-32s\n", $1, $2, $8, $9, $10, $14}' 2009-06-22* | cat -n
