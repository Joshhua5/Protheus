
 
camera = camera_create("camera2")
camera:setPosition(0, 0)
camera:setDimensions(18, 18)

avatar = avatar_create("hero1") 

id = avatar:getID()
engine:update()