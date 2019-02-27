#pragma once
#include "ModelInstance.h"

class ModelAnimInstance : public ModelInstance
{
public:
	ModelAnimInstance(Model* model, ModelClip* clip, wstring shaderFile);
	~ModelAnimInstance();

	void Render() override;

protected:
	void MappedData() override;

private:	
	class ModelClip* clip;

	struct FrameDesc
	{
		UINT Curr;
		UINT Next;
		float Time;
		float FrameTime;
	};
	FrameDesc frames[MAX_INST_MODEL];
};