#pragma once
#include <string>
#include <utility>
#include <vector>
#include <hash_map>
#include <boost/serialization/serialization.hpp>
//#include "Game.h"

namespace TDEngine
{

	enum TYPE;
	struct Event
	{
		std::string name;
		std::vector<std::pair<TYPE,void*>> Parameters;
	};

	class EventReceiver
	{
		friend class boost::serialization::access;
	public:
		virtual ~EventReceiver();
		virtual void ProceedEvent(Event&) = 0;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{}
	};

	class EventManager
	{
	public:
		EventManager();
		//Subcribe receiver to event with name identificator
		void Subscribe(EventReceiver*, std::string);
		//Unsubscribe from all events
		void Unsubscribe(EventReceiver*);
		//Unsubscribe from event
		void Unsubscribe(EventReceiver*, std::string);
		//Sends an event to receiver
		void SendEvent(EventReceiver*, Event&);
		//Sends an event to all event subscribers
		void SendEvent(Event&);
		//Send an event to all subscribed objects
		void SendGlobalEvent(Event&);
		bool IsObjectSubscribed(EventReceiver*, std::string);

		//All events have to be registered before usage
		void RegisterEvent(std::string, std::vector<TYPE>&);
		
		Event GetEventPattern(std::string);
	private:
		//Subscribers list. String is Event name.
		std::hash_map<std::string, std::vector<EventReceiver*>> Subscribers;
		//Event patterns
		std::vector<Event> Events;
		bool DoesEventExist(std::string);
	};
}