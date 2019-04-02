#include "framework.h"
#include "Keyboard.h"

Keyboard* Keyboard::instance = NULL;

Keyboard * Keyboard::Get()
{
	assert(instance != NULL);

	return instance;
}

void Keyboard::Create()
{
	assert(instance == NULL);

	instance = new Keyboard();
}

void Keyboard::Delete()
{
	SAFE_DELETE(instance);
}

void Keyboard::Update()
{
	memcpy(keyOldState, keyState, sizeof(keyOldState));

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));


	GetKeyboardState(keyState);

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		int oldState = keyOldState[i];
		int state = keyState[i];

		if (oldState == 0 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_DOWN; //이전 0, 현재 1 - KeyDown
		else if (oldState == 1 && state == 0)
			keyMap[i] = KEY_INPUT_STATUS_UP; //이전 1, 현재 0 - KeyUp
		else if (oldState == 1 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_PRESS; //이전 1, 현재 1 - KeyPress
		else
			keyMap[i] = KEY_INPUT_STATUS_NONE;
	}
}

float Keyboard::Move(DWORD positive, DWORD negative)
{
	if (Press(positive) == true)
		return +1.0f;

	if (Press(negative) == true)
		return -1.0f;

	return 0.0f;
}

void Keyboard::MovePosition(IN OUT D3DXVECTOR3 * position, float speed)
{
	MovePosition(position, speed, VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT);
}

void Keyboard::MovePosition(IN OUT D3DXVECTOR3 * position, float speed, DWORD upKey, DWORD downKey, DWORD rightKey, DWORD leftKey)
{
	float forward = Keyboard::Get()->Move(upKey, downKey);
	position->z += forward * speed * Time::Delta();

	float right = Keyboard::Get()->Move(rightKey, leftKey);
	position->x += right * 10 * Time::Delta();
}


Keyboard::Keyboard()
{
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}

Keyboard::~Keyboard()
{

}
