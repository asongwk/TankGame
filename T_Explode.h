#pragma once
#include "T_Config.h"
#include "T_Layer.h"
#include "T_Graph.h"

/*
爆炸类
主要是显示爆炸效果图

*/

class T_Explode
	:public T_Layer
{
public:
	T_Explode();
	~T_Explode();
	virtual void Draw(HDC hdc);

	void LoadExplodeImg(wstring path);
	void SetFrames(int f){ m_frames = f; }
	int GetFrames(){ return m_frames; }
	void IntoNextFrames(){ m_frames++; }

private:
	T_Graph *m_img;
	int m_frames;
	int m_allFrames;

};

