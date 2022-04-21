#pragma once

// Quick and dirty struct to handle input for the tank
class InputHandler
{
public:
	bool isWDown = false;
	bool isADown = false;
	bool isSDown = false;
	bool isDDown = false;

	bool isSpaceDown = false;
	bool isShiftDown = false;

	inline bool isMoveKeyDown() const { return isWDown || isADown || isSDown || isDDown; }
};