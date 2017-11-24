TEMPLATE = subdirs

SUBDIRS = Morenos_GUI Packet_Manager Simulation_Engine

Morenos_GUI.file = GUI_Elements/Morenos_GUI.pro
Packet_Manager.file = Packet_Manager/Packet_Manager.pro
Simulation_Engine.file = Simulation_Engine/Simulation_Engine.pro

Morenos_GUI.depends = Packet_Manager
Simulation_Engine.depends = Packet_Manager
