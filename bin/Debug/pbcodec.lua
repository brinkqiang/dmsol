package.path = package.path.. ";../?.lua;../../?.lua;../../bin/?.lua"
package.cpath = package.cpath..";./lib/?.so"
print(package.path)
pbcodec = {}

pbcodec.pb = require "pb"
pbcodec.protoc = require "protoc"
pbcodec.ser = require "serpent"
pbcodec.all_messages = {}

--pbcodec.protoc.paths[#pbcodec.protoc.paths + 1] = "../starve-lua/proto/Protocol"
pbcodec.protoc.paths[#pbcodec.protoc.paths + 1] = "../starve-lua/proto"
pbcodec.protoc.include_imports = true
pbcodec.pb.option("enum_as_value")

--[[
    pbcodec:load_file(file)        -- 加载proto
    pbcodec:load(str)              -- 加载文本消息
    pbcodec:find_message(message)  -- 是否有该消息定义
    pbcodec:encode(message, data)  -- 根据消息定义，序列化table
    pbcodec:decode(message, bytes) -- 根据消息定义，反序列话bytes
    head:pack({ length = 0 , type_flag = 0, control_flag = 0, magic_flag   = 0, reflect_flag = 0, reserve_flag = 0, extend_flag  = 0 }) --根据包头结构生成GMsgHead
    head:unpack(headStr)           --根据包头str,返回包头的table结构
--]]

function pbcodec:toHex(bytes)
    print(self.pb.tohex(bytes))
end

function pbcodec:showTable(t)
    print(self.ser.block(t))
end

function pbcodec:load_file(filename)

    local func1 = function() self.protoc:loadfile(filename) end
    if not pcall(func1)
    then
        print("load file failed")
        return false
    end
    print("load file ok")
    self:init_message()
    return true
end

function pbcodec:init_message()
    for message in self.pb.types() do
        self.all_messages[message] = message
        print("protoname=" .. message)
    end

    for name, basename, type in self.pb.types() do
        print(name, basename, type)
    end
end

function pbcodec:find_message(message)
    return self.pb.type("." .. message)
end

function pbcodec:encode(message, data)
    if self:find_message(message) == nil then
        print("encode err=" .. message)
        return ""
    end
    -- encode lua table data into binary format in lua string and return
    local bytes = assert(self.pb.encode(message, data))
    -- print(self.pb.tohex(bytes))

    -- and decode the binary data back into lua table
    -- local data2 = assert(self.pb.decode(message, bytes))
    -- print(self.ser.block(data2))

    return bytes
end

function pbcodec:decode(message, bytes)
    if self:find_message(message) == nil then
        print("decode err=" .. message)
        return {}
    end
    -- and decode the binary data back into lua table
    local data2 = assert(self.pb.decode(message, bytes))
    --print(require "serpent".block(data2))

    return data2
end

return pbcodec


