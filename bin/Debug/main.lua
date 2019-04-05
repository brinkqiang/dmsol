local pbload = require "scripts.pbcodec"
print("init lua engine, load proto files ")
local ret  = pbload:load_file("starve.proto") 
if (ret)  then 
    print("load proto file success") 
else 
    error("load proto file failed"); 
end 

dlog("init lua script"); 

---------------timer opt----------------------------
timer_handlers = {} 
function add_timer(handler, time) 
    local tid = add_user_timer(time)
    timer_handlers[tid] = handler
    return tid;
end 
function kill_timer(id)
    timer_handlers[id] = nil;
end

function handle_timeout(id)
    local fn = timer_handlers[id]
    if fn ~= nil  then 
        fn();
    end 
end 
--------------------------------------------

-- init here.
function init() 
end 

--add logic path.
package.path = package.path .. ";../starve-lua/?.lua";

-- init lua logic.
require("init")

-- local gandler.
local g_game_handler = g_proto_handler;

local function dispatch_message(cid, msgName, real_data)
    g_game_handler:Deal(cid, msgName, real_data);
end


function process_user_message(cid, msg, data, len)
    if msg and msg == 3 then
        print(cid .. " terminates");
        local role = g_obj_factory:FindRoleCId(cid);
        if role then
            role:LeaveScene();
        end
    else
        print("process user message from cid", cid, len, #data); 

        local head = pbload:decode("starve.MsgHead", data)
        if head ~= nil  and  head["proto"] ~= nil then 
            print("process user message : ",msg, head["proto"] ); 
        else 
            print("decode " .. data .. "failed")
            return ;
        end 

        local msg_name = head["proto"];
        local msg_data = head["data"];
        if not msg_name or not msg_data then
            print("msg name is nil or msg data is nil");
            return ;
        end

        local message = pbload:decode(msg_name, msg_data);
        dispatch_message(cid, msg_name, message); 
    end
end 

function process_server_message(cid, msg, data, len)
    print("process server message"); 
end 


function process_wsserver_message(cid ,msg ,data, len) 
    print("process websocket   message"); 
end 
