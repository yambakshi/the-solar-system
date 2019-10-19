#include "InputClass.h"


InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	for (int i = 0; i < 256; i++)
		m_Keys[i] = false;


	return;
}

void InputClass::Update(MSG& msg)
{
	if (msg.message == WM_KEYDOWN)
		m_Keys[(unsigned int)msg.wParam] = true;
	else if (msg.message == WM_KEYUP)
		m_Keys[(unsigned int)msg.wParam] = false;

	m_Message = msg;


	return;
}

void InputClass::KeyDown(unsigned int key)
{
	m_Keys[key] = true;


	return;
}

void InputClass::KeyUp(unsigned int key)
{
	m_Keys[key] = false;


	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_Keys[key];
}

MSG& InputClass::GetCurrentMessage()
{
	return m_Message;
}