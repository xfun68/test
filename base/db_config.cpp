#include "db_config.h"

#include <SysEncryptConfig.h>
#include <SysAlgorithmMD5.h>

#include <string.h>

typedef struct __SECURITY_KEY__
{
    uint8_t arrucKey[16];
    int32_t nBeginTime;
    int32_t nEndTime;
    char    strAuthor[32];
    uint8_t arrucCheckSum[16];
}SECURITY_KEY;

static uint8_t sgarrucKey[] = {
    0x01, 0xF4, 0x55, 0x12,
    0x0A, 0x9F, 0x6E, 0x01,
    0xEE, 0x10, 0x1A, 0xA0,
    0x8C, 0xBD, 0xD1, 0x11
};

static SECURITY_KEY sgSecurity_Key = {0};

int isSecurityKeyValid(SECURITY_KEY &security_Key)
{
    int nResult  = E_ERROR;
    int nRetCode = E_ERROR;

    uint8_t arrucInput[80];
    uint8_t arrucDigest[16];
    // 
    time_t tmCurrentTime = 0;
    memcpy(arrucInput, &security_Key, sizeof(SECURITY_KEY) - 16);
    memcpy(arrucInput + sizeof(SECURITY_KEY) - 16, sgarrucKey, 16);
    nRetCode = SysAlgorithmMD5::compute(arrucInput, sizeof(SECURITY_KEY), arrucDigest);
    ASSERT(nRetCode == S_SUCCESS);

    nRetCode = memcmp(arrucDigest, security_Key.arrucCheckSum, 16);
    if (nRetCode != 0)
    {
        goto ExitError;
    }

    security_Key.nBeginTime = htonl(security_Key.nBeginTime);
    security_Key.nEndTime   = htonl(security_Key.nEndTime);
    time(&tmCurrentTime);
    if ((int32_t)tmCurrentTime > security_Key.nEndTime)
    {
        //goto ExitError;
    }

    nResult = S_SUCCESS;
ExitError:
    return nResult;
}

int loadKey(const char *strKeyFile /* = NULL */)
{
    int nResult = E_ERROR;
    int nRetCode = E_ERROR;
    
    // uint8_t arrucKeyBuffer[80] = {0};

    size_t  stReadBufferLen = 0;
    // int     nIsSecurityKeyValid = false;
    // int     i = 0;

    FILE *pKeyFile = NULL;

    if (strKeyFile == NULL)
    {
        goto ExitError;
    }
    
    pKeyFile = fopen(strKeyFile, "rb");
    if (pKeyFile == NULL)
    {
        goto ExitError;
    }
    while (!feof(pKeyFile))
    {
        stReadBufferLen = fread(&sgSecurity_Key, sizeof(SECURITY_KEY), 1, pKeyFile);
    }
 
    nRetCode = isSecurityKeyValid(sgSecurity_Key);
    if (nRetCode != S_SUCCESS)
    {
        goto ExitError;
    }
   
//ExitOK:
    nResult = S_SUCCESS;
ExitError:
    if (pKeyFile != NULL)
    {
        fclose(pKeyFile);
    }
    return nResult;
}


DBConfig::DBConfig(void)
{
    memset(&db_param_, 0, sizeof(db_param_));
}

const DBConfig& DBConfig::operator = (const DBConfig& rhs)
{
    if (this == &rhs)
    {
        goto ExitOK;
    }

    memcpy(&db_param_, &rhs, sizeof(db_param_));

    db_param_.strHostName[sizeof(db_param_.strHostName) - 1] = '\0';
    db_param_.strUserName[sizeof(db_param_.strUserName) - 1] = '\0';
    db_param_.strServiceName[sizeof(db_param_.strServiceName) - 1] = '\0';
    db_param_.strPassword[sizeof(db_param_.strPassword) - 1] = '\0';

ExitOK:

    return *this;
}

DBConfig::~DBConfig(void)
{
}

const char* DBConfig::ip(void) const
{
    return db_param_.strHostName;
}

void DBConfig::set_ip(const char* new_ip)
{
    memcpy(db_param_.strHostName, new_ip, sizeof(db_param_.strHostName));
    db_param_.strHostName[sizeof(db_param_.strHostName) - 1] = '\0';
}

uint16_t DBConfig::port(void) const
{
    return db_param_.usPort;
}

void DBConfig::set_port(uint16_t new_port)
{
    db_param_.usPort = new_port;
}

