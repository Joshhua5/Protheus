
 
camera = camera_create("camera")
camera:setPosition(0, 0)
camera:setDimensions(18, 18)

avatar = avatar_create("hero1") 

avatarPosition = avatar:getPosition()
avatarPosition[1] = 300;

while not isCloseRequested(){
	if(avatarPosition[0] > 500
		avatarPosition[0]++;
	else
		acatarPosition[0]--;
	avatar:setPosition(avatarPosition[0], avatarPosition[1]);
	returnToEngine();
}




while(sscanFile.hasNextLine())
  {  
    scanFile.nextLine();
    wordNum++;
    System.out.println(wordNum);
    ver = 1; 
  }