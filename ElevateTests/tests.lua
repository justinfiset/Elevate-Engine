vendorPath = path.getabsolute(directory.."/../Vendor")
catchInclude = path.getabsolute(vendorPath.."/Catch2/src/")
filter {}
    includedirs
    {
        vendorPath.."/_config_headers/", 
        catchInclude 
    }
Logger.Info("Added Catch2 include directory: " .. catchInclude)