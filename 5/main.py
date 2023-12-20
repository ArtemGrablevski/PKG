import tkinter as tk
from tkinter import ttk, filedialog
from tkinter import *
from tkinter.messagebox import showerror


def liang_barsky(x_0, y_0, x_1, y_1, x_min, y_min, x_max, y_max):
    dx = x_1 - x_0
    dy = y_1 - y_0

    u1 = 0
    u2 = 1

    p = [-dx, dx, -dy, dy]
    q = [x_0 - x_min, x_max - x_0, y_0 - y_min, y_max - y_0]

    for i in range(4):
        if p[i] == 0:
            if q[i] < 0:
                return None
        else:
            u = q[i] / p[i]
            if p[i] < 0:
                u1 = max(u, u1)
            else:
                u2 = min(u, u2)
    if u1 > u2:
        return None

    x1_clip = x_0 + u1 * dx
    y1_clip = y_0 + u1 * dy
    x2_clip = x_0 + u2 * dx
    y2_clip = y_0 + u2 * dy

    return x1_clip, y1_clip, x2_clip, y2_clip


def cyrus_beck(x_a, y_a, x_b, y_b, polygon):
    p = []
    for i in range(0, len(polygon), 2):
        p.append([polygon[i], polygon[i+1]])

    t_min = 0
    t_max = 1
    for i in range(len(p) - 1):
        v1 = (p[i+1][0] - p[i][0]) * (y_b - y_a) - (p[i+1][1] - p[i][1]) * (x_b - x_a)
        v2 = (p[i+1][0] - p[i][0]) * (y_a - p[i][1]) - (p[i+1][1] - p[i][1]) * (x_a - p[i][0])

        if v1 > 0:
            temp = - v2 / v1
            if temp > t_min:
                t_min = temp
        else:
            temp = - v2 / v1
            if temp < t_max:
                t_max = temp
        if t_min > t_max:
            break

    x1_clip = x_a + (x_b - x_a) * t_min
    y1_clip = y_a + (y_b - y_a) * t_min
    x2_clip = x_a + (x_b - x_a) * t_max
    y2_clip = y_a + (y_b - y_a) * t_max

    return x1_clip, y1_clip, x2_clip, y2_clip

DEFAULT_FONT = ("Arial", 12, "bold")


