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
# UI
# =========================

st.set_page_config(page_title="Compiler Visualizer", layout="wide")

st.title("🚀 Compiler AST Visualizer (Centered & Zoomed)")

tab1, tab2 = st.tabs(["Tokens", "AST Graph"])

# =========================
# TOKENS VIEW
# =========================

with tab1:
    st.header("Lexer Output (Tokens)")

    if not tokens:
        st.warning("No tokens loaded")
    else:
        for i, t in enumerate(tokens):
            st.write(f"{i} → {t.get('type')} : {t.get('value')}")

# =========================
# COLORS
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
# LABEL BUILDER (FIXED)
# =========================

def make_label(node):
    node_type = node.get("type", "node")
    label = node_type

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
# RENDER AST
# =========================

def render_ast(ast_data):
    if not ast_data:
        st.warning("No AST loaded")
        return

    G = nx.DiGraph()

    # ROOT = PROGRAM (center node)
    root = {"type": "program", "body": ast_data}
    build_graph(G, root)

    net = Network(
        height="800px",
        width="100%",
        directed=True,
        bgcolor="#0b0f19",
        font_color="white"
    )

    # =========================
    # BETTER LAYOUT (IMPORTANT FIX)
    # =========================

    net.force_atlas_2based(
        gravity=-80,
        central_gravity=0.02,
        spring_length=180,
        spring_strength=0.03,
        damping=0.85
    )

    net.toggle_physics(True)

    # =========================
    # ADD NODES
    # =========================

    for node, data in G.nodes(data=True):
        net.add_node(
            node,
            label=data.get("label", ""),
            color=data.get("color", "#ccc"),
            size=18,
            font={"size": 13}
        )

    # edges
    for src, dst in G.edges():
        net.add_edge(src, dst, color="#555")

    # =========================
    # VISIBILITY + INTERACTION FIX
    # =========================

    net.set_options("""
    {
      "nodes": {
        "borderWidth": 1,
        "shape": "dot",
        "scaling": {
          "min": 10,
          "max": 30
        }
      },
      "edges": {
        "smooth": {
          "type": "continuous"
        }
      },
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

    # =========================
    # SAVE + AUTO ZOOM FIX
    # =========================

    net.save_graph("ast_clean.html")

    # inject auto-fit + center view (IMPORTANT FIX)
    with open("ast_clean.html", "r", encoding="utf-8") as f:
        html = f.read()

    html += """
    <script>
        setTimeout(() => {
            network.fit({
                animation: true
            });
            network.moveTo({
                scale: 0.8,
                position: {x: 0, y: 0}
            });
        }, 500);
    </script>
    """

    components.html(html, height=850, scrolling=True)

# =========================
# TAB VIEW
# =========================

with tab2:
    st.header("AST Graph (Centered & Fully Visible)")
    st.caption("Root is centered, zoom is auto-adjusted to fit full graph")

    render_ast(ast)

st.success("AST visualizer ready 🚀")