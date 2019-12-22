R"( -- BIOS STARTS HERE "

-- This is the default computer BIOS used in every Processing Unit created.
-- The BIOS of a State Machine is a small program that is executed on startup. Normally, it defines some critical
-- system functions and executes an operating system main file found in a filesystem. However, you can also program
-- it to do other things.


-- main() is called on machine startup. TODO: remove main and execute all BIOS contents, not just this function
function main()
	print("BIOS started")

	local function pull()
		repeat
			coroutine.yield()
		until latest_event[0] ~= "tick"

		return table.unpack(latest_event)
	end

	repeat
		print(pull())
	until false
end

-- BIOS ENDS HERE
)"