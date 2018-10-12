#pragma once
#include "T_Engine.h"
#include "T_Menu.h"
#include "T_Map.h"
#include "T_Tank.h"
#include "T_Bullet.h"
#include "T_Explode.h"
#include "T_Audio.h"


class GameTest :
	public T_Engine
{
public:
	GameTest(HINSTANCE hInstance, wstring wndName =L"", int wndWidth = 1024, int wndHeight = 768);
	virtual ~GameTest();
public:
	virtual void GameCreate();
	virtual void GamePaint(HDC hdc);
	virtual void GameLogic();
	virtual void GameKeyAction(WPARAM wParam, bool isUp = false);
	virtual void GameMouseAction(int x, int y, int action);
	virtual void GameEnd();
	virtual void ResetData(); //�����������ݣ���״̬ת��ʱ�õ�

	void GamePaintSinglePeople(HDC hdc);
	void GamePaintDoublePeople(HDC hdc);
	void GamePaintHelp(HDC hdc);

	void GameLogicSinglePeople();
	void GameLogicDoublePeople();

private:
	/*--------menu---------------------------------------------*/
	void LoadGameMenu();
	void DestoryGameMenu();

	/*------map------------------------------------------------*/
	void LoadGameMap();
	void DestoryGameMap();

	/*-----------tank------------------------------------------*/
	void LoadTank();
	void DestoryTank();

	void WanderOfTank(); //̹���ڵ�ͼ������

	void ToLeft(T_Tank * tank); //̹�������ƶ�
	void ToRight(T_Tank * tank);
	void ToUp(T_Tank *tank);
	void ToDown(T_Tank *tank);

	/*�ж�̹��֮�䡢̹�����ͼ֮��֪��������ײ*/
	bool IsCollideOfTankAndMap(T_Tank* t,Rect rec);
	bool IsOverlapBetweenTwoRect(int x1, int y1, int x2, int y2,
		int width, int height, int w = 0, int h = 0);
	/*����NPC̹�˵�֡ͼ����*/
	void UpdatedNpcTankFrames();

	/*------------mode change-----------------------------------------*/
	void LoadModeChange();
	void DestroyModeChange();
	void ModeUpgrade(HDC hdc);
	void ModeWin(HDC hdc);
	void ModeFail(HDC hdc);


	/*-----------sound -------------------------------------------------*/
	void LoadGameSound(HWND hwnd);
	void DestoryGameSound();
	void ReleaseDXBuffer(AudioDXBuffer* dxBuf);
	void PlayGameSound(SOUND_INDEX index);//������Ϸ�еĸ�����������


private:
	/*-------menu-------------------------------------------------------*/
	T_Menu *m_menu;

	/*-------map-------------------------------------------------------*/
	T_Map *m_map;

	/*-----------tank----------------------------------------------------*/
	T_Tank *m_playerTank1;
	T_Tank *m_playerTank2;
	vector<T_Tank*> mv_npcTank;
	int m_countOfNpc;

	/*-----------bullet--------------------------------------------------*/
	vector<T_Bullet*> mv_playerbullet;
	vector<T_Bullet*> mv_npcbullet;
	void AddBullet(T_Tank * t);
	void UpdateBulletPosition();/*��������ײ�������ӵ�λ��*/
	void BulletHitTank();
	void BulletHitBarrier();
	void DestoryAllBullet();
	void AddBulletForNpcTank();

	/*-------------explode-------------------------------------------*/
	vector<T_Explode*> mv_explode;


	/*-------mode change -----------------------------------------------*/
	T_Graph *m_upgradeImg;
	T_Graph *m_winImg;
	T_Graph *m_failImg;

	/*-------sound -----------------------------------------------*/
	AudioDX m_dxSnd; // DirectSound����
	AudioDXBuffer *m_soundFailSound; //��Ϸ��������
	AudioDXBuffer *m_soundWinSound; //��Ϸʤ������
	AudioDXBuffer *m_soundMenuMusic; //�˵�����
	AudioDXBuffer *m_soundRunMusic; //��Ϸ��������
	AudioDXBuffer *m_soundExplosionSound; //��ը����
	AudioDXBuffer *m_soundTurnSound; //̹��ת������


private:
	int m_level; //��ʾ��Ϸ�ȼ�
	int m_check; //������Ա���

	T_Graph *m_helpImg;
};





