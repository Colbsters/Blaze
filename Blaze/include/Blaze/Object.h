#pragma once

#ifndef BLAZE_OBJECT_H
#define BLAZE_OBJECT_H

#include <Blaze/Core.h>

namespace Blaze
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	// Identifier for an object
	using ObjectID = uint64_t;
	using ClassID = uint32_t;
	class Object;

	namespace Details
	{
		enum class InterfaceID : uint16_t
		{
			Null = 0,
			Invalid = Null,
			Object = 0x0010,

		};

		enum class ImplementationID : uint16_t
		{
			Null = 0,
			Invalid = 0,
			Generic = 1
		};

		// Makes a class ID
		constexpr ClassID MakeClassID(InterfaceID interfaceID, ImplementationID implID) { return (static_cast<uint32_t>(interfaceID) << 16) | static_cast<uint32_t>(implID); }
		// Makes an object ID
		// NOTE: objectID doesn't get registered
		constexpr ObjectID MakeObjectID(ClassID classID, uint32_t objectID) { return (static_cast<uint64_t>(classID) << 32) | static_cast<uint64_t>(objectID); }

		// Creates and registers a new object ID
		BLAZE_API uint32_t CreateObjectID(Object* object);
		// Destroys and unregisters a new object ID
		BLAZE_API void DestroyObjectID(uint32_t objectID);
	}

	// A base structure for all blaze create info structures
	struct ObjectCreateInfo
	{
	};

	// A base class for all blaze interfaces
	class Object
		:public std::enable_shared_from_this<Object>
	{
	public:
		virtual ~Object() = default;

		// Creates the object
		Result Create(const ObjectCreateInfo& createInfo) { return Create_Impl(createInfo); }
		// Destroys the object
		Result Destroy() { return Destroy_Impl(); }

		// Gets the id of the object
		ObjectID GetObjectID() { return GetObjectID_Impl(); }
		// This function is to be implemeted by any class/interface that inherits from this class
		// Gets the id of the class
		static ClassID GetClassID() { return Details::MakeClassID(Details::InterfaceID::Object, Details::ImplementationID::Invalid); }

		// Casts this object to another object, prefer the template version
		// Returns nullptr if the cast is to an invalid type 
		Ref<Object> CastTo(ClassID objectID) { return CastTo_Impl(objectID); }

		// Casts this object to another object, throws on invalid cast
		template<typename T>
		inline Ref<T> CastTo()
		{
			auto ptr = static_cast<Ref<T>>(CastTo(T::GetClassID()));
			if (!ptr)
				throw Exception(Result::InvalidCast, "Invalid CastTo type");
			return ptr;
		}
		// Casts this object to another object, returns nullptr on invalid cast
		template<typename T>
		inline Ref<T> CastTo(std::nothrow_t) { return static_cast<Ref<T>>(CastTo(T::GetClassID())); }

	private:
		virtual Result Create_Impl(const ObjectCreateInfo& createInfo) = 0;
		virtual Result Destroy_Impl() = 0;
		virtual ObjectID GetObjectID_Impl() = 0;
		virtual Ref<Object> CastTo_Impl(ClassID objectID) = 0;
	};
}

#endif // BLAZE_OBJECT_H