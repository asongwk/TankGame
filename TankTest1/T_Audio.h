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

	bool Open(const wchar_t *path); //打开并读取WAVE文件
	bool StartRead(); //读取并查询是否存在data区块

	//读取指定数据
	bool Read(unsigned long size, void *data, unsigned long *nread);
	bool Close(); //释放占用资源

	WAVEFORMATEX *GetFormat(){ return wfex; } //获取wave格式属性
	DWORD CkSize()const{ return ckIn.cksize; }//获取区块大小

protected:
	WAVEFORMATEX *wfex; //wave格式结构体指针
	HMMIO hmmio; // 文件句柄

	MMCKINFO ckIn; // MMCKINFO对象（查询用子块）
	MMCKINFO ckInRIFF; // MMCKINFO对象（父块，从文件获取）
};

//----------------------AudioDXBuffer class------------------------------------------
class AudioDXBuffer
{
public:
	AudioDXBuffer();
	~AudioDXBuffer();

	//创建辅助缓冲区，并设置基本属性
	bool CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size);

	//创建主缓冲区，并将主缓冲区设定为WAVE格式
	bool CreateMainBuffer(AudioDX &ds, int freq = 22050);

	//读入wave文件，并将文件数据写入缓冲区
	bool LoadWave(AudioDX &ds, const wchar_t *path);

	//释放所有资源
	HRESULT Release();

	//恢复缓冲区
	HRESULT Restore();

	//终止处理（恢复缓冲区并停止声音播放）
	void Terminate();

	//播放声音（参数决定是否循环）
	bool Play(bool loop = false)
	{
		if (sndBuf == NULL)
			return false;
		if (FAILED(sndBuf->Play(0, 0, loop ? DSBPLAY_LOOPING : 0)))
			return false;
		return true;
	}

	//停止声音播放
	bool Stop()
	{
		return sndBuf->Stop() == DS_OK;
	}

	//判断缓冲区是否处于活跃状态
	bool IsAlive()const
	{
		return sndBuf != 0;
	}

	//重载"->"操作符，表示sndBuf对象
	LPDIRECTSOUNDBUFFER operator->(){ return sndBuf; }
protected:
	LPDIRECTSOUNDBUFFER sndBuf; //缓冲区接口指针
	LONG volume; //音量
	LONG pan; //左右声道相对音量
	DWORD freq;//频率

};

//---------------AudioDX class-------------------
class AudioDX
{
public:
	AudioDX();
	~AudioDX();
	//创建DirectSound对象，设置合作级别并创建主缓冲区
	bool CreateDS(HWND hWnd, int freq = 22050);

	//释放DirectSound对象及DirectSound缓冲区对象
	void ReleaseAll();

	//失去缓冲区内存时则复原
	HRESULT RestoreAll();

	//重载"->"操作符，表示ds对象
	LPDIRECTSOUND operator->(){ return ds; }
protected:
	LPDIRECTSOUND ds; // DirectSound对象 lp direct sound
	AudioDXBuffer dsbuf; //DirectSound缓冲对象
};
