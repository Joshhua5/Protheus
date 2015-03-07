#include <Graphics/SpriteBatcher.h>
#include <Graphics/AnimatedSprite.h>

using namespace Pro;
using namespace Graphics;

class AnimatedSpriteBatcher : public SpriteBatcher{
	std::unordered_map<int, pair<unsigned, smart_ptr<AnimatedSprite>>> sprites_;

public:

	AnimatedSpriteBatcher(const Vector2<float>& window_dimensions = { 100, 100 }) : SpriteBatcher(window_dimensions){}

	// Returns an animation ID
	int AttachAnimation(smart_ptr<AnimatedSprite> animation){
		int id = AttachTexture(animation->GetFrame(0));
		sprites_.insert({ id, { 0, animation } });
		return id;
	}

	void IncrementAnimation(const unsigned animation_id){
		auto& spt = sprites_.at(animation_id);
		// first is the frame id
		// second is the animated sprite
		if (spt.first != spt.second->getFrameCount() - 1)
			spt.first++;
		else
			spt.first = 0;
		SwapTexture(animation_id, spt.second->GetFrame(spt.first));
	}
};
