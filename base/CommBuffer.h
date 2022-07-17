//
// Created by Administrator on 2022/7/17.
//

#ifndef TCPSERVER_COMMBUFFER_H
#define TCPSERVER_COMMBUFFER_H

#include <iostream>
#include <memory>
#include <cstring>

struct BuffData
{
    using DataPtr=std::shared_ptr<char>;
public:
    BuffData():m_cursize(0), m_maxsize(0), pdata(nullptr){}
    BuffData(int len):m_maxsize(len), m_cursize(0),pdata(new char[len]){}
    //扩充构造
    BuffData(int len, BuffData& val):m_maxsize(len), m_cursize(val.m_cursize),pdata(new char[len])
    {
        memmove(pdata.get(), val.pdata.get(), val.m_cursize);
    }
    ~BuffData(){}

    BuffData& operator=(const BuffData & val)
    {
        this->pdata = val.pdata;
        this->m_maxsize = val.m_maxsize;
        this->m_cursize = val.m_cursize;
        return *this;
    }
public:
    DataPtr pdata;
    int m_maxsize;
    int m_cursize;
};

class CommBuffer {
protected:
    BuffData m_data; /* 指向数据的指针 */
    int addVal(const char *pval,const int len)
    {
        if((nullptr == pval) || (0 >= len))
        {
            return -1;
        }
        if(nullptr == this->m_data.pdata)
        {
            BuffData temData(len << 1);
            this->m_data = temData;
        }
        int freeSize = this->m_data.m_maxsize - this->m_data.m_cursize;
        if(freeSize < len) // 剩余空间不足
        {
            int len = this->m_data.m_cursize + (len << 1);
            BuffData temData(len, this->m_data);
            this->m_data = temData;
        }

        memmove(this->m_data.pdata.get() + this->m_data.m_cursize, pval, len);
        this->m_data.m_cursize += len;
        return 0;
    }
public:
    CommBuffer()
    {}

    virtual ~CommBuffer(){}

    virtual int addData(const char *pval, const int len) = 0;
};


#endif //TCPSERVER_COMMBUFFER_H
