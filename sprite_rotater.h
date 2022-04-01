/* Sprite Rotater 
   DESCRIPTION: A class that rotates Sprites with a camera that rotates constantly. Could also be used with a static camera as well with the rotation() function. Assumes that frames are based on a trig chart (int, deg): [(0,0), (1,45), (2,90), (3, 135), (4, 180), (5,225), (6,270), (7,315)]
*/

#ifndef SPRITE_ROTATER_H
#define SPRITE_ROTATER_H

#include "core/reference.h"
/* Also get Godot's math functions from these. */
#include "core/math/vector2.h"
#include "core/math/vector3.h"
#include "scene/3d/sprite_3d.h"

class SpriteRotater : public Reference
{
	GDCLASS(SpriteRotater, Reference);
	
	float distance(Vector3 vec);
	float angles[3] = { 22.5, 67.5, 90 };
	//	# Our angle derived from our vector passed in
	float ang = 0.0;
	int frame = 0;
	enum direction {EAST, NORTHEAST, NORTH, NORTHWEST, 
					WEST, SOUTHWEST, SOUTH, SOUTHEAST};
	
	protected:
		static void _bind_methods();
		
	public:
		int rotationOffset(float rot, int frame);
		int frameChanger(int frame, int subtractor, int minFrame, int maxFrame);
		int adjustFrame(Vector3 dir, int frame, float rotation, int subtractor = 2);
		float getRotation(float rotation);
		int rotater(Vector3 vec);
		int get2DVectorFrame(Vector2 vec);
		int get3DVectorFrame(Vector3 vec);
		Vector2 get2DFrameVector(int frame);
		Vector3 get3DFrameVector(int frame);
		Vector3 setPerpendicular(Vector3 vec, int dir);
		Vector3 setPerpendicular3D(Vector3 vec, int dir1, int axis);
		Vector3 setDiagonal(Vector3 vec, int dir, bool norm = true);
		Vector3 setDiagonal3D(Vector3 vec, int dir, int axis, bool norm = true);
		Vector3 idleTurn(Object* p_sprite, int frame, int row, int curr_frame);
};

#endif // SPRITE_ROTATER_H