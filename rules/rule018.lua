function make_decision()
	output:clear()

	-- EDIT STARTS HERE -------------------------------------------------------

	print("[LUA] rule018 -> verify customer adultness")
	-- IN: customerId
	-- CFG: country -> adult age
	-- OUT: adult: 1->true | 0->false

	local dbdriver = require("lsqlite3complete")
	local db = dbdriver.open("rules/database.db")

	local adultness = {}
	adultness["CU"] = 16 -- Kuba
	adultness["MT"] = 16 -- Malta
	adultness["GB-SCT"] = 16 -- Szkocja
	adultness["KP"] = 17 -- Korea Północna
	adultness["ID"] = 17 -- Indonezja
	adultness["PL"] = 18 -- Polska
	adultness["RU"] = 18 -- Rosja
	adultness["CA"] = 19 -- Kanada
	adultness["KR"] = 19 -- Korea Południowa
	adultness["US-NE"] = 19 -- USA Nebraska
	adultness["US-AL"] = 19 -- USA Alabama
	adultness["JP"] = 20 -- Japonia (do 2022r)
	adultness["MG"] = 21 -- Madagaskar

	local adult = 0 -- default value 0 means false

	for row in db:nrows("SELECT age, country FROM customers " .. " WHERE id = " .. math.floor(customerId) .. " LIMIT 1") do
		-- rule logic:
		if row.age >= adultness[row.country] then
			adult = 1 -- 1 means true
		end

		print(
			"[LUA] customerId: "
				.. math.floor(customerId)
				.. " age: "
				.. row.age
				.. " country: "
				.. row.country
				.. " adultness["
				.. row.country
				.. "]: "
				.. adultness[row.country]
				.. " | adult? => "
				.. tostring(adult)
		)

		break
	end

	-- return result to C
	output:add("adult", tostring(adult), "bool")
end
