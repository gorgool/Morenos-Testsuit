TEMPLATE = subdirs

SUBDIRS = Application Composer Data_Models PlotPosition PortraitPlane TargetTable

Application.file = Application.pro
Composer.file = ComposerWidget/Composer.pro
Data_Models.file = Data_Models/Data_Models.pro
PlotPosition.file = PlotPositionWidget/PlotPosition.pro
PortraitPlane.file = PortraitPlaneWidget/PortraitPlane.pro
TargetTable.file = TargetTableWidget/TargetTable.pro

Application.depends = Composer Data_Models PlotPosition PortraitPlane TargetTable
