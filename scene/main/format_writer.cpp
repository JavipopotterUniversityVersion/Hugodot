/**************************************************************************/
/*  format_writer.cpp                                                     */
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

#include "format_writer.h"

#include "core/object/class_db.h"
#include "scene/resources/mesh.h"
#include "scene/resources/packed_scene.h"

String FormatWriter::format(const Variant &p_variant) {
	switch (p_variant.get_type()) {
		case Variant::VECTOR2: {
			Vector2 v = p_variant;
			return "VECTOR_2 " + String::num(v.x) + " " + String::num(v.y);
		}
		case Variant::VECTOR3: {
			Vector3 v = p_variant;
			return "VECTOR_3 " + String::num(v.x) + " " + String::num(v.y) + " " + String::num(v.z);
		}
		case Variant::VECTOR4: {
			Vector4 v = p_variant;
			return "VECTOR_4 " + String::num(v.x) + " " + String::num(v.y) + " " + String::num(v.z) + " " + String::num(v.w);
		}
		case Variant::OBJECT: {
			Object *obj = p_variant;
			if (obj) {
				Mesh *mesh = Object::cast_to<Mesh>(obj);
				if (mesh) {
					return "MESH " + mesh->get_path();
				}
				PackedScene *packed_scene = Object::cast_to<PackedScene>(obj);
				if (packed_scene) {
					return "PREFAB " + packed_scene->get_path();
				}
			}
			break;
		}
		case Variant::STRING: {
			return "STRING \n" + String(p_variant);
		}
		default:
			break;
	}
	return Variant::get_type_name(p_variant.get_type()).to_upper() + " " + String(p_variant);
}

void FormatWriter::_bind_methods() {
	ClassDB::bind_static_method("FormatWriter", D_METHOD("format", "variant"), &FormatWriter::format);
}
