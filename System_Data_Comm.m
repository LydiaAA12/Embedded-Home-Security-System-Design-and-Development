clc;
clear;              % Clear current variables in Matlab workspace
portstream = serialport('/dev/cu.usbmodemM41110161',9600,'Parity','none');
writeAPIKey = 'UVBZGO4VJKZJRNCP';
readAPIKey = '34NLOQGV4C0XO2U1';
channelId= 1597796;

while(true)
 pause(1)
 write(portstream, 1:26,"string");
 res = read(portstream, 26, "string");
 res_array = strsplit(res);
 disp(res_array(1:3));
 timestamp = datetime('now');
 ambience_value(res_array(3), channelId, writeAPIKey, timestamp);
 if(res_array(1)=='Motion' || res_array(2)=='Motion' || res_array(3)=='Motion')
    motion_detected_signal(channelId, writeAPIKey, timestamp);
 end

 flush(portstream,"input");
end


function motion_detected_signal(channelId, writeAPIKey, tStamp)
    pause(1);
   thingSpeakWrite(channelId,[1],'Fields', [2],'TimeStamp',[tStamp+seconds(1)], 'WriteKey',writeAPIKey);
  
end

function ambience_value(val, channelId, writeAPIKey, tStamp)
    ambience_val = str2double(val);
    thingSpeakWrite(channelId, ambience_val,'TimeStamp',[tStamp], 'WriteKey',writeAPIKey);
end



