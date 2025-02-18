#pragma once

class Life
{
public:
	Life();
	~Life();

	void Init();
	void Update();
	void Draw();

	void SetHandle(int handle) { m_handle = handle; };
	void SetIndex(int index) { m_index = index; };

private:
	// HP‚Ìƒnƒ“ƒhƒ‹
	int m_handle;
	int m_index;
};

