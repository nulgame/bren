function make_decision()
	output:clear()

	print("LUA: rule300")

	local level = 3 -- the highest level of acceptance required

	if paymentAmount > 10000 or fraudScore > 0.5 then
		level = 3
	elseif paymentAmount > 5000 or fraudScore > 0.3 then
		level = 2
	else
		level = 1
	end

	-- output:add("level", tostring(level), "string")
	output:add("level", tostring(level), "number")
end
