#include "UtilityFunctions.h"

#include <cassert>
#include <cstdlib>

namespace aie
{
	float RandomFloat(float min, float max)
	{
		assert(max > min);
		float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

		float range = max - min;
		return random * range + min;
	}

}
