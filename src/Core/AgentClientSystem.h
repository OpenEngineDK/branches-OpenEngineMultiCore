#ifndef _OE_AGENT_CLIENT_SYSTEM_H
#define _OE_AGENT_CLIENT_SYSTEM_H

#include <Core/Semaphore.h>

namespace OpenEngine {
namespace Core {

template<int BufferSize, typename T>
class AgentClientSystem
{
	public:
		T Data[BufferSize];
		OpenEngine::Core::Semaphore agent;
		unsigned int agentpos;
		OpenEngine::Core::Semaphore client;
		unsigned int clientpos;
	
		AgentClientSystem()
			: agent(BufferSize), client(0)
		{
		}
};

}}
#endif //_OE_AGENT_CLIENT_SYSTEM_H
