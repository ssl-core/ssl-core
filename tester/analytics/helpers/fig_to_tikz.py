import sys
import tikzplotlib as tkz
import matplotlib.pyplot as plt

def fig_to_tikz(fig, filename):
    """
    Save a matplotlib figure as a TikZ file.

    Parameters
    ----------
    fig : matplotlib.figure.Figure
        The figure to save.
    filename : str
        The name of the file to save the figure to.
    """
    # Save the figure as a TikZ file
    tkz.save(
        fig,
        f"{filename}.tex", 
        axis_height=f"\\{filename}H", 
        axis_width=f"\\{filename}W", 
        extra_groupstyle_parameters={f'vertical sep=\\{filename}VS'},
        strict=True
    )

if len(sys.argv) > 1:
    for i in range(1, len(sys.argv)):
        filename = sys.argv[i]
        fig = plt.open(filename)
        if fig is None:
            print(f"Error: Could not open figure {filename}")
            continue
        fig_to_tikz(fig, filename)        
else:
    print("Error; The correct usage of this script is `python3 script.py filename1 filename2 ...`")
    sys.exit(1)