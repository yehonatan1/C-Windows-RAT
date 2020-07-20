import tkinter as tk
from tkinter import *

value = None
startEntry = None
x = 1
root = tk.Tk()

t = tk.Text(root, height=50, width=100, font="Times 15")


def hi():
    global value, startEntry , x
    value = startEntry.get()
    # label = Label(root, height=10, width=30, text='Test Command', font="Times 25")
    # label.pack()
    # print(value)
    # t = tk.Text(root, height=10, width=30)
    t.pack()
    t.insert(tk.END, "{x} \n".format(x=x))
    # t.delete(tk.END, "Command Test")
    startEntry.delete(0, len(value))
    x += 1

startLabel = tk.Label(root, text="Enter Command: ", font="Times 15")
startEntry = tk.Entry(root, width=20, )

startLabel.pack()
startEntry.pack()

plotButton = tk.Button(root, text="Send", command=hi, font="Times 22", width=6)
plotButton.pack()

label = tk.Label(root)
label.pack()
root.mainloop()

# import tkinter as tk
#

# def message():
#     label['text'] = value
#     l = Label(root , text='Test Command')
#     l.pack()


# root = tk.Tk()
# T = tk.Text(root, height=50, width=150, font="Times 20")
# T.pack()
# x = 'input("Enter a text")'
# T.insert(tk.END, x)
# tk.l()
# message()
# root.mainloop()
