#ifndef _DB_CONFIG_H_
#define _DB_CONFIG_H_

#include "EDatabase.h"

#include <sys/types.h>
#include <netinet/in.h>

class DBConfig
{
public:
    DBConfig(void);
    const DBConfig& operator = (const DBConfig& rhs);
    ~DBConfig(void);

    const char* ip(void) const;
    void set_ip(const char* new_ip);

    uint16_t port(void) const;
    void set_port(uint16_t new_port);

    const char* user_name(void) const;
    void set_user_name(const char* new_user_name);

    const char* password(void) const;
    void set_password(const char* new_password);

    const char* service_name(void) const;
    void set_service_name(const char* new_service_name);

    int32_t max_time_out(void) const;
    void set_max_time_out(int32_t new_max_time_out);

    int32_t use_pool(void) const;
    void set_use_pool(int32_t new_use_pool);

    int32_t min_connection(void) const;
    void set_min_connection(int32_t new_min_connection);

    int32_t max_connection(void) const;
    void set_max_connection(int32_t new_max_connection);

    int32_t stm_cache_size(void) const;
    void set_stm_cache_size(int32_t new_stm_cache_size);

    const DATABASE_PARAM& db_param(void) const;
    void set_db_param(const DATABASE_PARAM& new_db_param);

private:
    DBConfig(const DBConfig& rhs);

    DATABASE_PARAM db_param_;
};

#endif // _DB_CONFIG_H_

