R"( -- BIOS STARTS HERE \""

-- This is the default computer BIOS used in every Processing Unit created.
-- The BIOS of a State Machine is a small program that is executed on startup. Normally, it defines some critical
-- system functions and executes an operating system main file found in a filesystem. However, you can also program
-- it to do other things.


-- main() is called on machine startup. TODO: remove main and execute all BIOS contents, not just this function
function main()
	local function pull()
		print("this is called from inside computer.pull(), before")
		coroutine.yield()
		print("this is called from inside computer.pull(), after")
		return table.unpack(latest_event)
	end

	print("this is from the bios! now i'll wait for a signal")
	event_name = pull()
	print("wow, an event came by! The event's name was: " .. event_name)
end

-- BIOS ENDS HERE
)"