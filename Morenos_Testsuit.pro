TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = Packet_Manager Config_Manager Morenos_GUI Simulation_Engine 

Morenos_GUI.file = GUI_Elements/Morenos_GUI.pro
Packet_Manager.file = Packet_Manager/Packet_Manager.pro
Simulation_Engine.file = Simulation_Engine/Simulation_Engine.pro
Config_Manager.file = Config_Manager/Config_Manager.pro

Morenos_GUI.depends = Packet_Manager Config_Manager
Simulation_Engine.depends = Packet_Manager Config_Manager
