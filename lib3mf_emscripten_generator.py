import xmltodict
import jinja2

# Load XML and convert to dictionary
def load_xml_as_json(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        xml_data = file.read()
    return xmltodict.parse(xml_data)

# Extract Enums
def extract_enums(lib3mf_idl):
    enums = []
    all_enums = lib3mf_idl["component"]["enum"]
    for enum in all_enums:
        name = enum["@name"]
        options = []
        all_options = enum["option"]
        if isinstance(all_options, list):
            for option in all_options:
                options.append({"name": option["@name"], "value": option["@value"]})
        else:
            options.append({"name": all_options["@name"], "value": all_options["@value"]})
        enums.append({"name": name, "options": options})
    return enums

# Extract Structs (handles arrays)
def extract_structs(lib3mf_idl):
    structs = []
    all_structs = lib3mf_idl["component"]["struct"]

    for struct in all_structs:
        name = struct["@name"]
        members = []
        members_data = struct["member"]
        if not isinstance(members_data, list):
            members_data = [members_data]
        for member in members_data:
            member_info = {
                "name": member["@name"],
                "type": member["@type"],
                "rows": int(member["@rows"]) if "@rows" in member else None,
                "cols": int(member["@columns"]) if "@columns" in member else None,
                "is_enum": "@class" in member and member["@type"] == "enum",
                "enum_class": member["@class"] if "@class" in member else None
            }
            members.append(member_info)
        structs.append({"name": name, "members": members})
    return structs

# Extract Classes and their methods
def extract_classes(lib3mf_idl):
    classes = []
    all_classes = lib3mf_idl["component"]["class"]

    for cls in all_classes:
        name = cls["@name"]
        parent = cls.get("@parent", None)
        methods = []

        methods_data = cls.get("method", [])
        if not isinstance(methods_data, list):
            methods_data = [methods_data]

        for method in methods_data:
            method_name = method["@name"]
            params_data = method.get("param", [])
            if not isinstance(params_data, list):
                params_data = [params_data]

            param_objects = []
            has_out_param = False
            has_callback_param = "Callback" in method_name  # heuristic based on method name

            for p in params_data:
                param_obj = {
                    "name": p["@name"],
                    "type": p["@type"],
                    "pass": p["@pass"],
                    "class": p.get("@class", None)
                }
                if p["@pass"] == "out":
                    has_out_param = True
                param_objects.append(param_obj)

            methods.append({
                "name": method_name,
                "params": param_objects,
                "comment_out": has_out_param or has_callback_param
            })

        classes.append({
            "name": name,
            "parent": parent,
            "methods": methods
        })

    return classes

def extract_wrapper_methods(lib3mf_idl):
    wrapper_methods = []
    all_methods = lib3mf_idl["component"]["global"]["method"]

    if not isinstance(all_methods, list):
        all_methods = [all_methods]

    for method in all_methods:
        method_name = method["@name"]
        params = method.get("param", [])
        if not isinstance(params, list):
            params = [params]

        has_nonconst_in = False
        parsed_params = []
        for param in params:
            param_type = param["@type"]
            param_pass = param["@pass"]
            param_class = param.get("@class", None)

            if param_pass == "in" and param_type not in ["string", "pointer", "handle"]:
                has_nonconst_in = True

            parsed_params.append({
                "name": param["@name"],
                "type": param_type,
                "pass": param_pass,
                "class": param_class
            })

        wrapper_methods.append({
            "name": method_name,
            "params": parsed_params,
            "comment_out": has_nonconst_in
        })

    return wrapper_methods


# Generate C++ file using Jinja
def generate_cpp(enums, structs, classes, wrapper_methods, template_file="lib3mf_bindings.jinja2", output_file="lib3mf_bindings.cpp"):
    with open(template_file, "r", encoding="utf-8") as file:
        template = jinja2.Template(file.read())
    cpp_code = template.render(enums=enums, structs=structs, classes=classes, wrapper_methods=wrapper_methods)
    with open(output_file, "w", encoding="utf-8") as file:
        file.write(cpp_code)
    print(f"✅ Generated {output_file}")


if __name__ == "__main__":
    xml_file = "lib3mf.xml"  # Update this if needed
    lib3mf_idl = load_xml_as_json(xml_file)
    enums = extract_enums(lib3mf_idl)
    structs = extract_structs(lib3mf_idl)
    classes = extract_classes(lib3mf_idl)
    wrapper_methods = extract_wrapper_methods(lib3mf_idl)
    generate_cpp(enums, structs, classes,wrapper_methods)