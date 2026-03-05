/**************************************************************************/
/*  scene_tracker.cpp                                                     */
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

#include "scene_tracker.h"

#include "core/object/callable_method_pointer.h"
#include "core/object/class_db.h"
#include "core/variant/variant.h"
#include "scene/3d/entity.h"
#include "scene/3d/entity_3d.h"
#include "scene/main/component.h"
#include "scene/main/scene_writer.h"

void SceneTracker::set_entities(const Dictionary &p_entities) {
	entities = p_entities;
}

Dictionary SceneTracker::get_entities() const {
	return entities;
}

void SceneTracker::set_systems(const Array &p_systems) {
	systems = p_systems;
}

Array SceneTracker::get_systems() const {
	return systems;
}

void SceneTracker::set_save_scene_callable(const Callable &p_callable) {
	save_scene_callable = p_callable;
}

Callable SceneTracker::get_save_scene_callable() const {
	return save_scene_callable;
}

void SceneTracker::create_entities() {
	entities_map = Dictionary();
	entities = Dictionary();
	current_entity_index = 0;

	Entity *self_entity = Object::cast_to<Entity>(this);
	if (self_entity) {
		add_entity(self_entity);
	}
	gather_entities(this);
}

void SceneTracker::add_entity(Entity *p_entity) {
	String entity_name = p_entity->get_name();
	while (entities.has(entity_name)) {
		entity_name = entity_name + "_O";
	}
	p_entity->set_name(StringName(entity_name));
	if (!entities.has(entity_name)) {
		entities[entity_name] = Dictionary();
	}
	if (!entities_map.has(entity_name)) {
		entities_map[entity_name] = current_entity_index;
	}
	current_entity_index++;
}

Dictionary SceneTracker::gather_entities(Node *p_parent) {
	for (int i = 0; i < p_parent->get_child_count(); i++) {
		Node *child = p_parent->get_child(i);

		Entity *child_entity = Object::cast_to<Entity>(child);
		if (child_entity) {
			add_entity(child_entity);

			Entity *parent_entity = Object::cast_to<Entity>(p_parent);
			if (parent_entity) {
				String child_name = child_entity->get_name();
				Dictionary child_dict = entities[child_name];
				if (!child_dict.has("ParentComponent")) {
					String parent_name = parent_entity->get_name();
					Dictionary parent_component;
					parent_component["value"] = "INT " + String::num_int64((int64_t)entities_map[parent_name]);
					child_dict["ParentComponent"] = parent_component;
				}
			}

			gather_entities(child_entity);

			Entity3D *child_entity3d = Object::cast_to<Entity3D>(child_entity);
			if (child_entity3d) {
				String child_name = child_entity->get_name();
				Dictionary child_dict = entities[child_name];
				if (!child_dict.has("TransformComponent")) {
					child_dict["TransformComponent"] = child_entity3d->get_component();
				}
			}
		}

		Component *child_component = Object::cast_to<Component>(child);
		if (child_component) {
			Entity *parent_entity = Object::cast_to<Entity>(p_parent);
			if (parent_entity) {
				String child_class_name = child->get_class();
				String parent_name = parent_entity->get_name();
				Dictionary parent_dict = entities[parent_name];
				if (!parent_dict.has(child_class_name)) {
					parent_dict[child_class_name] = child_component->get_component();
				}
			}
		}
	}
	return entities;
}

void SceneTracker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_entities", "entities"), &SceneTracker::set_entities);
	ClassDB::bind_method(D_METHOD("get_entities"), &SceneTracker::get_entities);
	ClassDB::bind_method(D_METHOD("set_systems", "systems"), &SceneTracker::set_systems);
	ClassDB::bind_method(D_METHOD("get_systems"), &SceneTracker::get_systems);
	ClassDB::bind_method(D_METHOD("set_save_scene_callable", "callable"), &SceneTracker::set_save_scene_callable);
	ClassDB::bind_method(D_METHOD("get_save_scene_callable"), &SceneTracker::get_save_scene_callable);

	ClassDB::bind_method(D_METHOD("create_entities"), &SceneTracker::create_entities);
	ClassDB::bind_method(D_METHOD("add_entity", "entity"), &SceneTracker::add_entity);
	ClassDB::bind_method(D_METHOD("gather_entities", "parent"), &SceneTracker::gather_entities);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "entities"), "set_entities", "get_entities");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "systems"), "set_systems", "get_systems");
	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "save_scene", PROPERTY_HINT_TOOL_BUTTON, "Save Scene"), "set_save_scene_callable", "get_save_scene_callable");

	BIND_ENUM_CONSTANT(MOVEMENT_SYSTEM);
	BIND_ENUM_CONSTANT(CHAT_SYSTEM);
	BIND_ENUM_CONSTANT(ZAPAS_SYSTEM);
	BIND_ENUM_CONSTANT(SKIBIDI_SYSTEM);
	BIND_ENUM_CONSTANT(HUGO_SYSTEM);
}

SceneTracker::SceneTracker() {
	save_scene_callable = callable_mp_static(&SceneWriter::write);
}
