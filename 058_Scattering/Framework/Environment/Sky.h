#pragma once

class Sky : public Renderer
{
public:
	Sky();
	~Sky();

	void Ready();
	void Update() override;
	void Render() override;
	void PostRender();

private:
	class Render2D* render2D;
	
	Texture* texture;
};