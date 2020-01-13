R"( -- BIOS STARTS HERE \""

-- This is the default computer BIOS used in every Processing Unit created.
-- The BIOS of a State Machine is a small program that is executed on startup. Normally, it defines some critical
-- system functions and executes an operating system main file found in a filesystem. However, you can also program
-- it to do other things.


-- main() is called on machine startup. TODO: remove main and execute all BIOS contents, not just this function
function main()
	print("Default Lua BIOS initializing...")

	local function pull()
		repeat
			coroutine.yield()
		until(latest_event[0] ~= "tick")
		return table.unpack(latest_event)
	end
	local function pull_delay()
		coroutine.yield()
		return table.unpack(latest_event)
	end
	local delay = coroutine.yield

	local function config()
		print("Lua BIOS config")
		print("Press Enter to apply and reboot")
		repeat
			local e, key = pull()
		while e ~= "key_press" and key ~= 50 -- todo: replace 50 by actual enter key id
		computer.reboot()
	end

	print("Press any key to config bios")
	secs_before = computer.secs()
	while computer.secs() - secs_before < 5 do
		local e = pull_delay()
		if e == "key_press" do
			config()
		end
	end
	print("Searching for filesystems...")
	delay()

	for id, conn in pairs(computer.connections) do
		-- check for filesystems...
	end
end

-- BIOS ENDS HERE
)"