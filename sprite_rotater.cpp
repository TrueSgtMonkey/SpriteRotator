#include "sprite_rotater.h"

/* A class that rotates sprites when there is a camera rotating constantly */

/* Just in case we need to do some trig stuff */
float SpriteRotater::distance(Vector3 vec)
{
	return sqrt((vec.x * vec.x) + (vec.z * vec.z));
}

/* Assumes frames are following the Trig Chart:
[(0,0), (1,45), (2,90), (3, 135), (4, 180), (5,225), (6,270), (7,315)]
offsets a frame number based on the angle passed in
180 degrees will offset by 4 for instance
Used to offset the sprite based on the rotation of the game's camera
*/
int SpriteRotater::rotationOffset(float rot, int frame)
{
	if (rot > 337.5 && rot <= 360.0 || rot > 0.0 && rot <= 22.5)
		return frame;
	else if (rot > 22.5 && rot <= 67.5)
		frame = frameChanger(frame, 1, 0, 8);
	else if (rot > 67.5 && rot <= 112.5)
		frame = frameChanger(frame, 2, 1, 8);
	else if (rot > 112.5 && rot <= 157.5)
		frame = frameChanger(frame, 3, 2, 8);
	else if(rot > 157.5 && rot <= 202.5)
		frame = frameChanger(frame, 4, 3, 8);
	else if(rot > 202.5 && rot <= 247.5)
		frame = frameChanger(frame, 5, 4, 8);
	else if(rot > 247.5 && rot <= 292.5)
		frame = frameChanger(frame, 6, 5, 8);
	else if(rot > 292.5 && rot <= 337.5)
		frame = frameChanger(frame, 7, 6, 8);
	return frame;
}

/* subtracts from the frame
   if the frame < 0, 
   keep subtracting based on what is left 
   Just a basic offseter
 */
int SpriteRotater::frameChanger(int frame, int subtractor, int minFrame, int maxFrame)
{
	if(frame > minFrame)
		frame -= subtractor;
	else
		frame = maxFrame - (subtractor - frame);
	return frame;
}

int SpriteRotater::adjustFrame(Vector3 dir, int frame, float rotation, int subtractor)
{
	// getting the frame based on direction moving
	frame = rotater(dir);
	
	// offsetting the frame based on the camera rotation
	frame = rotationOffset(rotation, frame);
	frame = frameChanger(frame, subtractor, subtractor - 1, 8);
	return frame;
}

/* Gets absolute angle */
float SpriteRotater::getRotation(float rotation)
{
	if (rotation < 0)
		rotation = 360 + rotation;
	return rotation;
}

/*
 Gets the rotation of the Sprite based on the Vector3 given.

 Basically, this gives the correct sprite frame based on which way the 
 kinematic body is moving.
*/
int SpriteRotater::rotater(Vector3 vec)
{
	ang = 0.0;
	//	# WARNING : will crash the game if z is ever 0
	//  WARNING: doesn't work at perfect 90 degree angles
	if(vec.z != 0)
		ang = Math::rad2deg(atan(vec.x / vec.z));
	frame = 0;
	if (vec.x < 0)
	{
		//# We are below the sprite on x-axis
		//	# Will use frames 5, 6, and 7
		//	# Negative angle == Quadrant 4
		//	# Positive angle == Quadrant 3
		if (ang > 0)
		{
			//southwest
			if (ang < angles[0]) frame = 4; //west
			else if (ang >= angles[0] && ang < angles[1]) frame = 5; //southwest
			else if (ang >= angles[1] && ang <= angles[2]) frame = 6; //south
		}
		else
		{
			if (ang > -angles[0]) frame = 0; //east
			else if (ang <= -angles[0] && ang > -angles[1]) frame = 7; //southeast
			else if (ang <= -angles[1] && ang > -angles[2]) frame = 6; //south
		}
	}
	else
	{
		//# We are above the sprite on x-axis
		//	# Will use frames 1, 2, 3
		//	# Negative angle == Quadrant 2
		//	# Positive angle == Quadrant 1
		if (ang > 0)
		{
			//#Northeast
			if (ang <= angles[0]) frame = 0; //east
			else if (ang > angles[0] && ang <= angles[1]) frame = 1; //northeast
			else if (ang > angles[1] && ang <= angles[2]) frame = 2; //north
		}
		else
		{
			//#Northwest
			if (ang >= -angles[0]) frame = 4; //west
			else if (ang < -angles[0] && ang >= -angles[1]) frame = 3; //northwest
			else if (ang < -angles[1] && ang >= -angles[2]) frame = 2; //north
		}
	}
	return frame;
}


