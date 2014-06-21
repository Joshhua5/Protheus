avatar = avatar_create("hero1") 
camera = camera_create("camera")
avatarPos = avatar:getPosition()
function Update() 
	if avatarPos[0] > 500 then
		avatarPos[0] = avatarPos[0] + 1;
	else
		avatarPos[0] = avatarPos[0] - 1;
	end
	avatar:setPosition(avatarPos[0], avatarPos[1]); 
end

function Render()
	
end

function Initialize() 
	camera:setPosition(0, 0)
	camera:setDimensions(18, 18)
 
	avatar:setPosition(12, 10) 
	avatarPos = avatar:getPosition() 
end