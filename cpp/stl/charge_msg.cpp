#include "charge_msg.h"

#include "configer.h"

#include <limits.h>

// constructors and destructors
ChargeMsg::ChargeMsg(void) :
    Message(Configer::TheConfig().GetUInt32("time_out", 60),
        Configer::TheConfig().GetUInt32("turn_count", 3)),
    detail_id_(0),
    account_id_(0),
    gateway_id_(0),
    subject_id_(0),
    audit_flag_(0),
    charge_amount_(0),
    charge_time_(0),
    result_(0),
    send_type_(0)
{
}

ChargeMsg::ChargeMsg(const ChargeMsg& other) :
    Message(other.time_out_seconds(), other.max_turn_count())
{
    detail_id_ = other.detail_id_;
    account_id_ = other.account_id_;
    gateway_id_ = other.gateway_id_;
    subject_id_ = other.subject_id_;
    audit_flag_ = other.audit_flag_;
    charge_amount_ = other.charge_amount_;
    charge_time_ = other.charge_time_;
    result_ = other.result_;
    send_type_ = other.send_type_;

    Time interval = 0;
    Times times = 0;

    for (uint32_t i = 0; i < Message::MAX_TURN_COUNT; ++i)
    {
        other.GetTurn(i, interval, times);
        SetTrun(i, interval, times);
    }
}

const ChargeMsg& ChargeMsg::operator = (const ChargeMsg& other)
{
    Time interval = 0;
    Times times = 0;

    if (this == &other)
    {
        goto ExitOK;
    }
    
    Message::operator = (other);

    detail_id_ = other.detail_id_;
    account_id_ = other.account_id_;
    gateway_id_ = other.gateway_id_;
    subject_id_ = other.subject_id_;
    audit_flag_ = other.audit_flag_;
    charge_amount_ = other.charge_amount_;
    charge_time_ = other.charge_time_;
    result_ = other.result_;
    send_type_ = other.send_type_;

    for (uint32_t i = 0; i < Message::MAX_TURN_COUNT; ++i)
    {
        other.GetTurn(i, interval, times);
        SetTrun(i, interval, times);
    }

ExitOK:

    return *this;
}

ChargeMsg::~ChargeMsg(void)
{
}

// operations
ChargeMsg::DetailID ChargeMsg::detail_id(void) const
{
    return detail_id_;
}

void ChargeMsg::set_detail_id(DetailID new_detail_id)
{
    detail_id_ = new_detail_id;
}

ChargeMsg::AccountID ChargeMsg::account_id(void) const
{
    return account_id_;
}

void ChargeMsg::set_account_id(AccountID new_account_id)
{
    account_id_ = new_account_id;
}

ChargeMsg::GatewayID ChargeMsg::gateway_id(void) const
{
    return gateway_id_;
}

void ChargeMsg::set_gateway_id(GatewayID new_gateway_id)
{
    gateway_id_ = new_gateway_id;
}

ChargeMsg::SubjectID ChargeMsg::subject_id(void) const
{
    return subject_id_;
}

void ChargeMsg::set_subject_id(SubjectID new_subject_id)
{
    subject_id_ = new_subject_id;
}

ChargeMsg::AuditFlag ChargeMsg::audit_flag(void) const
{
    return audit_flag_;
}

void ChargeMsg::set_audit_flag(AuditFlag new_audit_flag)
{
    audit_flag_ = new_audit_flag;
}

ChargeMsg::Amount ChargeMsg::charge_amount(void) const
{
    return charge_amount_;
}

void ChargeMsg::set_charge_amount(Amount new_charge_amount)
{
    charge_amount_ = new_charge_amount;
}

ChargeMsg::Time ChargeMsg::charge_time(void) const
{
    return charge_time_;
}

void ChargeMsg::set_charge_time(Time new_charge_time)
{
    charge_time_ = new_charge_time;
}

ChargeMsg::Result ChargeMsg::result(void) const
{
    return result_;
}

void ChargeMsg::set_result(Result new_result)
{
    result_ = new_result;
}

ChargeMsg::SendType ChargeMsg::send_type(void) const
{
    return send_type_;
}

void ChargeMsg::set_send_type(SendType new_send_type)
{
    send_type_ = new_send_type;
}