int SpriteRotater::get2DVectorFrame(Vector2 vec)
{
	if(vec.x > 0)
	{
		if(vec.y > 0) return 7;		//southeast
		else if(vec.y == 0) return 0;	//east
		else return 1;	//northeast
	}
	else if(vec.x == 0)
	{
		if(vec.y > 0) return 6;		//north
		else if(vec.y < 0) return 2;		//south
	}
	else
	{
		if(vec.y > 0) return 5;		//northwest
		else if(vec.y == 0) return 4;	//west
		else return 3;	//southwest
	}
	return -1;
}

int SpriteRotater::get3DVectorFrame(Vector3 vec)
{
	if(vec.x > 0)
	{
		if(vec.z > 0) return 1;		//northeast
		else if(vec.z == 0) return 0;	//east
		else return 7;	//southeast
	}
	else if(vec.x == 0)
	{
		if(vec.z > 0) return 2;		//north
		else if(vec.z < 0) return 6;		//south
	}
	else if(vec.x < 0)
	{
		if(vec.z > 0) return 3;		//northwest
		else if(vec.z == 0) return 4;	//west
		else return 5;	//southwest
	}
	return -1; //either vec == Vector3(0,0,0) or something else
}


Vector2 SpriteRotater::get2DFrameVector(int frame)
{
	Vector2 vec;
	switch(frame)
	{
		case 0:		//east
			vec.x = 1;
			vec.y = 0.01;
			break;
		case 1:		//northeast
			vec.x = 1;
			vec.y = -1;
			break;
		case 2:		//north
			vec.x = 0.01;
			vec.y = -1;
			break;
		case 3:		//northwest
			vec.x = -1;
			vec.y = -1;
			break;
		case 4:	//west
			vec.x = -1;
			vec.y = 0.01;
			break;
		case 5:		//southwest
			vec.x = -1;
			vec.y = 1;
			break;
		case 6:		//south
			vec.x = 0.01;
			vec.y = 1;
			break;
		case 7:		//southeast
			vec.x = 1;
			vec.y = 1;
			break;
		default:	//who knows?
			vec.x = 0.01;
			vec.y = 0.01;
	}
	return vec;
}

Vector3 SpriteRotater::get3DFrameVector(int frame)
{
	Vector3 vec;
	vec.y = 0;
	switch(frame)
	{
		case 0:		//east
			vec.x = 1;
			vec.z = 0.01;
			break;
		case 1:		//northeast
			vec.x = 1;
			vec.z = 1;
			break;
		case 2:		//north
			vec.x = 0.01;
			vec.z = 1;
			break;
		case 3:		//northwest
			vec.x = -1;
			vec.z = 1;
			break;
		case 4:		//west
			vec.x = -1;
			vec.z = 0.01;
			break;
		case 5:		//southwest
			vec.x = -1;
			vec.z = -1;
			break;
		case 6:		//south
			vec.x = 0.01;
			vec.z = -1;
			break;
		case 7:		//southeast
			vec.x = 1;
			vec.z = -1;
			break;
		default:	//who knows?
			vec.x = 0.01;
			vec.z = 0.01;
	}
	return vec;
}

Vector3 SpriteRotater::setPerpendicular(Vector3 vec, int dir)
{
	Vector3 tempVec = vec;
	vec.x = vec.z;
	vec.z = tempVec.x;
	if(dir == 0) vec.x *= -1;
	else vec.z *= -1;
	return vec;
}

