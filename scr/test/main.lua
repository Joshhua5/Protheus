
Sprite = 0
Position = 0
Position2 = 0
isServer = true
Server = 0
Client = 0

function Initialize()
	-- Load in a sprite 
	Sprite = SpriteManager:loadSprite("helloWorld", "hello.png")
	SpriteManager:loadSprite("sprite2", "image.png")
	
	-- Set it's position
	Position = Vector2(20, 20)
	Position2 = Vector2(20, 20)

	if isServer then
		Server = Network:startServer("connection", 9910)
	else
		Client = Network:connectToServer("localhost", "ClientConnection", 9910)
	end
end

function Update()  
	if Position:getY() < 30 then  
		Position:move(0, 1)
	else
		Position:setY(0)
	end
	
	-- have Position2 equal the position of the mouse 
	while EventHandler:mouseMotionHasNext() do
		Position2 = EventHandler:mouseMotionNext():getPosition() 
	end  

	while EventHandler:keyHasNext() do
		event = EventHandler:keyNext()
		if event:isDown() and event:getKey() == key["w"] then  
				Position:move(1, 0) 
		end
	end

	-- Under Development
	-- untested

	if isServer then
		if Server:peek() then
			connection = Server:recv()
			if connection:peek() then
				buffer = connection:recv()
				reader = BufferReader(buffer)
				readString = reader:readString()
				print(readString) 
			end
		end 
	else 
		if Client:peek() then
			-- :recv will return a buffer containing the 
			-- information in a package
			buffer = Client:recv()
			-- Creates a reader for the buffer
			reader = BufferReader(buffer)
			readString = reader:readString()
			print(readString) 
		end
	end

	-- We want to send a message to the server once connected
	 
	if Client:isConnected() then
		buffer = Buffer(32)
		writer = BufferWriter(writer)
		writer:writeString("Hello")
		Client:send(buffer) 
	end

	-- Prefered Syntax

	

	--if Server:peek() ~= 0 then
	--	connection = Server:recv()
	--	if connection:peek() ~= 0 then
	--		reader = connection:read()
	--		readString = reader:readString();
	--	end 
	--end
	 
end 


function Render()
	-- render the sprite to screen 
	-- if we don't store the sprite we can use
	Sprite2 = SpriteManager:getSprite(getID("sprite2"))
	-- test timer
	
	SpriteBatcher:push(Sprite2, Position2)
	SpriteBatcher:push(Sprite, Position)
end