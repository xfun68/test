tail -f `ls | tail -n1` | \
awk -F"|" '{printf "%s %s %s %s %s %s %s\n", $1, $6, $7, $8, $9, $10, $11, $NF}' | \
awk 'BEGIN{
}
{
    print $0
}
END{}'


    if ($4 == "UserIBPay") {
        printf "%s %s %5s %s %s %s PKG %s Cnt %3s %s AF %s PO %-4s PD %-4s %s %-8s\n",
        $2, $4, $5, $6, $13, $16, $19, $22, $35, $38, $40, $43, $46, $49
        next
    }
    if ($4 == "UserIBPayAudit") {
        printf "%s %s %5s %s Sbj %s Bgn %s End %s Cnt %3s\n",
                $2,$4,$5, $6,    $13,   $16,   $19,   $22
        next
    }
    print $0
