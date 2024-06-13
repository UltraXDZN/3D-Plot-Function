import tkinter as tk
import sympy as sp
import subprocess
import os

from tkinter import messagebox
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure


def sympy_to_tinyexpr(expr):
    return str(expr).replace('**', '^')

def update_plot():
    user_input = input_text.get("1.0", tk.END).strip()

    x, y = sp.symbols('x y')
    try:
        sympy_expr = sp.sympify(user_input)
    except sp.SympifyError:
        messagebox.showerror("Krivi izraz!", "Način na koji ste unijeli izraz nije ispravan.")
        return

    # Renderiranje LaTeX izraza
    latex_expr = sp.latex(sympy_expr)
    ax.clear()
    ax.text(0.5, 0.5, f"${latex_expr}$", horizontalalignment='center', verticalalignment='center', fontsize=16)
    canvas.draw()

def convert_and_run():
    user_input = input_text.get("1.0", tk.END).strip()

    x, y = sp.symbols('x y')
    try:
        sympy_expr = sp.sympify(user_input)
    except sp.SympifyError:
        messagebox.showerror("Krivi izraz!", "Način na koji ste unijeli izraz nije ispravan.")
        return

    tinyexpr_expr = sympy_to_tinyexpr(sympy_expr)

    #print(f"TinyExpr izraz: {tinyexpr_expr}")

    
    plot_exe_path = './plot.exe'
    #plot_binary_path = './plot'

    
    if not os.path.exists(plot_exe_path):
        messagebox.showerror("Binary nije pronadjen", f"Nije moguće pronaći {plot_exe_path}.")
        return
    #if not os.path.exists(plot_binary_path):
    #    messagebox.showerror("Binary nije pronadjen", f"Nije moguće pronaći {plot_binary_path}.")
    #    return

    # Pokretanje plot binarya zajedno s parametrom (izrazom)
    subprocess.run([plot_exe_path, tinyexpr_expr])


# Tkinter setup
root = tk.Tk()
root.title("3D Surface Plotter")


input_frame = tk.Frame(root)
input_frame.pack(pady=10)
input_label = tk.Label(input_frame, text="Input:")
input_label.pack(side=tk.LEFT)
input_text = tk.Text(input_frame, height=1, width=30, font=("Helvetica", 16))
input_text.pack(side=tk.LEFT)


update_button = tk.Button(root, text="Update Preview", command=update_plot)
update_button.pack(pady=5)


fig = Figure(figsize=(5, 2))
ax = fig.add_subplot(111)
ax.format_coord = lambda x, y: ''
ax.axis('off')  
canvas = FigureCanvasTkAgg(fig, master=root)
canvas.get_tk_widget().pack(pady=10)


submit_button = tk.Button(root, text="Plot", command=convert_and_run)
submit_button.pack(pady=10)


root.mainloop()
