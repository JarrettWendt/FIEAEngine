// MIT License Copyright (c) 2020 Jarrett Wendt

#include "pch.h"
#include "Math.h"

namespace Library::Math
{
	// Function local to this cpp file, doesn't really need to even be private to the Util class.
	void PopulateSieve(volatile char* sieve, const size_t start, const size_t end)
	{
		// Don't need to loop through the entire sieve,
		// because the largest composite with only prime factors will be within the first square root.
		for (size_t i = start, n = size_t(std::floor(std::sqrt(end))); i <= n; i += 2)
		{
#if defined(_WIN32)
			if (!_InterlockedCompareExchange8(sieve + i, -1, 0))
#else
			// If this index hasn't been worked yet
			if (!__sync_val_compare_and_swap(sieve + i, 0, -1))
#endif
			{
				// Mark all of its multiples
				for (size_t j = i * i; j < end; j += i)
				{
					sieve[j] = 1;
				}
			}
		}
	}
	
	bool IsPrime(const size_t n)
	{
		// This could have been implemented with an externally generated primes table,
		// however generating one manually with a multi-threaded Sieve of Eratosthenes is more fun.

		// This could have been a Vector but I didn't want to include Vector.h here.
		// 0 (default) means it is a prime who's multiples have not yet been marked off
		// -1 means it is a prime who's multiples have been or are being marked off
		// 1 means composite (except for 0 and 1 who are neither).
		// Even numbered indices are skipped.
		static uint8_t* sieve{ nullptr };
		static size_t sieveSize{ 0 };

		if (n >= sieveSize)
		{
			// Due to how the algorithm works, we have to recalculate the entire sieve every time.
			sieveSize = std::max(n, sieveSize / 2 + 1);
			delete[] sieve;
			sieve = new uint8_t[sieveSize]();

			// TODO: 16 is a placeholder best guess.
			// Do some runtime analysis of what the best thread count is.
			std::thread threads[16];
			for (std::thread& thread : threads)
			{
				thread = std::thread(PopulateSieve, reinterpret_cast<volatile char*>(const_cast<volatile uint8_t*>(sieve)), size_t(3), sieveSize);
			}
			for (std::thread& thread : threads)
			{
				thread.join();
			}
		}

		// 0 and 1 are not prime.
		// 2 is the only even prime.
		// No even numbers are tallied in the sieve.
		return n == 2 || (n > 1 && n % 2 && sieve[n] != 1);
	}

	size_t NextPrime(size_t num)
	{
		while (!IsPrime(++num));
		return num;
	}
}
