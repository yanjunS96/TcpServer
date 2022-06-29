//
// Created by Administrator on 2022/6/26.
//

#ifndef MYSERVER_COMMSINGLE_H
#define MYSERVER_COMMSINGLE_H
#include <mutex>

namespace SelfServer{
#define CREAT_SINGLE(classname)  \
public:                          \
    static classname* Instance() \
    {                            \
        static classname *p_tem = nullptr; \
        if(!p_tem)     \
        {                        \
            static std::once_flag _flag;   \
            std::call_once(_flag, [&]{      \
                p_tem = new (std::nothrow) classname(); \
            });\
        }                        \
        return p_tem; \
    }                            \
private:                         \
    classname();                 \
    classname(const classname &) = delete; \
    classname &operator=(const classname&) = delete;

}
#endif //MYSERVER_COMMSINGLE_H
