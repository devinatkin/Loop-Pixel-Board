

% Generate an Image
clear()
image = zeros([64,64]);
minVals=1024;
maxVals=0;
truesize([300 300]);
delete(gcp('nocreate'))
s = serialport("COM8",115200);

p= parpool("threads");
% flush(s)

while true
    tic
    data = readline(s);
    flush(s)
    parfeval(@processImage,1,data);
    toc
    
end

function image = processImage(data)
    image = zeros([64,64]);
    if strlength(data) > 10
        dt = split(data,":");
        if dt(1) == "i"
            
            pixelStrings = split(dt(2),",");
            pixels = str2double(pixelStrings(1:4096));
            image = reshape(pixels,64,64);
        end
    end
   return
end
