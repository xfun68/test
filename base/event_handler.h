#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <inttypes.h>       // for intxx_t
#include <string>           // for std::string

namespace socketor
{
    class Connection;
    class ConnectionManager;

    class EventHandler
    {
    public:
        EventHandler(std::string handler_name = std::string("EventHandler"));
        EventHandler(const EventHandler& rhs);
        const EventHandler& operator = (const EventHandler& rhs);
        virtual ~EventHandler(void);

        virtual int32_t onIdle(ConnectionManager* connection_manager);
        virtual int32_t onBeginLoop(ConnectionManager* connection_manager);
        virtual int32_t onEndLoop(ConnectionManager* connection_manager);

        virtual int32_t onListenFailed(Connection* connection);
        virtual int32_t onAccept(Connection* connection);
        virtual int32_t onAcceptFailed(Connection* connection);
        virtual int32_t onConnectEstablished(Connection* connection);
        virtual int32_t onConnectFailed(Connection* connection);
        virtual int32_t onClose(Connection* connection);
        virtual int32_t onRead(Connection* connection);
        virtual int32_t onWrite(Connection* connection);
        virtual int32_t onPrimaryData(Connection* connection);
        virtual int32_t onError(Connection* connection);
        virtual int32_t onHup(Connection* connection);
        virtual int32_t onUnknownEvent(Connection* connection);
        virtual int32_t onRelease(Connection* connection);

    private:
        std::string handler_name_;
    };

} // end of namespace socketor

#endif // _EVENT_HANDLER_H_

