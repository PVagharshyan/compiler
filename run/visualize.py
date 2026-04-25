import json
import streamlit as st
import networkx as nx
from pyvis.network import Network
import streamlit.components.v1 as components

# =========================
# LOAD DATA
# =========================

def load_json(file):
    try:
        with open(file, "r") as f:
            return json.load(f)
    except Exception:
        return []

tokens = load_json("tokens.json")
ast = load_json("ast.json")

# =========================
# UI CONFIG
# =========================

st.set_page_config(page_title="Compiler Visualizer", layout="wide")
st.title("🚀 Compiler Visualizer (Final Stable Version)")

tab1, tab2 = st.tabs(["🎨 Tokens", "🌐 AST Graph"])

# =========================
# TOKEN COLORS
# =========================

def token_color(token_type: str):
    colors = {
        "KEYWORD": "#FF7043",
        "IDENTIFIER": "#AED581",
        "NUMBER": "#FFF176",
        "STRING": "#4FC3F7",
        "OPERATOR": "#BA68C8",
        "SEPARATOR": "#90A4AE",
        "DEFAULT": "#B0BEC5"
    }
    return colors.get(token_type.upper(), colors["DEFAULT"])

# =========================
# LINE BREAK RULES
# =========================

def is_line_break(val: str):
    return val in [";", "{", "}"]

# =========================
# TOKENS VIEW (YOUR VERSION FIXED)
# =========================

with tab1:
    st.header("🎨 Lexer Output (Clean Code View)")

    if not tokens:
        st.warning("No tokens loaded")
    else:
        token_types = sorted(set(t.get("type", "UNKNOWN") for t in tokens))

        selected_types = st.multiselect(
            "Filter token types",
            token_types,
            default=token_types
        )

        filtered_tokens = [
            t for t in tokens if t.get("type") in selected_types
        ]

        view_mode = st.radio(
            "View mode",
            ["Code View", "Table View"],
            horizontal=True
        )

        if view_mode == "Code View":

            lines = []
            current_line = []

            for t in filtered_tokens:
                t_type = t.get("type", "UNKNOWN")
                t_val = str(t.get("value", ""))

                color = token_color(t_type)

                span = f"""
                <span title="{t_type}"
                      style="
                        color:{color};
                        padding:2px 5px;
                        margin:1px;
                        border-radius:4px;
                        background:rgba(255,255,255,0.04);
                        display:inline-block;
                      ">
                    {t_val}
                </span>
                """

                current_line.append(span)

                if is_line_break(t_val):
                    lines.append(" ".join(current_line))
                    current_line = []

            if current_line:
                lines.append(" ".join(current_line))

            html = """
            <div style="
                background:#0b0f19;
                padding:16px;
                border-radius:12px;
                font-family:monospace;
                white-space:normal;
                line-height:2.4;
                overflow-x:auto;
            ">
            """

            for line in lines:
                html += f"<div style='margin-bottom:10px; white-space:nowrap;'>{line}</div>"

            html += "</div>"

            components.html(html, height=500, scrolling=True)

        else:
            st.dataframe(
                [
                    {"index": i, "type": t.get("type"), "value": t.get("value")}
                    for i, t in enumerate(filtered_tokens)
                ],
                use_container_width=True
            )

# =========================
# AST COLORS (YOUR VERSION)
# =========================

def node_color(node_type: str):
    colors = {
        "program": "#4FC3F7",
        "if": "#FF7043",
        "while": "#AB47BC",
        "for": "#66BB6A",
        "assign": "#FFA726",
        "binary": "#29B6F6",
        "call": "#EF5350",
        "return": "#EC407A",
        "identifier": "#AED581",
        "number": "#FFF176",
        "default": "#B0BEC5",
    }
    return colors.get(node_type, colors["default"])

# =========================
# LABEL BUILDER (SAFE VERSION)
# =========================

def make_label(node):
    node_type = node.get("type", "node")
    label = str(node_type)

    if isinstance(node.get("name"), str):
        label += f"\nname: {node['name']}"

    if isinstance(node.get("value"), (str, int, float)):
        label += f"\nvalue: {node['value']}"

    if isinstance(node.get("op"), str):
        label += f"\nop: {node['op']}"

    return label

# =========================
# GRAPH BUILDER
# =========================

def build_graph(G, node, parent=None, idx=None):
    if idx is None:
        idx = [0]

    node_id = idx[0]
    idx[0] += 1

    node_type = node.get("type", "node")

    G.add_node(
        node_id,
        label=make_label(node),
        color=node_color(node_type)
    )

    if parent is not None:
        G.add_edge(parent, node_id)

    if isinstance(node, dict):
        for v in node.values():
            if isinstance(v, dict):
                build_graph(G, v, node_id, idx)
            elif isinstance(v, list):
                for item in v:
                    if isinstance(item, dict):
                        build_graph(G, item, node_id, idx)

# =========================
# AST RENDER (CENTERED + ZOOM FIX)
# =========================

def render_ast(ast_data):
    if not ast_data:
        st.warning("No AST loaded")
        return

    G = nx.DiGraph()
    root = {"type": "program", "body": ast_data}
    build_graph(G, root)

    net = Network(
        height="800px",
        width="100%",
        directed=True,
        bgcolor="#0b0f19",
        font_color="white"
    )

    net.force_atlas_2based(
        gravity=-80,
        central_gravity=0.02,
        spring_length=180,
        spring_strength=0.03,
        damping=0.85
    )

    net.toggle_physics(True)

    for node, data in G.nodes(data=True):
        net.add_node(
            node,
            label=data.get("label", ""),
            color=data.get("color", "#ccc"),
            size=18,
            font={"size": 13}
        )

    for src, dst in G.edges():
        net.add_edge(src, dst, color="#555")

    net.set_options("""
    {
      "interaction": {
        "hover": true,
        "zoomView": true,
        "dragView": true,
        "navigationButtons": true
      },
      "physics": {
        "enabled": true,
        "stabilization": {
          "iterations": 200
        }
      }
    }
    """)

    net.save_graph("ast.html")

    with open("ast.html", "r", encoding="utf-8") as f:
        html = f.read()

    # AUTO CENTER FIX
    html += """
    <script>
        setTimeout(() => {
            network.fit({ animation: true });
            network.moveTo({ scale: 0.9, position: {x: 0, y: 0} });
        }, 500);
    </script>
    """

    components.html(html, height=850, scrolling=True)

# =========================
# AST TAB
# =========================

with tab2:
    st.header("🌐 AST Graph (Centered & Zoomed)")
    render_ast(ast)

st.success("Compiler Visualizer Ready 🚀")