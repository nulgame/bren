-- rule001.lua
-- This rule allows you to start a system backup only on
-- working days between 0 and 4 o'clock.

function make_decision()
	output:clear()

	-- EDITING STARTS HERE -------------------------------------------
	--
	print("LUA: rule001")

	-- get system date and time
	local wday = tonumber(os.date("%w"))
	local hour = tonumber(os.date("%H"))
	local start_backup = 0 -- 0: do not start (default), 1: start

	-- RULE LOGIC --

	-- if not force flag presents in message
	if force == nil then
		force = 0
	end

	if force ~= 0 then
		start_backup = 1
	else
		if wday == 0 or wday == 6 then -- sunday or saturday
			start_backup = 0 -- do not start backup
		else
			if hour >= 0 and hour < 4 then
				start_backup = 1
			else
				start_backup = 0
			end
		end
	end

	-- RETURN RESULT
	output:add("start", tostring(start_backup), "bool")

	-- EDIT FINISHED HERE -----------------------------------------------------
end
