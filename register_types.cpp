/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "sprite_rotater.h"

void register_spriteRotater_types() {
    ClassDB::register_class<SpriteRotater>();
}

void unregister_spriteRotater_types() {
   // Nothing to do here in this example.
}