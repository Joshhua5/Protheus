
function Initialize()  
	-- the first font loaded is set as the active font
	-- if no font is loaded but text is set to be rendered,
	-- it'll be rendered when the first font is loaded.
	Text:loadFont("default", "PWCurvyregularscript.ttf")
	-- loads the font
	Text:loadFont("special", "PWCurvesAndDots.ttf")
	
		-- push text to be rendererd

end

function Update()   	 
	-- push text to be rendererd
	Text:pushText("Hello", Vector2(30, 30))
	-- set the font that's rendering the text
	Text:setActive("special")
	-- the scale is multiplied by 72 which is the default font size.
	Text:pushText("Special", Vector2(200, 200), 0.5)
	-- set font back to original
	Text:setActive("default")
end


function Render() 

end