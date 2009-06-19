#ifndef  __CONFIGER_H__
#define  __CONFIGER_H__

#include  "OperationCode.h"

#include  "tinyxml.h"

#include <inttypes.h>
#include <string.h>
#include <map>

class Configer
{
public:
    typedef std::map<std::string, std::string> ValueList;
    typedef std::map<std::string, std::string>::iterator ValueIt;

    enum {MAX_TAG_LEN = 128};

    Configer(void);
    ~Configer(void);

    static Configer& TheConfig(void);

    int LoadFile(const char* filename);
    void LoadConfig(TiXmlElement* proot);
    int PrintConfig(void);
    int GetConfigCnt(const char* prefix);

    const char* GetString(const char* key, const char* default_value);
    const char* GetString(const char* key, size_t index, const char* default_value);
    const char* GetString(const char* key, char* value, const char* default_value);
    int64_t GetInt64(const char* key);
    uint64_t GetUInt64(const char* key);
    int32_t GetInt32(const char* key, int32_t default_value);
    int32_t GetInt32(const char* key, size_t index, int32_t default_value);
    uint32_t GetUInt32(const char* key, uint32_t default_value);
    uint32_t GetUInt32(const char* key, size_t index, uint32_t default_value);
    int16_t GetInt16(const char* key);
    uint16_t GetUInt16(const char* key, uint16_t default_value);
    uint16_t GetUInt16(const char* key, size_t index, uint16_t default_value);
    int8_t GetInt8(const char* key);
    uint8_t GetUInt8(const char* key);
    double GetDouble(const char* key);

private:
    Configer(const Configer& other);
    Configer operator = (const Configer& other);

private:
    TiXmlDocument*  doc_;
    TiXmlElement*   proot_;

    ValueList       value_list_;

    int32_t         file_loaded_;
    int32_t         total_item_count_;
};

#endif  /*__CONFIGER_H__*/

