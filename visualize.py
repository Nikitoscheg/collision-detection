import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import pandas as pd

fig, ax = plt.subplots()
circles: [plt.Circle] = []

def init():
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)

def update(frame):
    global circles
    for c in circles:
        c.remove()
    circles = []

    for row in data[data['t'] == frame].itertuples():
        c = plt.Circle((row.x, row.y), row.r, color='blue')
        ax.add_patch(c)
        circles.append(c)


if __name__ == '__main__':
    data = pd.read_csv("out.csv", header=None)
    data.columns = ['x', 'y', 'r', 't']
    print(data)
    init()
    for frame in data['t'].unique():
        update(frame)
        plt.draw()
        plt.pause(0.2)

    plt.show()
