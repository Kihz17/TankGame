#include "LaserWeapon.h"

LaserWeapon::LaserWeapon()
	: Weapon("laser", 2.0f, 10.0f)
{

}

bool LaserWeapon::CanShoot()
{
	return true;
}