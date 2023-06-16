/* a simple escape sequence generation library
 *
 * ShellColor and all of its components are Licensed under "GNU General Public License Version 3".
 *
 */

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

namespace Color {
    /*
     * Color: Stores (Colors, Styles) Escape Codes
     * Color::Fore: Stores Foreground Escape Codes
     * Color::Back: Stores Background Escape Codes
     * Color::Style: Stores Style Escape Codes
     *
     * Color::PALETTE: "Template" of Escape Sequence Generating.
     * Color::RESET_ALL: A Color Code which resets all Properties (Colors, Styles ...)
     * Color::Map: A Map which links Fore, Back, Style Namespaces into strings.
     */
    namespace Fore {
        const char BLACK            = 30;
        const char BLUE             = 34;
        const char CYAN             = 36;
        const char GREEN            = 32;
        const char MAGENTA          = 35;
        const char RED              = 31;
        const char WHITE            = 37;
        const char YELLOW           = 33;
        const char LIGHTBLACK       = 90;
        const char LIGHTBLUE        = 94;
        const char LIGHTCYAN        = 96;
        const char LIGHTGREEN       = 92;
        const char LIGHTMAGENTA     = 95;
        const char LIGHTRED         = 91;
        const char LIGHTWHITE       = 97;
        const char LIGHTYELLOW      = 93;
        const char RESET            = 39;
    }
    namespace Back {
        const char BLACK            = 40;
        const char BLUE             = 44;
        const char CYAN             = 46;
        const char GREEN            = 42;
        const char MAGENTA          = 45;
        const char RED              = 41;
        const char WHITE            = 47;
        const char YELLOW           = 43;
        const char LIGHTBLACK       = 100;
        const char LIGHTBLUE        = 104;
        const char LIGHTCYAN        = 106;
        const char LIGHTGREEN       = 102;
        const char LIGHTMAGENTA     = 105;
        const char LIGHTRED         = 101;
        const char LIGHTWHITE       = 107;
        const char LIGHTYELLOW      = 103;
        const char RESET            = 49;
    }
    namespace Style {
        const char BRIGHT         = 1;
        const char DIM            = 2;
        const char ITALIC         = 3;
        const char UNDERLINE      = 4;
        const char BLINK          = 5;
        const char&SLOWBLINK      = BLINK;
        const char RAPIDBLINK     = 6;
        const char INVERT         = 7;
        const char HIDE           = 8;
        const char STRIKE         = 9;
        const char NOTUNDERLINED  = 24;
        const char NOTBLINKING    = 25;
        const char NOTREVERSED    = 27;
        const char REVEAL         = 28;
        const char NORMAL         = 22;
        const char RESET          = 0;
    }

    // const char * PALETTE     = "\x1b[{}m";
    const char * PALETTE     = "\e[{}m"; // Escape Sequence Template
    const std::string RESET_ALL = "0"; // Reset Code For Colors and Styles

