#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position = { parameters.attribute("x").as_int(), parameters.attribute("y").as_int() };
	startingPosition = position;
	texturePath = parameters.attribute("texturepath").as_string();
	jump = { false, 0, parameters.attribute("maxjumps").as_int(), 0, parameters.attribute("jumpimpulse").as_float()};

	

	return true;
}

bool Player::Start() {

	app->win->GetWindowSize(cameraXCorrection, cameraYCorrection);

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this; 

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect
	pickCoinFxId = app->audio->LoadFx(parameters.attribute("coinfxfolder").as_string());

	return true;
}

bool Player::Update()
{

	if (jump.bJumping) {
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) jump.bJumping = false;
		jump.timeSinceLastJump++;
	}

	b2Vec2 vel = pbody->body->GetLinearVelocity();
	b2Vec2 impulse = b2Vec2_zero;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (!jump.bJumping && jump.currentJumps <= jump.maxJumps) {
			jump.bJumping = true;
			jump.timeSinceLastJump = 0;
			jump.currentJumps++;

			impulse.y = jump.jumpImpulse * -1;

			pbody->body->SetLinearVelocity(b2Vec2(vel.x, 0));
			pbody->body->ApplyLinearImpulse(b2Vec2(0, impulse.y), pbody->body->GetWorldCenter(), true);
		}
	}

	float maxVel = 5.0f;
	bool moveCamera = false;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		impulse.x = b2Max(vel.x - 0.25f, maxVel * -1);
		moveCamera = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		impulse.x = b2Min(vel.x + 0.25f, maxVel);
		moveCamera = true;
	}
	if (!impulse.x) impulse.x = vel.x * 0.98f;

	pbody->body->SetLinearVelocity(b2Vec2(impulse.x, pbody->body->GetLinearVelocity().y));

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (moveCamera){
		if (app->render->camera.x <= 0 && position.x >= startingPosition.x) {
			if (abs(app->render->camera.x) + cameraXCorrection <= app->level->mapData.width * app->level->mapData.tileWidth) {
				app->render->camera.x -= (impulse.x * 0.90);
				if (app->render->camera.x > 0) app->render->camera.x = 0;
			}
			else if (impulse.x < 0) {
				app->render->camera.x -= (impulse.x * 0.90);
			}
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {

	}	
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {

	}
	
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		RestartLevel();
	}

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if ((pbody->body->GetPosition().y < physB->body->GetPosition().y)) {
				jump = { false, 0, jump.maxJumps, 0, jump.jumpImpulse };
			}
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
	}
}

void Player::RestartLevel()
{
	position = startingPosition;
	jump = { false, 0, parameters.attribute("maxjumps").as_int(), 0, parameters.attribute("jumpimpulse").as_float() };
	app->render->camera.x = 0;
	app->render->camera.y = 0;
}

