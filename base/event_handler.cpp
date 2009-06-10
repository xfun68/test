#include "event_handler.h"

#include "connection.h"
#include "OperationCode.h"

#include <iostream>

using namespace std;

namespace socketor
{

EventHandler::EventHandler(string handler_name) :
    handler_name_(handler_name)
{
#ifdef DEBUG_TRACE
    /*
    cout << "#[" << handler_name_ << "] "
        << "Constructor()"
        << endl;
    */
#endif // DEBUG_TRACE
}

EventHandler::EventHandler(const EventHandler& rhs)
{
    handler_name_ = rhs.handler_name_;
#ifdef DEBUG_TRACE
    /*
    cout << "#[" << handler_name_ << "] "
        << "Copy-Constructor()"
        << endl;
    */
#endif // DEBUG_TRACE
}

const EventHandler& EventHandler::operator = (const EventHandler& rhs)
{
    if (this != &rhs) {
        handler_name_ = rhs.handler_name_;
    }
#ifdef DEBUG_TRACE
    /*
    cout << "#[" << handler_name_ << "] "
        << "Assignment-Constructor()"
        << endl;
    */
#endif // DEBUG_TRACE
    return *this;
}

EventHandler::~EventHandler(void)
{
#ifdef DEBUG_TRACE
    /*
    cout << "#[" << handler_name_ << "] "
        << "Destructor()"
        << endl;
    */
#endif // DEBUG_TRACE
}

int32_t EventHandler::onIdle(ConnectionManager* connection_manager)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "] "
        << "onIdle()"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onBeginLoop(ConnectionManager* connection_manager)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "] "
        << "onBeginLoop()"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onEndLoop(ConnectionManager* connection_manager)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "] "
        << "onEndLoop()"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onListenFailed(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onListenFailed"
        << "] listen at "
        << connection->string_ip()
        << " "
        << connection->port()
        << " failed"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onAccept(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onAccept"
        << "] accept a connetion from "
        << connection->string_ip()
        << " "
        << connection->port()
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onAcceptFailed(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onAccept"
        << "] connection listen at "
        << connection->string_ip()
        << " "
        << connection->port()
        << " accept failed"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onConnectEstablished(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onConnectEstablished"
        << "] connection to "
        << connection->string_ip()
        << " "
        << connection->port()
        << " established"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onConnectFailed(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onConnectFailed"
        << "] connection to "
        << connection->string_ip()
        << " "
        << connection->port()
        << " failed"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onClose(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onClose"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " is gonna be closed"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onRead(Connection* connection)
{
#ifdef DEBUG_TRACE
    const int32_t BUF_SIZE = 1024;
    int32_t bytes = 0;
    int8_t buf[BUF_SIZE] = {0};
    connection->read(buf, BUF_SIZE, bytes);
    cout << "#[" << handler_name_ << "::"
        << "onRead"
        << "] recv "
        << bytes
        << " bytes data from "
        << connection->string_ip()
        << " "
        << connection->port()
        << endl << "\t\t"
        << "["
        << buf
        << "]"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onWrite(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onWrite"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " is ready for writing"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onPrimaryData(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onPrimaryData"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " has primary data for reading"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onError(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onError"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " error ocurred"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onHup(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onHup"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " huuuuuuup"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onUnknownEvent(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onUnknownEvent"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " find some unknow event to process"
        << endl;
#endif // DEBUG_TRACE
    return S_SUCCESS;
}

int32_t EventHandler::onRelease(Connection* connection)
{
#ifdef DEBUG_TRACE
    cout << "#[" << handler_name_ << "::"
        << "onRelease"
        << "] connection with "
        << connection->string_ip()
        << " "
        << connection->port()
        << " is gonna be released"
        << endl;
#endif // DEBUG_TRACE
    connection->set_auto_release(true);
    return S_SUCCESS;
}

} // end of namespace socketor

