#pragma once
#include "T_Config.h"
#include "T_Graph.h"
#include "T_Layer.h"

class T_Bullet
	:public T_Layer
{
public:
	T_Bullet();
	~T_Bullet();

	virtual void Draw(HDC hdc);

	/*¼ÓÔØ×Óµ¯µÄÍ¼Æ¬*/
	void LoadBulletImage(wstring path);
	Rect GetNextRect();
	Rect GetCurrentRect();
	void SetDir(GAME_DIR dir){ m_dir = dir; }
	GAME_DIR GetDir(){ return m_dir; }

	void SetSpeed(int sp){ m_speed = sp; }
	int GetSpeed(){ return m_speed; }

private:
	T_Graph *m_img;
	int m_speed;
	GAME_DIR m_dir;

};

