#pragma once
#include "GameObject.h"


#define MARIO_WALKING_SPEED		0.2f 
#define MARIO_ACCELERATION		0.008f
#define MARIO_RUNNING_SPEED		0.056f 
#define MARIO_SPEED_DOWN		0.01f 

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_FLYING_RIGHT	500
#define MARIO_STATE_FLYING_LEFT		600
#define MARIO_STATE_BRAKING_RIGHT	700	
#define MARIO_STATE_BRAKING_LEFT	800
#define MARIO_STATE_HOLDING_RIGHT	900
#define MARIO_STATE_HOLDING_LEFT	1000
#define MARIO_STATE_HOLDING			1100
#define MARIO_STATE_KICK			1200
#define MARIO_STATE_RUNNING_RIGHT	1300
#define MARIO_STATE_RUNNING_LEFT	1400
#define MARIO_STATE_SITDOWN			1500
#define MARIO_STATE_TURNING_TAIL	1600
#define MARIO_STATE_SPEED_DOWN		1700
#define MARIO_STATE_FALLING_DOWN	1800
#define MARIO_STATE_JUMP_HIGH		1900
#define MARIO_STATE_HITTED			2000
#define MARIO_STATE_LOOK_UP			2100
#define MARIO_STATE_PIPE_DOWNING	2200
#define MARIO_STATE_PIPE_UPPING		2300


#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_BIG_WALKING_RIGHT		2	
#define MARIO_ANI_BIG_WALKING_LEFT		3
#define MARIO_ANI_BIG_JUMPING_RIGHT		4
#define MARIO_ANI_BIG_JUMPING_LEFT		5
#define MARIO_ANI_BIG_FLYING_RIGHT		6	
#define MARIO_ANI_BIG_FLYING_LEFT		7
#define MARIO_ANI_BIG_BRAKING_RIGHT		8	
#define MARIO_ANI_BIG_BRAKING_LEFT		9
#define MARIO_ANI_BIG_HOLDING_RIGHT		10	
#define MARIO_ANI_BIG_HOLDING_LEFT		11
#define MARIO_ANI_BIG_KICKING_RIGHT		12	
#define MARIO_ANI_BIG_KICKING_LEFT		13
#define MARIO_ANI_BIG_RUNNING_RIGHT		14	
#define MARIO_ANI_BIG_RUNNING_LEFT		15
#define MARIO_ANI_BIG_SITDOWN_RIGHT		16	
#define MARIO_ANI_BIG_SITDOWN_LEFT		17
#define MARIO_ANI_BIG_JUMP_FALL_RIGHT	18	
#define MARIO_ANI_BIG_JUMP_FALL_LEFT	19

#define MARIO_ANI_SMALL_IDLE_RIGHT			20
#define MARIO_ANI_SMALL_IDLE_LEFT			21
#define MARIO_ANI_SMALL_WALKING_RIGHT		22	
#define MARIO_ANI_SMALL_WALKING_LEFT		23
#define MARIO_ANI_SMALL_JUMPING_RIGHT		24
#define MARIO_ANI_SMALL_JUMPING_LEFT		25
#define MARIO_ANI_SMALL_FLYING_RIGHT		26	
#define MARIO_ANI_SMALL_FLYING_LEFT			27
#define MARIO_ANI_SMALL_BRAKING_RIGHT		28	
#define MARIO_ANI_SMALL_BRAKING_LEFT		29
#define MARIO_ANI_SMALL_HOLDING_RIGHT		30	
#define MARIO_ANI_SMALL_HOLDING_LEFT		31
#define MARIO_ANI_SMALL_KICKING_RIGHT		32	
#define MARIO_ANI_SMALL_KICKING_LEFT		33
#define MARIO_ANI_SMALL_RUNNING_RIGHT		34	
#define MARIO_ANI_SMALL_RUNNING_LEFT		35



