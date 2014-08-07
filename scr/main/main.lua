grid = {}
cursorPosition = Vector2(3, 3)
cursorType = 1
designState = 0
playState = 0 

function designUpdate()
	while EventHandler:keyHasNext() do
		local event = EventHandler:keyNext()
		if event:isDown() and event:getKey() == key["w"] then  
			cursorPosition:move(0, -1) 
		elseif event:isDown() and event:getKey() == key["a"] then  
			cursorPosition:move(-1, 0) 
		elseif  event:isDown() and event:getKey() == key["s"] then  
			cursorPosition:move(0, 1) 
		elseif event:isDown() and event:getKey() == key["d"] then  
			cursorPosition:move(1, 0) 
		elseif event:isDown() and event:getKey() == key["TAB"] then
			Stack:push(playState, true)
			isDesignTheState = false
		end
	end
end

function designRender()
	if cursorType == 1 then
		local Sprite = SpriteManager:getSprite(getID("WhiteBlock"))
		SpriteBatcher:push(Sprite, Vector2(cursorPosition:getX() * 32, cursorPosition:getY() * 32))
	elseif cursorType == 2 then 
		local Sprite = SpriteManager:getSprite(getID("BlueBlock"))
		SpriteBatcher:push(Sprite, Vector2(cursorPosition:getX() * 32, cursorPosition:getY() * 32))
	elseif cursorType == 3 then 
		local Sprite = SpriteManager:getSprite(getID("EndBlock"))
		SpriteBatcher:push(Sprite, Vector2(cursorPosition:getX() * 32, cursorPosition:getY() * 32)) 
	end
end

function playUpdate()
	if event:isDown() and event:getKey() == key["TAB"] then
		Stack:pop() 
	end
end

function playRender()

end

function Initialize()
	for x = 0, 24 do
		grid[x] = {}
		for y = 0, 16 do 
			if(x == 0 or x == 24) then
				grid[x][y] = 1
			elseif(y == 0 or y == 16) then 
				grid[x][y] = 1
			else
				grid[x][y] = 0
			end
		end
	end 
	
	SpriteManager:loadSprite("WhiteBlock", "WhiteBlock.png")
	SpriteManager:loadSprite("BlueBlock", "BlueBlock.png")
	SpriteManager:loadSprite("EndBlock", "EndBlock.png")
	SpriteManager:loadSprite("StartBlock", "StartBlock.png")
	SpriteManager:loadSprite("MrBlockSingle", "MrBlockSingle.png")
	
	designState = GameState()
	designState:setRender("designRender")
	designState:setUpdate("designUpdate")
	designState:setReturn("")
	Stack:push(designState)
	
	
	playState = GameState()
	playState:setRender("playRender")
	playState:setUpdate("playUpdate")
	playState:setReturn("")
end

function Render()
	for x = 0, 24 do 
		for y = 0, 16 do  
			if grid[x][y] == 1 then
				local Sprite = SpriteManager:getSprite(getID("WhiteBlock"))
				SpriteBatcher:push(Sprite, Vector2(x * 32, y * 32))
			elseif grid[x][y] == 2 then 
				local Sprite = SpriteManager:getSprite(getID("BlueBlock"))
				SpriteBatcher:push(Sprite, Vector2(x * 32, y * 32))
			elseif grid[x][y] == 3 then 
				local Sprite = SpriteManager:getSprite(getID("EndBlock"))
				SpriteBatcher:push(Sprite, Vector2(x * 32, y * 32))
			end
		end
	end 
end

function Update() 

end