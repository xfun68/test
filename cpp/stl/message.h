#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdint.h>
#include <time.h>

class Message
{
public:
    typedef Message* MessagePtr;
    typedef Message& MessageRef;

    typedef uint32_t Time;
    typedef uint32_t Times;
    typedef int32_t TurnCount;
    typedef uint32_t Index;
    typedef uint32_t SequenceID;

    typedef struct _TURN_
    {
        Time interval;
        Times times;
    } Turn;

    enum _ENUM_CONST_VALUE_
    {
        MAX_TURN_COUNT = 3
    };

public:
    Message(void);
    Message(uint32_t time_out_second, uint32_t turn_count);
    Message(const Message& other);
    const Message& operator = (const Message& other);
    ~Message(void);

    bool operator < (const Message& other);
    bool operator == (const Message& other);

    SequenceID sequence_id(void) const;
    void set_sequence_id(SequenceID new_sequence_id);

    Index index(void) const;
    void set_index(Index new_index);

    TurnCount max_turn_count(void) const;
    void set_max_turn_count(TurnCount new_turn_count);

    TurnCount curr_turn_count(void) const;
    int32_t GetTurn(uint32_t turn_index, Time& interval, Times& times) const;
    int32_t SetTrun(uint32_t turn_index, Time new_interval, Times new_times);

    Time time_out_seconds(void) const;
    void set_time_out_seconds(Time new_time_out_seconds = 10);

    Time last_sent_time(void) const;
    void set_last_sent_time(Time new_last_sent_time = time(NULL));
    Times sent_times(void) const;
    void set_sent_times(Times new_sent_times);

    void IncreaseSentTimes(void);
    int32_t IsTimeout(void) const;
    int32_t NeedToSend(void) const;

private:
    static SequenceID serial_number_;
    SequenceID sequence_id_;
    Index index_;
    TurnCount max_turn_count_;
    TurnCount curr_turn_count_;
    Time time_out_seconds_;
    Time last_sent_time_;
    Times sent_times_;
    Turn turn_list_[MAX_TURN_COUNT+1];
};

#endif // _MESSAGE_H_

