#pragma once
class Player
{
private:
	int life;
	const char* name;
public:
	Player(int life, const char* name);
	void move();
};