const char* DBConfig::user_name(void) const
{
    return db_param_.strUserName;
}

void DBConfig::set_user_name(const char* new_user_name)
{
    memcpy(db_param_.strUserName, new_user_name, sizeof(db_param_.strUserName));
    db_param_.strHostName[sizeof(db_param_.strUserName) - 1] = '\0';
}

const char* DBConfig::password(void) const
{
    return db_param_.strPassword;
}

// 此处直接设置密文密码   
void DBConfig::set_password(const char* new_password)
{
    int32_t retcode = E_ERROR;
    char value[32] = {0};
    char encrPassword[128] = {0};
    char decrPassword[128] = {0};
    int32_t hexDigits[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    SysEncryptConfig gSysEncryptConfig;
    int outputLen;
    // decrypt db_psw
    retcode = loadKey("AGIP.key");
    if (retcode != S_SUCCESS)
    {
        printf("Cannot load \"AGIP.key\", program closed.\n");
    }

    snprintf(encrPassword, 127, "%s", new_password);

    for (size_t i = 0; i < strlen(encrPassword) / 2; i++)
    {
        int bigEndian;
        char temp = encrPassword[2 * i];

        if ((temp >= '0') && (temp <= '9'))
        {
            bigEndian = hexDigits[temp - '0'];
        }
        else 
        {
            bigEndian = hexDigits[temp - 'a' +10];
        }

        temp = encrPassword[2 * i + 1];

        if ((temp >= '0') && (temp <= '9'))
        {
            decrPassword[i] = (bigEndian << 4) + hexDigits[temp - '0'];
        }
        else 
        {
            decrPassword[i] = (bigEndian << 4) + hexDigits[temp - 'a' + 10];
        }
    }

    gSysEncryptConfig.decrypt(
        decrPassword,
        (int)strlen(encrPassword) / 2,
        sgSecurity_Key.arrucKey,
        16,
        value,
        128,
        &outputLen
    );

    value[outputLen] = '\0';

    memcpy(db_param_.strPassword, value, sizeof(db_param_.strPassword));
    db_param_.strPassword[sizeof(db_param_.strPassword) - 1] = '\0';
}

const char* DBConfig::service_name(void) const
{
    return db_param_.strServiceName;
}

void DBConfig::set_service_name(const char* new_service_name)
{
    memcpy(db_param_.strServiceName, new_service_name, sizeof(db_param_.strServiceName));
    db_param_.strServiceName[sizeof(db_param_.strServiceName) - 1] = '\0';
}

int32_t DBConfig::max_time_out(void) const
{
    return db_param_.nMaxTimeOut;
}

void DBConfig::set_max_time_out(int32_t new_max_time_out)
{
    db_param_.nMaxTimeOut = new_max_time_out;
}

int32_t DBConfig::use_pool(void) const
{
    return db_param_.nUseDatabasePool;
}

void DBConfig::set_use_pool(int32_t new_use_pool)
{
    db_param_.nUseDatabasePool = new_use_pool;
}

int32_t DBConfig::min_connection(void) const
{
    return db_param_.nMinConnection;
}

void DBConfig::set_min_connection(int32_t new_min_connection)
{
    db_param_.nMinConnection = new_min_connection;
}

int32_t DBConfig::max_connection(void) const
{
    return db_param_.nMaxConnection;
}

void DBConfig::set_max_connection(int32_t new_max_connection)
{
    db_param_.nMaxConnection = new_max_connection;
}

int32_t DBConfig::stm_cache_size(void) const
{
    return db_param_.nStatementCacheSize;
}

void DBConfig::set_stm_cache_size(int32_t new_stm_cache_size)
{
    db_param_.nStatementCacheSize = new_stm_cache_size;
}

const DATABASE_PARAM& DBConfig::db_param(void) const
{
    return db_param_;
}

void DBConfig::set_db_param(const DATABASE_PARAM& new_db_param)
{
    memcpy(&db_param_, &new_db_param, sizeof(db_param_));

    db_param_.strHostName[sizeof(db_param_.strHostName) - 1] = '\0';
    db_param_.strUserName[sizeof(db_param_.strUserName) - 1] = '\0';
    db_param_.strServiceName[sizeof(db_param_.strServiceName) - 1] = '\0';
    db_param_.strPassword[sizeof(db_param_.strPassword) - 1] = '\0';
}

