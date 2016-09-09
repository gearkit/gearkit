#ifndef __KIT_BUFFER_FACTORY_H__
#define __KIT_BUFFER_FACTORY_H__

#include "Singleton.h"
#include "Buffer.h"

namespace kit {

class BufferFactory 
{
public:
    Buffer* createBuffer(size_t size = 16);
    void destroyBuffer(Buffer* buffer);

};

} // namespace kit

#endif

