#include <pch.h>
#include <Blaze/Object.h>

namespace Blaze
{
	namespace Details
	{
#ifdef BLAZE_DEBUG

		// Increments everytime a new object is created, used to assign unique object IDs
		static uint32_t objectIDCounter = 0;
		// Map of all currently registered IDs
		static std::map<uint32_t, Object*> objectIDMap;
		// Mutex lock for the counter and map
		static std::mutex objectIDMutex;

		uint32_t CreateObjectID(Object* object)
		{
			std::lock_guard<std::mutex> guard{ objectIDMutex };
			objectIDCounter++;
			objectIDMap[objectIDCounter] = object;
			return objectIDCounter;
		}

		void DestroyObjectID(uint32_t objectID)
		{
			std::lock_guard<std::mutex> guard{ objectIDMutex };
			objectIDMap.erase(objectID);
		}

#else // ^^^ BLAZE_DEBUG / !BLAZE_DEBUG vvv

		// Increments everytime a new object is created, used to assign unique object IDs
		static uint32_t objectIDCounter = 0;
		static std::mutex objectIDCounterMutex;

		uint32_t CreateObjectID(Object* object)
		{
			std::lock_guard<std::mutex> guard{ objectIDCounterMutex };
			objectIDCounter++;
			return objectIDCounter;
		}

		void DestroyObjectID(uint32_t objectID)
		{
		}
#endif // ^^^ !BLAZE_DEBUG
	}
}