#define MARIO_ANI_TAIL_IDLE_RIGHT		40
#define MARIO_ANI_TAIL_IDLE_LEFT		41
#define MARIO_ANI_TAIL_WALKING_RIGHT	42	
#define MARIO_ANI_TAIL_WALKING_LEFT		43        
#define MARIO_ANI_TAIL_JUMPING_RIGHT	44
#define MARIO_ANI_TAIL_JUMPING_LEFT		45
#define MARIO_ANI_TAIL_FLYING_RIGHT		46	
#define MARIO_ANI_TAIL_FLYING_LEFT		47
#define MARIO_ANI_TAIL_BRAKING_RIGHT	48	
#define MARIO_ANI_TAIL_BRAKING_LEFT		49
#define MARIO_ANI_TAIL_HOLDING_RIGHT	50	
#define MARIO_ANI_TAIL_HOLDING_LEFT		51
#define MARIO_ANI_TAIL_KICKING_RIGHT	52	
#define MARIO_ANI_TAIL_KICKING_LEFT		53
#define MARIO_ANI_TAIL_RUNNING_RIGHT	54	
#define MARIO_ANI_TAIL_RUNNING_LEFT		55
#define MARIO_ANI_TAIL_SITDOWN_RIGHT	56
#define MARIO_ANI_TAIL_SITDOWN_LEFT		57
#define MARIO_ANI_TAIL_TURNING_RIGHT	58	
#define MARIO_ANI_TAIL_TURNING_LEFT		59
#define MARIO_ANI_TAIL_JUMP_FALL_RIGHT	60	
#define MARIO_ANI_TAIL_JUMP_FALL_LEFT	61

#define MARIO_ANI_FIRE_IDLE_RIGHT		62
#define MARIO_ANI_FIRE_IDLE_LEFT		63
#define MARIO_ANI_FIRE_WALKING_RIGHT	64	
#define MARIO_ANI_FIRE_WALKING_LEFT		65
#define MARIO_ANI_FIRE_JUMPING_RIGHT	66
#define MARIO_ANI_FIRE_JUMPING_LEFT		67
#define MARIO_ANI_FIRE_FLYING_RIGHT		68	
#define MARIO_ANI_FIRE_FLYING_LEFT		69
#define MARIO_ANI_FIRE_BRAKING_RIGHT	70	
#define MARIO_ANI_FIRE_BRAKING_LEFT		71
#define MARIO_ANI_FIRE_HOLDING_RIGHT	72
#define MARIO_ANI_FIRE_HOLDING_LEFT		73
#define MARIO_ANI_FIRE_KICKING_RIGHT	74	
#define MARIO_ANI_FIRE_KICKING_LEFT		75
#define MARIO_ANI_FIRE_RUNNING_RIGHT	76	
#define MARIO_ANI_FIRE_RUNNING_LEFT		77
#define MARIO_ANI_FIRE_SITDOWN_RIGHT	78
#define MARIO_ANI_FIRE_SITDOWN_LEFT		79
#define MARIO_ANI_FIRE_SHOOTING_RIGHT	80	
#define MARIO_ANI_FIRE_SHOOTING_LEFT	81
#define MARIO_ANI_BIG_MAX_SPEED_RIGHT	82
#define MARIO_ANI_BIG_MAX_SPEED_LEFT	83
#define MARIO_ANI_FIRE_MAX_SPEED_RIGHT	84
#define MARIO_ANI_FIRE_MAX_SPEED_LEFT	85
#define MARIO_ANI_SMALL_MAX_SPEED_RIGHT	86
#define MARIO_ANI_SMALL_MAX_SPEED_LEFT	87
#define MARIO_ANI_TAIL_MAX_SPEED_RIGHT	88
#define MARIO_ANI_TAIL_MAX_SPEED_LEFT	89
#define	MARIO_ANI_FALLING_LEFT			90
#define MARIO_ANI_FALLING_RIGHT			91
#define	MARIO_ANI_FIRE_JUMP_FALL_RIGHT	92
#define MARIO_ANI_FIRE_JUMP_FALL_LEFT	93

#define MARIO_ANI_BIG_IDLE_HOLDING_RIGHT		94	
#define MARIO_ANI_BIG_IDLE_HOLDING_LEFT			95
#define MARIO_ANI_SMALL_IDLE_HOLDING_RIGHT		96	
#define MARIO_ANI_SMALL_IDLE_HOLDING_LEFT		97
#define MARIO_ANI_TAIL_IDLE_HOLDING_RIGHT		98	
#define MARIO_ANI_TAIL_IDLE_HOLDING_LEFT		99
#define MARIO_ANI_FIRE_IDLE_HOLDING_RIGHT		100	
#define MARIO_ANI_FIRE_IDLE_HOLDING_LEFT		101


