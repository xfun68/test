#include "message.h"

#include "configer.h"
#include "OperationCode.h"

#include <string.h>
#include <limits.h>

Message::SequenceID Message::serial_number_ = 0x00000000U;

Message::Message(void) :
    sequence_id_(++serial_number_),
    index_(0),
    max_turn_count_(1),
    curr_turn_count_(0),
    time_out_seconds_(10),
    last_sent_time_(0),
    sent_times_(0)
{
    memset(turn_list_, 0, sizeof(turn_list_));

    for (int32_t i = 0; i < max_turn_count(); ++i)
    {
        SetTrun(i,
            Configer::TheConfig().GetUInt32("turn_interval", i, 10),
            Configer::TheConfig().GetUInt32("turn_times", i, 10));
    }

    SetTrun(max_turn_count(),
        Configer::TheConfig().GetUInt32("turn_interval", max_turn_count(), 10),
        Configer::TheConfig().GetUInt32("turn_times", max_turn_count(), UINT_MAX));
}

Message::Message(uint32_t time_out_second, uint32_t turn_count) :
    sequence_id_(++serial_number_),
    index_(0),
    max_turn_count_(turn_count),
    curr_turn_count_(0),
    time_out_seconds_(time_out_second),
    last_sent_time_(0),
    sent_times_(0)
{
    set_max_turn_count(turn_count);
    memset(turn_list_, 0, sizeof(turn_list_));

    for (int32_t i = 0; i < max_turn_count(); ++i)
    {
        SetTrun(i,
            Configer::TheConfig().GetUInt32("turn_interval", i, 10),
            Configer::TheConfig().GetUInt32("turn_times", i, 10));
    }

    SetTrun(max_turn_count(),
        Configer::TheConfig().GetUInt32("turn_interval", max_turn_count(), 10),
        Configer::TheConfig().GetUInt32("turn_times", max_turn_count(), UINT_MAX));
}

Message::Message(const Message& other)
{
    sequence_id_ = other.sequence_id_;
    index_ = other.index_;
    max_turn_count_ = other.max_turn_count_;
    curr_turn_count_ = other.curr_turn_count_;
    time_out_seconds_ = other.time_out_seconds_;
    last_sent_time_ = other.last_sent_time_;
    sent_times_ = other.sent_times_;

    memcpy(turn_list_, other.turn_list_, sizeof(turn_list_));
}

const Message& Message::operator = (const Message& other)
{
    if (this == &other)
    {
        goto ExitOK;
    }
    
    sequence_id_ = other.sequence_id_;
    index_ = other.index_;
    max_turn_count_ = other.max_turn_count_;
    curr_turn_count_ = other.curr_turn_count_;
    time_out_seconds_ = other.time_out_seconds_;
    last_sent_time_ = other.last_sent_time_;
    sent_times_ = other.sent_times_;

    memcpy(turn_list_, other.turn_list_, sizeof(turn_list_));

ExitOK:

    return *this;
}

Message::~Message(void)
{
}

bool Message::operator < (const Message& other)
{
    return sequence_id() < other.sequence_id();
}

bool Message::operator == (const Message& other)
{
    return sequence_id() == other.sequence_id();
}

Message::SequenceID Message::sequence_id(void) const
{
    return sequence_id_;
}

void Message::set_sequence_id(SequenceID new_sequence_id)
{
    sequence_id_ = new_sequence_id;
}

Message::Index Message::index(void) const
{
    return index_;
}

void Message::set_index(Index new_index)
{
    index_ = new_index;
}

Message::TurnCount Message::max_turn_count(void) const
{
    return max_turn_count_;
}

void Message::set_max_turn_count(TurnCount new_turn_count)
{
    if (new_turn_count < 1)
    {
        max_turn_count_ = 1;
    }
    else if (new_turn_count > MAX_TURN_COUNT)
    {
        max_turn_count_ = MAX_TURN_COUNT;
    }
    else
    {
        max_turn_count_ = new_turn_count;
    }
}

Message::TurnCount Message::curr_turn_count(void) const
{
    return curr_turn_count_;
}

int32_t Message::GetTurn(uint32_t turn_index, Time& interval, Times& times) const
{
    int32_t result = 0;

    if (turn_index >= MAX_TURN_COUNT)
    {
        goto ExitError;
    }

    interval = turn_list_[turn_index].interval;
    times = turn_list_[turn_index].times;

    result = 1;
ExitError:

    return result;
}

int32_t Message::SetTrun(uint32_t turn_index, Time new_interval, Times new_times)
{
    int32_t result = 0;

    if (turn_index >= MAX_TURN_COUNT)
    {
        goto ExitError;
    }

    turn_list_[turn_index].interval = new_interval;
    turn_list_[turn_index].times = new_times;

    result = 1;
ExitError:

    return result;
}

Message::Time Message::time_out_seconds(void) const
{
    return time_out_seconds_;
}

void Message::set_time_out_seconds(Time new_time_out_seconds)
{
    time_out_seconds_ = new_time_out_seconds;
}

Message::Time Message::last_sent_time(void) const
{
    return last_sent_time_;
}

void Message::set_last_sent_time(Time new_last_sent_time)
{
    last_sent_time_ = new_last_sent_time;
}

Message::Times Message::sent_times(void) const
{
    return sent_times_;
}

void Message::set_sent_times(Times new_sent_times)
{
    sent_times_ = new_sent_times;
}

// 更新发送时间、发送次数和发送轮次值
void Message::IncreaseSentTimes(void)
{
    Times turn_send_times = 0;      // 轮次发送次数，累计值。

    last_sent_time_ = time(NULL);
    ++sent_times_;

    for (TurnCount turn_count = 0; turn_count <= MAX_TURN_COUNT; ++turn_count)
    {
        turn_send_times += turn_list_[turn_count].times;

        if (sent_times_ < turn_send_times)
        {
            curr_turn_count_ = turn_count;
            break;
        }
    }
}

// 仅作超时判定
int32_t Message::IsTimeout(void) const
{
    Time pass_secs = time(NULL) - last_sent_time_;

    if (pass_secs >= time_out_seconds_)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// 解决重发间隔判定的问题
int32_t Message::NeedToSend(void) const
{
    Time pass_secs = time(NULL) - last_sent_time_;

    if (curr_turn_count_ < max_turn_count())
    {
        if (pass_secs >= turn_list_[curr_turn_count_].interval)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return E_SYS_NET_OUT_OF_MAX_TIMES;
    }
}

