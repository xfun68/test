#include "configer.h"

const char* DEF_CONF_FILE_NAME = "conf";

Configer::Configer(void)
: doc_(NULL) 
, proot_(NULL)
, file_loaded_(E_ERROR)
    , total_item_count_(0)
{
}

Configer::~Configer(void)
{
}


int Configer::LoadFile(const char* filename)
{
    int32_t result = E_ERROR;
    int32_t retcode = E_ERROR;

    file_loaded_ = E_ERROR;
    total_item_count_ = 0;
    value_list_.clear();

    if (NULL == filename)
    {
        filename = DEF_CONF_FILE_NAME;
    }

    if (NULL == (doc_ = new TiXmlDocument(filename)))
    {
        result = E_ERROR;
        goto ExitError;
    }

    if (S_SUCCESS != (retcode = doc_->LoadFile(filename)))
    {
        result = E_SYS_FILE_NOT_FOUND;
        goto ExitError;
    }

    if (NULL == (proot_ = doc_->RootElement()))
    {
        result = E_ERROR;
        goto ExitError;
    }

    file_loaded_ = S_SUCCESS;

    LoadConfig(proot_);

    result = S_SUCCESS;
ExitError:
    delete doc_;
    doc_ = NULL;

    return result;
}

void Configer::LoadConfig(TiXmlElement* proot)
{
    int32_t same_tag_count = 0;
    static int32_t lvl = -1;
    char buf[MAX_TAG_LEN] = {0};
    TiXmlElement* ptag = NULL;
    ++lvl;

    if (lvl > 0)
    {
        for (ptag = proot->FirstChildElement();
            ptag != NULL; 
            ptag = ptag->NextSiblingElement())
        {
            if (ptag->FirstChild()->Type() == TiXmlNode::ELEMENT)
            {
                LoadConfig(ptag->FirstChildElement());
            }
            else
            {
                snprintf(buf, MAX_TAG_LEN, "%s_%d", ptag->Value(), same_tag_count);
                value_list_[buf] = ptag->GetText();
                ++same_tag_count;
                ++total_item_count_;
            }
        }
    }
    else
    {
        for (ptag = proot->FirstChildElement();
            ptag != NULL; 
            ptag = ptag->NextSiblingElement())
        {
            if (ptag->FirstChild()->Type() == TiXmlNode::ELEMENT)
            {
                LoadConfig(ptag);
            }
            else
            {
                value_list_[ptag->ValueStr()] = ptag->GetText();
                ++total_item_count_;
            }
        }
    }
}

int Configer::GetConfigCnt(const char* prefix)
{
    int cnt = 0;
    char buf[MAX_TAG_LEN] = {0};

    for (cnt = 0; ; ++cnt)
    {
        snprintf(buf, MAX_TAG_LEN, "%s_%d", prefix, cnt);
        if (value_list_.find(buf) == value_list_.end())
        {
            break;
        }
    }

    return cnt;
}

int Configer::PrintConfig(void)
{
    int32_t     result = S_SUCCESS;
    int32_t     i = 0;

    if (GetInt32("print_config", 0) == 0)
    {
        result = E_ERROR;
        goto ExitError;
    }

    std::cout << "Total configuratino items count: " << total_item_count_ << std::endl;
    for (ValueIt it(value_list_.begin());
        it != value_list_.end();
        ++it)
    {
        std::cout.width(3);
        std::cout << i++ << ">";
        std::cout.width(32);
        std::cout << it->first << "  " << it->second << std::endl;
    }

    result = S_SUCCESS;
ExitError:

    return result; 
}

const char* Configer::GetString(const char* key, const char* default_value)
{
    if (value_list_.end() == value_list_.find(key))
    {
        return default_value;
    }
    else
    {
        return value_list_[key].c_str();
    }
}

const char* Configer::GetString(const char* key, char* value, const char* default_value)
{
    if (value_list_.end() == value_list_.find(key))
    {
        sprintf(value, "%s", default_value);
        return default_value;
    }
    else
    {
        sprintf(value, "%s", value_list_[key].c_str());
        return value_list_[key].c_str();
    }
}

const char* Configer::GetString(const char* key, uint32_t index, const char* default_value)
{
    char buf[MAX_TAG_LEN] = {0};

    snprintf(buf, MAX_TAG_LEN, "%s_%d", key, index);

    if (value_list_.end() == value_list_.find(buf))
    {
        return default_value;
    }
    else
    {
        return value_list_[buf].c_str();
    }
}

int64_t Configer::GetInt64(const char* key)
{
    return static_cast<int64_t>(strtoll(value_list_[key].c_str(), NULL, 0));
}

uint64_t Configer::GetUInt64(const char* key)
{
    return static_cast<uint64_t>(strtoull(value_list_[key].c_str(), NULL, 0));
}

int32_t Configer::GetInt32(const char* key, int32_t default_value)
{
    if (value_list_.end() == value_list_.find(key))
    {
        return default_value;
    }
    else
    {
        return static_cast<int32_t>(strtol(value_list_[key].c_str(), NULL, 0));
    }
}

int32_t Configer::GetInt32(const char* key, size_t index, int32_t default_value)
{
    char buf[MAX_TAG_LEN] = {0};

    snprintf(buf, MAX_TAG_LEN, "%s_%d", key, index);

    if (value_list_.end() == value_list_.find(buf))
    {
        return default_value;
    }
    else
    {
        return static_cast<int32_t>(strtol(value_list_[buf].c_str(), NULL, 0));
    }
}

uint32_t Configer::GetUInt32(const char* key, uint32_t default_value)
{
    if (value_list_.end() == value_list_.find(key))
    {
        return default_value;
    }
    else
    {
        return static_cast<uint32_t>(strtoul(value_list_[key].c_str(), NULL, 0));
    }
}

uint32_t Configer::GetUInt32(const char* key, size_t index, uint32_t default_value)
{
    char buf[MAX_TAG_LEN] = {0};
    snprintf(buf, MAX_TAG_LEN, "%s_%d", key, index);

    if (value_list_.end() == value_list_.find(buf))
    {
        return default_value;
    }
    else
    {
        return static_cast<uint32_t>(strtoul(value_list_[buf].c_str(), NULL, 0));
    }
}

int16_t Configer::GetInt16(const char* key)
{
    return static_cast<int16_t>(strtol(value_list_[key].c_str(), NULL, 0));
}

uint16_t Configer::GetUInt16(const char* key, uint16_t default_value)
{
    if (value_list_.end() == value_list_.find(key))
    {
        return default_value;
    }
    else
    {
        return static_cast<uint16_t>(strtoul(value_list_[key].c_str(), NULL, 0));
    }
}

uint16_t Configer::GetUInt16(const char* key, size_t index, uint16_t default_value)
{
    char buf[MAX_TAG_LEN] = {0};

    snprintf(buf, MAX_TAG_LEN, "%s_%d", key, index);

    if (value_list_.end() == value_list_.find(buf))
    {
        return default_value;
    }
    else
    {
        return static_cast<uint16_t>(strtoul(value_list_[buf].c_str(), NULL, 0));
    }
}

int8_t  Configer::GetInt8(const char* key)
{
    return static_cast<int8_t>(strtol(value_list_[key].c_str(), NULL, 0));
}

uint8_t Configer::GetUInt8(const char* key)
{
    return static_cast<uint8_t>(strtoul(value_list_[key].c_str(), NULL, 0));
}

double  Configer::GetDouble(const char* key)
{
    return static_cast<double>(strtod(value_list_[key].c_str(), NULL));
}

Configer& Configer::TheConfig(void)
{
    static Configer configer;

    return configer;
}

