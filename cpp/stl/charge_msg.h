#ifndef _CHARGE_MSG_H_
#define _CHARGE_MSG_H_

#include "message.h"

class ChargeMsg : public Message
{
public:
    typedef ChargeMsg& ChargeMsgRef;
    typedef ChargeMsg* ChargemsgPtr;

    typedef uint64_t DetailID;
    typedef uint32_t AccountID;
    typedef uint32_t GatewayID;
    typedef int16_t SubjectID;
    typedef int16_t AuditFlag;
    typedef int32_t Amount;

    typedef int32_t Result;
    typedef int32_t SendType;

    enum AUDIT_FLAG
    {
        AUDIT = 1, // 需要返回充值总额  
        SEND,      // 首发或重发消息，无需返回充值总额  
        REISSUE,   // 重发消息，GS需要根据此条消息更新本地记录，无需返回充值总额  
        ERASE      // 重发消息，GS需要根据此条消息删除相应充值记录并扣减帐号余额，无需返回充值总额  
    };

public:
    // constructors and destructors
    ChargeMsg(void);
    ChargeMsg(const ChargeMsg& other);
    const ChargeMsg& operator = (const ChargeMsg& other);
    ~ChargeMsg(void);

    // operations
    DetailID detail_id(void) const;
    void set_detail_id(DetailID new_detail_id);

    AccountID account_id(void) const;
    void set_account_id(AccountID new_account_id);

    GatewayID gateway_id(void) const;
    void set_gateway_id(GatewayID new_gateway_id);

    SubjectID subject_id(void) const;
    void set_subject_id(SubjectID new_subject_id);

    AuditFlag audit_flag(void) const;
    void set_audit_flag(AuditFlag new_audit_flag);

    Amount charge_amount(void) const;
    void set_charge_amount(Amount new_charge_amount);

    Time charge_time(void) const;
    void set_charge_time(Time new_charge_time);

    Result result(void) const;
    void set_result(Result new_result);

    SendType send_type(void) const;
    void set_send_type(SendType new_send_type);

private:
    // methods

    // attributes
    DetailID detail_id_;
    AccountID account_id_;
    GatewayID gateway_id_;
    SubjectID subject_id_;
    AuditFlag audit_flag_;
    Amount charge_amount_;
    Time charge_time_;

    Result result_;
    SendType send_type_;
};

#endif // _CHARGE_MSG_H_
