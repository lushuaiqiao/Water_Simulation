//***************************************************************************************
//LU SHUAI QIAO 2020
//Frame reference by Introduction to 3D Game Programming with Directx 12
//***************************************************************************************
#ifndef GAMETIMER_H
#define GAMETIMER_H

class GameTimer
{
public:
	GameTimer();

	float TotalTime()const;//��ǰʱ��
	float DeltaTime()const;//ÿ֡���ʱ��
	//��ʱ������
	void Reset();
	void Start(); 
	void Stop(); 
	void Tick(); 

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};

#endif // GAMETIMER_H