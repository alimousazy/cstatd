require("socket")
f = io.open("current", "w")
function process(key, value) 
	f:write(key .. "\n")
	f:flush()
end
return process
