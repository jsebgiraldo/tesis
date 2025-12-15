import os
from pathlib import Path

# Try different SVG to PNG conversion methods
svg_dir = Path("figures")
svg_files = list(svg_dir.glob("*.svg"))

print(f"Found {len(svg_files)} SVG files to convert")

# Method 1: Try cairosvg (fast, good quality)
try:
    import cairosvg
    print("Using cairosvg...")
    for svg_file in svg_files:
        png_file = svg_file.with_suffix('.png')
        cairosvg.svg2png(url=str(svg_file), write_to=str(png_file), dpi=300)
        print(f"   {png_file.name}")
    print(f"\n Converted {len(svg_files)} files successfully")
    exit(0)
except ImportError:
    print("cairosvg not available, trying alternative...")

# Method 2: Try svglib + reportlab
try:
    from svglib.svglib import svg2rlg
    from reportlab.graphics import renderPM
    print("Using svglib + reportlab...")
    for svg_file in svg_files:
        png_file = svg_file.with_suffix('.png')
        drawing = svg2rlg(str(svg_file))
        renderPM.drawToFile(drawing, str(png_file), fmt="PNG", dpi=300)
        print(f"   {png_file.name}")
    print(f"\n Converted {len(svg_files)} files successfully")
    exit(0)
except ImportError:
    print("svglib not available")

# Method 3: PIL/Pillow (basic support)
try:
    from PIL import Image
    print("Using Pillow (basic conversion)...")
    for svg_file in svg_files:
        png_file = svg_file.with_suffix('.png')
        # Note: PIL doesn't handle SVG well, this is a fallback
        print(f"   {svg_file.name} - PIL has limited SVG support")
    print("\nPlease install: pip install cairosvg")
    exit(1)
except ImportError:
    print("No conversion method available")
    print("\nInstall one of:")
    print("  pip install cairosvg")
    print("  pip install svglib reportlab")
    exit(1)
