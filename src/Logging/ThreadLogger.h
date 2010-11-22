// Generic thread stream logger.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS)
//
// This program is free software; It is covered by the GNU General
// Public License version 2 or any later version.
// See the GNU General Public License for more details (see LICENSE).
// --------------------------------------------------------------------

#ifndef _THREAD_LOGGER_H_
#define _THREAD_LOGGER_H_

#include <Logging/ILogger.h>
#include <Core/Thread.h>
#include <Core/Semaphore.h>
#include <Meta/Time.h>
#include <Utils/DateTime.h>

#include <string>
#include <ostream>

namespace OpenEngine {
namespace Logging {

using std::ostream;
using std::string;
using OpenEngine::Core::Semaphore;
using OpenEngine::Core::Thread;

struct LogLine
{
	LoggerType type;
	string msg;
};

template<int BufferSize, typename T>
class AgentClientSystem
{
	public:
		T Data[BufferSize];
		Semaphore agent;
		unsigned int agentpos;
		Semaphore client;
		unsigned int clientpos;
	
		AgentClientSystem()
			: agent(BufferSize), client(0)
		{
		}
};

template<int BufferSize, typename T>
class WriterThread : public Thread
{
    ostream* stream;
	AgentClientSystem<BufferSize, T>* asys;
	
	public:
	WriterThread(ostream* stream, AgentClientSystem<BufferSize, T>* asys)
		: stream(stream), asys(asys)
	{
	}

	~WriterThread()
	{
		if (stream!=NULL)
		{
        	stream->flush();
        	delete stream;
    	}
	}

	void Run()
	{
		while(1)
		{
			asys->client.Wait();
   			*stream << TypeToString(asys->Data[asys->clientpos].type) << " ";
    		*stream << Utils::DateTime::GetCurrent() << ": ";
    		*stream << asys->Data[asys->clientpos].msg << std::endl;
			asys->clientpos++;
			if(asys->clientpos==BufferSize)
				asys->clientpos=0;
			asys->agent.Post();
		}
	}

	string TypeToString(LoggerType type)
	{
    	string str;
    	if (type == Error)
        	str ="[EE]";
    	else if (type == Warning)
        	str = "[WW]";
    	else if (type == Info)
        	str = "[II]";
    	else
		{
        	str = "[";
        	str += type;
        	str += "]";
    	}
    	return str;
	}
};


/**
 * Generic stream logger.
 * @todo Add some usage code to the documentation.
 *
 * @class StreamLogger StreamLogger.h Logging/StreamLogger.h
 */
template<int BufferSize=10, typename T=LogLine>
class ThreadLogger : public ILogger
{
private:
	WriterThread<BufferSize, T> Writer;
	AgentClientSystem<BufferSize, T> asys;
public:
    ThreadLogger(ostream* stream)
	: Writer(stream, &asys)
	{
		Writer.Start();
	}

	virtual ~ThreadLogger(){}

    void Write(LoggerType type, string msg)
	{
		asys.agent.Wait();
		asys.Data[asys.agentpos].type = type;
		asys.Data[asys.agentpos].msg = msg;
		asys.agentpos++;
		if(asys.agentpos==BufferSize)
			asys.agentpos=0;
		asys.client.Post();
	}
};

} //NS Logger
} //NS OpenEngine

#endif // _THREAD_LOGGER_H_
