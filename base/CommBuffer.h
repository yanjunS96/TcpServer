//
// Created by Administrator on 2022/7/17.
//

#ifndef TCPSERVER_COMMBUFFER_H
#define TCPSERVER_COMMBUFFER_H

#include <iostream>
#include <memory>
#include <cstring>

class CommBuffer {
    using DataPtr=std::shared_ptr<char>;
protected:
    DataPtr p_data; /* 指向数据的指针 */
    int m_size; /* 申请的空间的大小 */
    int m_curlen; /* 当前已经使用的长度 */
    int addVal(const char *pval,const int len)
    {
        if((nullptr == pval) || (0 >= len))
        {
            return -1;
        }
        if(nullptr == this->p_data)
        {
            this->m_size = (len << 1);
            this->m_curlen = 0;
            this->p_data = std::shared_ptr<char>(new char[this->m_size]); //申请两倍的空间
        }
        int freeSize = this->m_size - this->m_curlen;
        if(freeSize < len) // 剩余空间不足
        {
            this->m_size = this->m_curlen + (len << 1); //保证还拥有一个 len长度的剩余空间
            std::shared_ptr<char> tem_ptr(new char[this->m_size]);
            memmove(tem_ptr.get(), this->p_data.get(), this->m_curlen);
            this->p_data = tem_ptr;
        }

        memmove(this->p_data.get() + this->m_curlen, pval, len);
        this->m_curlen += len;
        return 0;
    }
public:
    CommBuffer():p_data(nullptr), m_size(0), m_curlen(0)
    {}

    virtual ~CommBuffer(){}

    virtual int addData(const char *pval, const int len) = 0;
};


#endif //TCPSERVER_COMMBUFFER_H
