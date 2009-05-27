// =====================================================================================
// 
//       Filename:  test-map.cc
// 
//    Description:  test stl map
// 
//        Version:  1.0
//        Created:  04/28/2009 02:19:58 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Wang Xiaofeng (xfun68), xfun68@gmail.com
//        Company:  Linekong
// 
// =====================================================================================

#include	"charge_msg.h"


#include	<inttypes.h>
#include	<limits.h>
#include    <cstdlib>
#include    <fstream>
#include    <iomanip>
#include    <iostream>
#include    <map>

using namespace std;

typedef map<ChargeMsg::SequenceID, ChargeMsg> MsgQ;

/*
 * 时间转换函数，用于将以秒为单位的时间转换为可读格式  
 */
const char* time2String(time_t tm)
{
    struct tm* timeptr = localtime(&tm);
    static char result[64];

    sprintf(result, "%d-%02d-%02d_%02d:%02d:%02d", 
        1900 + timeptr->tm_year, 
        timeptr->tm_mon+1, 
        timeptr->tm_mday, 
        timeptr->tm_hour,
        timeptr->tm_min, 
        timeptr->tm_sec);

    return result;
}

int loadChargeMsg(MsgQ& charge_q, int count)
{
    static int total_msg_count = 0;
    static uint64_t cur_detail_id = 0;
    
    if ((count < 1) || (count > 100000)) {
        return 0;
    }

    for (int i = 0; i < count; ++i) {
        ChargeMsg cm;
        cm.set_result(0);
        cm.set_detail_id(++cur_detail_id);
        cm.set_account_id(123456789);
        cm.set_gateway_id(1001);
        cm.set_subject_id(3);
        cm.set_charge_amount(10000);
        cm.set_charge_time(time(NULL));
        cm.set_audit_flag(1);

        pair<MsgQ::iterator, bool> p = charge_q.insert(make_pair(cm.sequence_id(), cm));
        if (p.second) {
            ++total_msg_count;
        }
    }

    return total_msg_count;
} // -----  end of function loadChargeMsg  -----

void reportMsgQState(const MsgQ& charge_q)
{
    cout << "Q current size is " << setw(7) << charge_q.size() << ", "
        << "max_size is " << charge_q.max_size() << "."
        << endl;

    return;
} // -----  end of function reportQState  -----

class ShowChargeMsg
{
public:
    ShowChargeMsg(void) {}                             // constructor
    void operator ()(MsgQ::value_type& vt) const;
}; // -----  end of class ShowChargeMsg  -----

void ShowChargeMsg::operator ()(MsgQ::value_type& vt) const
{
    const ChargeMsg& cm = vt.second;
    cout << "Key=" << setfill('0') << setw(6) << vt.first
        << " DID=" << setfill('0') << setw(15) << cm.detail_id()
        << " GwID=" <<  cm.gateway_id()
        << " UserID=" << cm.account_id()
        << " Sbj=" << cm.subject_id()
        << " Amt=" << cm.charge_amount()
        << " Time=" << time2String(cm.charge_time())
        << endl;

    return;
}

class DelChargeMsg
{
public:
    DelChargeMsg(MsgQ& msg_q) : 
        msg_q_(msg_q),
        count_(0) 
    {
    }

    void operator ()(MsgQ::value_type& vt) 
    {
        cout << "In DelChargeMsg ... " << endl;
        const MsgQ::mapped_type& value = vt.second;

        cout << "Checking "
            << "DID=" << setfill('0') << setw(15) << value.detail_id()
            << " GwID=" <<  value.gateway_id()
            << " UserID=" << value.account_id()
            << " Sbj=" << value.subject_id()
            << " Amt=" << value.charge_amount()
            << " Time=" << time2String(value.charge_time())
            << endl;

        if (value.detail_id() % 2 == 0) {
            if (msg_q_.erase(vt.first) > 0) {
            // if (msg_q_.find(vt.first) != msg_q_.end()) {
                ++count_;
                cout << "Deleting "
                    << "DID=" << setfill('0') << setw(15) << value.detail_id()
                    << " GwID=" <<  value.gateway_id()
                    << " UserID=" << value.account_id()
                    << " Sbj=" << value.subject_id()
                    << " Amt=" << value.charge_amount()
                    << " Time=" << time2String(value.charge_time())
                    << endl;
            }
        }
        cout << "Leave DelChargeMsg ... " << endl;
        return;
    }

    void showStat(void) {
        cout << " # " << count_ << " charge message(s) deleted." << endl;
    }

private:
    MsgQ& msg_q_;
    int count_;
};

// ===  FUNCTION  ======================================================================
//         Name:  main
//  Description:  main function
// =====================================================================================
int main (int argc, char *argv[])
{
    MsgQ charge_q;

    reportMsgQState(charge_q);

    loadChargeMsg(charge_q, 10);
    reportMsgQState(charge_q);

    for_each(charge_q.begin(), charge_q.end(), ShowChargeMsg());
    reportMsgQState(charge_q);

    for (MsgQ::iterator it(charge_q.begin()); it != charge_q.end(); ) {
        cout << "Checking DID= " << it->second.detail_id() << endl;
        if (it->second.detail_id() % 2 == 0) {
            ChargeMsg cs = it++->second;
            if (charge_q.erase(cs.detail_id()) > 0) {
                cout << "Delete DID= " << cs.detail_id() << endl;
            }
        } else {
            ++it;
        }
    }
    for_each(charge_q.begin(), charge_q.end(), ShowChargeMsg());
    reportMsgQState(charge_q);

    return EXIT_SUCCESS;
} // ----------  end of function main  ---------- 