#define MARIO_GREEN_ANI_BIG_IDLE_RIGHT			102
#define MARIO_GREEN_ANI_BIG_IDLE_LEFT			103
#define MARIO_GREEN_ANI_BIG_WALKING_RIGHT		104
#define MARIO_GREEN_ANI_BIG_WALKING_LEFT		105
#define MARIO_GREEN_ANI_BIG_JUMPING_RIGHT		106
#define MARIO_GREEN_ANI_BIG_JUMPING_LEFT		107
#define MARIO_GREEN_ANI_BIG_HOLDING_IDLE_RIGHT	108
#define MARIO_GREEN_ANI_BIG_HOLDING_IDLE_LEFT	109
#define MARIO_GREEN_ANI_BIG_HOLDING_RIGHT		110
#define MARIO_GREEN_ANI_BIG_HOLDING_LEFT		111
#define MARIO_GREEN_ANI_BIG_KICKING_RIGHT		112
#define MARIO_GREEN_ANI_BIG_KICKING_LEFT		113
#define MARIO_GREEN_ANI_JUMP_FALL_RIGHT			114
#define MARIO_GREEN_ANI_JUMP_FALL_LEFT			115
#define MARIO_GREEN_ANI_BIG_RUNNING_RIGHT		116
#define MARIO_GREEN_ANI_BIG_RUNNING_LEFT		117

#define MARIO_RED_ANI_HITTED					118
#define MARIO_RED_LOOKING_UP					119

#define MARIO_ANI_DIE							120

#define MARIO_ANI_BIG_PIPE						121
#define MARIO_ANI_SMALL_PIPE					122
#define MARIO_ANI_TAIL_PIPE						123
#define MARIO_ANI_FIRE_PIPE						124

#define MARIO_TRANSFORM_LEFT					125
#define MARIO_TRANSFORM_RIGHT					126

#define MARIO_SMOKE_TRANSFORM_LEFT				127
#define MARIO_SMOKE_TRANSFORM_RIGHT				128

#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_TAIL	3
#define	MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_TAIL_BBOX_WIDTH  22
#define MARIO_TAIL_BBOX_HEIGHT 27

#define MARIO_FIRE_BBOX_WIDTH  14
#define MARIO_FIRE_BBOX_HEIGHT 27

#define MARIO_UNTOUCHABLE_TIME	 5000
#define MARIO_TURNING_TIME		 400
#define MARIO_FIRING_RECOG_TIME	 100	 
#define MARIO_KICKING_TIME		 200	
#define MARIO_FIRING_TIME		 400	
#define MARIO_RUNNING_LIMIT_TIME 300
#define MARIO_FLYING_LIMIT_TIME	 7000
#define MARIO_TURNING_BONUS_HEIGHT	8

#define MARIO_TYPE_RED			1
#define MARIO_TYPE_GREEN		2

#define MARIO_MAX_STACK			 7	


#define MARIO_DIFFERENCE_HEIGHT 12


class CMario : public CGameObject
{
	int level;
	int untouchable;
	int time_mario = 0;

	bool isTransforming = false;

	bool lose_control = false;

	bool isAllowToSetLifeDown = true;
	
	int type;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;

	bool isAppear = true;

	bool isJumping = false;
	bool isTurning = false;
	bool isKicking = false;
	bool isHolding = false;
	bool isFiring = false;
	bool isFlying = false;
	bool isFalling = false;
	bool isFired = false;
	bool canBrake;
	bool canHold = false;
	bool canFall = false;
	bool canFly = false;
	bool fireRecog = false;

	bool canPipeDowning = false;
	bool canPipeUpping = false;

	bool isAtTheTunnel = false;
	bool setPositionOutOfTunnel = false;

