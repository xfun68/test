#!/bin/sh

# 查询银元宝消费的code和time
cat 2009-06-16* | grep "|UserIBPay|.*Subject_ID = 4" | awk -F"," '{printf "%s %s\n", $5, $8}'

# 查询所有失败的银元宝消费的code和time
cat 2009-06-1* | grep "|UserIBPay|-.*Subject_ID = 4" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq

# 查询所有失败的金元宝消费的code和time
cat 2009-06-1* | grep "|UserIBPay|-.*Subject_ID = 3" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"," '{printf "%s%s%s%s%s%s\n", $5, $8, $10, $11, $12, $13}' | sort | uniq
# 金元宝消费记录 -1454 或者 -1451 Code
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s\n", $15}' | sort | uniq
# 金元宝消费记录 -1454 或者 -1451 Code Time Subject DeitailID
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s %s %s %s\n", $15, $18, $20, $24}' | sort | uniq
# 金元宝消费记录 -1454 或者 -1451 Code Time Subject Price DiscountPrice DeitailID
cat 2009-06-1* | grep "|UserIBPay|-145[14].*Subject_ID = 3" | awk -F"[|,]" '{printf "%s %s %s%s%s%s\n", $15, $18, $20, $22, $23, $24}' | sort | uniq

