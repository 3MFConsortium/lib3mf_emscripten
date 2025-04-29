import xmltodict
import jinja2
from pathlib import Path

def load_xml_as_json(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        return xmltodict.parse(file.read())

def extract_enums(lib3mf_idl):
    enums = []
    all_enums = lib3mf_idl["component"]["enum"]
    for enum in all_enums:
        name = enum["@name"]
        options = enum["option"]
        if not isinstance(options, list):
            options = [options]
        enums.append({
            "name": name,
            "options": [{"name": o["@name"], "value": o["@value"]} for o in options]
        })
    return enums

def extract_structs(lib3mf_idl):
    structs = []
    for struct in lib3mf_idl["component"]["struct"]:
        members_data = struct["member"]
        if not isinstance(members_data, list):
            members_data = [members_data]
        members = []
        for m in members_data:
            rows = int(m["@rows"]) if "@rows" in m else None
            cols = int(m["@columns"]) if "@columns" in m else None
            is_array = rows is not None or cols is not None
            is_2d_array = rows is not None and cols is not None
            members.append({
                "name": m["@name"],
                "type": m["@type"],
                "rows": rows,
                "cols": cols,
                "is_enum": "@class" in m and m["@type"] == "enum",
                "enum_class": m.get("@class"),
                "is_array": is_array,
                "is_2d_array": is_2d_array
            })
        structs.append({"name": struct["@name"], "members": members})
    return structs

def extract_classes(lib3mf_idl):
    classes = []
    for cls in lib3mf_idl["component"]["class"]:
        methods_data = cls.get("method", [])
        if not isinstance(methods_data, list):
            methods_data = [methods_data]
        methods = []
        for method in methods_data:
            params_data = method.get("param", [])
            if not isinstance(params_data, list):
                params_data = [params_data]
            param_objects = []
            has_out_param = False
            has_struct_param = False
            has_callback_param = "Callback" in method["@name"]
            for p in params_data:
                if p["@type"] == "struct":
                    has_struct_param = True
                if p["@pass"] == "out":
                    has_out_param = True
                param_objects.append({
                    "name": p["@name"],
                    "type": p["@type"],
                    "pass": p["@pass"],
                    "class": p.get("@class", None)
                })
            returns_struct = param_objects and param_objects[-1]["pass"] == "return" and param_objects[-1]["type"] == "struct"
            struct_class = param_objects[-1]["class"] if returns_struct else None
            methods.append({
                "name": method["@name"],
                "params": param_objects,
                "comment_out": has_out_param or has_callback_param,
                "returns_struct": returns_struct,
                "struct_class": struct_class,
                "has_struct_param": has_struct_param
            })
        classes.append({
            "name": cls["@name"],
            "parent": cls.get("@parent"),
            "methods": methods
        })
    return classes

def extract_wrapper_methods(lib3mf_idl):
    wrapper_methods = []
    all_methods = lib3mf_idl["component"]["global"]["method"]
    if not isinstance(all_methods, list):
        all_methods = [all_methods]
    for method in all_methods:
        params = method.get("param", [])
        if not isinstance(params, list):
            params = [params]
        parsed_params = []
        has_out_param = False
        has_struct_param = False
        for p in params:
            if p["@pass"] == "out":
                has_out_param = True
            if p["@type"] == "struct":
                has_struct_param = True
            parsed_params.append({
                "name": p["@name"],
                "type": p["@type"],
                "pass": p["@pass"],
                "class": p.get("@class")
            })
        returns_struct = parsed_params and parsed_params[-1]["pass"] == "return" and parsed_params[-1]["type"] == "struct"
        struct_class = parsed_params[-1]["class"] if returns_struct else None
        wrapper_methods.append({
            "name": method["@name"],
            "params": parsed_params,
            "comment_out": has_out_param,
            "returns_struct": returns_struct,
            "struct_class": struct_class,
            "has_struct_param": has_struct_param
        })
    return wrapper_methods

def generate_static_wrapper_entries(classes, wrapper_methods):
    static_wrappers = []

    def resolve_type(p):
        if p["type"] == "struct":
            return f"s{p['class']}Wrapper"
        elif p["type"] == "structarray":
            return f"std::vector<s{p['class']}>"
        elif p["type"] == "basicarray":
            return f"std::vector<Lib3MF_{p['class']}>"
        elif p["type"] == "handle" and p.get("class"):
            return f"P{p['class']}"
        elif p["type"] == "class":
            return f"P{p['class']}"
        elif p["type"] == "enum":
            return f"e{p['class']}"
        elif p["type"] == "string":
            return "std::string"
        elif p["type"] == "bool":
            return "bool"
        else:
            return f"Lib3MF_{p['type']}"

    def wrap_method(class_name, method, is_global):
        # Skip methods that have any out parameter (handled in out_param_wrappers)
        if any(p["pass"] == "out" for p in method["params"]):
            return None

        if not (method["returns_struct"] or method["has_struct_param"]):
            return None

        return_param = next((p for p in method["params"] if p["pass"] == "return"), None)
        param_decls, call_args, out_decls, out_sets = [], [], [], []
        use_output_object = False

        for p in method["params"]:
            if p["pass"] == "return":
                continue
            t = resolve_type(p)

            if p["type"] == "struct":
                param_decls.append(f"const s{p['class']}Wrapper& {p['name']}")
                call_args.append(f"{p['name']}.toStruct()")
            else:
                param_decls.append(f"{t} {p['name']}")
                call_args.append(p["name"])

        if method["returns_struct"]:
            return_type = f"s{method['struct_class']}Wrapper"
            return_expression = "wrapper"
            has_return = True
            returns_wrapper_directly = True
        elif return_param:
            return_type = resolve_type(return_param)
            return_expression = "result"
            has_return = True
            returns_wrapper_directly = False
        else:
            return_type = "void"
            return_expression = ""
            has_return = False
            returns_wrapper_directly = False

        return {
            "name": method["name"],
            "class": class_name,
            "is_global": is_global,
            "return_type": return_type,
            "param_decls": param_decls,
            "call_args": call_args,
            "out_decls": out_decls,
            "out_sets": out_sets,
            "returns_wrapper_directly": returns_wrapper_directly,
            "return_expression": return_expression,
            "has_return": has_return
        }

    for cls in classes:
        for method in cls["methods"]:
            result = wrap_method(cls["name"], method, is_global=False)
            if result:
                static_wrappers.append(result)

    for method in wrapper_methods:
        result = wrap_method("Wrapper", method, is_global=True)
        if result:
            static_wrappers.append(result)

    return static_wrappers


def generate_out_param_wrappers(classes, wrapper_methods):
    out_param_wrappers = []

    def resolve_type(p):
        if p["type"] == "struct":
            return f"s{p['class']}Wrapper"
        elif p["type"] == "structarray":
            return f"std::vector<s{p['class']}>"
        elif p["type"] == "basicarray":
            return f"std::vector<Lib3MF_{p['class']}>"
        elif p["type"] == "handle" and p.get("class"):
            return f"P{p['class']}"
        elif p["type"] == "class":
            return f"P{p['class']}"
        elif p["type"] == "enum":
            return f"e{p['class']}"
        elif p["type"] == "string":
            return "std::string"
        elif p["type"] == "bool":
            return "bool"
        else:
            return f"Lib3MF_{p['type']}"

    def wrap_out_method(class_name, method, is_global):
        if not any(p["pass"] == "out" for p in method["params"]):
            return None

        return_param = next((p for p in method["params"] if p["pass"] == "return"), None)
        param_decls, call_args, var_decls, out_sets = [], [], [], []
        call_stmt = ""

        for p in method["params"]:
            if p["pass"] == "return":
                continue

            t = resolve_type(p)

            if p["pass"] == "out":
                var_decls.append(f"{t} {p['name']};")
                call_args.append(p["name"] if p["type"] != "struct" else f"{p['name']}.value")
                out_sets.append(f'output.set("{p["name"]}", {p["name"]});')
            elif p["type"] == "struct":
                param_decls.append(f"const s{p['class']}Wrapper& {p['name']}")
                call_args.append(f"{p['name']}.toStruct()")
            else:
                param_decls.append(f"{t} {p['name']}")
                call_args.append(p["name"])

        # Handle return value
        if return_param:
            ret_type = resolve_type(return_param)
            var_decls.append(f"{ret_type} return_value = self.{method['name']}({', '.join(call_args)});")
            out_sets.insert(0, 'output.set("return", return_value);')
            call_stmt = None
        else:
            call_stmt = f"self.{method['name']}({', '.join(call_args)});"

        var_decls.append("emscripten::val output = emscripten::val::object();")

        return {
            "name": method["name"],
            "class": class_name,
            "is_global": is_global,
            "return_type": "emscripten::val",
            "param_decls": param_decls,
            "out_decls": var_decls,
            "call_stmt": call_stmt,
            "out_sets": out_sets,
            "return_expression": "output"
        }

    for cls in classes:
        for method in cls["methods"]:
            result = wrap_out_method(cls["name"], method, is_global=False)
            if result:
                out_param_wrappers.append(result)

    for method in wrapper_methods:
        result = wrap_out_method("Wrapper", method, is_global=True)
        if result:
            out_param_wrappers.append(result)

    return out_param_wrappers

def generate_cpp(enums, structs, classes, wrapper_methods, template_file="lib3mf_bindings.jinja2", output_file="lib3mf_bindings.cpp"):
    all_methods = []
    for cls in classes:
        for method in cls["methods"]:
            all_methods.append({"is_global": False, "class": cls["name"], "parent": cls.get("parent"), "method": method})
    for method in wrapper_methods:
        all_methods.append({"is_global": True, "class": "Wrapper", "method": method})

    static_wrappers = generate_static_wrapper_entries(classes, wrapper_methods)
    out_param_wrappers = generate_out_param_wrappers(classes, wrapper_methods)

    # 🔧 NEW: collect all wrapper function names for lookup in template
    wrapper_names = [f"wrap_{w['class']}_{w['name']}" for w in static_wrappers + out_param_wrappers]

    with open(template_file, "r", encoding="utf-8") as file:
        template = jinja2.Template(file.read())

    cpp_code = template.render(
        enums=enums,
        structs=structs,
        classes=classes,
        wrapper_methods=wrapper_methods,
        all_methods=all_methods,
        static_wrappers=static_wrappers,
        out_param_wrappers=out_param_wrappers,
        wrapper_names=wrapper_names  # ✅ Added to context
    )

    Path(output_file).write_text("\n".join([line for line in cpp_code.splitlines() if line.strip() != ""]), encoding="utf-8")
    print(f"✅ Generated {output_file} (empty lines removed)")


if __name__ == "__main__":
    xml_file = "lib3mf.xml"
    lib3mf_idl = load_xml_as_json(xml_file)
    enums = extract_enums(lib3mf_idl)
    structs = extract_structs(lib3mf_idl)
    classes = extract_classes(lib3mf_idl)
    wrapper_methods = extract_wrapper_methods(lib3mf_idl)
    generate_cpp(enums, structs, classes, wrapper_methods)