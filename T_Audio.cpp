#include "T_Audio.h"

//------------AudioWave class--------------------------------
AudioWave::AudioWave()
:hmmio(0), wfex(0)
{
}


AudioWave::~AudioWave()
{
	Close();
}

/*
Open()函数打开指定的wave格式的波形文件，
然后将文件句柄保存到数据成员hmmio中，
具体的波形文件数据则保存到数据成员wfex中
*/
bool AudioWave::Open(const wchar_t *path)
{
	//打开指定路径下的WAVE文件，并返回文件句柄
	hmmio = mmioOpenW((wchar_t*)path, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (NULL == hmmio)
		return false;

	//将文件数据移动到RIFF区块
	if (mmioDescend(hmmio, &ckInRIFF, NULL, 0) != 0)
		return false;

	//判断文件类型是否为wave类型
	if (ckInRIFF.ckid != FOURCC_RIFF || ckInRIFF.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
		return false;

	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' '); //搜索文件数据fmt区块
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
		return false;

	//如果fmt区块小于PCMWAVEFORMAT,表示fmt区块太小
	if (ckIn.cksize < sizeof(PCMWAVEFORMAT))
		return false;
	wfex = (WAVEFORMATEX *)new char[ckIn.cksize];
	if (wfex == 0)
		return false;
	//将fmt区块读入wfex结构体
	if (mmioRead(hmmio, (HPSTR)wfex, ckIn.cksize) != (LONG)ckIn.cksize)
		return false;
	//操作完毕，跳出fmt区块
	if (mmioAscend(hmmio, &ckIn, 0) != 0)
		return false;
	return true;
}

//读取并查询是否存在data数据块
bool AudioWave::StartRead()
{
	if (mmioSeek(hmmio, ckInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)
		return false;
	ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a'); //搜索文件数据data区块

	//如果没有data区块
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
		return false;
	return true;
}

//读取指定数据
bool AudioWave::Read(unsigned long size, void *data, unsigned long *nread)
{
	*nread = 0;
	int result = mmioRead(hmmio, (HPSTR)data, size);
	if (result == -1)
		return false;
	*nread = (unsigned)result;
	return true;
}

bool AudioWave::Close()
{
	delete[](char*)wfex;
	wfex = 0;
	if (hmmio != NULL)
		return mmioClose(hmmio, 0) == 0;
	return true;
}


//------------AudioDXBuffer class --------------------------------------------------
AudioDXBuffer::AudioDXBuffer()
:sndBuf(NULL), volume(0), pan(0), freq(0)
{

}

AudioDXBuffer::~AudioDXBuffer()
{
	if (sndBuf)
		sndBuf->Release();
}

//创建辅助缓冲区，并设置基本属性，然后进行初始化
bool AudioDXBuffer::CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd)); //使用0填充dsbd内存块
	//设置dsbd属性值
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
		DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = format;
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &sndBuf, NULL); //创建缓冲区
	if (FAILED(result))
		return false;

	//获取缓冲区属性值并使用获取的值初始化相关变量
	sndBuf->GetVolume(&volume);
	sndBuf->GetPan(&pan);
	sndBuf->GetFrequency(&freq);
	return true;
}

//创建主缓冲区，并将主缓冲区设定为WAVE格式
bool AudioDXBuffer::CreateMainBuffer(AudioDX &ds, int f)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd)); //使用0填充dsbd内存块
	//设置dsbd属性值
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN |
		DSBCAPS_PRIMARYBUFFER; //该标志表示要创建主缓冲区
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	//调用DirectSound对象创建主缓冲区
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &sndBuf, NULL);
	if (FAILED(result))
		return false;
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(wfex));
	wfex.wFormatTag = WAVE_FORMAT_PCM; //设定WAVE格式对象属性
	wfex.nChannels = 2;
	wfex.nSamplesPerSec = f;
	wfex.nBlockAlign = 4;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;
	wfex.wBitsPerSample = 16;
	result = sndBuf->SetFormat(&wfex);

	if (FAILED(result))
		return false;
	sndBuf->GetVolume(&volume);
	sndBuf->GetPan(&pan);
	sndBuf->GetFrequency(&freq);
	return true;
}

//读入wave文件，并将文件数据写入缓冲区
bool AudioDXBuffer::LoadWave(AudioDX &ds, const wchar_t *path)
{
	AudioWave file;
	if (!file.Open(path) || !file.StartRead())
		return false;
	Release(); // 如果已经有缓冲区，就释放
	//使用wave文件大小建立缓冲区
	if (!CreateBuffer(ds, file.GetFormat(), file.CkSize()))
		return false;
	//将wave文件数据写入缓冲区
	void *ptr1, *ptr2;
	DWORD bytes1, bytes2;
	HRESULT result = sndBuf->Lock(0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);

	//如果缓冲区丢失，则恢复，然后锁定并写入数据
	if (result == DSERR_BUFFERLOST)
	{
		Restore();
		result = sndBuf->Lock(0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);
	}
	//写入成功后，解除锁定
	if (SUCCEEDED(result))
	{
		bool readerror = false;
		DWORD nread;
		//判断是否存在错误
		if (!file.Read(bytes1, (LPBYTE)ptr1, &nread) || bytes1 != nread)
			readerror = true;
		if (bytes2)
		{
			if (!file.Read(bytes2, (LPBYTE)ptr2, &nread) || bytes2 != nread)
				readerror = true;
		}
		//接触锁定
		result = sndBuf->Unlock(ptr1, bytes1, ptr2, bytes2);
		if (readerror)
			return false;
		else
			return true;
	}
	return false;
}

//释放所有资源
HRESULT AudioDXBuffer::Release()
{
	HRESULT result = DS_OK;
	if (sndBuf)
		result = sndBuf->Release();
	sndBuf = NULL;
	return result;
}

//恢复缓冲区
HRESULT AudioDXBuffer::Restore()
{
	HRESULT result = DS_OK;
	if (sndBuf)
		return sndBuf->Restore();
	return DS_OK;
}

//终止处理（恢复缓冲区并停止声音播放）
void AudioDXBuffer::Terminate()
{
	if (sndBuf)
	{
		if (FAILED(sndBuf->Restore()))
			return;
		if (FAILED(sndBuf->Stop()))
			return;
	}
}


//-------------------Audio class ----------------------------------------
AudioDX::AudioDX()
{
	ds = 0;
}

AudioDX::~AudioDX()
{
	ReleaseAll();
}

//创建DirectSound对象，设置合作级别并创建主缓冲区
bool AudioDX::CreateDS(HWND hWnd, int freq)
{
	//创建DirectSound对象
	HRESULT result = DirectSoundCreate(NULL, &ds, NULL);
	if (FAILED(result))
		return false;

	//设置DirectSound合作级别（优先级）
	result = ds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;
	//创建DirectSound缓冲区
	return dsbuf.CreateMainBuffer(*this, freq);
}

//释放DirectSound对象及DirectSound缓冲区对象
void AudioDX::ReleaseAll()
{
	if (ds)
	{
		dsbuf.Release();
		ds->Release();
		ds = 0;
	}
}

//失去缓冲区内存时则复原
HRESULT AudioDX::RestoreAll()
{
	HRESULT result = dsbuf.Restore();
	if (result != DS_OK)
		return result;
	return DS_OK;
}