class MyApp:
    def __init__(self):
        self.root = tk.Tk()
        self.root.resizable(True, True)
        self.style = ttk.Style()
        self.style.configure("TFrame", background="#0000ff", foreground="white")

        self.root.geometry("1000x600")
        self.root.title("Cutting off")
        self.root.minsize(600, 800)

        self.frame = ttk.Frame(self.root)
        self.frame.pack(expand=True, fill="both", side="top")

        # Create a frame for the menu (buttons and scale) on the right side
        self.menu_frame = ttk.Frame(self.frame)
        self.menu_frame.pack(side="right", padx=12, pady=12)

        self.btn_file_path = Button(self.menu_frame, text="Browse", command=self.get_file, width=10, bg="#1bcade",
                                    font=("Arial", 12, "bold"), foreground="#FFFFFF")
        self.btn_file_path.pack(side="top", pady=12)

        self.file_path = Entry(self.menu_frame, width=30, bg="#1bcade", font=("Arial", 12, "bold"),
                               foreground="#FFFFFF")
        self.file_path.pack(side="top", pady=12)
        self.file_path.insert(INSERT, "Select a file...")

        self.algorithm_row = ttk.Frame(self.menu_frame)
        self.algorithm_row.pack(side="top", pady=12)

        self.btn_liang = Button(self.algorithm_row, text="Алгоритм Лианга-Барски", command=self.draw_liang, width=30,
                                bg="#1bcade",
                                font=("Arial", 12, "bold"), foreground="#FFFFFF")
        self.btn_liang.pack(side="top", pady=12)

        self.btn_cyrus = Button(self.algorithm_row, text="Алгоритм Кируса-Бека", command=self.draw_cyrusa, width=30,
                                bg="#1bcade",
                                font=("Arial", 12, "bold"), foreground="#FFFFFF")
        self.btn_cyrus.pack(side="top", pady=12)

        self.scale_label = ttk.Label(self.menu_frame, text="Scale", background="#ffffff", font=("Arial", 12, "bold"))
        self.scale_label.pack(side="top")

        self.scale_input = ttk.Spinbox(self.menu_frame, from_=2, to=2000, command=self.update_all)
        self.scale_input.pack(side="top", pady=12)

        # Create a frame for the canvas on the left side
        self.canvas_frame = ttk.Frame(self.frame)
        self.canvas_frame.pack(expand=True, fill="both", side="left", padx=12, pady=12)

        self.canvas = tk.Canvas(self.canvas_frame, background="#FFFFFF")
        self.canvas.pack(expand=True, fill="both", side="top")

        self.root.bind("<Configure>", self.on_resize)

        self.path = ""
        self.scale = 12
        self.scale_input.set(self.scale)
        self.dots = []
        self.clip = []
        self.window = []
        self.flag = False

    def on_resize(self, event):
        self.update_all()

    def run(self):
        self.root.mainloop()

    def get_file(self):
        self.path = filedialog.askopenfilename()
        if self.path == "":
            return
        if not self.path.endswith("txt"):
            showerror("Error", "Wrong file type. \n Choose txt file.")
            return
        self.flag = True
        self.file_path.delete(0, END)
        self.file_path.insert(INSERT, self.path)
        self.read_file()

    def read_file(self):
        with open(self.path, "r") as file:
            n = file.readline()
            self.dots = []
            for _ in range(int(n)):
                self.dots.append([int(a) for a in file.readline().split()])
            self.window = [int(a) for a in file.readline().split()]
            if len(self.window) < 2 or len(self.window) % 2 != 0:
                showerror("Error", "Polygon should have at least 2 points and an even number of coordinates.")
                return
            self.window.append(self.window[0])
            self.window.append(self.window[1])

    def draw_axes(self):
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        self.canvas.create_line(0, height / 2, width - 10, height / 2, arrow=tk.LAST)
        self.canvas.create_line(width / 2, height, width / 2, 10, arrow=tk.LAST)

    def draw_grid(self):
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()

        x_spacing = width / (2 * self.scale)
        y_spacing = x_spacing

        for i in range(-self.scale, self.scale + 1):
            if i == 0:
                continue
            x = width / 2 + i * x_spacing
            self.canvas.create_line(x, 0, x, height, dash=(2, 2), fill="lightgray")

            if i != self.scale and i != -self.scale:

                if 25 <= self.scale < 50:
                    if i % 2 == 0:
                        self.canvas.create_text(x, height / 2 + 4, text=str(i), anchor=tk.N, font=DEFAULT_FONT)
                elif self.scale >= 50:
                    if i % 5 == 0:
                        self.canvas.create_text(x, height / 2 + 4, text=str(i), anchor=tk.N, font=DEFAULT_FONT)
                else:
                    self.canvas.create_text(x, height / 2 + 4, text=str(i), anchor=tk.N, font=DEFAULT_FONT)

        y_scale = int(height / (2 * y_spacing))
        for i in range(-y_scale, y_scale + 1):
            if i == 0:
                continue
            y = height / 2 + i * y_spacing
            self.canvas.create_line(0, y, width, y, dash=(2, 2), fill="lightgray")

            if i != y_scale and i != -y_scale:

                if 25 <= self.scale < 50:
                    if i % 2 == 0:
                        self.canvas.create_text(width / 2 + 4, y, text=str(-i), anchor=tk.NW, font=DEFAULT_FONT)
                elif self.scale >= 50:
                    if i % 5 == 0:
                        self.canvas.create_text(width / 2 + 4, y, text=str(-i), anchor=tk.NW, font=DEFAULT_FONT)
                else:
                    self.canvas.create_text(width / 2 + 4, y, text=str(-i), anchor=tk.NW, font=DEFAULT_FONT)

        # draw zero
        self.canvas.create_text(width / 2 + 4, height / 2 + 4, text="0", anchor=tk.NW, font=DEFAULT_FONT)

    def update_scale(self, event=None):
        self.scale = int(self.scale_input.get())
        self.canvas.delete("all")
        self.draw_axes()
        self.draw_grid()

    def update_all(self, event=None):
        self.canvas.delete("all")
        self.update_scale(event)
        if self.flag:
            self.draw_polygon()
            self.draw_lines()
        self.draw_grid()
        self.draw_axes()

    def draw_lines(self):
        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        x_spacing = width / (2 * self.scale)
        y_spacing = x_spacing

        for i in range(len(self.dots)):
            x1 = width / 2 + self.dots[i][0] * x_spacing
            y1 = height / 2 - self.dots[i][1] * y_spacing
            x2 = width / 2 + self.dots[i][2] * x_spacing
            y2 = height / 2 - self.dots[i][3] * y_spacing
            self.canvas.create_line(x1, y1, x2, y2, fill="black", width=2, dash=(4, 2))

        for i in range(len(self.clip)):
            x1 = width / 2 + self.clip[i][0] * x_spacing
            y1 = height / 2 - self.clip[i][1] * y_spacing
            x2 = width / 2 + self.clip[i][2] * x_spacing
            y2 = height / 2 - self.clip[i][3] * y_spacing
            self.canvas.create_line(x1, y1, x2, y2, fill="#8203a8", width=3)

    def draw_polygon(self):

        width = self.canvas.winfo_width()
        height = self.canvas.winfo_height()
        x_spacing = width / (2 * self.scale)
        y_spacing = x_spacing

        new_window = []

        for i in range(0, len(self.window), 2):
            new_window.append(width / 2 + self.window[i] * x_spacing)
            new_window.append(height / 2 - self.window[i + 1] * y_spacing)

        self.canvas.create_polygon(new_window, fill="#ff75f1", outline="black")

    def draw_liang(self):
        self.clip = []
        for i in range(len(self.dots)):
            self.clip.append(liang_barsky(*self.dots[i], *self.window[:4]))
        new_window = [self.window[0], self.window[1], self.window[2], self.window[1], self.window[2], self.window[3],
                      self.window[0], self.window[3]]
        self.window = new_window
        self.update_all()

    def draw_cyrusa(self):
        self.clip = []
        for i in range(len(self.dots)):
            self.clip.append(cyrus_beck(*self.dots[i], self.window))
        self.update_all()


if __name__ == "__main__":
    app = MyApp()
    app.run()