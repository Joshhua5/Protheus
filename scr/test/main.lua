
helloSprite = 0 
helloPosition = Vector2(20, 20);

function Initialize()
	-- Load in a sprite
	helloSprite = Game.getSpriteManager():loadSprite("helloWorld", "hello.png")
	-- Set it's position
end

function Update()
	-- do nothing
end 


function Render()
	-- render the sprite to screen
	local spt = Game.getSpriteManager():getSprite(helloSprite)
	Game.getRenderer():getBatcher():push(spt, helloPosition)
end