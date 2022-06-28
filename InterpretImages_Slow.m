% Generate an Image
clear()
image = zeros([64,64]);

s = serialport("COM29",115200);
flush(s)

while true
   line = readline(s);
   pixel = split(line,",");
   X = str2double(pixel(1));
   Y = str2double(pixel(2));
   light = str2double(pixel(3));
   
   image(X+1,Y+1) = light;
   imshow(image,InitialMagnification = 900)
   
end