    /* std::unordered_map<std::string, char> Map:
     *  A Map which links Fore, Back, Style Namespaces inner Codes into strings
     */
    std::unordered_map<std::string, char> Map = {
        {"FBLACK", Fore::BLACK},
        {"FBLUE", Fore::BLUE},
        {"FCYAN", Fore::CYAN},
        {"FGREEN", Fore::GREEN},
        {"FMAGENTA", Fore::MAGENTA},
        {"FRED", Fore::RED},
        {"FWHITE", Fore::WHITE},
        {"FYELLOW", Fore::YELLOW},
        {"FLIGHTBLACK", Fore::LIGHTBLACK},
        {"FLIGHTBLUE", Fore::LIGHTBLUE},
        {"FLIGHTCYAN", Fore::LIGHTCYAN},
        {"FLIGHTGREEN", Fore::LIGHTGREEN},
        {"FLIGHTMAGENTA", Fore::LIGHTMAGENTA},
        {"FLIGHTRED", Fore::LIGHTRED},
        {"FLIGHTWHITE", Fore::LIGHTWHITE},
        {"FLIGHTYELLOW", Fore::LIGHTYELLOW},
        {"FRESET", Fore::RESET},

        {"BBLACK", Back::BLACK},
        {"BBLUE", Back::BLUE},
        {"BCYAN", Back::CYAN},
        {"BGREEN", Back::GREEN},
        {"BMAGENTA", Back::MAGENTA},
        {"BRED", Back::RED},
        {"BWHITE", Back::WHITE},
        {"BYELLOW", Back::YELLOW},
        {"BLIGHTBLACK", Back::LIGHTBLACK},
        {"BLIGHTBLUE", Back::LIGHTBLUE},
        {"BLIGHTCYAN", Back::LIGHTCYAN},
        {"BLIGHTGREEN", Back::LIGHTGREEN},
        {"BLIGHTMAGENTA", Back::LIGHTMAGENTA},
        {"BLIGHTRED", Back::LIGHTRED},
        {"BLIGHTWHITE", Back::LIGHTWHITE},
        {"BLIGHTYELLOW", Back::LIGHTYELLOW},
        {"BRESET", Back::RESET},

        {"BRIGHT", Style::BRIGHT},
        {"DIM", Style::DIM},
        {"ITALIC", Style::ITALIC},
        {"UNDERLINE", Style::UNDERLINE},
        {"BLINK", Style::BLINK},
        {"SLOWBLINK", Style::SLOWBLINK},
        {"RAPIDBLINK", Style::RAPIDBLINK},
        {"INVERT", Style::INVERT},
        {"HIDE", Style::HIDE},
        {"STRIKE", Style::STRIKE},
        {"NOTUNDERLINED", Style::NOTUNDERLINED},
        {"NOTBLINKING", Style::NOTBLINKING},
        {"NOTREVERSED", Style::NOTREVERSED},
        {"REVEAL", Style::REVEAL},
        {"NORMAL", Style::NORMAL},
        {"RESET_ALL", Style::RESET}
    };
}
namespace colormerge {
    inline std::string inpalette(std::string input, std::string palette)  {
	/* inpalette: Generates a valid escape sequence according to a palette (Template)
     * by Replacing The Braces {} which indicate the position in the string "palette" to merge the "input" to the "palette"
     */

        return palette.replace(palette.find("{}"), 2, input);
    }
    inline std::string merge(std::vector<char> codes) {
        /* merge: merges every item in codes into a single string.
         *
         * Ex.: merge({1,2,3}) -> "1;2;3"
         */

        std::string output;
        for (unsigned int x=0; x<codes.size(); x++) {
            output += std::to_string(codes[x]) + (x+1<codes.size()?";":"");
        }
        return output;
    }

    inline std::string generate(std::vector<char> codes, std::string input, std::string palette, bool reset=true) {
        /* generate: generates an Escape sequence according the parameters (codes, input, palette, reset)
        *           if "reset" is true, the Escape Code Color::RESET_ALL (Resets (Background and Foreground) Colors, Style)
        *           will be added to the End of the Escape Sequence.
        */

        return inpalette(merge(codes), palette) + input + (reset?inpalette(Color::RESET_ALL, palette):"");
    }

    inline void printc(std::string input, std::vector<char> codes, bool reset=true, std::string endl="\n") {
        /* printc: prints an escape sequence which is generated from the parameters (input, codes, reset, endl),
        *
        */
        printf("%s", (generate(codes, input, Color::PALETTE, reset) + endl).c_str());
    }
}

namespace colorformat {
    std::vector<int> find(std::string input) {
        // find: locates format-able parts positions in "input"
        std::vector<int> output;
        bool opened = false;
        for (unsigned int x=0; x<input.size(); x++) {
            if (input[x] == '*' && input[x+1] == '{') { output.emplace_back(x); opened = true;}
            if (input[x] == '}' && opened == true) { output.emplace_back(x-output[0]); opened = false; }
        }
        return output;
    }
    std::vector<char> pass_arg(std::vector<char> container, std::string arg) {
        if (arg != "NULL") {
            for (unsigned int x=0, x2=0; x < arg.size(); x++) {
                if (arg[x] == ':' || x==arg.size()-1) {
                    container.emplace_back(Color::Map[arg.substr(x2, x==arg.size()-1?x-x2+1:x-x2)]);
                    x2 = x+1;
                }
            }
        }
        return container;
    }
    std::string parse(std::string input) {
        // parse: translates multiple colors in single Code Indicator (*{__:__:__}) to an Escape Sequence
        return colormerge::inpalette(colormerge::merge(pass_arg({}, input.substr(2, input.size()-3))),
                                     Color::PALETTE);
    }
    std::string format(std::string input) {
        /* format: fully parses and translates color indicators "input" into a consistent escape sequence,
        * Ex.: *{FMAGENTA:BBLACK}Hello World!*{RESET_ALL}
        *      -> "\x1b[35;40mHello World!\x1b[0m"
        */

        while (input.find("*{") != std::string::npos) {
            std::vector<int> points = find(input);
            input = input.replace(points[0], points[1]+1, parse(input.substr(points[0], points[1]+1)));
        }
        return input;
    }
}
