#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

class ArgsParser {
public:
    std::map<std::string, std::string> identifiers; // "identifiers": holds names of every flag/parameter/argument/option | <flag, name>
    std::map<std::string, std::string> types; // "types": holds types of arguments name | <name, type>
    std::map<std::string, std::string> data; // "data": is the final output of the parsing operation will be/has been deduced from "parse()" function | <name, value>
    std::vector<std::string> odd_args; // "odd_args": stores non-registered (considered as "Odd") arguments, which are inserted by the "parse()" function

    void add_option(std::string name, std::string type, std::string defval="", std::string primary_flag="", std::string secondary_flag="") {
	// add_option: registers an option/argument to the context.
	// - Assigns the name of the flags (primary, secondary) to "identifiers",
	// - Assigns the type of every flag which is represented by its name.
	// - Assigns default value according to "defval" if it's present.
        if (primary_flag != "") identifiers.insert(std::make_pair(primary_flag, name));
        if (secondary_flag != "") identifiers.insert(std::make_pair(secondary_flag, name));
        types[name] = type;

        if (type == "bool" && defval != "0" && defval != "1") defval = "0"; // overrides default value for a boolean if defval is not has been given.

        if (defval != "") data[name] = defval;
    }
    std::map<std::string, std::string> parse(int argc, char ** argv) {
	// parse: parses "argv", and stores the value of an argument in "data",
	// which has Two Argument Types (string, bool)
	bool isvalue = false;
        for (unsigned int index=0; index<argc; index++) {
            if (identifiers.count(argv[index])) {
		isvalue = true;
                if ((types[identifiers[argv[index]]]) == "string")
                    data[identifiers[argv[index]]] = argv[index+1];
                if ((types[identifiers[argv[index]]]) == "bool")
			data[identifiers[argv[index]]] = "1";
            }
            else {
		if (isvalue != true) {
	                odd_args.emplace_back(argv[index]);
		}
		else isvalue = false;
            }
        }

        return data;
    }
    inline std::string get(std::string id) {
        // get: returns the value of a flag or its name (identifier).
        if (identifiers.count(id)) return get(identifiers[id]);
        else {
            return data[id];
        }
    }
};
