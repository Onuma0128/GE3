#pragma once

#include <memory>
#include <string>

#include "Sprite.h"

class FadeScene
{
public:

	struct Fade {
		std::unique_ptr<Sprite> sprite_;
		float fadeFrame_;
		float alpha_;
	};


	void Init(float setAlpha);

	void Update();

	void Draw();

	void FadeIn(const std::string& newSceneName, const Vector3& color, float fadeFrame);

private:

	Fade fade_{};

};