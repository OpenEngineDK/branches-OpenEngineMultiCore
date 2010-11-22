// Basic Event.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _OE_EVENT_H_
#define _OE_EVENT_H_

#include <Core/IEvent.h>
#include <Core/IListener.h>
#include <Core/Mutex.h>
#include <list>

namespace OpenEngine {
namespace Core {

using std::list;

/**
 * Basic Event.
 * This is a basic implementation of a event list. It maintains a list
 * of attached listeners and invokes their handlers immediately on
 * event notification.
 * 
 * @class Event Event.h Core/Event.h
 * @tparam EventArg Argument type of the event.
 * @see Listener
 */
template <typename EventArg>
class Event : public IEvent<EventArg> {
protected:
    //! list of listeners
    list<IListener<EventArg>*> ls;
	Mutex container;

public:

    virtual void Attach(IListener<EventArg>& listener)
	{
		container.Lock();
        ls.push_back(&listener);
		container.Unlock();
    }

    virtual void Detach(IListener<EventArg>& listener)
	{
		container.Lock();
        ls.remove(&listener);
		container.Unlock();
    }
    
    /**
     * Notify all listeners.
     * This will immediately invoke the Handle(EventArg) method on each
     * attached listener.
     */
    virtual void Notify(EventArg arg)
	{
		container.Lock();
        typename list<IListener<EventArg>*>::iterator itr;
        for (itr=ls.begin(); itr != ls.end(); itr++)
            (*itr)->Handle(arg);
		container.Unlock();
    }

    unsigned int Size()
	{
		container.Lock();
        return ls.size();
		container.Unlock();
	}

};

} // NS Core
} // NS OpenEngine

#endif // _OE_EVENT_H_
