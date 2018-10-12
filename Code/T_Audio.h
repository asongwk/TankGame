#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

class AudioDX;
//--------------------AudioWave class----------------------------------
class AudioWave
{
public:
	AudioWave();
	virtual ~AudioWave();

	bool Open(const wchar_t *path); //�򿪲���ȡWAVE�ļ�
	bool StartRead(); //��ȡ����ѯ�Ƿ����data����

	//��ȡָ������
	bool Read(unsigned long size, void *data, unsigned long *nread);
	bool Close(); //�ͷ�ռ����Դ

	WAVEFORMATEX *GetFormat(){ return wfex; } //��ȡwave��ʽ����
	DWORD CkSize()const{ return ckIn.cksize; }//��ȡ�����С

protected:
	WAVEFORMATEX *wfex; //wave��ʽ�ṹ��ָ��
	HMMIO hmmio; // �ļ����

	MMCKINFO ckIn; // MMCKINFO���󣨲�ѯ���ӿ飩
	MMCKINFO ckInRIFF; // MMCKINFO���󣨸��飬���ļ���ȡ��
};

//----------------------AudioDXBuffer class------------------------------------------
class AudioDXBuffer
{
public:
	AudioDXBuffer();
	~AudioDXBuffer();

	//���������������������û�������
	bool CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size);

	//���������������������������趨ΪWAVE��ʽ
	bool CreateMainBuffer(AudioDX &ds, int freq = 22050);

	//����wave�ļ��������ļ�����д�뻺����
	bool LoadWave(AudioDX &ds, const wchar_t *path);

	//�ͷ�������Դ
	HRESULT Release();

	//�ָ�������
	HRESULT Restore();

	//��ֹ�����ָ���������ֹͣ�������ţ�
	void Terminate();

	//�������������������Ƿ�ѭ����
	bool Play(bool loop = false)
	{
		if (sndBuf == NULL)
			return false;
		if (FAILED(sndBuf->Play(0, 0, loop ? DSBPLAY_LOOPING : 0)))
			return false;
		return true;
	}

	//ֹͣ��������
	bool Stop()
	{
		return sndBuf->Stop() == DS_OK;
	}

	//�жϻ������Ƿ��ڻ�Ծ״̬
	bool IsAlive()const
	{
		return sndBuf != 0;
	}

	//����"->"����������ʾsndBuf����
	LPDIRECTSOUNDBUFFER operator->(){ return sndBuf; }
protected:
	LPDIRECTSOUNDBUFFER sndBuf; //�������ӿ�ָ��
	LONG volume; //����
	LONG pan; //���������������
	DWORD freq;//Ƶ��

};

//---------------AudioDX class-------------------
class AudioDX
{
public:
	AudioDX();
	~AudioDX();
	//����DirectSound�������ú������𲢴�����������
	bool CreateDS(HWND hWnd, int freq = 22050);

	//�ͷ�DirectSound����DirectSound����������
	void ReleaseAll();

	//ʧȥ�������ڴ�ʱ��ԭ
	HRESULT RestoreAll();

	//����"->"����������ʾds����
	LPDIRECTSOUND operator->(){ return ds; }
protected:
	LPDIRECTSOUND ds; // DirectSound���� lp direct sound
	AudioDXBuffer dsbuf; //DirectSound�������
};
