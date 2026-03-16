Logger = Logger or {}

function Logger.Info(message) 
	print(" > " .. message)
end

function Logger.Success(message)
	print(" + " .. message)
end

function Logger.Warning(message)
	print(" * " .. message)
end