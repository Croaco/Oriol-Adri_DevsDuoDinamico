#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once
#include "Entity.h"
#include "Point.h"
#include "Window.h"
#include "Map.h"
#include "DynArray.h"
#include <map>

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum animationNames {
	IDLE,
	WALKING,
	DEAD
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void RestartLevel();

public:



private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	entityJump jump;

	iPoint startingPosition;
	int pickCoinFxId;
	int deathFxId;
	uint cameraXCorrection, cameraYCorrection;

	std::map<animationNames, DynArray<char*>> animations;

};

#endif // __PLAYER_H__