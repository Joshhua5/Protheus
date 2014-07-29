 
isServer = true
Server = 0
Client = 0

function Initialize() 
	if isServer then
		Server = Network:startServer("connection", 9910, 32)
	else
		Client = Network:connectToServer("192.168.1.111", "ClientConnection", 9910)
	end

end

function Update()  
	print("client 1")
	if isServer then
		if Server:peek() ~= 0 then
			connection = Server:recv()
			if connection:peek()  ~= 0 then
				buffer = connection:recv()
				reader = BufferReader(buffer)
				readString = reader:readString()
				print(readString) 
			end
		end 
	else 
		print("client peek")
		if Client:peek() ~= 0 then
			print("client recv")
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
	print("client isConnected")
	if Client:isConnected() == true then
		print("1")
		buffer = Buffer(5)
		print("2")
		writer = BufferWriter(buffer)
		print("3")
		writer:writeString("Hello")
		print("4") 
		print("client send")
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

end