Window = 0

function buttonCallback()
	Window:hide()
end

function Initialize()
	-- GUIContext is a global context for Windows's it's created when Protheus starts  
	-- The position scales with the window with 0 being the far left/top to 1000 being the far right/bottom
	-- The name is used to access the elements being created
	-- addButton also returns the button created, but we don't use it.
	-- Vector4(positionX, positionY, Width, Height
	Window = GUIContext:createWindow("Window1")
	Window:addButton(Vector4(20, 20, 300, 100), "Button1")
	button = Window:get("Button1")
	-- Adds text to the button
	button:setText("Hide")
	-- if the button is clicked the callback function is executed
	button:setCallback("buttonCallback")
	-- the names must be unique
	Window:addButton(Vector(40, 30), "Button2")
	-- shows that the button doesn't have to be stored to call a function on it
	Window:get("Button2"):setCallback("buttonCallback")
	-- allows the windows to be rendered
	Window:show() 
end

function Update()
	
end
 
function Render() 

end