#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <pthread.h>

namespace OpenEngine {
    namespace Core {

class Mutex {
    
private:

    pthread_mutex_t _mutex;

public:
    Mutex() {
        pthread_mutex_init(&_mutex,NULL);
    }
    ~Mutex() {
        pthread_mutex_init(&_mutex,NULL);
    }
    void Lock() {
        pthread_mutex_lock(&_mutex);
    }
    void Unlock() {
        pthread_mutex_unlock(&_mutex);
    }
};

    }
}

#endif
