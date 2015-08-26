require("socket")
f = io.open("current", "w")
p = {}
function process(key, value) 
	f:write(key .. "\n")
	f:flush()
end
return process
