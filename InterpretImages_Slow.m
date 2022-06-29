% Generate an Image
clear()
image = zeros([64,64]);
image2 = zeros([64,64]);
image3 = zeros([64,64]);

s = serialport("COM29",115200);
flush(s)

while true
   line = readline(s);
   pixel = split(line,",");
   X = str2double(pixel(1));
   Y = str2double(pixel(2));
   light = str2double(pixel(3))/str2double(pixel(4));
   light2 = str2double(pixel(3));
   light3 = str2double(pixel(4));
   image(X+1,Y+1) = light;
   image2(X+1,Y+1) = light2;
   image3(X+1,Y+1) = light3;
   if(X==0)
      if(Y==0)
         maxVal = max(max(image));
         minVal = min(min(image));
         imshow(image,DisplayRange=[minVal maxVal],InitialMagnification = 900)
         
      end
   end
end