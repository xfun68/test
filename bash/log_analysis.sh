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

