
function setupWindow()
	windowPosX = 0
	windowPosY = 0
	windowWidth = 800
	windowHeight = 600
	setWindowPosition(windowPosX, windowPosY)
	setWindowDimensions(windowWidth, windowHeight)
	setWindowTitle("Protheus")
end


function config()
	setupWindow()
	setLuaResources("FilePath to list of resources to load")
	setLuaMain("FilePath to game start")
end