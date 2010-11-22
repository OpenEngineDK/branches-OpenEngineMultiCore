#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <sys/types.h>
#include <semaphore.h>

namespace OpenEngine {
namespace Core {

class Semaphore
{    
	//----------
	//Functions:
	public:
		inline Semaphore(int initVal = 1)
		{
			sem_init(&semaphore, 0, initVal);
		}
		inline ~Semaphore()
		{
			sem_destroy(&semaphore);
		}
		inline void Wait()
		{
			sem_wait(&semaphore);
		}
		inline void Post()
		{
			sem_post(&semaphore);
		}
	protected:
	private:
	//----------
	//Variables:
	public:
	protected:
	private:
		sem_t semaphore;
};

}}
#endif
