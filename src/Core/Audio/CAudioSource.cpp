#include "CAudioSource.h"

using namespace Pro;
using namespace Audio;

CAudioSource::CAudioSource(ALuint _id)
{
	id = _id;
	alSourcei(id, AL_REFERENCE_DISTANCE, 1);
	alSourcei(id, AL_MAX_DISTANCE, 100);
}

CAudioSource::~CAudioSource()
{
}