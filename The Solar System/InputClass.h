#ifndef INPUTCLASS_H
#define INPUTCLASS_H

// INCLUDES //
#include <Windows.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();
	void Update(MSG &msg);
	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	bool IsKeyDown(unsigned int key);
	MSG& GetCurrentMessage();

private:
	bool m_Keys[256];
	MSG m_Message;
};

#endif

