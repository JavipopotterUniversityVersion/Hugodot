/**************************************************************************/
/*  scene_writer.cpp                                                      */
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

#include "scene_writer.h"

#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/file_access.h"
#include "core/object/class_db.h"
#include "core/object/callable_method_pointer.h"
#include "scene/hugo/entity.h"
#include "scene/hugo/scene_tracker.h"

#ifdef TOOLS_ENABLED
#include "editor/editor_interface.h"
#endif

// Names matching the SceneTracker::System enum order.
static const char *SYSTEM_NAMES[] = {
	"MOVEMENT_SYSTEM",
	"CHAT_SYSTEM",
	"ZAPAS_SYSTEM",
	"SKIBIDI_SYSTEM",
	"HUGO_SYSTEM",
};

void SceneWriter::write() {
	EditorInterface *editor = EditorInterface::get_singleton();
	ERR_FAIL_NULL_MSG(editor, "SceneWriter::write() must be called from the editor.");

	Node *root_node = editor->get_edited_scene_root();
	ERR_FAIL_NULL_MSG(root_node, "No edited scene root found.");

	SceneTracker *root = Object::cast_to<SceneTracker>(root_node);
	ERR_FAIL_NULL_MSG(root, "Scene root is not a SceneTracker.");

	root->create_entities();
	Dictionary entity_map = root->get_entities_map();

	String scene_name = root->get_name();
	String chats_path = GLOBAL_GET("hugo/chats_path");
	String file_path = chats_path.path_join(scene_name + ".chat");

	// Ensure output directory exists before attempting to write.
	Error dir_err = DirAccess::make_dir_recursive_absolute(chats_path);
	ERR_FAIL_COND_MSG(dir_err != OK && dir_err != ERR_ALREADY_EXISTS,
			vformat("Could not create directory: %s", chats_path));

	Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::WRITE);
	ERR_FAIL_NULL_MSG(file, vformat("Could not open file for writing: %s", file_path));

	Entity *root_entity = Object::cast_to<Entity>(root);
	if (root_entity == nullptr) {
		file->store_line("SCENE");
		Array systems = root->get_systems();
		for (int i = 0; i < systems.size(); i++) {
			int system_idx = (int)systems[i];
			if (system_idx >= 0 && system_idx < (int)(sizeof(SYSTEM_NAMES) / sizeof(SYSTEM_NAMES[0]))) {
				file->store_line(String("SYSTEM ") + SYSTEM_NAMES[system_idx]);
			} else {
				WARN_PRINT(vformat("SceneWriter: invalid system index %d in systems array, skipping.", system_idx));
			}
		}
		file->store_line("");
	} else {
		file->store_line("PREFAB");
	}

	Dictionary entities = root->get_entities();
	Array entity_keys = entities.keys();
	for (int i = 0; i < entity_keys.size(); i++) {
		String entity_key = entity_keys[i];

		ERR_CONTINUE_MSG(!entity_map.has(entity_key),
				vformat("SceneWriter: entity '%s' not found in entities_map, skipping.", entity_key));

		file->store_line("ENTITY " + entity_key + " " + String::num_int64((int64_t)entity_map[entity_key]));

		Dictionary entity = entities[entity_key];
		Array component_keys = entity.keys();
		for (int j = 0; j < component_keys.size(); j++) {
			String component_key = component_keys[j];
			file->store_line("\tCOMPONENT " + component_key);

			Dictionary component = entity[component_key];
			Array parameter_keys = component.keys();
			for (int k = 0; k < parameter_keys.size(); k++) {
				String parameter_key = parameter_keys[k];
				String value = component[parameter_key];
				file->store_line("\t\t[" + parameter_key + "] " + value);
			}
		}
		file->store_line("");
	}

	print_line("Written to: " + file_path);
}

void SceneWriter::_bind_methods() {
	ClassDB::bind_static_method("SceneWriter", D_METHOD("write"), &SceneWriter::write);
}