	bool isAllowToShowBush = false;
	bool isAllowToShowMenuGame = false;
	bool isAllowToShowKoopasLine = false;
	bool isAllowToShowKoopasFaster = false;

	bool isAllowToThroughMario = false;
	bool transformRecog = false;

	float CheckPosition;
	float MushroomCheckPosition;

	DWORD turning_start = 0;
	DWORD running_start = 0;
	DWORD kicking_start = 0;
	DWORD firing_start = 0;
	DWORD flying_start = 0;
	DWORD hitted_start = 0;
	DWORD switch_scene_start = 0;
	DWORD fire_recog_start = 0;

	DWORD pipe_downing_start = 0;
	DWORD pipe_upping_start = 0;

	DWORD on_the_air_start = 0;

	DWORD transforming_start = 0;
	//DWORD count_down_time_start = 0;
public:
	CMario(int ctype,float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy);
	void SetState(int state);
	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartRunning() { running_start = GetTickCount(); }
	void StartTurning() { turning_start = GetTickCount(); }
	void StartKicking() { kicking_start = GetTickCount(); }
	void StartFiring() { firing_start = GetTickCount(); }
	void StartFlying() { flying_start = GetTickCount(); }
	void StartOnTheAir() { on_the_air_start = GetTickCount(); }
	void StartHitted() { hitted_start = GetTickCount(); }
	/*void StartCountDownTimePicker()
	{
		if (count_down_time_start == 0)
			count_down_time_start = GetTickCount();
	}*/
	void StartTransforming()
	{
		if (transforming_start == 0)
			transforming_start = GetTickCount();
	}
	void StartSwitchScene() 
	{
		if(switch_scene_start==0)
		switch_scene_start = GetTickCount(); 
	}
	void StartPipeDowning()
	{
		if(pipe_downing_start==0)
		pipe_downing_start = GetTickCount();
	}
	void StartPipeUpping()
	{
		if (pipe_upping_start == 0)
			pipe_upping_start = GetTickCount();
	}
	void StartFireRecog()
	{
		fire_recog_start = GetTickCount();
	}
	bool GetTransformRecog()
	{
		return transformRecog;
	}
	void SetTransformRecog(bool transformRecogBool)
	{
		transformRecog = transformRecogBool;
	}

