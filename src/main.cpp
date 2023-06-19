/* a simple escape sequence generation command-line Application
 *
 * ShellColor and all of its components are Licensed under "GNU General Public License Version 3".
 *
 */

#include <iostream>
#include "shellcolor.hpp"
#include "shellopts.hpp"

// "APPNAME" is Application Name which will be used in help() function
#define APPNAME "shellcolor"
#define APPVERSION 0.9

using namespace std;

class App {
public:
    ShellOpts shellopts;
    int Run(int argc, char ** argv) {
        // Run: initial operation such as arguments parsing, and requests executing
        shellopts.add_option("text", shellopts.make_option(0, "text", "t", "input value to append to an escape sequence"));
        shellopts.add_option("background", shellopts.make_option(0, "background", "b", "output escape sequence background color"));
        shellopts.add_option("foreground", shellopts.make_option(0, "foreground", "f", "output escape sequence foreground color"));
        shellopts.add_option("style", shellopts.make_option(0, "style", "s", "output escape sequence style"));
        shellopts.add_option("format", shellopts.make_option(0, "format", "F", "output a formatted input", ""));
        shellopts.add_option("list", shellopts.make_option(1, "list", "l", "list supported escape codes"));
        shellopts.add_option("no_reset", shellopts.make_option(1, "no_reset", "r", "append a reset escape"));
        shellopts.add_option("no_newline", shellopts.make_option(1, "no_newline", "n", "disable newline escape sequence"));
        shellopts.add_option("help", shellopts.make_option(1, "help", "h", "print this help message"));
        shellopts.add_option("version", shellopts.make_option(1, "version", "v", "print current program version"));
        shellopts.parse_args(argc, argv);

        /* Checks if help flag is present,
         * if it is, help message will be printed and other options will be ignored
         */
        if (eval(shellopts.get_content("help")) || argc<=1 || shellopts.unregistered_options.size() > 0) shellopts.print_help("usage: " + colormerge::generate({Color::Style::BRIGHT, Color::Fore::BLUE}, std::string(argv[0]), Color::PALETTE) + " <options> ");
        else main();

        if (argc<=1 || shellopts.unregistered_options.size() > 0) {
            std::cout << colormerge::generate({Color::Fore::RED, Color::Style::BRIGHT}, "Error: ", Color::PALETTE) + "Invalid Argument" << (shellopts.unregistered_options.size()>1?"s":"") << ":";
            for (std::string op : shellopts.unregistered_options) std::cout << " " << op;
            std::cout << std::endl;
            return 1;
        }

        return 0;
    }

    void main() {
        if (eval(shellopts.get_content("version"))) version();
        if (eval(shellopts.get_content("list"))) {
            colormerge::printc("Available Color: ", {Color::Fore::BLUE, Color::Style::BRIGHT}, true, "\n\n");
            for (std::pair<std::string, char> code : Color::Map) {
                printf("%s",
                       ("\t- " + code.first + " : " + colormerge::generate({code.second}, code.first, Color::PALETTE) + "\n").c_str());
            }
        }
        else if (!shellopts.get_content("format").empty())
                // Priority to "format" operation, when it is request operations other will be ignored
                printf("%s",
                       ((colorformat::format(shellopts.get_content("format"))+(shellopts.get_content("no_newline")=="true"?"":"\n"))).c_str());

            else {
                std::vector<char> codes;
                if (!shellopts.get_content("background").empty()) codes.emplace_back(Color::Map["B" + shellopts.get_content("background")]);
                if (!shellopts.get_content("foreground").empty()) codes.emplace_back(Color::Map["F" + shellopts.get_content("foreground")]);
                codes = colorformat::pass_arg(codes, shellopts.get_content("style"));
                // Prints colored text according to text value and other arguments
                if (!shellopts.get_content("text").empty()) {
                    colormerge::printc(
                        shellopts.get_content("text"),
                        {codes},
                        (!eval(shellopts.get_content("no_reset"))),
                        (shellopts.get_content("no_newline")=="true"?"":"\n")
                    );
                }
            }
    }

    void version() {
        printf("%s %.1f \n",
               colormerge::generate({Color::Fore::BLUE, Color::Style::BRIGHT}, APPNAME, Color::PALETTE).c_str(),
               APPVERSION
               );
    }
private:
    bool eval(std::string string_boolean) {
        if (string_boolean == "true") return true;
        return false;
    }
};

int main(int argc, char ** argv) {
    App app;
    return app.Run(argc, argv);
}