Vector3 SpriteRotater::setPerpendicular3D(Vector3 vec, int dir, int axis)
{
	Vector3 tempVec = vec;
	switch(axis)
	{
		case 0: // x and y
			vec.x = vec.y;
			vec.y = tempVec.x;
			if(dir == 0) vec.x *= -1;
			else vec.y *= -1;
			break;
		case 1: // x and z
			vec.x = vec.z;
			vec.z = tempVec.x;
			if(dir == 0) vec.x *= -1;
			else vec.z *= -1;
			break;
		default: // y and z
			vec.y = vec.z;
			vec.z = tempVec.y;
			if(dir == 0) vec.y *= -1;
			else vec.z *= -1;
			break;
	}
	return vec;
}

/* Gets the average of a perpendicular vector and an intended vector to return 
a diagonal vector
   If norm is true, then the vector is returned normalized. */
Vector3 SpriteRotater::setDiagonal(Vector3 vec, int dir, bool norm)
{
	Vector3 perpVec = setPerpendicular(vec, dir);
	vec = (vec + perpVec) / 2.0;
	if(norm) vec = vec.normalized();
	return vec;
}

/* Gets the average of a perpendicular vector and an intended vector to return 
a diagonal vector
   axis == 0: switch x and y
   axis == 1: switch x and z
   axis == 2+: switch y and z
   If norm is true, then the vector is returned normalized. */
Vector3 SpriteRotater::setDiagonal3D(Vector3 vec, int dir, int axis, bool norm)
{
	Vector3 perpVec = setPerpendicular3D(vec, dir, axis);
	vec = (vec + perpVec) / 2.0;
	if(norm) vec = vec.normalized();
	return vec;
}


Vector3 SpriteRotater::idleTurn(Object* p_sprite, int frame, int row, int curr_frame)
{
	Sprite3D* sprite = Object::cast_to<Sprite3D>(p_sprite);
	sprite->set_frame(frame * row);
	sprite->set_frame(sprite->get_frame() + (curr_frame % row));
	return get3DFrameVector(frame);
}


void SpriteRotater::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("rotationOffset", "rot", "frame"), &SpriteRotater::rotationOffset);
	ClassDB::bind_method(D_METHOD("frameChanger", "frame", "subtractor", "minFrame", "maxFrame"), &SpriteRotater::frameChanger);
	ClassDB::bind_method(D_METHOD("getRotation", "rotation"), &SpriteRotater::getRotation);
	ClassDB::bind_method(D_METHOD("rotater", "vec"), &SpriteRotater::rotater);
	ClassDB::bind_method(D_METHOD("get2DVectorFrame", "vec"), &SpriteRotater::get2DVectorFrame);
	ClassDB::bind_method(D_METHOD("get3DVectorFrame", "vec"), &SpriteRotater::get3DVectorFrame);
	ClassDB::bind_method(D_METHOD("get2DFrameVector", "frame"), &SpriteRotater::get2DFrameVector);
	ClassDB::bind_method(D_METHOD("get3DFrameVector", "frame"), &SpriteRotater::get3DFrameVector);
	ClassDB::bind_method(D_METHOD("adjustFrame", "dir", "frame", "rotation", "subtractor"), &SpriteRotater::adjustFrame);
	ClassDB::bind_method(D_METHOD("setPerpendicular", "vec", "dir"), &SpriteRotater::setPerpendicular);
	ClassDB::bind_method(D_METHOD("setDiagonal", "vec", "dir", "norm"), &SpriteRotater::setDiagonal);
	ClassDB::bind_method(D_METHOD("setDiagonal3D", "vec", "dir", "axis", "norm"), &SpriteRotater::setDiagonal3D);
	ClassDB::bind_method(D_METHOD("setPerpendicular3D", "vec", "dir", "axis"), &SpriteRotater::setPerpendicular3D);
	ClassDB::bind_method(D_METHOD("idleTurn", "sprite", "frame", "row", "curr_frame"), &SpriteRotater::idleTurn);
}