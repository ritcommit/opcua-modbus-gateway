import json
import tkinter as tk
from tkinter import ttk, messagebox

class ConfigUI:
    def __init__(self, root):
        self.root = root
        self.root.title("@ritcommit's Modbus to OPC UA Config Generator")

        self.data_entries = []

        self.build_modbus_frame()
        self.build_opcua_frame()
        self.build_poll_frame()
        self.build_data_frame()
        self.build_buttons()

    def build_modbus_frame(self):
        frame = ttk.LabelFrame(self.root, text="Modbus Config")
        frame.pack(fill="x", padx=10, pady=5)

        self.mb_type = tk.StringVar(value="rtu")
        self.mb_address = tk.StringVar(value="/dev/ttyUSB0")
        self.mb_baud = tk.IntVar(value=9600)
        self.mb_parity = tk.StringVar(value="none")
        self.mb_port = tk.IntVar(value=502)
        self.mb_slave = tk.IntVar(value=1)

        fields = [
            ("Type", self.mb_type),
            ("Address", self.mb_address),
            ("Baudrate", self.mb_baud),
            ("Parity", self.mb_parity),
            ("Port", self.mb_port),
            ("Slave ID", self.mb_slave),
        ]

        # Row 0: Type (dropdown)
        ttk.Label(frame, text="Type").grid(row=0, column=0, sticky="w")
        self.mb_type_cb = ttk.Combobox(frame, textvariable=self.mb_type, state="readonly",
                                       values=["rtu", "tcp"]) 
        self.mb_type_cb.grid(row=0, column=1, sticky="ew")

        # Row 1: Address (entry)
        ttk.Label(frame, text="Address").grid(row=1, column=0, sticky="w")
        ttk.Entry(frame, textvariable=self.mb_address).grid(row=1, column=1, sticky="ew")

        # Row 2: Baudrate (dropdown)
        ttk.Label(frame, text="Baudrate").grid(row=2, column=0, sticky="w")
        self.mb_baud_cb = ttk.Combobox(frame, textvariable=self.mb_baud, state="readonly",
                                       values=[9600, 19200, 38400, 57600, 115200])
        self.mb_baud_cb.grid(row=2, column=1, sticky="ew")

        # Row 3: Parity (dropdown)
        ttk.Label(frame, text="Parity").grid(row=3, column=0, sticky="w")
        self.mb_parity_cb = ttk.Combobox(frame, textvariable=self.mb_parity, state="readonly",
                                         values=["none", "even", "odd"])
        self.mb_parity_cb.grid(row=3, column=1, sticky="ew")

        # Row 4: Port (entry)
        ttk.Label(frame, text="Port").grid(row=4, column=0, sticky="w")
        ttk.Entry(frame, textvariable=self.mb_port).grid(row=4, column=1, sticky="ew")
        

        # Row 5: Slave ID (entry)
        ttk.Label(frame, text="Slave ID").grid(row=5, column=0, sticky="w")
        ttk.Entry(frame, textvariable=self.mb_slave).grid(row=5, column=1, sticky="ew")

    def build_opcua_frame(self):
        frame = ttk.LabelFrame(self.root, text="OPC UA Config")
        frame.pack(fill="x", padx=10, pady=5)

        self.ua_port = tk.IntVar(value=4841)
        self.ua_sec = tk.StringVar(value="none")

        ttk.Label(frame, text="Port").grid(row=0, column=0)
        ttk.Entry(frame, textvariable=self.ua_port).grid(row=0, column=1)

        ttk.Label(frame, text="Security Policy").grid(row=1, column=0)
        ttk.Entry(frame, textvariable=self.ua_sec).grid(row=1, column=1)

    def build_poll_frame(self):
        frame = ttk.Frame(self.root)
        frame.pack(fill="x", padx=10, pady=5)

        self.poll_interval = tk.IntVar(value=1000)

        ttk.Label(frame, text="Poll Interval (ms)").grid(row=0, column=0)
        ttk.Entry(frame, textvariable=self.poll_interval).grid(row=0, column=1)

    def build_data_frame(self):
        frame = ttk.LabelFrame(self.root, text="Data Mapping")
        frame.pack(fill="both", expand=True, padx=10, pady=5)

        self.data_frame = frame

        header = ttk.Frame(self.data_frame)
        header.pack(fill="x", pady=2)

        headers = ["MB Register", "MB Length", "UA NodeId", "UA Description", "UA DataType"]
        for i, text in enumerate(headers):
            ttk.Label(header, text=text, width=15).grid(row=0, column=i)

        self.add_data_row()

    def add_data_row(self):
        row_frame = ttk.Frame(self.data_frame)
        row_frame.pack(fill="x", pady=2)

        mb_reg = tk.IntVar()
        mb_len = tk.IntVar(value=2)
        ua_node = tk.StringVar()
        ua_desc = tk.StringVar()
        ua_dtype = tk.StringVar(value="float")

        vars_tuple = (mb_reg, mb_len, ua_node, ua_desc, ua_dtype)
        self.data_entries.append(vars_tuple)

        for i, var in enumerate(vars_tuple):
            ttk.Entry(row_frame, textvariable=var, width=15).grid(row=0, column=i)

        # UA DataType (dropdown)
        ua_dtype_cb = ttk.Combobox(row_frame, textvariable=ua_dtype, state="readonly",
                                   values=["int16", "int32", "int", "float", "bool"], width=13)
        ua_dtype_cb.grid(row=0, column=4)

    def build_buttons(self):
        frame = ttk.Frame(self.root)
        frame.pack(fill="x", padx=10, pady=10)

        ttk.Button(frame, text="Add Row", command=self.add_data_row).pack(side="left")
        ttk.Button(frame, text="Preview JSON", command=self.generate_json).pack(side="left", padx=5)
        ttk.Button(frame, text="Save", command=self.save_file).pack(side="right")

    def generate_config(self):
        config = {
            "modbus": {
                "type": self.mb_type.get(),
                "address": self.mb_address.get(),
                "baudrate": self.mb_baud.get(),
                "parity": self.mb_parity.get(),
                "port": self.mb_port.get(),
                "slave_id": self.mb_slave.get()
            },
            "opcua": {
                "port": self.ua_port.get(),
                "security_policy": self.ua_sec.get()
            },
            "poll_interval_ms": self.poll_interval.get(),
            "data": []
        }

        for entry in self.data_entries:
            config["data"].append({
                "mb_register": entry[0].get(),
                "mb_dlen": entry[1].get(),
                "ua_nodeid": "ns=1;s="+entry[2].get(),
                "ua_description": entry[3].get(),
                "ua_dtype": entry[4].get()
            })

        return config

    def generate_json(self):
        config = self.generate_config()
        messagebox.showinfo("Preview JSON", json.dumps(config, indent=4))

    def save_file(self):
        import os

        config = self.generate_config()

        # Create config directory if it does not exist
        config_dir = os.path.join(os.getcwd(), "config")
        os.makedirs(config_dir, exist_ok=True)

        file_path = os.path.join(config_dir, "config.json")

        try:
            with open(file_path, "w") as f:
                json.dump(config, f, indent=4)
            messagebox.showinfo("Saved", f"Configuration saved to {file_path}")
        except Exception as e:
            messagebox.showerror("Error", str(e))


if __name__ == "__main__":
    root = tk.Tk()
    app = ConfigUI(root)
    root.mainloop()
