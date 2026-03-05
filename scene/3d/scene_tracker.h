/**************************************************************************/
/*  scene_tracker.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "scene/3d/node_3d.h"

class Entity;

class SceneTracker : public Node3D {
	GDCLASS(SceneTracker, Node3D);

public:
	enum System {
		MOVEMENT_SYSTEM,
		CHAT_SYSTEM,
		ZAPAS_SYSTEM,
		SKIBIDI_SYSTEM,
		HUGO_SYSTEM,
	};

private:
	Dictionary entities;
	Array systems;
	int current_entity_index = 0;
	Dictionary entities_map;
	Callable save_scene_callable;

protected:
	static void _bind_methods();

public:
	void set_entities(const Dictionary &p_entities);
	Dictionary get_entities() const;

	void set_systems(const Array &p_systems);
	Array get_systems() const;

	void set_save_scene_callable(const Callable &p_callable);
	Callable get_save_scene_callable() const;

	void create_entities();
	void add_entity(Entity *p_entity);
	Dictionary gather_entities(Node *p_parent);

	SceneTracker();
};

VARIANT_ENUM_CAST(SceneTracker::System);
