#!/bin/bash
#1 2009-04-24
#2 16:16:31
#3 1002
#4 UserIBPay
#5 -1454
#6 10000
#7 200700
#8 [Gender:1,
#9 Occupation:0,
#10 Level:0],
#11 Rating_ID
#12 =
#13 1002,
#14 IB_Code
#15 =
#16 88272,
#17 Pkg_Flg
#18 =
#19 1,
#20 IB_Count
#21 =
#22 10,
#23 Pay_Time
#24 =
#25 Fri
#26 Apr
#27 24
#28 16:17:28
#29 2009,
#30 User_IP
#31 =
#32 2101707875,
#33 Subject_ID
#34 =
#35 3,
#36 Audit_Flag
#37 =
#38 1,
#39 Price=
#40 96,
#41 Discount_Price
#42 =
#43 96,
#44 Detail_ID
#45 =
#46 105000022000011,
#47 Total_Pay_Amount
#48 =
#49 0
#2009-04-24 -------------------- UserIBPayAudit
#17:07:57
#1002
#UserIBPayAudit
#1
#10000
#0
#Rating_ID
#=
#1002,
#Subject
#=
#3,
#Bgn_DID
#=
#105000022000005,
#End_DID
#=
#105000022000028,
#Ret_Audit_Cnt
#=
#10,
#Detail_ID[0]
#=
#105000022000027,
#Pay_Amount[0]
#=
#128
#...

tail -f -n100 `ls | tail -n1` | \
awk -F'[ |]' 'BEGIN{
}
{
    print $0
    print $2 " " $3
}'

