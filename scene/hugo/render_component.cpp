#include "render_component.h"
#include "scene/resources/mesh.h"
#include "core/io/resource.h"
#include "scene/hugo/format_writer.h"

void RenderComponent::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_component"), &RenderComponent::get_component);
	ClassDB::bind_method(D_METHOD("get_material_name"), &RenderComponent::get_material_name);
	ClassDB::bind_method(D_METHOD("set_material_name", "material_name"), &RenderComponent::set_material_name);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "material_name"), "set_material_name", "get_material_name");
}

Dictionary RenderComponent::get_component() const {
	return {
    	{"mesh", FormatWriter::format(get_mesh())},
		{"material_name", FormatWriter::format(material_name)}
	};
}


String RenderComponent::get_material_name() const {
	return material_name;
}

void RenderComponent::set_material_name(const String &p_material_name) {
	material_name = p_material_name;
}
