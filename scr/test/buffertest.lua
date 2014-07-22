
buffer = 0

function Initialize() 
	buffer = Buffer(32)
	writer = BufferWriter(buffer)
	writer:writeString("Hello") 
end

function Update() 
	reader = BufferReader(buffer)
	print(reader:readString()) 
end 


function Render() 

end