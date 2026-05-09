import json
import sys

# ============================================================
# C Code Generator From AST JSON
# ============================================================

class CCodeGenerator:

    def __init__(self):
        self.indent_level = 1

    # ========================================================
    # INDENTATION
    # ========================================================

    def indent(self):
        return "    " * self.indent_level

    # ========================================================
    # MAIN GENERATION
    # ========================================================

    def generate(self, ast):

        code = []

        code.append("#include <stdio.h>")
        code.append("")
        code.append("int main()")
        code.append("{")

        for stmt in ast:
            code.append(self.generate_stmt(stmt))

        code.append("")
        code.append("    return 0;")
        code.append("}")

        return "\n".join(code)

    # ========================================================
    # STATEMENTS
    # ========================================================

    def generate_stmt(self, node):

        node_type = node["type"]

        if node_type == "var_decl":
            return self.generate_var_decl(node)

        elif node_type == "assign_stmt":
            return self.generate_assign_stmt(node)

        elif node_type == "if":
            return self.generate_if(node)

        elif node_type == "while":
            return self.generate_while(node)

        elif node_type == "for":
            return self.generate_for(node)

        else:
            raise Exception(
                f"Unknown statement type: {node_type}"
            )

    # ========================================================
    # VARIABLE DECLARATION
    # ========================================================

    def generate_var_decl(self, node):

        name = node["name"]
        value = node["value"]

        # ----------------------------------------------------
        # ARRAY DECLARATION
        # ----------------------------------------------------

        if value["type"] == "array":

            elements = value["elements"]
            size = len(elements)

            values = ", ".join(
                self.generate_expr(elem)
                for elem in elements
            )

            return (
                f"{self.indent()}int {name}[{size}] = "
                f"{{{values}}};"
            )

        # ----------------------------------------------------
        # NORMAL INTEGER DECLARATION
        # ----------------------------------------------------

        expr = self.generate_expr(value)

        return (
            f"{self.indent()}int {name} = {expr};"
        )

    # ========================================================
    # ASSIGNMENT
    # ========================================================

    def generate_assign_stmt(self, node):

        target = self.generate_target(
            node["target"]
        )

        value = self.generate_expr(
            node["value"]
        )

        return (
            f"{self.indent()}{target} = {value};"
        )

    # ========================================================
    # IF STATEMENT
    # ========================================================

    def generate_if(self, node):

        condition = self.generate_expr(
            node["condition"]
        )

        code = []

        code.append(
            f"{self.indent()}if ({condition})"
        )

        code.append(
            f"{self.indent()}{{"
        )

        self.indent_level += 1

        for stmt in node["then"]:
            code.append(
                self.generate_stmt(stmt)
            )

        self.indent_level -= 1

        code.append(
            f"{self.indent()}}}"
        )

        # ----------------------------------------------------
        # ELSE BLOCK
        # ----------------------------------------------------

        if node.get("else"):

            code.append(
                f"{self.indent()}else"
            )

            code.append(
                f"{self.indent()}{{"
            )

            self.indent_level += 1

            for stmt in node["else"]:
                code.append(
                    self.generate_stmt(stmt)
                )

            self.indent_level -= 1

            code.append(
                f"{self.indent()}}}"
            )

        return "\n".join(code)

    # ========================================================
    # WHILE LOOP
    # ========================================================

    def generate_while(self, node):

        condition = self.generate_expr(
            node["condition"]
        )

        code = []

        code.append(
            f"{self.indent()}while ({condition})"
        )

        code.append(
            f"{self.indent()}{{"
        )

        self.indent_level += 1

        for stmt in node["body"]:
            code.append(
                self.generate_stmt(stmt)
            )

        self.indent_level -= 1

        code.append(
            f"{self.indent()}}}"
        )

        return "\n".join(code)

    # ========================================================
    # FOR LOOP
    # ========================================================

    def generate_for(self, node):

        init = self.generate_inline_assign(
            node["init"]
        )

        condition = self.generate_expr(
            node["condition"]
        )

        update = self.generate_inline_assign(
            node["update"]
        )

        code = []

        code.append(
            f"{self.indent()}for ({init}; {condition}; {update})"
        )

        code.append(
            f"{self.indent()}{{"
        )

        self.indent_level += 1

        for stmt in node["body"]:
            code.append(
                self.generate_stmt(stmt)
            )

        self.indent_level -= 1

        code.append(
            f"{self.indent()}}}"
        )

        return "\n".join(code)

    # ========================================================
    # INLINE ASSIGN
    # USED IN FOR LOOP HEADER
    # ========================================================

    def generate_inline_assign(self, node):

        # Expected AST format:
        #
        # {
        #   "type": "assign_expr",
        #   "name": "i",
        #   "value": ...
        # }

        name = node["name"]

        value = self.generate_expr(
            node["value"]
        )

        return f"{name} = {value}"

    # ========================================================
    # TARGETS
    # ========================================================

    def generate_target(self, node):

        node_type = node["type"]

        # ----------------------------------------------------
        # IDENTIFIER
        # ----------------------------------------------------

        if node_type == "identifier":
            return node["name"]

        # ----------------------------------------------------
        # ARRAY ACCESS
        # ----------------------------------------------------

        elif node_type == "array_access":

            index = self.generate_expr(
                node["index"]
            )

            return f'{node["name"]}[{index}]'

        else:
            raise Exception(
                f"Unknown target type: {node_type}"
            )

    # ========================================================
    # EXPRESSIONS
    # ========================================================

    def generate_expr(self, node):

        node_type = node["type"]

        # ----------------------------------------------------
        # NUMBER
        # ----------------------------------------------------

        if node_type == "number":
            return str(node["value"])

        # ----------------------------------------------------
        # IDENTIFIER
        # ----------------------------------------------------

        elif node_type == "identifier":
            return node["name"]

        # ----------------------------------------------------
        # BINARY OPERATION
        # ----------------------------------------------------

        elif node_type == "binary":

            left = self.generate_expr(
                node["left"]
            )

            right = self.generate_expr(
                node["right"]
            )

            op = node["op"]

            return f"({left} {op} {right})"

        # ----------------------------------------------------
        # ARRAY ACCESS
        # ----------------------------------------------------

        elif node_type == "array_access":

            index = self.generate_expr(
                node["index"]
            )

            return f'{node["name"]}[{index}]'

        # ----------------------------------------------------
        # ARRAY LITERAL
        # ----------------------------------------------------

        elif node_type == "array":

            values = ", ".join(
                self.generate_expr(elem)
                for elem in node["elements"]
            )

            return f"{{{values}}}"

        else:
            raise Exception(
                f"Unknown expression type: {node_type}"
            )


# ============================================================
# LOAD AST JSON FILE
# ============================================================

def load_ast(filename):

    with open(filename, "r") as f:
        return json.load(f)


# ============================================================
# SAVE GENERATED C CODE
# ============================================================

def save_c_code(filename, code):

    with open(filename, "w") as f:
        f.write(code)


# ============================================================
# MAIN
# ============================================================

def main():

    if len(sys.argv) != 3:

        print("Usage:")
        print(
            "python3 code_generator.py input_ast.json output.c"
        )

        return

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    # --------------------------------------------------------
    # LOAD AST
    # --------------------------------------------------------

    ast = load_ast(input_file)

    # --------------------------------------------------------
    # GENERATE C CODE
    # --------------------------------------------------------

    generator = CCodeGenerator()

    c_code = generator.generate(ast)

    # --------------------------------------------------------
    # SAVE RESULT
    # --------------------------------------------------------

    save_c_code(output_file, c_code)

    print(
        f"C code generated successfully: {output_file}"
    )


# ============================================================
# ENTRY POINT
# ============================================================

if __name__ == "__main__":
    main()