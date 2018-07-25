#pragma once
#include "T_Config.h"
#include "T_Graph.h"
#include "T_Layer.h"

class T_Tank
	:public T_Layer
{
public:
	T_Tank();
	~T_Tank();

public:
	virtual void Draw(HDC hdc);
	
	Rect GetCurrentTankRect();
	Rect GetNextTankRect();

	T_Graph* GetImage()const;
	void SetImage(wstring path);

	GAME_DIR GetDir()const;
	void SetDir(GAME_DIR dir);

	void SetSpeed(int sp);
	int GetSpeed()const;

	void IntoNextFrames();
	int GetCurrentFrames();

	void SetActive(bool b){ m_active = b; }
	bool GetActive()const{ return m_active; }

	DWORD GetLastTime(){ return m_lastTime; }
	void SetLastBulletTime(DWORD time){ m_lastTime = time; }
	DWORD GetGapTime(){ return m_gapTime; }
	void SetGapTime(DWORD d){ m_gapTime = d; }

	void SetLife(int l){ m_life = l; }
	int GetLife(){
		if (m_life > 0)
			return m_life;
		else 
			return 0;
	}
	void IncreaseLife(){ m_life++; }
	void DecreaseLief(){ m_life--; }
	bool IsDead(){ return m_life <= 0; }

private:
	bool m_active;  //表示是否可以移动
	T_Graph *m_img;  //表示坦克帧图

	GAME_DIR m_dir;  //方向
	int m_speed;  //速度

	int m_frames;//动画帧数组索引
	int m_allFrames;//动画帧总数

	DWORD m_lastTime;  //表示上一次发射炮弹的时刻
	DWORD m_gapTime;//表示连续两次子弹相差的最少时间间隔

	int m_life;//坦克的生命值 
private:
	static int FRAME_LEFT[4];
	static int FRAME_RIGHT[4];
	static int FRAME_UP[4];
	static int FRAME_DOWN[4];

};