	bool GetIsAllowToThroughMario()
	{
		return isAllowToThroughMario;
	}
	void SetIsAllowToThroughMario(bool isAllowToThroughMarioBool)
	{
		isAllowToThroughMario = isAllowToThroughMarioBool;
	}
	bool GetIsTransforming()
	{
		return isTransforming;
	}
	void SetIsTransforming(bool isTransformingBool)
	{
		isTransforming = isTransformingBool;
	}
	bool GetIsAtTheTunnel()
	{
		return isAtTheTunnel;
	}
	void SetIsAtTheTunnel(bool isAtTheTunnelBool)
	{
		this->isAtTheTunnel = isAtTheTunnelBool;
	}
	bool GetLoseControl()
	{
		return lose_control;
	}
	bool GetFireRecog()
	{
		return fireRecog;
	}
	void SetFireRecog(bool fireRecogBool)
	{
		fireRecog = fireRecogBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	bool GetCanPipeDowning()
	{
		return canPipeDowning;
	}
	void SetCanPipeDowning(bool canPipeDowningBool)
	{
		this->canPipeDowning = canPipeDowningBool;
	}
	bool GetCanPipeUpping()
	{
		return canPipeUpping;
	}
	void SetCanPipeUpping(bool canPipeUppingBool)
	{
		this->canPipeUpping = canPipeUppingBool;
	}
	bool GetIsJumping()
	{
		return isJumping;
	}
	void SetIsJumping(bool isTurningBool)
	{
		this->isJumping = isTurningBool;
	}
	bool GetIsTurning()
	{
		return isTurning;
	}
	void SetIsTurning(bool isTurningBool)
	{
		this->isTurning = isTurningBool;
	}
	bool GetIsKicking()
	{
		return isKicking;
	}
	void SetIsKicking(bool isKickingBool)
	{
		this->isKicking = isKickingBool;
	}
	bool GetIsHolding()
	{
		return isHolding;
	}
	void SetIsHolding(bool isHoldingBool)
	{
		this->isHolding = isHoldingBool;
	}
	bool GetIsFiring()
	{
		return isFiring;
	}
	void SetIsFiring(bool isFiringBool)
	{
		this->isFiring = isFiringBool;
	}
	bool GetIsFlying()
	{
		return isFlying;
	}
	void SetIsFlying(bool isFallingBool)
	{
		this->isFlying = isFallingBool;
	}
	bool GetIsFalling()
	{
		return isFalling;
	}
	void SetIsFalling(bool isFallingBool)
	{
		this->isFalling = isFallingBool;
	}
	bool GetIsFired()
	{
		return isFired;
	}
	void SetIsFired(bool isFiredBool)
	{
		isFired = isFiredBool;
	}
	bool GetCanBrake()
	{
		return canBrake;
	}
	void SetCanBrake(bool brakeBool)
	{
		canBrake = brakeBool;
		return;
	}
	bool GetCanHold()
	{
		return canHold;
	}
	void SetCanHold(bool holdBool)
	{
		canHold = holdBool;
		return;
	}
	bool GetCanFly()
	{
		return canFly;
	}
	void SetCanFly(bool flyBool)
	{
		canFly = flyBool;
		return;
	}
	bool GetCanFall()
	{
		return canFall;
	}
	void SetCanFall(bool fallBool)
	{
		canFall = fallBool;
		return;
	}
	DWORD GetFlyingStart()
	{
		return flying_start;
	}
	DWORD GetRunningStart()
	{
		return running_start;
	}
	DWORD GetHittedStart()
	{
		return hitted_start;
	}
	DWORD GetOnTheAir()
	{
		return on_the_air_start;
	}
	int GetMarioTime()
	{
		return time_mario;
	}
	void SetMarioTime(int mario_time)
	{
		time_mario = mario_time;
	}
	bool GetIsAllowToShowBush()
	{
		return isAllowToShowBush;
	}
	void SetIsAllowToShowBush(bool isAllowToShowBushBool)
	{
		isAllowToShowBush = isAllowToShowBushBool;
	}
	bool GetIsAllowToShowMenuGame()
	{
		return isAllowToShowMenuGame;
	}
	void SetIsAllowToShowMenuGame(bool isAllowToShowMenuGameBool)
	{
		isAllowToShowMenuGame = isAllowToShowMenuGameBool;
	}
	bool GetIsAllowToShowKoopasLine()
	{
		return isAllowToShowKoopasLine;
	}
	void SetIsAllowToShowKoopasLine(bool isAllowToShowKoopasLineBool)
	{
		isAllowToShowKoopasLine = isAllowToShowKoopasLineBool;
	}
	bool GetIsAllowToShowKoopasFaster()
	{
		return isAllowToShowKoopasFaster;
	}
	void SetIsAllowToShowKoopasFaster(bool isAllowToShowKoopasFasterBool)
	{
		isAllowToShowKoopasFaster = isAllowToShowKoopasFasterBool;
	}
	float GetMushroomCheckPosition()
	{
		return MushroomCheckPosition;
	}
	void Reset();
	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void CalcTheMarioTimeUp()
	{
		if (isJumping)
		{
			if (GetTickCount() - on_the_air_start >= 7000)
			{
				time_mario = 0;
			}
		}
		else if (GetTickCount() - running_start >= MARIO_RUNNING_LIMIT_TIME && time_mario < MARIO_MAX_STACK)
		{
			running_start = 0;
			time_mario++;
		}
	}

	void CalcTheMarioTimeDown()
	{
		 if (GetTickCount() - running_start >= MARIO_RUNNING_LIMIT_TIME && time_mario >0 )
		{
			running_start = GetTickCount();
			time_mario--;
		}
	}

	bool BrakingCalculation()
	{
		if (lose_control)
			return false;
		if (nx*vx < 0)
		{
			if (nx > 0)
			{
				vx += MARIO_WALKING_SPEED / 30;
			}
			else
			{
				vx -= MARIO_WALKING_SPEED / 30;
			}
			canBrake = true;
			return true;
		}
		else
		{
			canBrake = false;
		}

		return false;
	}
};