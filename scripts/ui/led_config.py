# This file is part of ArduRGB, an app to control lighting devices.
# Copyright (C) 2021 Milan Sorak.
#
# ArduRGB is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ArduRGB is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ArduRGB.  If not, see <https://www.gnu.org/licenses/>.

import sys
import tkinter as tk
from copy import deepcopy
from configparser import ConfigParser
from os.path import abspath, join, dirname, isfile
from tkinter import ttk


class MainWindow(tk.Frame):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.winfo_toplevel().title('ArduRGB LED Configuration')
        self.base_path = abspath(dirname(__file__))
        self.config_path = abspath(
            join(self.base_path, '..', '..', 'user_config.ini'))
        self.config = ConfigParser()
        self.init_data = None
        self.getLeds()
        self.draw()

    def draw(self, drawn=[]):
        # Store page elements
        self.elements = []
        if not drawn:
            # Initialise input validator
            self.vcmd = self.register(self.validate)
            drawn.append(1)
        # Add all physical LED strips
        for i, p_strip in enumerate(self.led_data):
            self.parent = None
            self.this = i
            # Section header
            self.createSeparator(size='LARGE')
            self.elements.append(
                [0, tk.Label(self, text=f'LED Strip {i}')])
            self.elements.append(
                [1, tk.Button(self, text='Remove Physical Strip',
                              command=lambda i=i: self.delPhysicalStrip(i))])
            self.createSeparator(size='LARGE')
            # Section body
            self.createEntry(text='Pin: ', validatecommand=(
                self.vcmd, '%P'), insert=p_strip['pin'])
            self.createEntry(text='Type: ', insert=p_strip['type'])
            self.createEntry(text='Order: ', insert=p_strip['order'])
            self.createEntry(text='Correction: ', insert=p_strip['correction'])
            self.createEntry(text='LEDs: ', validatecommand=(
                self.vcmd, '%P'), insert=p_strip['leds'])
            # Add all virtual LED strips for each physical LED strip
            for j, v_strip in enumerate(p_strip['strips']):
                self.parent = i
                self.this = j
                # Section header
                self.createSeparator(size='SMALL')
                self.elements.append(
                    [1, tk.Button(self, text='Remove Virtual Strip',
                                  command=lambda i=i, j=j: self.delVirtualStrip(i, j))])
                self.createEntry(text='LEDs: ', validatecommand=(
                    self.vcmd, '%P'), insert=v_strip['leds'])
                self.createEntry(text='Delay: ', validatecommand=(
                    self.vcmd, '%P'), insert=v_strip['delay'])
                self.createEntry(text='Effect: ', insert=v_strip['effect'])
                self.createCheckbutton(
                    text='Reversed: ', insert=v_strip['reversed'])
            # Section footer
            self.createSeparator(size='SMALL')
            self.elements.append(
                [1, tk.Button(self, text='New Virtual Strip',
                              command=lambda i=i: self.newVirtualStrip(i))])
            self.createSeparator(size='LARGE')
        # Sidebar
        self.elements.append(
            [1, tk.Button(self, text='New Physical Strip',
                          command=self.newPhysicalStrip)])
        self.elements.append(
            [1, tk.Button(self, text='Undo All Changes ',
                          command=self.undoChanges)])
        self.elements.append(
            [1, tk.Button(self, text='    Save Changes    ',
                          command=self.setLeds)])
        # Display all elements
        self.current_row = 0
        self.dividers = [3, 0]
        self.columns = 0
        self.columns_width = 2
        for element in self.elements:
            element[1].grid(row=self.current_row, column=element[0] +
                            (self.columns * self.columns_width))
            if element[0] != 0:
                self.current_row += 1
            # Track each physical strip section
            try:
                if element[2]:
                    self.dividers[1] += 1
            except:
                pass
            # Start drawing on next 'column' section
            if self.dividers[1] == self.dividers[0]:
                self.columns += 1
                self.current_row = 0
                self.dividers[1] = 0

    def redraw(self):
        # Clear existing ui elements
        for element in self.elements:
            element[1].grid_forget()
        # Draw ui elements again
        self.draw()

    def newPhysicalStrip(self):
        self.led_data.append(
            {'pin': 0,
             'type': 'WS2812',
             'order': 'RGB',
             'correction': '',
             'leds': 0,
             'strips': []})
        self.redraw()

    def delPhysicalStrip(self, this):
        self.led_data.pop(this)
        self.redraw()

    def newVirtualStrip(self, parent):
        self.led_data[parent]['strips'].append(
            {'leds': 0,
             'delay': 0,
             'effect': '',
             'reversed': 'false'})
        self.redraw()

    def delVirtualStrip(self, parent, this):
        self.led_data[parent]['strips'].pop(this)
        self.redraw()

    def undoChanges(self):
        self.led_data = deepcopy(self.init_data)
        self.redraw()

    def createEntry(self, text, validatecommand=None, insert=None):
        # Add label on left-hand side
        self.elements.append(
            [0, tk.Label(self, text=text)])
        # Create entry widget
        self.entry = tk.Entry(
            self, validate='all', validatecommand=validatecommand)
        # Update led data on user input
        self.entry.bind('<KeyRelease>', lambda event, parent=self.parent, this=self.this,
                        option=text, entry=self.entry: self.releaseEvent(parent, this, option, entry))
        # Add existing value to entry
        if not insert is None:
            self.entry.insert(0, insert)
        # Add label on right-hand side
        self.elements.append([1, self.entry])

    def createCheckbutton(self, text, insert=None):
        # Add label on left-hand side
        self.elements.append(
            [0, tk.Label(self, text=text)])
        # Create checkbutton widget
        self.checkbutton = ttk.Checkbutton(self)
        # Update led data on user input
        self.checkbutton.configure(command=lambda parent=self.parent, this=self.this, option=text,
                                   button=self.checkbutton: self.releaseEvent(parent, this, option, button))
        # Remove 'alternate' state from button on init
        self.checkbutton.state(['!alternate'])
        # Set button as checked
        if insert == 'true':
            self.checkbutton.state(['selected'])
        # Set button as un-checked
        else:
            self.checkbutton.state(['!selected'])
        # Add button on right-hand side
        self.elements.append([1, self.checkbutton])

    def createSeparator(self, size=None):
        if size == 'LARGE':
            # Left-hand side
            self.elements.append(
                [0, tk.Label(self, text='<<<<<<<<<<<<<<<<<<<<')])
            # Right-hand side
            self.elements.append(
                [1, tk.Label(self, text='>>>>>>>>>>>>>>>>>>>>'), True])
        elif size == 'SMALL':
            # Left-hand side
            self.elements.append(
                [0, tk.Label(self, text='--------------------')])

    def validate(self, P):
        # Only allow integer input
        return str.isdigit(P) or not P

    def releaseEvent(self, parent, this, option, widget):
        try:
            # Entry widget
            self.data = widget.get()
        except:
            # Checkbutton widget
            self.data = 'true' if 'selected' in widget.state() else 'false'
        # Clear text formatting
        self.option = option.lower().split(':')[0]
        if self.data:
            # Convert input to int if possible
            try:
                self.data = int(self.data)
            except:
                pass
        # Set option of physical strip
        if parent is None:
            self.led_data[this][self.option] = self.data
        # Set option of virtual strip
        else:
            self.led_data[parent]['strips'][this][self.option] = self.data

    def getLeds(self):
        if isfile(self.config_path):
            self.config.read(self.config_path)
        else:
            sys.exit(
                f'Could not locate user configuration file {self.config_path}')
        self.led_config = self.config['leds']
        # Get LED strips
        self.physical_strips = 0
        self.virtual_strips = 0
        for key in self.led_config:
            if 'physical_strip_' in key:
                self.physical_strips += 1
            elif 'led_strip_' in key:
                self.virtual_strips += 1
        # Get virtual strip parents
        self.v_strip_parents = []
        for i in range(self.virtual_strips):
            self.v_strip_parents.append(int(self.led_config[f'led_strip_{i}']))
        self.led_data = []
        # Position in arrays corresponds to strip identifiers
        for i in range(self.physical_strips):
            self.strip_data = {
                'pin': self.led_config[f'physical_pin_{i}'],
                'type': self.led_config[f'physical_type_{i}'].upper(),
                'order': self.led_config[f'physical_order_{i}'].upper(),
                'correction': self.led_config[f'physical_correction_{i}'],
                'leds': self.led_config[f'physical_leds_{i}'],
                'strips': []}
            for j, parent in enumerate(self.v_strip_parents):
                if parent == i:
                    self.v_strip_data = {
                        'leds': self.led_config[f'led_num_{j}'],
                        'delay': self.led_config[f'led_delay_{j}'],
                        'effect': self.led_config[f'led_effect_{j}'],
                        'reversed': self.led_config[f'led_reversed_{j}'].lower()}
                    self.strip_data['strips'].append(self.v_strip_data)
            self.led_data.append(self.strip_data)
        # Save initial state for 'revert changes' button
        if self.init_data is None:
            self.init_data = deepcopy(self.led_data)

    def setLeds(self):
        # Get current config file data
        with open(self.config_path, 'r') as file:
            self.config_data = file.readlines()
        # Set new config file data
        with open(self.config_path, 'w') as file:
            in_leds = None
            for line in self.config_data:
                if '[leds]' in line and in_leds is None:
                    file.write(f'{line}'
                               f'; Physical LED strips\n; How to define strips: \n')
                    # Track number of virtual strips
                    j = 0
                    # Iterate through all physical strips
                    for i, p_strip in enumerate(self.led_data):
                        if i:
                            file.write(';\n')
                        file.write(f'physical_strip_{i} = {i}\n')
                        for key, value in p_strip.items():
                            # Physical strip definition
                            if not isinstance(value, list):
                                file.write(f'physical_{key}_{i} = {value}\n')
                            # Virtual strip definition
                            else:
                                # Iterate through virtual strips in physical strip
                                for v_strip in value:
                                    file.write(f';\nled_strip_{j} = {i}\n')
                                    for key_, value_ in v_strip.items():
                                        # Translate ui name to config name
                                        if key_ == 'leds':
                                            key_ = 'num'
                                        file.write(
                                            f'led_{key_}_{j} = {value_}\n')
                                    j += 1
                    in_leds = True
                elif '[build]' in line and in_leds:
                    file.write(f'\n{line}')
                    in_leds = False
                elif not in_leds:
                    file.write(line)


if __name__ == '__main__':
    root = tk.Tk()
    MainWindow(root).grid(column=0, row=0)
    root.mainloop()
