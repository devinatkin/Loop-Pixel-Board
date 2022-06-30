% Generate an Image
clear()
image = zeros([64,64]);
image2 = zeros([64,64]);
image3 = zeros([64,64]);

s = serialport("COM29",115200, Timeout = 60);
flush(s)

lastX = 64;
lastY = 64;
avecnt = 0;
while true
   line = readline(s);
   pixel = split(line,",");
   X = str2double(pixel(1))+ 1;
   Y = str2double(pixel(2))+ 1;
   light = str2double(pixel(3));
   
   
   if((X == lastX) && (Y == lastY))
      image(X,Y) = image(X,Y) + light;
      avecnt = avecnt + 1;
      
       lastX = X;
       lastY = Y;
   else
       
       image(lastX,lastY) = image(lastX,lastY)/avecnt;
       disp([lastX lastY image(lastX,lastY)])
       
        if((X == 1) && (Y == 1))
            minVal = min(min(image(1:16,1:16)));
            maxVal = max(max(image(1:16,1:16)));
            imshow(image,DisplayRange=[minVal maxVal],InitialMagnification = 900) 
        end
       
       image(X,Y) = light;
       avecnt = 1;
       
       lastX = X;
       lastY = Y;
   end
   

end