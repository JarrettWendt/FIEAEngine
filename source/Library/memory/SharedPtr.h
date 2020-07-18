#pragma once
#include "SmartPtr.h"

namespace Library
{
	/**
	 * Reference counted SmartPtr.
	 * RAII on destruction.
	 */
	template<typename T>
	class SharedPtr : public SmartPtr<T>
	{
		using Base = SmartPtr<T>;

	public:
		SharedPtr() noexcept = default;
		explicit SharedPtr(T* ptr) noexcept;
		SharedPtr(const SharedPtr& other) noexcept;
		SharedPtr(SharedPtr&& other) noexcept;
		SharedPtr& operator=(const SharedPtr& other) noexcept;
		SharedPtr& operator=(SharedPtr&& other) noexcept;
		~SharedPtr();

		/**
		 * Factory for instantiating a SharedPtr with a value.
		 * The functionality of this can't be rolled into the ctor because
		 * then there would be ambiguity for the default ctor.
		 * Do we want a SharedPtr to a default constructed T? Or a SharedPtr to nullptr?
		 *
		 * @param <Args>	types for arguments to T's ctor
		 * @param args		value for arguments to T's ctor
		 * @returns			newly constructed SharedPtr (not null)
		 */
		template<typename... Args>
		static SharedPtr Make(Args... args)
		{
			return SharedPtr(new T(std::forward<Args>(args)...));
		}
	};
}

#include "SharedPtr.inl"
