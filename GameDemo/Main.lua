
 
camera = camera_create("camera")
camera:setPosition(0, 0)
camera:setDimensions(18, 18)

avatar = avatar_create("hero1") 
avatar:setPosition(12, 10) 
avatarPos = avatar:getPosition() 

while true do
	if avatarPos[0] > 500 then
		avatarPos[0] = avatarPos[0] + 1;
	else
		avatarPos[0] = avatarPos[0] - 1;
	end
	avatar:setPosition(avatarPos[0], avatarPos[1]);
	returnToEngine();
end