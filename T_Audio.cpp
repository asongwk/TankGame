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
Open()������ָ����wave��ʽ�Ĳ����ļ���
Ȼ���ļ�������浽���ݳ�Աhmmio�У�
����Ĳ����ļ������򱣴浽���ݳ�Աwfex��
*/
bool AudioWave::Open(const wchar_t *path)
{
	//��ָ��·���µ�WAVE�ļ����������ļ����
	hmmio = mmioOpenW((wchar_t*)path, NULL, MMIO_ALLOCBUF | MMIO_READ);
	if (NULL == hmmio)
		return false;

	//���ļ������ƶ���RIFF����
	if (mmioDescend(hmmio, &ckInRIFF, NULL, 0) != 0)
		return false;

	//�ж��ļ������Ƿ�Ϊwave����
	if (ckInRIFF.ckid != FOURCC_RIFF || ckInRIFF.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
		return false;

	ckIn.ckid = mmioFOURCC('f', 'm', 't', ' '); //�����ļ�����fmt����
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
		return false;

	//���fmt����С��PCMWAVEFORMAT,��ʾfmt����̫С
	if (ckIn.cksize < sizeof(PCMWAVEFORMAT))
		return false;
	wfex = (WAVEFORMATEX *)new char[ckIn.cksize];
	if (wfex == 0)
		return false;
	//��fmt�������wfex�ṹ��
	if (mmioRead(hmmio, (HPSTR)wfex, ckIn.cksize) != (LONG)ckIn.cksize)
		return false;
	//������ϣ�����fmt����
	if (mmioAscend(hmmio, &ckIn, 0) != 0)
		return false;
	return true;
}

//��ȡ����ѯ�Ƿ����data���ݿ�
bool AudioWave::StartRead()
{
	if (mmioSeek(hmmio, ckInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET) == -1)
		return false;
	ckIn.ckid = mmioFOURCC('d', 'a', 't', 'a'); //�����ļ�����data����

	//���û��data����
	if (mmioDescend(hmmio, &ckIn, &ckInRIFF, MMIO_FINDCHUNK) != 0)
		return false;
	return true;
}

//��ȡָ������
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

//���������������������û������ԣ�Ȼ����г�ʼ��
bool AudioDXBuffer::CreateBuffer(AudioDX &ds, WAVEFORMATEX *format, int size)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd)); //ʹ��0���dsbd�ڴ��
	//����dsbd����ֵ
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |
		DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_CTRLVOLUME |
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = format;
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &sndBuf, NULL); //����������
	if (FAILED(result))
		return false;

	//��ȡ����������ֵ��ʹ�û�ȡ��ֵ��ʼ����ر���
	sndBuf->GetVolume(&volume);
	sndBuf->GetPan(&pan);
	sndBuf->GetFrequency(&freq);
	return true;
}

//���������������������������趨ΪWAVE��ʽ
bool AudioDXBuffer::CreateMainBuffer(AudioDX &ds, int f)
{
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(dsbd)); //ʹ��0���dsbd�ڴ��
	//����dsbd����ֵ
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN |
		DSBCAPS_PRIMARYBUFFER; //�ñ�־��ʾҪ������������
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	//����DirectSound���󴴽���������
	HRESULT result = ds->CreateSoundBuffer(&dsbd, &sndBuf, NULL);
	if (FAILED(result))
		return false;
	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(wfex));
	wfex.wFormatTag = WAVE_FORMAT_PCM; //�趨WAVE��ʽ��������
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

//����wave�ļ��������ļ�����д�뻺����
bool AudioDXBuffer::LoadWave(AudioDX &ds, const wchar_t *path)
{
	AudioWave file;
	if (!file.Open(path) || !file.StartRead())
		return false;
	Release(); // ����Ѿ��л����������ͷ�
	//ʹ��wave�ļ���С����������
	if (!CreateBuffer(ds, file.GetFormat(), file.CkSize()))
		return false;
	//��wave�ļ�����д�뻺����
	void *ptr1, *ptr2;
	DWORD bytes1, bytes2;
	HRESULT result = sndBuf->Lock(0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);

	//�����������ʧ����ָ���Ȼ��������д������
	if (result == DSERR_BUFFERLOST)
	{
		Restore();
		result = sndBuf->Lock(0, file.CkSize(), &ptr1, &bytes1, &ptr2, &bytes2, 0);
	}
	//д��ɹ��󣬽������
	if (SUCCEEDED(result))
	{
		bool readerror = false;
		DWORD nread;
		//�ж��Ƿ���ڴ���
		if (!file.Read(bytes1, (LPBYTE)ptr1, &nread) || bytes1 != nread)
			readerror = true;
		if (bytes2)
		{
			if (!file.Read(bytes2, (LPBYTE)ptr2, &nread) || bytes2 != nread)
				readerror = true;
		}
		//�Ӵ�����
		result = sndBuf->Unlock(ptr1, bytes1, ptr2, bytes2);
		if (readerror)
			return false;
		else
			return true;
	}
	return false;
}

//�ͷ�������Դ
HRESULT AudioDXBuffer::Release()
{
	HRESULT result = DS_OK;
	if (sndBuf)
		result = sndBuf->Release();
	sndBuf = NULL;
	return result;
}

//�ָ�������
HRESULT AudioDXBuffer::Restore()
{
	HRESULT result = DS_OK;
	if (sndBuf)
		return sndBuf->Restore();
	return DS_OK;
}

//��ֹ�����ָ���������ֹͣ�������ţ�
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

//����DirectSound�������ú������𲢴�����������
bool AudioDX::CreateDS(HWND hWnd, int freq)
{
	//����DirectSound����
	HRESULT result = DirectSoundCreate(NULL, &ds, NULL);
	if (FAILED(result))
		return false;

	//����DirectSound�����������ȼ���
	result = ds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))
		return false;
	//����DirectSound������
	return dsbuf.CreateMainBuffer(*this, freq);
}

//�ͷ�DirectSound����DirectSound����������
void AudioDX::ReleaseAll()
{
	if (ds)
	{
		dsbuf.Release();
		ds->Release();
		ds = 0;
	}
}

//ʧȥ�������ڴ�ʱ��ԭ
HRESULT AudioDX::RestoreAll()
{
	HRESULT result = dsbuf.Restore();
	if (result != DS_OK)
		return result;
	return DS_OK;
}


